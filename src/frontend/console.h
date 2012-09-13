/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Script Console
	Copyright (C)20042012 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

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
#include <wx/app.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class Console: public wxDialog
{
	public:

		Console(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Console();

		//(*Declarations(Console)
		wxButton* bSend;
		wxButton* bClose;
		wxTextCtrl* tCommand;
		//*)

	protected:

		//(*Identifiers(Console)
		static const long ID_tCommand;
		static const long ID_bSend;
		static const long ID_bClose;
		//*)

	private:

		//(*Handlers(Console)
		void On_bSend_Click(wxCommandEvent& event);
		void On_bClose_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
