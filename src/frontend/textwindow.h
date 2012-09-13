/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Text Dialog
	Copyright (C)2003-2012 Ryan Thoryk
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

#ifndef TEXTWINDOW_H
#define TEXTWINDOW_H

//(*Headers(TextWindow)
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class TextWindow: public wxDialog
{
	public:

		TextWindow(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~TextWindow();

		//(*Declarations(TextWindow)
		wxBoxSizer* BoxSizer1;
		wxFlexGridSizer* FlexGridSizer1;
		wxButton* bOK;
		wxTextCtrl* tMain;
		//*)

	protected:

		//(*Identifiers(TextWindow)
		static const long ID_tMain;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(TextWindow)
		void On_bOK_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
