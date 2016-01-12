/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Load Building Dialog
	Copyright (C)2003-2016 Ryan Thoryk
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

//(*InternalHeaders(LoadDialog)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include "loaddialog.h"

//(*IdInit(LoadDialog)
//*)

BEGIN_EVENT_TABLE(LoadDialog,wxDialog)
	//(*EventTable(LoadDialog)
	//*)
END_EVENT_TABLE()

LoadDialog::LoadDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(LoadDialog)
	Create(parent, id, _("Load Building"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

LoadDialog::~LoadDialog()
{
	//(*Destroy(LoadDialog)
	//*)
}

