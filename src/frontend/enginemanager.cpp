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

//(*InternalHeaders(EngineManager)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "enginemanager.h"
#include "skyscraper.h"

namespace Skyscraper {

//(*IdInit(EngineManager)
const long EngineManager::ID_LISTBOX1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(EngineManager,wxDialog)
	//(*EventTable(EngineManager)
	//*)
END_EVENT_TABLE()

EngineManager::EngineManager(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EngineManager)
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* BoxSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	ListBox1 = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	FlexGridSizer1->Add(ListBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
	OnInit();
}

EngineManager::~EngineManager()
{
	//(*Destroy(EngineManager)
	//*)
}

void EngineManager::OnInit()
{
	Simcore = skyscraper->GetActiveEngine()->GetSystem();
}

void EngineManager::Loop()
{
	//if active engine has changed, refresh values
	if (skyscraper->GetActiveEngine())
	{
		if (Simcore != skyscraper->GetActiveEngine()->GetSystem())
			OnInit();
	}
	else
		return;
}

}

