/*
	Skyscraper 1.12 Alpha - Mesh Control Form
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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
#include <wx/sizer.h>
//*)

namespace Skyscraper {

class MeshControl: public wxDialog
{
	public:

		MeshControl(DebugPanel* parent,wxWindowID id = -1);
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
		static const long ID_chkInterfloor;
		static const long ID_chkColumnFrame;
		static const long ID_chkAllFloors;
		static const long ID_chkAllShafts;
		static const long ID_chkAllStairs;
		static const long ID_chkAllInterfloors;
		static const long ID_chkAllColumnFrames;
		static const long ID_bOk;
		//*)
		void Loop();

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
		void On_chkColumnFrame_Click(wxCommandEvent& event);
		void On_chkAllColumnFrames_Click(wxCommandEvent& event);
		void On_chkInterfloor_Click(wxCommandEvent& event);
		void On_chkAllInterfloors_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(MeshControl)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxButton* bOk;
		wxCheckBox* chkAllColumnFrames;
		wxCheckBox* chkAllFloors;
		wxCheckBox* chkAllInterfloors;
		wxCheckBox* chkAllShafts;
		wxCheckBox* chkAllStairs;
		wxCheckBox* chkBuildings;
		wxCheckBox* chkColumnFrame;
		wxCheckBox* chkElevators;
		wxCheckBox* chkExternal;
		wxCheckBox* chkFloor;
		wxCheckBox* chkInterfloor;
		wxCheckBox* chkLandscape;
		wxCheckBox* chkShafts;
		wxCheckBox* chkSky;
		wxCheckBox* chkStairs;
		//*)

	private:

		SBS::SBS *Simcore;
		DebugPanel *panel;

		DECLARE_EVENT_TABLE()
};

}

#endif
