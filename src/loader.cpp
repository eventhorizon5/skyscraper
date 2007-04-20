/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Loader Form
	Copyright (C)2005-2007 Ryan Thoryk
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

//(*InternalHeaders(Loader)
#include <wx/bitmap.h>
#include <wx/font.h>
#include <wx/fontenum.h>
#include <wx/fontmap.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/settings.h>
//*)
#include "loader.h"

//(*IdInit(Loader)
const long Loader::ID_SELECTOR = wxNewId();
const long Loader::ID_BUTTON1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Loader,wxDialog)
	//(*EventTable(Loader)
	//*)
END_EVENT_TABLE()

Loader::Loader(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Loader)
	Create(parent,id,_("Select a Building"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE,_T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	Selector = new wxGenericDirCtrl(this,ID_SELECTOR,wxEmptyString,wxPoint(16,17),wxSize(196,167),0,wxEmptyString,0,_T("ID_SELECTOR"));
	BoxSizer1->Add(Selector,0,wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,10);
	bOK = new wxButton(this,ID_BUTTON1,_("OK"),wxPoint(71,199),wxDefaultSize,0,wxDefaultValidator,_T("ID_BUTTON1"));
	BoxSizer1->Add(bOK,0,wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,10);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Loader::On_bOK_Click);
	//*)
	Selector->SetFilter(_("Building files (*.bld)|*.bld"));
	//Selector->SetDefaultPath(
}

Loader::~Loader()
{
}

void Loader::On_bOK_Click(wxCommandEvent& event)
{

}
