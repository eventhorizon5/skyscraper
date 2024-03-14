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
		wxButton* bListPlaying;
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
		static const long ID_STATICTEXT1;
		static const long ID_SoundList;
		static const long ID_STATICTEXT2;
		static const long ID_HandleList;
		static const long ID_lblStats;
		static const long ID_STATICTEXT4;
		static const long ID_tName;
		static const long ID_STATICTEXT3;
		static const long ID_tParent;
		static const long ID_STATICTEXT5;
		static const long ID_tLength;
		static const long ID_STATICTEXT6;
		static const long ID_tPlaying;
		static const long ID_STATICTEXT7;
		static const long ID_tPosition;
		static const long ID_STATICTEXT8;
		static const long ID_tVolume;
		static const long ID_bSetVolume;
		static const long ID_STATICTEXT9;
		static const long ID_tLoop;
		static const long ID_bSetLoop;
		static const long ID_STATICTEXT10;
		static const long ID_tPaused;
		static const long ID_bSetPause;
		static const long ID_STATICTEXT11;
		static const long ID_tSpeed;
		static const long ID_bSetSpeed;
		static const long ID_STATICTEXT12;
		static const long ID_tPlayPosition;
		static const long ID_bPlay;
		static const long ID_bStop;
		static const long ID_bReset;
		static const long ID_bUnload;
		static const long ID_bCleanup;
		static const long ID_bOK;
		static const long ID_bListPlaying;
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

		DECLARE_EVENT_TABLE()
};

}

#endif
