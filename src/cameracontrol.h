/* $Id$ */

/*
	Skyscraper 1.2 Alpha - Camera Control Form
	Copyright (C)2005-2008 Ryan Thoryk
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

#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

//(*Headers(CameraControl)
#include <wx/dialog.h>
//*)

class CameraControl: public wxDialog
{
	public:

		CameraControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~CameraControl();

		//(*Identifiers(CameraControl)
		//*)

	protected:

		//(*Handlers(CameraControl)
		void OnInit(wxInitDialogEvent& event);
		//*)

		//(*Declarations(CameraControl)
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
