/* $Id$ */

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

#include "loader.h"

BEGIN_EVENT_TABLE(Loader,wxDialog)
//(*EventTable(Loader)
EVT_BUTTON(ID_bOK,Loader::On_bOK_Click)
//*)
END_EVENT_TABLE()

Loader::Loader(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Loader)
	Create(parent,id,_("Building Loader"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxTHICK_FRAME|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxNO_3D,_T(""));
	Selector = new wxGenericDirCtrl( this, ID_SELECTOR, _T(""), wxPoint(16,17),wxSize(196,167), 0, _T(""), 0, _("ID_SELECTOR"));
	bOK = new wxButton(this,ID_bOK,_("OK"),wxPoint(71,199),wxDefaultSize,0,wxDefaultValidator,_("ID_bOK"));
	if (false) bOK->SetDefault();
	//*)
	Selector->SetFilter("Building files (*.bld)|*.bld");
	//Selector->SetDefaultPath(
}

Loader::~Loader()
{
}


void Loader::OnInit(wxInitDialogEvent& event)
{

}

void Loader::On_bOK_Click(wxCommandEvent& event)
{

}
