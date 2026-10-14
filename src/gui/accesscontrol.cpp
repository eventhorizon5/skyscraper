/*
	Skyscraper 2.1 - Access Control Window
	Copyright (C)2003-2026 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <cstdlib>
#include <algorithm>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/statbox.h>
#include "globals.h"
#include "sbs.h"
#include "access.h"
#include "debugpanel.h"
#include "accesscontrol.h"

namespace Skyscraper {

static wxString W(const std::string &text)
{
	return wxString::FromUTF8(text.c_str());
}

static std::string S(const wxString &text)
{
	return std::string(text.ToUTF8().data());
}

static std::string Trim(std::string text)
{
	SBS::TrimString(text);
	return text;
}

AccessControlWindow::AccessControlWindow(DebugPanel *parent, wxWindowID id)
{
	Create(parent, id, _("Access Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, _T("AccessControl"));

	panel = parent;
	Simcore = 0;
	access = 0;
	refresh_counter = 0;
	last_log_total = 0;
	last_version = (unsigned long)-1;

	wxBoxSizer *main = new wxBoxSizer(wxVERTICAL);

	lblStatus = new wxStaticText(this, wxID_ANY, _("No building loaded"));
	main->Add(lblStatus, 0, wxALL | wxEXPAND, 6);

	book = new wxNotebook(this, wxID_ANY);
	book->AddPage(BuildCardsPage(book), _("Cards"));
	book->AddPage(BuildLevelsPage(book), _("Access Levels"));
	book->AddPage(BuildProfilesPage(book), _("Time Profiles"));
	book->AddPage(BuildStatusPage(book), _("Doors, Elevators and Zones"));
	book->AddPage(BuildLogPage(book), _("Live Log"));
	book->AddPage(BuildReaderPage(book), _("Badge Reader Input"));
	main->Add(book, 1, wxALL | wxEXPAND, 6);

	wxButton *bClose = new wxButton(this, wxID_ANY, _("Close"));
	main->Add(bClose, 0, wxALL | wxALIGN_RIGHT, 6);
	bClose->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { Hide(); });

	SetSizer(main);
	SetMinSize(wxSize(820, 640));
	main->SetSizeHints(this);
	SetSize(wxSize(880, 700));

	Bind(wxEVT_CLOSE_WINDOW, &AccessControlWindow::OnClose, this);

	EnableControls(false);
}

AccessControlWindow::~AccessControlWindow()
{
}

//
// page construction
//

wxPanel* AccessControlWindow::BuildCardsPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	CardList = new wxListCtrl(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 220), wxLC_REPORT | wxLC_SINGLE_SEL);
	CardList->AppendColumn(_("Card"), wxLIST_FORMAT_LEFT, 90);
	CardList->AppendColumn(_("Holder"), wxLIST_FORMAT_LEFT, 150);
	CardList->AppendColumn(_("Groups"), wxLIST_FORMAT_LEFT, 200);
	CardList->AppendColumn(_("Home floor"), wxLIST_FORMAT_LEFT, 80);
	CardList->AppendColumn(_("Status"), wxLIST_FORMAT_LEFT, 80);
	CardList->AppendColumn(_("Wallet"), wxLIST_FORMAT_LEFT, 80);
	sizer->Add(CardList, 1, wxALL | wxEXPAND, 4);
	CardList->Bind(wxEVT_LIST_ITEM_SELECTED, &AccessControlWindow::OnCardSelected, this);

	wxFlexGridSizer *grid = new wxFlexGridSizer(2, 4, 4);
	grid->AddGrowableCol(1);
	grid->Add(new wxStaticText(page, wxID_ANY, _("Facility code (0-255):")), 0, wxALIGN_CENTER_VERTICAL);
	tFacility = new wxTextCtrl(page, wxID_ANY);
	grid->Add(tFacility, 1, wxEXPAND);
	grid->Add(new wxStaticText(page, wxID_ANY, _("Card number (0-65535):")), 0, wxALIGN_CENTER_VERTICAL);
	tNumber = new wxTextCtrl(page, wxID_ANY);
	grid->Add(tNumber, 1, wxEXPAND);
	grid->Add(new wxStaticText(page, wxID_ANY, _("Holder name:")), 0, wxALIGN_CENTER_VERTICAL);
	tHolder = new wxTextCtrl(page, wxID_ANY);
	grid->Add(tHolder, 1, wxEXPAND);
	grid->Add(new wxStaticText(page, wxID_ANY, _("Home floor (button label):")), 0, wxALIGN_CENTER_VERTICAL);
	tHome = new wxTextCtrl(page, wxID_ANY);
	grid->Add(tHome, 1, wxEXPAND);
	sizer->Add(grid, 0, wxALL | wxEXPAND, 4);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Access levels for this card (tick all that apply; a card gets everything its levels allow):")), 0, wxLEFT | wxTOP, 4);
	clGroups = new wxCheckListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 90));
	sizer->Add(clGroups, 0, wxALL | wxEXPAND, 4);

	wxBoxSizer *buttons = new wxBoxSizer(wxHORIZONTAL);
	bSaveCard = new wxButton(page, wxID_ANY, _("Add / Update Card"));
	bEnable = new wxButton(page, wxID_ANY, _("Enable"));
	bDisable = new wxButton(page, wxID_ANY, _("Disable (lost badge)"));
	buttons->Add(bSaveCard, 0, wxRIGHT, 4);
	buttons->Add(bEnable, 0, wxRIGHT, 4);
	buttons->Add(bDisable, 0, wxRIGHT, 4);
	sizer->Add(buttons, 0, wxALL, 4);

	wxBoxSizer *buttons2 = new wxBoxSizer(wxHORIZONTAL);
	bWalletAdd = new wxButton(page, wxID_ANY, _("Give Me This Badge"));
	bWalletRemove = new wxButton(page, wxID_ANY, _("Take Badge Back"));
	bActive = new wxButton(page, wxID_ANY, _("Use This Badge"));
	bExportCards = new wxButton(page, wxID_ANY, _("Export as Script..."));
	buttons2->Add(bWalletAdd, 0, wxRIGHT, 4);
	buttons2->Add(bWalletRemove, 0, wxRIGHT, 4);
	buttons2->Add(bActive, 0, wxRIGHT, 4);
	buttons2->Add(bExportCards, 0, wxRIGHT, 4);
	sizer->Add(buttons2, 0, wxALL, 4);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Changes made in this window last until the building is reloaded. Use Export as Script to keep them\n(it writes the whole <Access> section: time profiles, access levels, rules and cards).")), 0, wxALL, 4);

	bSaveCard->Bind(wxEVT_BUTTON, &AccessControlWindow::OnSaveCard, this);
	bEnable->Bind(wxEVT_BUTTON, &AccessControlWindow::OnEnable, this);
	bDisable->Bind(wxEVT_BUTTON, &AccessControlWindow::OnDisable, this);
	bWalletAdd->Bind(wxEVT_BUTTON, &AccessControlWindow::OnWalletAdd, this);
	bWalletRemove->Bind(wxEVT_BUTTON, &AccessControlWindow::OnWalletRemove, this);
	bActive->Bind(wxEVT_BUTTON, &AccessControlWindow::OnActive, this);
	bExportCards->Bind(wxEVT_BUTTON, &AccessControlWindow::OnExportCards, this);

	page->SetSizer(sizer);
	return page;
}

wxPanel* AccessControlWindow::BuildProfilesPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(page, wxID_ANY,
		_("A time profile says when access is allowed. It's made of one or more time windows (days plus a start and end time).\n"
		  "Times follow the sky clock (Sky Control). A window that ends before it starts runs overnight. Always and Never are built in.")),
		0, wxALL, 6);

	wxBoxSizer *columns = new wxBoxSizer(wxHORIZONTAL);

	//profiles column
	wxBoxSizer *left = new wxBoxSizer(wxVERTICAL);
	left->Add(new wxStaticText(page, wxID_ANY, _("Time profiles:")), 0, wxBOTTOM, 2);
	ScheduleList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(220, 220));
	left->Add(ScheduleList, 1, wxEXPAND | wxBOTTOM, 4);
	wxBoxSizer *newrow = new wxBoxSizer(wxHORIZONTAL);
	tScheduleName = new wxTextCtrl(page, wxID_ANY);
	tScheduleName->SetHint(_("New profile name"));
	newrow->Add(tScheduleName, 1, wxRIGHT, 4);
	bNewSchedule = new wxButton(page, wxID_ANY, _("Create"));
	newrow->Add(bNewSchedule, 0);
	left->Add(newrow, 0, wxEXPAND | wxBOTTOM, 4);
	bDeleteSchedule = new wxButton(page, wxID_ANY, _("Delete Profile"));
	left->Add(bDeleteSchedule, 0);
	columns->Add(left, 0, wxALL | wxEXPAND, 6);

	//windows column
	wxBoxSizer *right = new wxBoxSizer(wxVERTICAL);
	right->Add(new wxStaticText(page, wxID_ANY, _("Time windows in the selected profile:")), 0, wxBOTTOM, 2);
	WindowList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 140));
	right->Add(WindowList, 1, wxEXPAND | wxBOTTOM, 4);
	bRemoveWindow = new wxButton(page, wxID_ANY, _("Remove Window"));
	right->Add(bRemoveWindow, 0, wxBOTTOM, 8);

	wxStaticBoxSizer *add = new wxStaticBoxSizer(wxVERTICAL, page, _("Add a time window"));
	wxBoxSizer *days = new wxBoxSizer(wxHORIZONTAL);
	static const char *names[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	for (int d = 0; d < 7; d++)
	{
		chkDay[d] = new wxCheckBox(add->GetStaticBox(), wxID_ANY, wxString::FromAscii(names[d]));
		chkDay[d]->SetValue(d >= 1 && d <= 5);
		days->Add(chkDay[d], 0, wxRIGHT, 6);
	}
	add->Add(days, 0, wxALL, 4);
	wxBoxSizer *times = new wxBoxSizer(wxHORIZONTAL);
	times->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("From")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	tStart = new wxTextCtrl(add->GetStaticBox(), wxID_ANY, _("07:00"), wxDefaultPosition, wxSize(60, -1));
	times->Add(tStart, 0, wxRIGHT, 8);
	times->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("to")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	tEnd = new wxTextCtrl(add->GetStaticBox(), wxID_ANY, _("19:00"), wxDefaultPosition, wxSize(60, -1));
	times->Add(tEnd, 0, wxRIGHT, 8);
	times->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("(24-hour, HH:MM)")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 12);
	bAddWindow = new wxButton(add->GetStaticBox(), wxID_ANY, _("Add Window"));
	times->Add(bAddWindow, 0);
	add->Add(times, 0, wxALL, 4);
	right->Add(add, 0, wxEXPAND);
	columns->Add(right, 1, wxALL | wxEXPAND, 6);

	sizer->Add(columns, 1, wxEXPAND);

	ScheduleList->Bind(wxEVT_LISTBOX, &AccessControlWindow::OnScheduleSelected, this);
	bNewSchedule->Bind(wxEVT_BUTTON, &AccessControlWindow::OnNewSchedule, this);
	bDeleteSchedule->Bind(wxEVT_BUTTON, &AccessControlWindow::OnDeleteSchedule, this);
	bAddWindow->Bind(wxEVT_BUTTON, &AccessControlWindow::OnAddWindow, this);
	bRemoveWindow->Bind(wxEVT_BUTTON, &AccessControlWindow::OnRemoveWindow, this);

	page->SetSizer(sizer);
	return page;
}

wxPanel* AccessControlWindow::BuildLevelsPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(page, wxID_ANY,
		_("An access level (group) is a set of rules. Each rule pairs a place (a door, or elevator floors by button label)\n"
		  "with a time profile. Give cards access levels on the Cards tab.")),
		0, wxALL, 6);

	wxBoxSizer *columns = new wxBoxSizer(wxHORIZONTAL);

	//levels column
	wxBoxSizer *left = new wxBoxSizer(wxVERTICAL);
	left->Add(new wxStaticText(page, wxID_ANY, _("Access levels:")), 0, wxBOTTOM, 2);
	GroupList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(220, 200));
	left->Add(GroupList, 1, wxEXPAND | wxBOTTOM, 4);
	wxBoxSizer *newrow = new wxBoxSizer(wxHORIZONTAL);
	tGroupName = new wxTextCtrl(page, wxID_ANY);
	tGroupName->SetHint(_("New access level name"));
	newrow->Add(tGroupName, 1, wxRIGHT, 4);
	bNewGroup = new wxButton(page, wxID_ANY, _("Create"));
	newrow->Add(bNewGroup, 0);
	left->Add(newrow, 0, wxEXPAND | wxBOTTOM, 4);
	bDeleteGroup = new wxButton(page, wxID_ANY, _("Delete Level"));
	left->Add(bDeleteGroup, 0, wxBOTTOM, 4);
	chkOverride = new wxCheckBox(page, wxID_ANY, _("Override: everywhere, any time,\neven during lockdown"));
	left->Add(chkOverride, 0);
	columns->Add(left, 0, wxALL | wxEXPAND, 6);

	//rules column
	wxBoxSizer *right = new wxBoxSizer(wxVERTICAL);
	right->Add(new wxStaticText(page, wxID_ANY, _("Rules in the selected level (place - time profile):")), 0, wxBOTTOM, 2);
	RuleList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 120));
	right->Add(RuleList, 1, wxEXPAND | wxBOTTOM, 4);
	bRemoveRule = new wxButton(page, wxID_ANY, _("Remove Rule"));
	right->Add(bRemoveRule, 0, wxBOTTOM, 8);

	wxStaticBoxSizer *add = new wxStaticBoxSizer(wxVERTICAL, page, _("Add a rule"));
	wxFlexGridSizer *grid = new wxFlexGridSizer(2, 4, 6);
	grid->AddGrowableCol(1);
	grid->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("Place:")), 0, wxALIGN_CENTER_VERTICAL);
	cPlace = new wxComboBox(add->GetStaticBox(), wxID_ANY);
	grid->Add(cPlace, 1, wxEXPAND);
	grid->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("Time profile:")), 0, wxALIGN_CENTER_VERTICAL);
	cRuleSchedule = new wxChoice(add->GetStaticBox(), wxID_ANY);
	grid->Add(cRuleSchedule, 1, wxEXPAND);
	add->Add(grid, 0, wxALL | wxEXPAND, 4);
	add->Add(new wxStaticText(add->GetStaticBox(), wxID_ANY, _("For elevators, edit the floor list after choosing, such as Elevator 1:Floors 7-9 or Elevator 1:Floor R.")), 0, wxLEFT | wxRIGHT, 4);
	bAddRule = new wxButton(add->GetStaticBox(), wxID_ANY, _("Add Rule"));
	add->Add(bAddRule, 0, wxALL, 4);
	right->Add(add, 0, wxEXPAND);
	columns->Add(right, 1, wxALL | wxEXPAND, 6);
	sizer->Add(columns, 1, wxEXPAND);

	//access check
	wxStaticBoxSizer *check = new wxStaticBoxSizer(wxVERTICAL, page, _("Check access right now (doesn't unlock anything or write to the log)"));
	wxBoxSizer *row = new wxBoxSizer(wxHORIZONTAL);
	row->Add(new wxStaticText(check->GetStaticBox(), wxID_ANY, _("Card:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	cCheckCard = new wxChoice(check->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(200, -1));
	row->Add(cCheckCard, 0, wxRIGHT, 8);
	row->Add(new wxStaticText(check->GetStaticBox(), wxID_ANY, _("Place:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	cCheckPlace = new wxComboBox(check->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(220, -1));
	row->Add(cCheckPlace, 1, wxRIGHT, 8);
	bCheck = new wxButton(check->GetStaticBox(), wxID_ANY, _("Check"));
	row->Add(bCheck, 0);
	check->Add(row, 0, wxALL | wxEXPAND, 4);
	lblCheck = new wxStaticText(check->GetStaticBox(), wxID_ANY, wxEmptyString);
	check->Add(lblCheck, 0, wxALL | wxEXPAND, 4);
	sizer->Add(check, 0, wxALL | wxEXPAND, 6);

	GroupList->Bind(wxEVT_LISTBOX, &AccessControlWindow::OnGroupSelected, this);
	bNewGroup->Bind(wxEVT_BUTTON, &AccessControlWindow::OnNewGroup, this);
	bDeleteGroup->Bind(wxEVT_BUTTON, &AccessControlWindow::OnDeleteGroup, this);
	chkOverride->Bind(wxEVT_CHECKBOX, &AccessControlWindow::OnOverride, this);
	bAddRule->Bind(wxEVT_BUTTON, &AccessControlWindow::OnAddRule, this);
	bRemoveRule->Bind(wxEVT_BUTTON, &AccessControlWindow::OnRemoveRule, this);
	bCheck->Bind(wxEVT_BUTTON, &AccessControlWindow::OnCheck, this);

	page->SetSizer(sizer);
	return page;
}

wxPanel* AccessControlWindow::BuildStatusPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Access points (doors):")), 0, wxLEFT | wxTOP, 4);
	PointList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 130));
	sizer->Add(PointList, 1, wxALL | wxEXPAND, 4);

	wxBoxSizer *pointButtons = new wxBoxSizer(wxHORIZONTAL);
	bGrant = new wxButton(page, wxID_ANY, _("Momentary Unlock"));
	bHoldUnlock = new wxButton(page, wxID_ANY, _("Hold Unlocked"));
	bNormal = new wxButton(page, wxID_ANY, _("Return to Normal"));
	pointButtons->Add(bGrant, 0, wxRIGHT, 4);
	pointButtons->Add(bHoldUnlock, 0, wxRIGHT, 4);
	pointButtons->Add(bNormal, 0, wxRIGHT, 12);
	pointButtons->Add(new wxStaticText(page, wxID_ANY, _("Auto-close all doors (s, 0 = off):")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	tAutoClose = new wxTextCtrl(page, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1));
	pointButtons->Add(tAutoClose, 0, wxRIGHT, 4);
	bSetAutoClose = new wxButton(page, wxID_ANY, _("Set"));
	pointButtons->Add(bSetAutoClose, 0);
	sizer->Add(pointButtons, 0, wxALL, 4);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Secured elevators:")), 0, wxLEFT | wxTOP, 4);
	ElevatorList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	sizer->Add(ElevatorList, 0, wxALL | wxEXPAND, 4);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Secured zones:")), 0, wxLEFT | wxTOP, 4);
	ZoneList = new wxListBox(page, wxID_ANY, wxDefaultPosition, wxSize(-1, 70));
	sizer->Add(ZoneList, 0, wxALL | wxEXPAND, 4);

	wxBoxSizer *zoneButtons = new wxBoxSizer(wxHORIZONTAL);
	bResetZone = new wxButton(page, wxID_ANY, _("Reset Zone (alarm and anti-passback)"));
	zoneButtons->Add(bResetZone, 0, wxRIGHT, 12);
	chkLockdown = new wxCheckBox(page, wxID_ANY, _("Building lockdown (only override groups get in)"));
	zoneButtons->Add(chkLockdown, 0, wxALIGN_CENTER_VERTICAL);
	sizer->Add(zoneButtons, 0, wxALL, 4);

	bGrant->Bind(wxEVT_BUTTON, &AccessControlWindow::OnGrant, this);
	bHoldUnlock->Bind(wxEVT_BUTTON, &AccessControlWindow::OnHoldUnlock, this);
	bNormal->Bind(wxEVT_BUTTON, &AccessControlWindow::OnNormal, this);
	bSetAutoClose->Bind(wxEVT_BUTTON, &AccessControlWindow::OnSetAutoClose, this);
	bResetZone->Bind(wxEVT_BUTTON, &AccessControlWindow::OnResetZone, this);
	chkLockdown->Bind(wxEVT_CHECKBOX, &AccessControlWindow::OnLockdown, this);

	page->SetSizer(sizer);
	return page;
}

wxPanel* AccessControlWindow::BuildLogPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *top = new wxBoxSizer(wxHORIZONTAL);
	top->Add(new wxStaticText(page, wxID_ANY, _("Filter (any text, such as a name, card or door):")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);
	tFilter = new wxTextCtrl(page, wxID_ANY);
	top->Add(tFilter, 1, wxRIGHT, 8);
	bExportLog = new wxButton(page, wxID_ANY, _("Export CSV..."));
	top->Add(bExportLog, 0);
	sizer->Add(top, 0, wxALL | wxEXPAND, 4);

	LogList = new wxListCtrl(page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
	LogList->AppendColumn(_("Time"), wxLIST_FORMAT_LEFT, 60);
	LogList->AppendColumn(_("Event"), wxLIST_FORMAT_LEFT, 75);
	LogList->AppendColumn(_("Where"), wxLIST_FORMAT_LEFT, 170);
	LogList->AppendColumn(_("Card"), wxLIST_FORMAT_LEFT, 80);
	LogList->AppendColumn(_("Holder"), wxLIST_FORMAT_LEFT, 130);
	LogList->AppendColumn(_("Detail"), wxLIST_FORMAT_LEFT, 220);
	sizer->Add(LogList, 1, wxALL | wxEXPAND, 4);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Newest events are at the top.")), 0, wxALL, 4);

	tFilter->Bind(wxEVT_TEXT, &AccessControlWindow::OnFilter, this);
	bExportLog->Bind(wxEVT_BUTTON, &AccessControlWindow::OnExportLog, this);

	page->SetSizer(sizer);
	return page;
}

wxPanel* AccessControlWindow::BuildReaderPage(wxNotebook *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(new wxStaticText(page, wxID_ANY,
		_("Tap badges here, from the keyboard or a USB badge reader set to keyboard-emulation mode.\n"
		  "Choose where the badge is presented, click in the input box, then tap the badge (or type and press Enter).")),
		0, wxALL, 6);

	wxFlexGridSizer *grid = new wxFlexGridSizer(2, 4, 6);
	grid->AddGrowableCol(1);

	grid->Add(new wxStaticText(page, wxID_ANY, _("Present at:")), 0, wxALIGN_CENTER_VERTICAL);
	cTarget = new wxComboBox(page, wxID_ANY);
	grid->Add(cTarget, 1, wxEXPAND);

	grid->Add(new wxStaticText(page, wxID_ANY, _("Facility code for card-number-only input:")), 0, wxALIGN_CENTER_VERTICAL);
	tDefaultFacility = new wxTextCtrl(page, wxID_ANY, _("42"));
	grid->Add(tDefaultFacility, 1, wxEXPAND);
	sizer->Add(grid, 0, wxALL | wxEXPAND, 6);

	wxString formats[] = {
		_("Facility:Card (for example 42:10001)"),
		_("Card number only (uses the facility code above)"),
		_("Raw 26-bit Wiegand, decimal"),
		_("Raw 26-bit Wiegand, hexadecimal")
	};
	rFormat = new wxRadioBox(page, wxID_ANY, _("Reader output format"), wxDefaultPosition, wxDefaultSize, 4, formats, 1, wxRA_SPECIFY_COLS);
	sizer->Add(rFormat, 0, wxALL | wxEXPAND, 6);

	sizer->Add(new wxStaticText(page, wxID_ANY, _("Badge input:")), 0, wxLEFT | wxTOP, 6);
	tReaderInput = new wxTextCtrl(page, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	sizer->Add(tReaderInput, 0, wxALL | wxEXPAND, 6);

	lblReaderResult = new wxStaticText(page, wxID_ANY, _("Waiting for a badge."));
	sizer->Add(lblReaderResult, 0, wxALL | wxEXPAND, 6);

	sizer->Add(new wxStaticText(page, wxID_ANY,
		_("Tip: unknown badges are logged with their numbers, so to enroll a real badge, tap it here,\n"
		  "then copy its number from the Live Log into a new card on the Cards page.")),
		0, wxALL, 6);

	tReaderInput->Bind(wxEVT_TEXT_ENTER, &AccessControlWindow::OnReaderInput, this);

	page->SetSizer(sizer);
	return page;
}

//
// refresh
//

void AccessControlWindow::EnableControls(bool value)
{
	book->Enable(value);
}

void AccessControlWindow::Loop()
{
	//called by the control panel's timer while the window is shown

	if (Simcore != panel->GetSystem())
	{
		Simcore = panel->GetSystem();
		access = 0;
		last_log_total = 0;
		last_version = (unsigned long)-1;
		refresh_counter = 0;
	}

	if (!Simcore)
	{
		lblStatus->SetLabel(_("No building loaded"));
		EnableControls(false);
		return;
	}

	SBS::AccessController *current = Simcore->GetAccessController(false);
	if (current != access)
	{
		access = current;
		last_log_total = 0;
		last_version = (unsigned long)-1;
		refresh_counter = 0;
		CardList->DeleteAllItems();
		LogList->DeleteAllItems();
		RefreshTargets();
	}

	if (!access)
	{
		lblStatus->SetLabel(_("This building doesn't use access control (it has no <Access> section)."));
		EnableControls(false);
		return;
	}

	EnableControls(true);

	//refresh about twice a second
	if (refresh_counter-- > 0)
		return;
	refresh_counter = 12;

	int facility = 0, number = 0;
	std::string active = "none";
	if (access->GetActiveBadge(facility, number))
	{
		const SBS::Credential *c = access->GetCore()->GetCredential(facility, number);
		active = SBS::ToString(facility) + ":" + SBS::ToString(number) + (c ? " (" + c->holder + ")" : "");
	}
	std::string state = access->GetCore()->GetLockdown() ? "   LOCKDOWN" : "";
	lblStatus->SetLabel(W("Building: " + Simcore->BuildingName + "   Your active badge: " + active + state));

	//rebuild the editing lists only when something changed
	if (access->GetCore()->GetVersion() != last_version)
	{
		last_version = access->GetCore()->GetVersion();
		RefreshGroupChoices();
		RefreshCards();
		RefreshProfiles();
		RefreshLevels();
	}
	RefreshStatus();
	RefreshLog(false);
}

void AccessControlWindow::RefreshCards()
{
	if (!access)
		return;

	long selected = CardList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

	std::vector<const SBS::Credential*> cards = access->GetCore()->GetCredentials();

	int active_facility = -1, active_number = -1;
	access->GetActiveBadge(active_facility, active_number);

	CardList->Freeze();
	CardList->DeleteAllItems();
	for (size_t i = 0; i < cards.size(); i++)
	{
		const SBS::Credential *c = cards[i];
		std::string groups;
		for (size_t j = 0; j < c->groups.size(); j++)
		{
			if (j > 0)
				groups += "|";
			groups += c->groups[j];
		}

		std::string wallet = "";
		if (access->IsWalletBadge(c->facility_code, c->card_number))
			wallet = (c->facility_code == active_facility && c->card_number == active_number) ? "yes (active)" : "yes";

		long row = CardList->InsertItem((long)i, W(SBS::ToString(c->facility_code) + ":" + SBS::ToString(c->card_number)));
		CardList->SetItem(row, 1, W(c->holder));
		CardList->SetItem(row, 2, W(groups));
		CardList->SetItem(row, 3, W(c->home_floor));
		CardList->SetItem(row, 4, c->enabled ? _("enabled") : _("DISABLED"));
		CardList->SetItem(row, 5, W(wallet));
	}
	if (selected >= 0 && selected < CardList->GetItemCount())
		CardList->SetItemState(selected, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	CardList->Thaw();

}

static void SetListLines(wxListBox *list, const std::vector<std::string> &lines)
{
	//update a list box in place, keeping the selection
	int selected = list->GetSelection();
	if ((int)list->GetCount() != (int)lines.size())
	{
		list->Clear();
		for (size_t i = 0; i < lines.size(); i++)
			list->Append(W(lines[i]));
	}
	else
	{
		for (size_t i = 0; i < lines.size(); i++)
		{
			wxString value = W(lines[i]);
			if (list->GetString((unsigned int)i) != value)
				list->SetString((unsigned int)i, value);
		}
	}
	if (selected >= 0 && selected < (int)list->GetCount())
		list->SetSelection(selected);
}

void AccessControlWindow::RefreshStatus()
{
	if (!access)
		return;

	std::vector<std::string> points;
	for (int i = 0; i < access->GetAccessPointCount(); i++)
	{
		SBS::AccessPoint *point = access->GetAccessPoint(i);
		int close = point->GetAutoCloseTime();
		points.emplace_back(point->GetName() + " - doors: " + SBS::ToString(point->GetDoorCount()) + ", " + point->GetStatus() +
			(close > 0 ? ", auto-close " + SBS::ToString(close / 1000.0) + " s" : ", auto-close off"));
	}
	SetListLines(PointList, points);
	SetListLines(ElevatorList, access->GetElevatorStatus());
	SetListLines(ZoneList, access->GetZoneStatus());

	if (chkLockdown->GetValue() != access->GetCore()->GetLockdown())
		chkLockdown->SetValue(access->GetCore()->GetLockdown());

	if (tAutoClose->HasFocus() == false)
		tAutoClose->ChangeValue(W(SBS::ToString(access->GetDefaultAutoClose() / 1000.0)));
}

void AccessControlWindow::RefreshLog(bool rebuild)
{
	if (!access)
		return;

	const std::deque<SBS::AccessEvent> &log = access->GetCore()->GetLog();
	unsigned long total = access->GetCore()->GetLogTotal();

	if (rebuild == false && total == last_log_total)
		return;

	//events added since the last refresh (all of them if rebuilding, or if more arrived than the log keeps)
	size_t fresh = (size_t)(total - last_log_total);
	if (rebuild == true || fresh > log.size())
	{
		LogList->DeleteAllItems();
		fresh = log.size();
	}
	last_log_total = total;

	std::string filter = S(tFilter->GetValue());
	SBS::SetCase(filter, false);
	filter = Trim(filter);

	LogList->Freeze();
	for (size_t i = log.size() - fresh; i < log.size(); i++)
	{
		const SBS::AccessEvent &e = log[i];
		std::string card = (e.facility_code >= 0 && e.card_number >= 0) ? SBS::ToString(e.facility_code) + ":" + SBS::ToString(e.card_number) : "-";

		if (!filter.empty())
		{
			std::string haystack = e.clock + " " + e.kind + " " + e.access_point + " " + card + " " + e.holder + " " + e.detail;
			SBS::SetCase(haystack, false);
			if (haystack.find(filter) == std::string::npos)
				continue;
		}

		//newest at the top
		long row = LogList->InsertItem(0, W(e.clock));
		LogList->SetItem(row, 1, W(e.kind));
		LogList->SetItem(row, 2, W(e.access_point));
		LogList->SetItem(row, 3, W(card));
		LogList->SetItem(row, 4, W(e.holder));
		LogList->SetItem(row, 5, W(e.detail));

		if (e.kind == "DENIED")
			LogList->SetItemTextColour(row, wxColour(200, 0, 0));
		else if (e.kind == "ALARM")
			LogList->SetItemTextColour(row, wxColour(200, 100, 0));
		else if (e.kind == "GRANTED")
			LogList->SetItemTextColour(row, wxColour(0, 130, 0));
	}

	//keep the view to a reasonable size
	while (LogList->GetItemCount() > 2000)
		LogList->DeleteItem(LogList->GetItemCount() - 1);
	LogList->Thaw();
}

void AccessControlWindow::RefreshTargets()
{
	//fill the reader page's target list with known access points and secured elevators
	cTarget->Clear();
	if (!access)
		return;

	for (int i = 0; i < access->GetAccessPointCount(); i++)
		cTarget->Append(W(access->GetAccessPoint(i)->GetName()));

	std::vector<std::string> elevators = access->GetElevatorStatus();
	for (size_t i = 0; i < elevators.size(); i++)
	{
		size_t dash = elevators[i].find(" - ");
		if (dash != std::string::npos)
			cTarget->Append(W(elevators[i].substr(0, dash)));
	}

	if (cTarget->GetCount() > 0)
		cTarget->SetSelection(0);
}

//
// cards page
//

bool AccessControlWindow::GetSelectedCard(int &facility, int &number)
{
	long selected = CardList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if (selected < 0)
	{
		//fall back to the edit fields
		return ReadCardFields(facility, number);
	}

	std::string text = S(CardList->GetItemText(selected));
	size_t colon = text.find(':');
	if (colon == std::string::npos)
		return false;
	facility = atoi(text.substr(0, colon).c_str());
	number = atoi(text.substr(colon + 1).c_str());
	return true;
}

bool AccessControlWindow::ReadCardFields(int &facility, int &number)
{
	std::string f = Trim(S(tFacility->GetValue()));
	std::string n = Trim(S(tNumber->GetValue()));
	if (!SBS::IsNumeric(f, facility) || !SBS::IsNumeric(n, number))
	{
		wxMessageBox(_("Enter a numeric facility code and card number."), _("Access Control"), wxOK | wxICON_WARNING, this);
		return false;
	}
	return true;
}

void AccessControlWindow::OnCardSelected(wxListEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;

	const SBS::Credential *c = access->GetCore()->GetCredential(facility, number);
	if (!c)
		return;

	std::string groups;
	for (size_t j = 0; j < c->groups.size(); j++)
	{
		if (j > 0)
			groups += "|";
		groups += c->groups[j];
	}

	tFacility->SetValue(W(SBS::ToString(c->facility_code)));
	tNumber->SetValue(W(SBS::ToString(c->card_number)));
	tHolder->SetValue(W(c->holder));
	tHome->SetValue(W(c->home_floor));

	//tick this card's access levels
	for (unsigned int i = 0; i < clGroups->GetCount(); i++)
	{
		std::string level = SBS::AccessCore::Normalize(S(clGroups->GetString(i)));
		bool member = false;
		for (size_t j = 0; j < c->groups.size(); j++)
		{
			if (SBS::AccessCore::Normalize(c->groups[j]) == level)
				member = true;
		}
		clGroups->Check(i, member);
	}
}

void AccessControlWindow::OnSaveCard(wxCommandEvent &event)
{
	if (!access)
		return;

	SBS::Credential credential;
	if (!ReadCardFields(credential.facility_code, credential.card_number))
		return;

	credential.holder = Trim(S(tHolder->GetValue()));
	credential.home_floor = Trim(S(tHome->GetValue()));

	for (unsigned int i = 0; i < clGroups->GetCount(); i++)
	{
		if (clGroups->IsChecked(i))
			credential.groups.emplace_back(S(clGroups->GetString(i)));
	}

	//keep a disabled card disabled when updating it
	const SBS::Credential *existing = access->GetCore()->GetCredential(credential.facility_code, credential.card_number);
	if (existing)
		credential.enabled = existing->enabled;

	std::string error;
	if (access->GetCore()->AddCredential(credential, error) == false)
	{
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
		return;
	}

	access->LogEvent("INFO", "Cards", credential.facility_code, credential.card_number, existing ? "Updated" : "Added");
	RefreshCards();
}

void AccessControlWindow::OnEnable(wxCommandEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;
	if (access->GetCore()->SetCredentialEnabled(facility, number, true))
		access->LogEvent("INFO", "Cards", facility, number, "Enabled");
	RefreshCards();
}

void AccessControlWindow::OnDisable(wxCommandEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;
	if (access->GetCore()->SetCredentialEnabled(facility, number, false))
		access->LogEvent("INFO", "Cards", facility, number, "Disabled");
	RefreshCards();
}

void AccessControlWindow::OnWalletAdd(wxCommandEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;
	access->WalletAdd(facility, number);
	RefreshCards();
}

void AccessControlWindow::OnWalletRemove(wxCommandEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;
	access->WalletRemove(facility, number);
	RefreshCards();
}

void AccessControlWindow::OnActive(wxCommandEvent &event)
{
	int facility = 0, number = 0;
	if (!access || !GetSelectedCard(facility, number))
		return;
	if (!access->IsWalletBadge(facility, number))
		access->WalletAdd(facility, number);
	access->SetActiveBadge(facility, number);
	RefreshCards();
}

void AccessControlWindow::OnExportCards(wxCommandEvent &event)
{
	if (!access)
		return;
	wxFileDialog dialog(this, _("Export access control as script"), wxEmptyString, _("access_section.txt"), _("Text files (*.txt)|*.txt|All files (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() != wxID_OK)
		return;
	if (access->ExportScript(S(dialog.GetPath())))
		wxMessageBox(_("Exported. Replace the building's <Access> section with the exported one to keep these changes."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
}

//
// shared lists
//

static void FillListKeepSelection(wxListBox *list, const std::vector<std::string> &items, const std::string &select)
{
	list->Freeze();
	list->Clear();
	int selection = -1;
	for (size_t i = 0; i < items.size(); i++)
	{
		list->Append(W(items[i]));
		if (SBS::AccessCore::Normalize(items[i]) == SBS::AccessCore::Normalize(select))
			selection = (int)i;
	}
	if (selection >= 0)
		list->SetSelection(selection);
	list->Thaw();
}

void AccessControlWindow::RefreshGroupChoices()
{
	if (!access)
		return;

	//card editor: access level checklist (keep ticks by name)
	std::vector<std::string> checked;
	for (unsigned int i = 0; i < clGroups->GetCount(); i++)
	{
		if (clGroups->IsChecked(i))
			checked.push_back(SBS::AccessCore::Normalize(S(clGroups->GetString(i))));
	}
	std::vector<std::string> groups = access->GetCore()->GetGroupNames();
	clGroups->Clear();
	for (size_t i = 0; i < groups.size(); i++)
	{
		clGroups->Append(W(groups[i]));
		if (std::find(checked.begin(), checked.end(), SBS::AccessCore::Normalize(groups[i])) != checked.end())
			clGroups->Check((unsigned int)i, true);
	}

	//rule editor: time profile choice
	wxString current = cRuleSchedule->GetStringSelection();
	std::vector<std::string> schedules = access->GetCore()->GetScheduleNames();
	cRuleSchedule->Clear();
	for (size_t i = 0; i < schedules.size(); i++)
		cRuleSchedule->Append(W(schedules[i]));
	if (!current.empty() && cRuleSchedule->FindString(current) != wxNOT_FOUND)
		cRuleSchedule->SetStringSelection(current);
	else if (cRuleSchedule->FindString(_("Always")) != wxNOT_FOUND)
		cRuleSchedule->SetStringSelection(_("Always"));

	//places for rules and access checks (keeping whatever is typed in)
	std::vector<std::string> places = access->GetPlaceNames();
	wxComboBox *boxes[2] = { cPlace, cCheckPlace };
	for (int b = 0; b < 2; b++)
	{
		wxString typed = boxes[b]->GetValue();
		boxes[b]->Clear();
		for (size_t i = 0; i < places.size(); i++)
			boxes[b]->Append(W(places[i]));
		boxes[b]->ChangeValue(typed);
	}

	//access check: cards
	wxString card = cCheckCard->GetStringSelection();
	std::vector<const SBS::Credential*> cards = access->GetCore()->GetCredentials();
	cCheckCard->Clear();
	for (size_t i = 0; i < cards.size(); i++)
		cCheckCard->Append(W(SBS::ToString(cards[i]->facility_code) + ":" + SBS::ToString(cards[i]->card_number) + " " + cards[i]->holder));
	if (!card.empty() && cCheckCard->FindString(card) != wxNOT_FOUND)
		cCheckCard->SetStringSelection(card);
}

//
// time profiles page
//

std::string AccessControlWindow::SelectedSchedule()
{
	int selection = ScheduleList->GetSelection();
	if (selection < 0)
		return "";
	return S(ScheduleList->GetString(selection));
}

void AccessControlWindow::RefreshProfiles()
{
	if (!access)
		return;
	FillListKeepSelection(ScheduleList, access->GetCore()->GetScheduleNames(), SelectedSchedule());
	RefreshWindows();
}

void AccessControlWindow::RefreshWindows()
{
	WindowList->Clear();
	if (!access)
		return;

	std::string name = SelectedSchedule();
	const SBS::Schedule *schedule = name.empty() ? 0 : access->GetCore()->GetSchedule(name);
	if (!schedule)
		return;

	if (SBS::AccessCore::Normalize(name) == "always")
		WindowList->Append(_("(built in: every day, all day)"));
	else if (SBS::AccessCore::Normalize(name) == "never")
		WindowList->Append(_("(built in: never)"));
	else if (schedule->windows.empty())
		WindowList->Append(_("(no windows yet: add one below; until then this profile never allows access)"));

	for (size_t i = 0; i < schedule->windows.size(); i++)
	{
		const SBS::TimeWindow &w = schedule->windows[i];
		if (SBS::AccessCore::Normalize(name) == "always")
			break;
		std::string end = (w.end_minute == 1440) ? "24:00" : SBS::AccessCore::FormatClock(w.end_minute);
		std::string overnight = (w.end_minute < w.start_minute) ? "  (overnight)" : "";
		WindowList->Append(W(SBS::AccessCore::FormatDays(w.days_mask) + "   " + SBS::AccessCore::FormatClock(w.start_minute) + " to " + end + overnight));
	}
}

void AccessControlWindow::OnScheduleSelected(wxCommandEvent &event)
{
	RefreshWindows();
}

void AccessControlWindow::OnNewSchedule(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = Trim(S(tScheduleName->GetValue()));
	if (name.empty() || name.find_first_of(",|") != std::string::npos)
	{
		wxMessageBox(_("Enter a name for the new time profile (no commas or | characters)."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (access->GetCore()->GetSchedule(name))
	{
		wxMessageBox(_("A time profile with that name already exists."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	//a profile starts with the time window set below it, so it's useful straight away
	tScheduleName->Clear();
	ScheduleList->Append(W(name));
	ScheduleList->SetStringSelection(W(name));
	OnAddWindow(event);
	if (!access->GetCore()->GetSchedule(name))
		ScheduleList->Delete(ScheduleList->GetSelection()); //window was invalid; nothing was created
}

void AccessControlWindow::OnDeleteSchedule(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = SelectedSchedule();
	if (name.empty())
		return;
	std::string error;
	if (access->GetCore()->RemoveSchedule(name, error) == false)
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_INFORMATION, this);
	else
		access->LogEvent("INFO", "Time Profiles", -1, -1, "Deleted " + name);
}

void AccessControlWindow::OnAddWindow(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = SelectedSchedule();
	if (name.empty())
	{
		wxMessageBox(_("Select a time profile first, or create one."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (SBS::AccessCore::IsBuiltInSchedule(name))
	{
		wxMessageBox(_("The built-in profiles Always and Never can't be changed. Create a new profile instead."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	int mask = 0;
	for (int d = 0; d < 7; d++)
	{
		if (chkDay[d]->GetValue())
			mask |= (1 << d);
	}
	int start = 0, end = 0;
	if (mask == 0)
	{
		wxMessageBox(_("Tick at least one day."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (!SBS::AccessCore::ParseClock(S(tStart->GetValue()), start) || start > 1439 || !SBS::AccessCore::ParseClock(S(tEnd->GetValue()), end))
	{
		wxMessageBox(_("Enter the start and end times as HH:MM on the 24-hour clock, such as 07:00 and 19:00."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (start == end)
	{
		wxMessageBox(_("The start and end times are the same, so this window would never allow access."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	std::string error;
	if (access->GetCore()->AddScheduleWindow(name, mask, start, end, error) == false)
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
	else
		access->LogEvent("INFO", "Time Profiles", -1, -1, name + ": added " + SBS::AccessCore::FormatWindow(access->GetCore()->GetSchedule(name)->windows.back()));
}

void AccessControlWindow::OnRemoveWindow(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = SelectedSchedule();
	const SBS::Schedule *schedule = name.empty() ? 0 : access->GetCore()->GetSchedule(name);
	int index = WindowList->GetSelection();
	if (!schedule || index < 0 || schedule->windows.empty())
		return;

	std::string error;
	if (access->GetCore()->RemoveScheduleWindow(name, index, error) == false)
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_INFORMATION, this);
}

//
// access levels page
//

std::string AccessControlWindow::SelectedGroup()
{
	int selection = GroupList->GetSelection();
	if (selection < 0)
		return "";
	return S(GroupList->GetString(selection));
}

void AccessControlWindow::RefreshLevels()
{
	if (!access)
		return;
	FillListKeepSelection(GroupList, access->GetCore()->GetGroupNames(), SelectedGroup());
	RefreshRules();
}

void AccessControlWindow::RefreshRules()
{
	RuleList->Clear();
	if (!access)
		return;

	std::string name = SelectedGroup();
	const SBS::AccessGroup *group = name.empty() ? 0 : access->GetCore()->GetGroup(name);
	chkOverride->SetValue(group ? group->override_lockdown : false);
	chkOverride->Enable(group != 0);
	if (!group)
		return;

	if (group->override_lockdown == true)
		RuleList->Append(_("(override: this level opens everything, any time, so rules don't matter)"));
	for (size_t i = 0; i < group->rules.size(); i++)
		RuleList->Append(W(group->rules[i].display + "  -  " + group->rules[i].schedule));
}

void AccessControlWindow::OnGroupSelected(wxCommandEvent &event)
{
	RefreshRules();
}

void AccessControlWindow::OnNewGroup(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = Trim(S(tGroupName->GetValue()));
	if (name.empty() || name.find_first_of(",|") != std::string::npos)
	{
		wxMessageBox(_("Enter a name for the new access level (no commas or | characters)."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (access->GetCore()->GetGroup(name))
	{
		wxMessageBox(_("An access level with that name already exists."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	std::string error;
	if (access->GetCore()->AddGroup(name, false, error) == false)
	{
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
		return;
	}
	tGroupName->Clear();
	access->LogEvent("INFO", "Access Levels", -1, -1, "Created " + name);
	RefreshLevels();
	GroupList->SetStringSelection(W(name));
	RefreshRules();
}

void AccessControlWindow::OnDeleteGroup(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = SelectedGroup();
	if (name.empty())
		return;
	if (wxMessageBox(W("Delete the access level " + name + "? Cards that have it will lose it."), _("Access Control"), wxYES_NO | wxICON_QUESTION, this) != wxYES)
		return;
	std::string error;
	if (access->GetCore()->RemoveGroup(name, error) == false)
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
	else
		access->LogEvent("INFO", "Access Levels", -1, -1, "Deleted " + name);
}

void AccessControlWindow::OnOverride(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string name = SelectedGroup();
	if (name.empty())
		return;
	access->GetCore()->SetGroupOverride(name, chkOverride->GetValue());
	access->LogEvent("INFO", "Access Levels", -1, -1, name + (chkOverride->GetValue() ? ": override on" : ": override off"));
}

void AccessControlWindow::OnAddRule(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string group = SelectedGroup();
	std::string place = Trim(S(cPlace->GetValue()));
	std::string schedule = S(cRuleSchedule->GetStringSelection());
	if (group.empty())
	{
		wxMessageBox(_("Select an access level first, or create one."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (place.empty() || schedule.empty())
	{
		wxMessageBox(_("Choose a place and a time profile."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	if (place.find(',') != std::string::npos)
	{
		wxMessageBox(_("Places can't contain commas. Separate floor lists with |, such as Elevator 1:Floors 6-9|R."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	std::string error;
	if (access->GetCore()->AddGroupRule(group, place, schedule, error) == false)
	{
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
		return;
	}

	//a rule for a new door name creates its access point, ready for doors and readers
	int elevator = 0;
	std::string spec;
	if (SBS::AccessCore::ParseElevatorRule(place, elevator, spec) == false)
		access->GetAccessPoint(place, true);

	access->LogEvent("INFO", "Access Levels", -1, -1, group + ": added " + place + " - " + schedule);
}

void AccessControlWindow::OnRemoveRule(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string group = SelectedGroup();
	const SBS::AccessGroup *g = group.empty() ? 0 : access->GetCore()->GetGroup(group);
	int index = RuleList->GetSelection();
	if (!g || index < 0)
		return;
	if (g->override_lockdown == true)
		index--; //skip the override note line
	if (index < 0 || index >= (int)g->rules.size())
		return;

	std::string removed = g->rules[index].display + " - " + g->rules[index].schedule;
	std::string error;
	if (access->GetCore()->RemoveGroupRule(group, index, error) == false)
		wxMessageBox(W(error), _("Access Control"), wxOK | wxICON_WARNING, this);
	else
		access->LogEvent("INFO", "Access Levels", -1, -1, group + ": removed " + removed);
}

void AccessControlWindow::OnCheck(wxCommandEvent &event)
{
	if (!access)
		return;
	std::string card = S(cCheckCard->GetStringSelection());
	std::string place = Trim(S(cCheckPlace->GetValue()));
	int facility = 0, number = 0;
	size_t colon = card.find(':');
	size_t space = card.find(' ');
	if (colon == std::string::npos || place.empty())
	{
		lblCheck->SetLabel(_("Choose a card and a place."));
		return;
	}
	facility = atoi(card.substr(0, colon).c_str());
	number = atoi(card.substr(colon + 1, space == std::string::npos ? std::string::npos : space - colon - 1).c_str());

	std::string detail;
	SBS::AccessResult result = access->CheckAccess(facility, number, place, detail);

	int day = 0, minute = 0;
	access->GetClock(day, minute);
	static const char *days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	std::string when = std::string(days[day % 7]) + " " + SBS::AccessCore::FormatClock(minute);

	std::string outcome = (result == SBS::AccessResult::Granted) ? "ALLOWED" : std::string("REFUSED (") + SBS::AccessResultName(result) + ")";
	lblCheck->SetLabel(W(outcome + " at " + when + (detail.empty() ? "" : " - " + detail)));
}

//
// status page
//

static SBS::AccessPoint* SelectedPoint(SBS::AccessController *access, wxListBox *list)
{
	if (!access)
		return 0;
	int selected = list->GetSelection();
	if (selected < 0)
		return 0;
	return access->GetAccessPoint(selected);
}

void AccessControlWindow::OnGrant(wxCommandEvent &event)
{
	SBS::AccessPoint *point = SelectedPoint(access, PointList);
	if (!point)
		return;
	access->LogEvent("GRANTED", point->GetName(), -1, -1, "Manual (Access Control window)");
	point->Grant();
}

void AccessControlWindow::OnHoldUnlock(wxCommandEvent &event)
{
	SBS::AccessPoint *point = SelectedPoint(access, PointList);
	if (point)
		point->SetHeldUnlocked(true);
}

void AccessControlWindow::OnNormal(wxCommandEvent &event)
{
	SBS::AccessPoint *point = SelectedPoint(access, PointList);
	if (point)
		point->SetHeldUnlocked(false);
}

void AccessControlWindow::OnResetZone(wxCommandEvent &event)
{
	if (!access)
		return;
	int selected = ZoneList->GetSelection();
	std::vector<std::string> names = access->GetZoneNames();
	if (selected < 0 || selected >= (int)names.size())
	{
		wxMessageBox(_("Select a zone first."), _("Access Control"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	access->ResetZone(names[selected]);
}

void AccessControlWindow::OnLockdown(wxCommandEvent &event)
{
	if (access)
		access->Lockdown(chkLockdown->GetValue());
}

void AccessControlWindow::OnSetAutoClose(wxCommandEvent &event)
{
	if (!access)
		return;
	Real seconds = 0;
	if (!SBS::IsNumeric(Trim(S(tAutoClose->GetValue())), seconds) || seconds < 0)
	{
		wxMessageBox(_("Enter the auto-close time in seconds (0 turns it off)."), _("Access Control"), wxOK | wxICON_WARNING, this);
		return;
	}
	access->SetDefaultAutoClose((int)(seconds * 1000));
	access->LogEvent("INFO", "Doors", -1, -1, "AutoClose " + SBS::ToString(seconds) + " s");
}

//
// log page
//

void AccessControlWindow::OnFilter(wxCommandEvent &event)
{
	RefreshLog(true);
}

void AccessControlWindow::OnExportLog(wxCommandEvent &event)
{
	if (!access)
		return;
	wxFileDialog dialog(this, _("Export access log"), wxEmptyString, _("access_log.csv"), _("CSV files (*.csv)|*.csv|All files (*.*)|*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dialog.ShowModal() != wxID_OK)
		return;
	access->ExportLog(S(dialog.GetPath()));
}

//
// badge reader input page
//

void AccessControlWindow::OnReaderInput(wxCommandEvent &event)
{
	if (!access)
		return;

	std::string input = Trim(S(tReaderInput->GetValue()));
	tReaderInput->Clear();
	if (input.empty())
		return;

	std::string target = Trim(S(cTarget->GetValue()));
	if (target.empty())
	{
		lblReaderResult->SetLabel(_("Choose where to present the badge first."));
		return;
	}

	int facility = -1, number = -1;
	int format = rFormat->GetSelection();
	bool ok = false;

	if (format == 0)
	{
		//facility:card (also accepts a comma or space)
		size_t sep = input.find_first_of(":, ");
		if (sep != std::string::npos)
			ok = SBS::IsNumeric(Trim(input.substr(0, sep)), facility) && SBS::IsNumeric(Trim(input.substr(sep + 1)), number);
	}
	else if (format == 1)
	{
		ok = SBS::IsNumeric(input, number) && SBS::IsNumeric(Trim(S(tDefaultFacility->GetValue())), facility);
	}
	else
	{
		//raw 26-bit Wiegand value
		char *end = 0;
		unsigned long raw = strtoul(input.c_str(), &end, format == 3 ? 16 : 10);
		if (end && *end == 0)
			ok = SBS::AccessCore::DecodeH10301((uint32_t)raw, facility, number);
		if (!ok)
		{
			lblReaderResult->SetLabel(W("Couldn't read '" + input + "' as a 26-bit badge (bad parity or too many bits). Try another format."));
			return;
		}
	}

	if (!ok || facility < 0 || number < 0)
	{
		lblReaderResult->SetLabel(W("Couldn't read '" + input + "' in the selected format."));
		return;
	}

	SBS::AccessResult result = access->Present(target, facility, number);
	const SBS::Credential *c = access->GetCore()->GetCredential(facility, number);
	std::string who = c ? " (" + c->holder + ")" : " (not enrolled)";
	std::string outcome = (result == SBS::AccessResult::Granted) ? "GRANTED" : std::string("DENIED - ") + SBS::AccessResultName(result);
	lblReaderResult->SetLabel(W("Badge " + SBS::ToString(facility) + ":" + SBS::ToString(number) + who + " at " + target + ": " + outcome));
}

void AccessControlWindow::OnClose(wxCloseEvent &event)
{
	Hide();
}

}
