/*
	Skyscraper 1.12 Alpha - Script Console
	Copyright (C)2004-2023 Ryan Thoryk
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

#ifndef CONSOLE_H
#define CONSOLE_H

//(*Headers(Console)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class Console: public wxFrame
{
	public:

		Console(Skyscraper *root, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Console();

		//(*Declarations(Console)
		wxButton* bClear;
		wxButton* bSend;
		wxCheckBox* chkEcho;
		wxPanel* Panel1;
		wxTextCtrl* tCommand;
		wxTextCtrl* tConsole;
		//*)
		void Write(const std::string &message);

	protected:

		//(*Identifiers(Console)
		static const long ID_tConsole;
		static const long ID_tCommand;
		static const long ID_bSend;
		static const long ID_bClear;
		static const long ID_chkEcho;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(Console)
		void On_bSend_Click(wxCommandEvent& event);
		void On_bClose_Click(wxCommandEvent& event);
		void On_Close(wxCloseEvent& event);
		void On_bClear_Click(wxCommandEvent& event);
		//*)

		SBS::SBS *Simcore;
		Skyscraper *skyscraper;

		DECLARE_EVENT_TABLE()
};

}

#endif
