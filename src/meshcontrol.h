/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Mesh Control Form
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

#ifndef MESHCONTROL_H
#define MESHCONTROL_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(MeshControl)
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class MeshControl: public wxDialog
{
	friend class DebugPanel;
	public:

		MeshControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~MeshControl();

		//(*Identifiers(MeshControl)
		static const long ID_chkExternal;
		static const long ID_chkBuildings;
		static const long ID_chkLandscape;
		static const long ID_chkSky;
		static const long ID_chkElevators;
		static const long ID_chkFloor;
		static const long ID_chkShafts;
		static const long ID_chkStairs;
		static const long ID_chkAllFloors;
		static const long ID_chkAllShafts;
		static const long ID_chkAllStairs;
		static const long ID_bOk;
		//*)

	protected:

		//(*Handlers(MeshControl)
		void On_bOk_Click(wxCommandEvent& event);
		void On_chkSky_Click(wxCommandEvent& event);
		void On_chkLandscape_Click(wxCommandEvent& event);
		void On_chkBuildings_Click(wxCommandEvent& event);
		void On_chkExternal_Click(wxCommandEvent& event);
		void On_chkFloor_Click(wxCommandEvent& event);
		void On_chkElevators_Click(wxCommandEvent& event);
		void On_chkShafts_Click(wxCommandEvent& event);
		void On_chkStairs_Click(wxCommandEvent& event);
		void On_chkAllFloors_Click(wxCommandEvent& event);
		void On_chkAllShafts_Click(wxCommandEvent& event);
		void On_chkAllStairs_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(MeshControl)
		wxCheckBox* chkAllShafts;
		wxCheckBox* chkAllFloors;
		wxCheckBox* chkShafts;
		wxCheckBox* chkBuildings;
		wxCheckBox* chkExternal;
		wxBoxSizer* BoxSizer2;
		wxCheckBox* chkFloor;
		wxCheckBox* chkElevators;
		wxCheckBox* chkAllStairs;
		wxCheckBox* chkStairs;
		wxButton* bOk;
		wxBoxSizer* BoxSizer1;
		wxCheckBox* chkSky;
		wxCheckBox* chkLandscape;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
