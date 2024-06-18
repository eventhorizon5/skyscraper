/*
	Skyscraper 1.12 Alpha - Sound Manager Dialog
	Copyright (C)2003-2024 Ryan Thoryk
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
const long SoundManager::ID_lblStats = wxNewId();
const long SoundManager::ID_STATICTEXT4 = wxNewId();
const long SoundManager::ID_tName = wxNewId();
const long SoundManager::ID_STATICTEXT3 = wxNewId();
const long SoundManager::ID_tParent = wxNewId();
const long SoundManager::ID_STATICTEXT5 = wxNewId();
const long SoundManager::ID_tLength = wxNewId();
const long SoundManager::ID_STATICTEXT6 = wxNewId();
const long SoundManager::ID_tPlaying = wxNewId();
const long SoundManager::ID_STATICTEXT7 = wxNewId();
const long SoundManager::ID_tPosition = wxNewId();
const long SoundManager::ID_STATICTEXT8 = wxNewId();
const long SoundManager::ID_tVolume = wxNewId();
const long SoundManager::ID_bSetVolume = wxNewId();
const long SoundManager::ID_STATICTEXT9 = wxNewId();
const long SoundManager::ID_tLoop = wxNewId();
const long SoundManager::ID_bSetLoop = wxNewId();
const long SoundManager::ID_STATICTEXT10 = wxNewId();
const long SoundManager::ID_tPaused = wxNewId();
const long SoundManager::ID_bSetPause = wxNewId();
const long SoundManager::ID_STATICTEXT11 = wxNewId();
const long SoundManager::ID_tSpeed = wxNewId();
const long SoundManager::ID_bSetSpeed = wxNewId();
const long SoundManager::ID_STATICTEXT12 = wxNewId();
const long SoundManager::ID_tPlayPosition = wxNewId();
const long SoundManager::ID_bPlay = wxNewId();
const long SoundManager::ID_bStop = wxNewId();
const long SoundManager::ID_bReset = wxNewId();
const long SoundManager::ID_bUnload = wxNewId();
const long SoundManager::ID_bCleanup = wxNewId();
const long SoundManager::ID_bOK = wxNewId();
const long SoundManager::ID_bListPlaying = wxNewId();
//*)

BEGIN_EVENT_TABLE(SoundManager,wxDialog)
	//(*EventTable(SoundManager)
	//*)
END_EVENT_TABLE()

SoundManager::SoundManager(DebugPanel* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SoundManager)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;

	Create(parent, wxID_ANY, _("Sound Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer5->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SoundList = new wxListBox(this, ID_SoundList, wxDefaultPosition, wxSize(200,300), 0, 0, 0, wxDefaultValidator, _T("ID_SoundList"));
	FlexGridSizer5->Add(SoundList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Sound Objects (handles)"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer6->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	HandleList = new wxListBox(this, ID_HandleList, wxDefaultPosition, wxSize(250,300), 0, 0, 0, wxDefaultValidator, _T("ID_HandleList"));
	FlexGridSizer6->Add(HandleList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	lblStats = new wxStaticText(this, ID_lblStats, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblStats"));
	FlexGridSizer7->Add(lblStats, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2->Add(FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxSize(200,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tName"));
	FlexGridSizer4->Add(tName, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParent = new wxTextCtrl(this, ID_tParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tParent"));
	FlexGridSizer4->Add(tParent, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Length:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tLength = new wxTextCtrl(this, ID_tLength, wxEmptyString, wxDefaultPosition, wxSize(100,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tLength"));
	FlexGridSizer4->Add(tLength, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Is Playing:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer4->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPlaying = new wxTextCtrl(this, ID_tPlaying, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPlaying"));
	FlexGridSizer4->Add(tPlaying, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer4->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosition = new wxTextCtrl(this, ID_tPosition, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPosition"));
	FlexGridSizer4->Add(tPosition, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Volume:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer4->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tVolume = new wxTextCtrl(this, ID_tVolume, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tVolume"));
	FlexGridSizer4->Add(tVolume, 1, wxEXPAND, 5);
	bSetVolume = new wxButton(this, ID_bSetVolume, _("Set"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bSetVolume"));
	FlexGridSizer4->Add(bSetVolume, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Loop:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer4->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tLoop = new wxTextCtrl(this, ID_tLoop, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tLoop"));
	FlexGridSizer4->Add(tLoop, 1, wxEXPAND, 5);
	bSetLoop = new wxButton(this, ID_bSetLoop, _("Set"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bSetLoop"));
	FlexGridSizer4->Add(bSetLoop, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Paused:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer4->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPaused = new wxTextCtrl(this, ID_tPaused, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tPaused"));
	FlexGridSizer4->Add(tPaused, 1, wxEXPAND, 5);
	bSetPause = new wxButton(this, ID_bSetPause, _("Set"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bSetPause"));
	FlexGridSizer4->Add(bSetPause, 1, wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Speed:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer4->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tSpeed = new wxTextCtrl(this, ID_tSpeed, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tSpeed"));
	FlexGridSizer4->Add(tSpeed, 1, wxEXPAND, 5);
	bSetSpeed = new wxButton(this, ID_bSetSpeed, _("Set"), wxDefaultPosition, wxSize(50,-1), 0, wxDefaultValidator, _T("ID_bSetSpeed"));
	FlexGridSizer4->Add(bSetSpeed, 1, wxLEFT, 5);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Play Position:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer4->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPlayPosition = new wxTextCtrl(this, ID_tPlayPosition, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPlayPosition"));
	FlexGridSizer4->Add(tPlayPosition, 1, wxEXPAND, 5);
	FlexGridSizer4->Add(-1,-1,1, wxALL|wxEXPAND, 5);
	FlexGridSizer3->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bPlay = new wxButton(this, ID_bPlay, _("Play"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bPlay"));
	BoxSizer2->Add(bPlay, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bStop = new wxButton(this, ID_bStop, _("Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStop"));
	BoxSizer2->Add(bStop, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bReset = new wxButton(this, ID_bReset, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReset"));
	BoxSizer2->Add(bReset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer2, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	bUnload = new wxButton(this, ID_bUnload, _("Unload"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bUnload"));
	BoxSizer4->Add(bUnload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bCleanup = new wxButton(this, ID_bCleanup, _("Cleanup"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCleanup"));
	BoxSizer4->Add(bCleanup, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3->Add(BoxSizer4, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer1->Add(bOK, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bListPlaying = new wxButton(this, ID_bListPlaying, _("List Playing Sounds"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bListPlaying"));
	BoxSizer1->Add(bListPlaying, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_bSetVolume,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bSetVolume_Click);
	Connect(ID_bSetLoop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bSetLoop_Click);
	Connect(ID_bSetPause,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bSetPause_Click);
	Connect(ID_bSetSpeed,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bSetSpeed_Click);
	Connect(ID_bPlay,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bPlay_Click);
	Connect(ID_bStop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bStop_Click);
	Connect(ID_bReset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bReset_Click);
	Connect(ID_bUnload,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bUnload_Click);
	Connect(ID_bCleanup,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bCleanup_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bOK_Click);
	Connect(ID_bListPlaying,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&SoundManager::On_bListPlaying_Click);
	//*)
	panel = parent;
	Simcore = 0;
	sound = 0;
	handle = 0;
	lastcount = 0;
	lasthandlecount = 0;
	volume = 0;
	speed = 0;
}

SoundManager::~SoundManager()
{
	//(*Destroy(SoundManager)
	//*)
}

void SoundManager::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildSoundList();
	BuildHandleList();

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
			tVolume->SetValue(TruncateNumber(handle->GetVolume(), 2));
			tSpeed->SetValue(SBS::ToString(handle->GetSpeed()));
			Real length = Simcore->GetSoundSystem()->GetLength(sound);
			tLength->SetValue(TruncateNumber(length / 1000, 2));
		}
	}
	else
		handle = 0;

	//set dynamic values

	if (!handle)
		return;

	if (handle->IsPlaying() == true)
		tPlaying->SetValue("True");
	else
		tPlaying->SetValue("False");

	tPosition->SetValue(TruncateNumber(handle->GetPosition().x, 2) + wxT(", ") + TruncateNumber(handle->GetPosition().y, 2) + wxT(", ") + TruncateNumber(handle->GetPosition().z, 2));

	if (handle->GetLoopState() == true)
		tLoop->SetValue("True");
	else
		tLoop->SetValue("False");

	if (handle->IsPaused() == true)
		tPaused->SetValue("True");
	else
		tPaused->SetValue("False");

	tPlayPosition->SetValue(TruncateNumber(handle->GetPlayPosition(), 2));

	lblStats->SetLabelText("Channels: " + SBS::ToString(sound->GetChannelCount()) + " - Playing Sounds: " + SBS::ToString(Simcore->GetSoundSystem()->GetPlayingCount()));
}

void SoundManager::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void SoundManager::BuildSoundList()
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
			if (old_selection < SoundList->GetCount())
				SoundList->SetSelection(old_selection);
			else
				SoundList->SetSelection(0);
		}
		else
		{
			//clear values
			tLength->Clear();
			tName->Clear();
			tParent->Clear();
			tPlaying->Clear();
			tPosition->Clear();
			tVolume->Clear();
			tLoop->Clear();
			tPaused->Clear();
			tSpeed->Clear();
			tPlayPosition->Clear();
		}
	}
}

void SoundManager::BuildHandleList()
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
			if (old_selection < HandleList->GetCount())
				HandleList->SetSelection(old_selection);
			else
				HandleList->SetSelection(0);
		}
		else
		{
			//clear values
			tName->Clear();
			tParent->Clear();
			tPlaying->Clear();
			tPosition->Clear();
			tVolume->Clear();
			tLoop->Clear();
			tPaused->Clear();
			tSpeed->Clear();
			tPlayPosition->Clear();
			tLength->Clear();
		}
	}
}

void SoundManager::On_bListPlaying_Click(wxCommandEvent& event)
{
	if (Simcore)
		Simcore->GetSoundSystem()->ShowPlayingSounds();
}

void SoundManager::On_bSetVolume_Click(wxCommandEvent& event)
{
	if (handle)
		handle->SetVolume(atof(tVolume->GetValue()));
}

void SoundManager::On_bSetLoop_Click(wxCommandEvent& event)
{
	if (!handle)
		return;

	bool loop = handle->GetLoopState();
	handle->SetLoopState(!loop);
}

void SoundManager::On_bSetPause_Click(wxCommandEvent& event)
{
	if (!handle)
		return;

	bool pause = handle->IsPaused();
	handle->Pause(!pause);
}

void SoundManager::On_bSetSpeed_Click(wxCommandEvent& event)
{
	if (handle)
		handle->SetSpeed(atoi(tSpeed->GetValue()));
}

void SoundManager::On_bPlay_Click(wxCommandEvent& event)
{
	if (handle)
		handle->Play();
}

void SoundManager::On_bStop_Click(wxCommandEvent& event)
{
	if (handle)
		handle->Stop();
}

void SoundManager::On_bReset_Click(wxCommandEvent& event)
{
	if (handle)
		handle->Reset();
}

void SoundManager::On_bUnload_Click(wxCommandEvent& event)
{
	if (handle)
	{
		handle->Unload();
		return;
	}

	if (sound && Simcore)
		Simcore->GetSoundSystem()->Cleanup(SoundList->GetSelection());
}

void SoundManager::On_bCleanup_Click(wxCommandEvent& event)
{
	if (Simcore)
		Simcore->GetSoundSystem()->Cleanup();
}

}
