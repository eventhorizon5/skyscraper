/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Sky Control Dialog
	Copyright (C)2003-2018 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

//(*InternalHeaders(SkyControl)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include <wx/object.h>

#include "Caelum.h"
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "skycontrol.h"

namespace Skyscraper {

//(*IdInit(SkyControl)
const long SkyControl::ID_STATICTEXT1 = wxNewId();
const long SkyControl::ID_tLatitude = wxNewId();
const long SkyControl::ID_bSetLatitude = wxNewId();
const long SkyControl::ID_STATICTEXT2 = wxNewId();
const long SkyControl::ID_tLongitude = wxNewId();
const long SkyControl::ID_bSetLongitude = wxNewId();
const long SkyControl::ID_STATICTEXT11 = wxNewId();
const long SkyControl::ID_tJulian = wxNewId();
const long SkyControl::ID_STATICLINE1 = wxNewId();
const long SkyControl::ID_STATICLINE2 = wxNewId();
const long SkyControl::ID_STATICTEXT3 = wxNewId();
const long SkyControl::ID_tYear = wxNewId();
const long SkyControl::ID_STATICTEXT4 = wxNewId();
const long SkyControl::ID_tMonth = wxNewId();
const long SkyControl::ID_STATICTEXT5 = wxNewId();
const long SkyControl::ID_tDay = wxNewId();
const long SkyControl::ID_STATICTEXT6 = wxNewId();
const long SkyControl::ID_tHour = wxNewId();
const long SkyControl::ID_STATICTEXT7 = wxNewId();
const long SkyControl::ID_tMinute = wxNewId();
const long SkyControl::ID_STATICTEXT8 = wxNewId();
const long SkyControl::ID_tSecond = wxNewId();
const long SkyControl::ID_bSet = wxNewId();
const long SkyControl::ID_bGetSystemTime = wxNewId();
const long SkyControl::ID_STATICTEXT9 = wxNewId();
const long SkyControl::ID_tMultiplier = wxNewId();
const long SkyControl::ID_bSetMultiplier = wxNewId();
//*)

BEGIN_EVENT_TABLE(SkyControl,wxDialog)
	//(*EventTable(SkyControl)
	//*)
END_EVENT_TABLE()

SkyControl::SkyControl(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SkyControl)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer5;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Sky Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Observer Location"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Latitude:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tLatitude = new wxTextCtrl(this, ID_tLatitude, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tLatitude"));
	FlexGridSizer2->Add(tLatitude, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLatitude = new wxButton(this, ID_bSetLatitude, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLatitude"));
	FlexGridSizer2->Add(bSetLatitude, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Longitude:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tLongitude = new wxTextCtrl(this, ID_tLongitude, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tLongitude"));
	FlexGridSizer2->Add(tLongitude, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLongitude = new wxButton(this, ID_bSetLongitude, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetLongitude"));
	FlexGridSizer2->Add(bSetLongitude, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Date and Time"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Julian:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer4->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tJulian = new wxTextCtrl(this, ID_tJulian, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tJulian"));
	FlexGridSizer4->Add(tJulian, 1, wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer4->Add(StaticLine1, 1, wxBOTTOM|wxEXPAND, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer4->Add(StaticLine2, 1, wxBOTTOM|wxEXPAND, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Year:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tYear = new wxTextCtrl(this, ID_tYear, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tYear"));
	FlexGridSizer4->Add(tYear, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Month:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMonth = new wxTextCtrl(this, ID_tMonth, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tMonth"));
	FlexGridSizer4->Add(tMonth, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Day:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tDay = new wxTextCtrl(this, ID_tDay, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tDay"));
	FlexGridSizer4->Add(tDay, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Hour:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer4->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tHour = new wxTextCtrl(this, ID_tHour, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tHour"));
	FlexGridSizer4->Add(tHour, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Minute:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer4->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMinute = new wxTextCtrl(this, ID_tMinute, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tMinute"));
	FlexGridSizer4->Add(tMinute, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Second:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer4->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tSecond = new wxTextCtrl(this, ID_tSecond, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tSecond"));
	FlexGridSizer4->Add(tSecond, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer4, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSet = new wxToggleButton(this, ID_bSet, _("Set"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSet"));
	FlexGridSizer5->Add(bSet, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bGetSystemTime = new wxButton(this, ID_bGetSystemTime, _("Get System Time"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bGetSystemTime"));
	FlexGridSizer5->Add(bGetSystemTime, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Misc Options"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Time Scale:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer6->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMultiplier = new wxTextCtrl(this, ID_tMultiplier, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_tMultiplier"));
	FlexGridSizer6->Add(tMultiplier, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetMultiplier = new wxButton(this, ID_bSetMultiplier, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSetMultiplier"));
	FlexGridSizer6->Add(bSetMultiplier, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_bSetLatitude,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkyControl::On_bSetLatitude_Click);
	Connect(ID_bSetLongitude,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkyControl::On_bSetLongitude_Click);
	Connect(ID_bSet,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&SkyControl::On_bSet_Toggle);
	Connect(ID_bGetSystemTime,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkyControl::On_bGetSystemTime_Click);
	Connect(ID_bSetMultiplier,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SkyControl::On_bSetMultiplier_Click);
	//*)
	panel = parent;
	system = 0;
	OnInit();
}

SkyControl::~SkyControl()
{
	//(*Destroy(SkyControl)
	//*)
}


void SkyControl::OnInit()
{
	system = panel->GetRoot()->GetCaelumSystem();

	if (!system)
		return;

	tLatitude->SetValue(TruncateNumber(Ogre::Degree(system->getObserverLatitude()).valueDegrees(), 4));
	tLongitude->SetValue(TruncateNumber(Ogre::Degree(system->getObserverLongitude()).valueDegrees(), 4));
	tMultiplier->SetValue(TruncateNumber(system->getTimeScale(), 4));
}

void SkyControl::Loop()
{
	if (system != panel->GetRoot()->GetCaelumSystem())
		OnInit();

	if (!system)
		return;

	Caelum::LongReal julian = system->getJulianDay(), second;
	int year, month, day, hour, minute;
	Caelum::Astronomy::getGregorianDateTimeFromJulianDay(julian, year, month, day, hour, minute, second);
	second = (int)second;

	if (bSet->GetValue() == false)
	{
		tJulian->SetValue(TruncateNumber(system->getJulianDay(), 4));
		tYear->SetValue(SBS::ToString(year));
		tMonth->SetValue(SBS::ToString(month));
		tDay->SetValue(SBS::ToString(day));
		tHour->SetValue(SBS::ToString(hour));
		tMinute->SetValue(SBS::ToString(minute));
		tSecond->SetValue(SBS::ToString(second));
	}
}

void SkyControl::On_bSetLatitude_Click(wxCommandEvent& event)
{
	if (system)
		system->setObserverLatitude(Ogre::Degree(atof(tLatitude->GetValue())));
}

void SkyControl::On_bSetLongitude_Click(wxCommandEvent& event)
{
	if (system)
		system->setObserverLongitude(Ogre::Degree(atof(tLongitude->GetValue())));
}

void SkyControl::On_bSet_Toggle(wxCommandEvent& event)
{
	if (!system)
		return;

	static wxString prev_julian;

	if (bSet->GetValue() == false)
	{
		wxString julian = tJulian->GetValue();
		bool julian_changed = false;

		if (julian != prev_julian)
		{
			prev_julian = julian;
			julian_changed = true;
		}

		if (julian_changed == true)
		{
			system->setJulianDay(atof(julian));
			julian_changed = false;
		}
		else
		{
			int year = atoi(tYear->GetValue());
			int month = atoi(tMonth->GetValue());
			int day = atoi(tDay->GetValue());
			int hour = atoi(tHour->GetValue());
			int minute = atoi(tMinute->GetValue());
			double second = atof(tSecond->GetValue());
			system->getUniversalClock()->setGregorianDateTime(year, month, day, hour, minute, second);
		}

		//disable text boxes
		tJulian->SetEditable(false);
		tYear->SetEditable(false);
		tMonth->SetEditable(false);
		tDay->SetEditable(false);
		tHour->SetEditable(false);
		tMinute->SetEditable(false);
		tSecond->SetEditable(false);
	}
	else
	{
		prev_julian = tJulian->GetValue();

		//enable text boxes
		tJulian->SetEditable(true);
		tYear->SetEditable(true);
		tMonth->SetEditable(true);
		tDay->SetEditable(true);
		tHour->SetEditable(true);
		tMinute->SetEditable(true);
		tSecond->SetEditable(true);
	}
}

void SkyControl::On_bSetMultiplier_Click(wxCommandEvent& event)
{
	panel->GetRoot()->SkyMult = atof(tMultiplier->GetValue());
}

void SkyControl::On_bGetSystemTime_Click(wxCommandEvent& event)
{
	panel->GetRoot()->SetDateTimeNow();
}

}

