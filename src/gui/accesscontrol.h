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

#ifndef ACCESSCONTROL_H
#define ACCESSCONTROL_H

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/choice.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace Skyscraper {

class DebugPanel;

//Access Control window: manage cards and the badge wallet, watch door, elevator and zone
//status, read the live event log, and accept input from a USB badge reader
class AccessControlWindow : public wxDialog
{
public:
	AccessControlWindow(DebugPanel *parent, wxWindowID id = wxID_ANY);
	virtual ~AccessControlWindow();
	void Loop();

private:
	DebugPanel *panel;
	::SBS::SBS *Simcore;
	::SBS::AccessController *access;
	int refresh_counter;
	unsigned long last_log_total;
	unsigned long last_version;

	wxStaticText *lblStatus;
	wxNotebook *book;

	//cards page
	wxListCtrl *CardList;
	wxTextCtrl *tFacility, *tNumber, *tHolder, *tHome;
	wxCheckListBox *clGroups;
	wxButton *bSaveCard, *bEnable, *bDisable, *bWalletAdd, *bWalletRemove, *bActive, *bExportCards;

	//status page
	wxListBox *PointList, *ElevatorList, *ZoneList;
	wxButton *bGrant, *bHoldUnlock, *bNormal, *bResetZone, *bSetAutoClose;
	wxCheckBox *chkLockdown;
	wxTextCtrl *tAutoClose;

	//time profiles page
	wxListBox *ScheduleList, *WindowList;
	wxTextCtrl *tScheduleName, *tStart, *tEnd;
	wxCheckBox *chkDay[7];
	wxButton *bNewSchedule, *bDeleteSchedule, *bAddWindow, *bRemoveWindow;

	//access levels page
	wxListBox *GroupList, *RuleList;
	wxTextCtrl *tGroupName;
	wxCheckBox *chkOverride;
	wxComboBox *cPlace, *cCheckPlace;
	wxChoice *cRuleSchedule, *cCheckCard;
	wxButton *bNewGroup, *bDeleteGroup, *bAddRule, *bRemoveRule, *bCheck;
	wxStaticText *lblCheck;

	//log page
	wxListCtrl *LogList;
	wxTextCtrl *tFilter;
	wxButton *bExportLog;

	//reader input page
	wxComboBox *cTarget;
	wxRadioBox *rFormat;
	wxTextCtrl *tDefaultFacility, *tReaderInput;
	wxStaticText *lblReaderResult;

	wxPanel* BuildCardsPage(wxNotebook *parent);
	wxPanel* BuildProfilesPage(wxNotebook *parent);
	wxPanel* BuildLevelsPage(wxNotebook *parent);
	wxPanel* BuildStatusPage(wxNotebook *parent);
	wxPanel* BuildLogPage(wxNotebook *parent);
	wxPanel* BuildReaderPage(wxNotebook *parent);

	void RefreshCards();
	void RefreshStatus();
	void RefreshProfiles();
	void RefreshWindows();
	void RefreshLevels();
	void RefreshRules();
	void RefreshGroupChoices();
	std::string SelectedSchedule();
	std::string SelectedGroup();
	void RefreshLog(bool rebuild);
	void RefreshTargets();
	void EnableControls(bool value);
	bool GetSelectedCard(int &facility, int &number);
	bool ReadCardFields(int &facility, int &number);

	void OnCardSelected(wxListEvent &event);
	void OnSaveCard(wxCommandEvent &event);
	void OnEnable(wxCommandEvent &event);
	void OnDisable(wxCommandEvent &event);
	void OnWalletAdd(wxCommandEvent &event);
	void OnWalletRemove(wxCommandEvent &event);
	void OnActive(wxCommandEvent &event);
	void OnExportCards(wxCommandEvent &event);
	void OnScheduleSelected(wxCommandEvent &event);
	void OnNewSchedule(wxCommandEvent &event);
	void OnDeleteSchedule(wxCommandEvent &event);
	void OnAddWindow(wxCommandEvent &event);
	void OnRemoveWindow(wxCommandEvent &event);
	void OnGroupSelected(wxCommandEvent &event);
	void OnNewGroup(wxCommandEvent &event);
	void OnDeleteGroup(wxCommandEvent &event);
	void OnOverride(wxCommandEvent &event);
	void OnAddRule(wxCommandEvent &event);
	void OnRemoveRule(wxCommandEvent &event);
	void OnCheck(wxCommandEvent &event);
	void OnGrant(wxCommandEvent &event);
	void OnHoldUnlock(wxCommandEvent &event);
	void OnNormal(wxCommandEvent &event);
	void OnResetZone(wxCommandEvent &event);
	void OnLockdown(wxCommandEvent &event);
	void OnSetAutoClose(wxCommandEvent &event);
	void OnFilter(wxCommandEvent &event);
	void OnExportLog(wxCommandEvent &event);
	void OnReaderInput(wxCommandEvent &event);
	void OnClose(wxCloseEvent &event);
};

}

#endif
