/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Sky Control Dialog
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

#include "skycontrol.h"

//(*InternalHeaders(SkyControl)
#include <wx/string.h>
#include <wx/intl.h>
//*)

namespace Skyscraper {

//(*IdInit(SkyControl)
//*)

BEGIN_EVENT_TABLE(SkyControl,wxDialog)
	//(*EventTable(SkyControl)
	//*)
END_EVENT_TABLE()

SkyControl::SkyControl(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(SkyControl)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	//*)
}

SkyControl::~SkyControl()
{
	//(*Destroy(SkyControl)
	//*)
}


void SkyControl::OnInit(wxInitDialogEvent& event)
{
}

}
