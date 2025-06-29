/*
	Skyscraper 2.1 - Control Reference Form
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef KEYDIALOG_H
#define KEYDIALOG_H

//(*Headers(KeyDialog)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class KeyDialog: public wxDialog
{
	public:

		KeyDialog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~KeyDialog();

		//(*Declarations(KeyDialog)
		wxTextCtrl* TextCtrl1;
		//*)

	protected:

		//(*Identifiers(KeyDialog)
		static const long ID_TEXTCTRL1;
		//*)

	private:

		//(*Handlers(KeyDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

}

#endif
