/*
	Skyscraper 1.12 Alpha - Sound Manager Dialog
	Copyright (C)2003-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

//(*InternalHeaders(SoundManager)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "soundsystem.h"
#include "sound.h"
#include "soundmanager.h"

namespace Skyscraper {

//(*IdInit(SoundManager)
const long SoundManager::ID_STATICTEXT1 = wxNewId();
const long SoundManager::ID_SoundList = wxNewId();
const long SoundManager::ID_STATICTEXT2 = wxNewId();
const long SoundManager::ID_HandleList = wxNewId();
const long SoundManager::ID_STATICTEXT4 = wxNewId();
const long SoundManager::ID_tName = wxNewId();
const long SoundManager::ID_STATICTEXT3 = wxNewId();
const long SoundManager::ID_tParent = wxNewId();
const long SoundManager::ID_STATICTEXT5 = wxNewId();
const long SoundManager::ID_tChannels = wxNewId();
const long SoundManager::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(SoundManager,wxDialog)
	//(*EventTable(SoundManager)
	//*)
END_EVENT_TABLE()

SoundManager::SoundManager(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SoundManager)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer3;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer5->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SoundList = new wxListBox(this, ID_SoundList, wxDefaultPosition, wxSize(200,300), 0, 0, 0, wxDefaultValidator, _T("ID_SoundList"));
	FlexGridSizer5->Add(SoundList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Sound Objects (handles)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer6->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	HandleList = new wxListBox(this, ID_HandleList, wxDefaultPosition, wxSize(300,300), 0, 0, 0, wxDefaultValidator, _T("ID_HandleList"));
	FlexGridSizer6->Add(HandleList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tName"));
	FlexGridSizer4->Add(tName, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParent = new wxTextCtrl(this, ID_tParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tParent"));
	FlexGridSizer4->Add(tParent, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Channels:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tChannels = new wxTextCtrl(this, ID_tChannels, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_tChannels"));
	FlexGridSizer4->Add(tChannels, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer3->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer1->Add(bOK, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bOK_Click);
	//*)
	panel = parent;
	Simcore = 0;
	sound = 0;
	handle = 0;
	lastcount = 0;
	lasthandlecount = 0;
}

SoundManager::~SoundManager()
{
	//(*Destroy(SoundManager)
	//*)
}

void SoundManager::Loop()
{
	bool reset = false;

	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildSoundList(true);
	BuildHandleList(true);

	int selection = SoundList->GetSelection();

	if (selection >= 0)
	{
		SBS::SoundData *newsound = Simcore->GetSoundSystem()->GetSoundData(selection);

		//if a new sound has been selected, update values
		if (newsound && sound != newsound)
		{
			sound = newsound;
			handle = 0;
			HandleList->Clear();

			tChannels->SetValue(SBS::ToString(sound->GetChannelCount()));
			reset = true;
		}
	}
	else
	{
		sound = 0;
		handle = 0;
	}

	if (!sound)
		return;

	int handleselect = HandleList->GetSelection();

	if (handleselect >= 0)
	{
		SBS::Sound *newhandle = sound->handles[handleselect];

		//if a new handle has been selected, update values
		if (newhandle && handle != newhandle)
		{
			handle = sound->handles[handleselect];

			tName->SetValue(handle->GetName());
			tParent->SetValue(handle->GetParent()->GetName());
			reset = true;
		}
	}
	else
		handle = 0;

	//set dynamic values
}

void SoundManager::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void SoundManager::BuildSoundList(bool restore_selection)
{
	int count = Simcore->GetSoundSystem()->GetSoundCount();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = SoundList->GetSelection();
		SoundList->Clear();

		for (int i = 0; i < count; i++)
			SoundList->Append(SBS::ToString(i + 1) + wxT(": ") + Simcore->GetSoundSystem()->GetSoundData(i)->filename);

		if (count > 0)
		{
			if (restore_selection == false)
				SoundList->SetSelection(0);
			else if (old_selection < SoundList->GetCount())
				SoundList->SetSelection(old_selection);
			else
				SoundList->SetSelection(0);
		}
		else
		{
			//clear values
			tChannels->Clear();
			tName->Clear();
			tParent->Clear();
		}
	}
}

void SoundManager::BuildHandleList(bool restore_selection)
{
	if (!sound)
		return;

	int count = sound->GetHandleCount();

	if (count != lasthandlecount || handle == 0)
	{
		lasthandlecount = count;
		int old_selection = HandleList->GetSelection();
		HandleList->Clear();

		for (int i = 0; i < count; i++)
			HandleList->Append(SBS::ToString(i + 1) + wxT(": ") + sound->handles[i]->GetName());

		if (count > 0)
		{
			if (restore_selection == false)
				HandleList->SetSelection(0);
			else if (old_selection < HandleList->GetCount())
				HandleList->SetSelection(old_selection);
			else
				HandleList->SetSelection(0);
		}
		else
		{
			//clear values
			tName->Clear();
			tParent->Clear();
		}
	}
}

}
