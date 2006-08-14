/* 	$Id$ */

/*
	Skyscraper 1.1 Alpha - Loader Form
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#ifndef LOADER_H
#define LOADER_H

//(*Headers(Loader)
#include <wx/dialog.h>
#include <wx/intl.h>
//*)

class Loader: public wxDialog
{
	public:

		Loader(wxWindow* parent,wxWindowID id = -1);
		virtual ~Loader();

		//(*Identifiers(Loader)
        enum Identifiers
        {
        };
        //*)

	protected:

		//(*Handlers(Loader)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(Loader)
        //*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
