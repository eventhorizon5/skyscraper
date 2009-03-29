/* $Id$ */

/*
	Skyscraper 1.4 Alpha - Camera Control Form
	Copyright (C)2005-2009 Ryan Thoryk
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

//(*InternalHeaders(CameraControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "cameracontrol.h"

//(*IdInit(CameraControl)
//*)

BEGIN_EVENT_TABLE(CameraControl,wxDialog)
//(*EventTable(CameraControl)
//*)
END_EVENT_TABLE()

CameraControl::CameraControl(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(CameraControl)
	Create(parent, id, _("Camera Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	//*)
}

CameraControl::~CameraControl()
{

}


void CameraControl::OnInit(wxInitDialogEvent& event)
{

}
