/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Mesh Control Form
	Copyright (C)2005-2007 Ryan Thoryk
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

#ifndef MESHCONTROL_H
#define MESHCONTROL_H

//(*Headers(MeshControl)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/stattext.h>
//*)

class MeshControl: public wxDialog
{
	friend class Timer;
	public:

		MeshControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~MeshControl();

		//(*Identifiers(MeshControl)
		static const long ID_bOk;
		static const long ID_chkFloor;
		static const long ID_STATICTEXT2;
		static const long ID_chkColumnFrame;
		static const long ID_chkSky;
		static const long ID_chkLandscape;
		static const long ID_chkBuildings;
		static const long ID_chkExternal;
		static const long ID_STATICTEXT1;
		//*)

	protected:

		//(*Handlers(MeshControl)
		void On_bOk_Click(wxCommandEvent& event);
		void On_chkExternal_Change(wxCommandEvent& event);
		void On_chkBuildings_Change(wxCommandEvent& event);
		void On_chkLandscape_Change(wxCommandEvent& event);
		void On_chkSky_Change(wxCommandEvent& event);
		void On_chkColumnFrame_Change(wxCommandEvent& event);
		void On_chkFloor_Change(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(MeshControl)
		wxButton*  bOk;
		wxCheckBox*  chkFloor;
		wxStaticText*  StaticText2;
		wxCheckBox*  chkColumnFrame;
		wxCheckBox*  chkSky;
		wxCheckBox*  chkLandscape;
		wxCheckBox*  chkBuildings;
		wxCheckBox*  chkExternal;
		wxStaticText*  StaticText1;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
