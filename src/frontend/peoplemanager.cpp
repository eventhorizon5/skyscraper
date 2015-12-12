/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Engine Manager Dialog
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

//(*InternalHeaders(PeopleManager)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "skyscraper.h"
#include "peoplemanager.h"

namespace Skyscraper {

//(*IdInit(PeopleManager)
const long PeopleManager::ID_PersonList = wxNewId();
//*)

BEGIN_EVENT_TABLE(PeopleManager,wxDialog)
	//(*EventTable(PeopleManager)
	//*)
END_EVENT_TABLE()

PeopleManager::PeopleManager(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(PeopleManager)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	PersonList = new wxListBox(this, ID_PersonList, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_PersonList"));
	FlexGridSizer1->Add(PersonList, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)

	lastcount = 0;
	Simcore = 0;
}

PeopleManager::~PeopleManager()
{
	//(*Destroy(PeopleManager)
	//*)
}

void PeopleManager::Loop()
{
	if (skyscraper->GetActiveEngine())
	{
		SBS::SBS *active = skyscraper->GetActiveEngine()->GetSystem();
		if (Simcore != active)
		{
			//if active engine has changed, refresh values
			Simcore = active;
			lastcount = 0;
		}
	}
	else
		return;

	if (Simcore->GetPersonCount() != lastcount)
	{
		lastcount = Simcore->GetPersonCount();
		//PersonList->Clear();

		//for (int i = 0; i < Simcore->GetPersonCount(); i++)
			//PersonList->Append(wxVariant(i + 1).GetString() + wxT(" - ") + wxString::FromAscii(Simcore->GetPerson(i)->GetName().c_str()));
	}
}

}

