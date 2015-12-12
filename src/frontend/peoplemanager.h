/* $Id$ */

/*
	Skyscraper 1.10 Alpha - People Manager Dialog
	Copyright (C)2003-2015 Ryan Thoryk
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

#ifndef PEOPLEMANAGER_H
#define PEOPLEMANAGER_H

//(*Headers(PeopleManager)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
//*)

namespace Skyscraper {

class PeopleManager: public wxDialog
{
	public:

		PeopleManager(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PeopleManager();

		//(*Declarations(PeopleManager)
		wxListBox* PersonList;
		//*)
		void Loop();

	protected:

		//(*Identifiers(PeopleManager)
		static const long ID_PersonList;
		//*)

	private:

		//(*Handlers(PeopleManager)
		//*)
		SBS::SBS *Simcore;
		int lastcount;

		DECLARE_EVENT_TABLE()
};

}

#endif

