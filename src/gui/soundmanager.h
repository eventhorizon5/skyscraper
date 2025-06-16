/*
	Skyscraper 2.1 - Sound Manager Dialog
	Copyright (C)2003-2025 Ryan Thoryk
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

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

//(*Headers(SoundManager)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class SoundManager: public wxDialog
{
	public:

		SoundManager(DebugPanel* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SoundManager();

		//(*Declarations(SoundManager)
		wxButton* bCleanup;
		wxButton* bEnabled;
		wxButton* bListPlaying;
		wxButton* bListPlayingAll;
		wxButton* bMove;
		wxButton* bOK;
		wxButton* bPlay;
		wxButton* bReset;
		wxButton* bSetLoop;
		wxButton* bSetPause;
		wxButton* bSetSpeed;
		wxButton* bSetVolume;
		wxButton* bStop;
		wxButton* bUnload;
		wxListBox* HandleList;
		wxListBox* SoundList;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* lblStats;
		wxTextCtrl* tEnabled;
		wxTextCtrl* tLength;
		wxTextCtrl* tLoop;
		wxTextCtrl* tName;
		wxTextCtrl* tParent;
		wxTextCtrl* tPaused;
		wxTextCtrl* tPlayPosition;
		wxTextCtrl* tPlaying;
		wxTextCtrl* tPosition;
		wxTextCtrl* tSpeed;
		wxTextCtrl* tVolume;
		//*)
		void Loop();

	protected:

		//(*Identifiers(SoundManager)
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_SoundList;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_HandleList;
		static const wxWindowID ID_lblStats;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_tName;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_tParent;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_tLength;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_tPlaying;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_tPosition;
		static const wxWindowID ID_bMove;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_tVolume;
		static const wxWindowID ID_bSetVolume;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_tLoop;
		static const wxWindowID ID_bSetLoop;
		static const wxWindowID ID_STATICTEXT10;
		static const wxWindowID ID_tPaused;
		static const wxWindowID ID_bSetPause;
		static const wxWindowID ID_STATICTEXT13;
		static const wxWindowID ID_tEnabled;
		static const wxWindowID ID_bEnabled;
		static const wxWindowID ID_STATICTEXT11;
		static const wxWindowID ID_tSpeed;
		static const wxWindowID ID_bSetSpeed;
		static const wxWindowID ID_STATICTEXT12;
		static const wxWindowID ID_tPlayPosition;
		static const wxWindowID ID_bPlay;
		static const wxWindowID ID_bStop;
		static const wxWindowID ID_bReset;
		static const wxWindowID ID_bUnload;
		static const wxWindowID ID_bCleanup;
		static const wxWindowID ID_bOK;
		static const wxWindowID ID_bListPlaying;
		static const wxWindowID ID_bListPlayingAll;
		//*)

	private:

		//(*Handlers(SoundManager)
		void On_bOK_Click(wxCommandEvent& event);
		void On_bListPlaying_Click(wxCommandEvent& event);
		void On_bSetVolume_Click(wxCommandEvent& event);
		void On_bSetLoop_Click(wxCommandEvent& event);
		void On_bSetPause_Click(wxCommandEvent& event);
		void On_bSetSpeed_Click(wxCommandEvent& event);
		void On_bPlay_Click(wxCommandEvent& event);
		void On_bStop_Click(wxCommandEvent& event);
		void On_bReset_Click(wxCommandEvent& event);
		void On_bUnload_Click(wxCommandEvent& event);
		void On_bCleanup_Click(wxCommandEvent& event);
		void On_bMove_Click(wxCommandEvent& event);
		void On_bEnabled_Click(wxCommandEvent& event);
		void On_bListPlayingAll_Click(wxCommandEvent& event);
		//*)
		void BuildSoundList();
		void BuildHandleList();

		SBS::SBS *Simcore;
		DebugPanel *panel;
		SBS::SoundData *sound;
		SBS::Sound *handle;
		int lastcount;
		int lasthandlecount;
		Real volume;
		Real speed;
		MoveObject *moveobject;

		DECLARE_EVENT_TABLE()
};

}

#endif
