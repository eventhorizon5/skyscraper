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
		wxButton* bOK;
		wxListBox* HandleList;
		wxListBox* SoundList;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxTextCtrl* tChannels;
		wxTextCtrl* tName;
		wxTextCtrl* tParent;
		//*)
		void Loop();

	protected:

		//(*Identifiers(SoundManager)
		static const long ID_STATICTEXT1;
		static const long ID_SoundList;
		static const long ID_STATICTEXT2;
		static const long ID_HandleList;
		static const long ID_STATICTEXT4;
		static const long ID_tName;
		static const long ID_STATICTEXT3;
		static const long ID_tParent;
		static const long ID_STATICTEXT5;
		static const long ID_tChannels;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(SoundManager)
		void On_bOK_Click(wxCommandEvent& event);
		//*)
		void BuildSoundList(bool restore_selection = false);
		void BuildHandleList(bool restore_selection = false);

		SBS::SBS *Simcore;
		DebugPanel *panel;
		SBS::SoundData *sound;
		SBS::Sound *handle;
		int lastcount;
		int lasthandlecount;

		DECLARE_EVENT_TABLE()
};

}

#endif
