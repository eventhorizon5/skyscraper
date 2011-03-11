/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Profiler GUI
	Copyright (C)2003-2011 Ryan Thoryk
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

#include "globals.h"
//#include "sbs.h"
#include "profilergui.h"
#include "profiler.h"

//(*InternalHeaders(Profiler)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(Profiler)
const long Profiler::ID_txtMain = wxNewId();
//*)

BEGIN_EVENT_TABLE(Profiler,wxDialog)
	//(*EventTable(Profiler)
	//*)
END_EVENT_TABLE()

Profiler::Profiler(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Profiler)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, _("Profiler"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	txtMain = new wxStaticText(this, ID_txtMain, wxEmptyString, wxDefaultPosition, wxSize(400,400), wxALIGN_LEFT, _T("ID_txtMain"));
	BoxSizer1->Add(txtMain, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	//*)
}

Profiler::~Profiler()
{
	//(*Destroy(Profiler)
	//*)
}

void Profiler::Loop()
{
	SBSProfileIterator* profileIterator = 0;
	profileIterator = SBSProfileManager::Get_Iterator();

	std::string output;
	SBSProfileManager::dumpRecursive(output, profileIterator,0);
	txtMain->SetLabel(wxString::FromAscii(output.c_str()));

	SBSProfileManager::Release_Iterator(profileIterator);
}
