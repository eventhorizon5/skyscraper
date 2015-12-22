/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Object Creation Dialog
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

#ifndef CREATEOBJECT_H
#define CREATEOBJECT_H

//(*Headers(CreateObject)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
//*)

namespace Skyscraper {

class CreateObject: public wxDialog
{
	public:

		CreateObject(DebugPanel *root, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CreateObject();

		//(*Declarations(CreateObject)
		wxButton* bFloorCut;
		wxButton* bShaftDoorComponent;
		wxButton* bAddShaftWall;
		wxButton* bAddWall;
		wxButton* bColumnWallBox;
		wxButton* bAddShaftStdDoor;
		wxButton* bFloorAddModel;
		wxButton* bFloorAddSound;
		wxButton* bFinishShaftDoor;
		wxButton* bAddDoor;
		wxButton* bFloorCutAll;
		wxButton* bAddFloorIndicator;
		wxButton* bShaft;
		wxButton* bAddStairsDoor;
		wxButton* bAddFillerWalls;
		wxButton* bAddShaftDoor;
		wxButton* bCreateCallButtons;
		wxButton* bLoadTexture;
		wxButton* bAddDirectionalIndicator;
		wxButton* bAddShaftModel;
		wxButton* bAddStairs;
		wxButton* bColumnWallBox2;
		wxButton* bAddFloor;
		wxButton* bAddStairsModel;
		wxButton* bAddStairsFloor;
		wxButton* bAddInterfloorFloor;
		wxButton* bFloor;
		wxButton* bElevator;
		wxButton* bAddShaftFloor;
		wxButton* bAddInterfloorWall;
		wxButton* bAddStairsWall;
		//*)

	protected:

		//(*Identifiers(CreateObject)
		static const long ID_bLoadTexture;
		static const long ID_bFloor;
		static const long ID_bAddFloor;
		static const long ID_bAddWall;
		static const long ID_bAddInterfloorFloor;
		static const long ID_bAddInterfloorWall;
		static const long ID_bAddShaftFloor;
		static const long ID_bAddShaftWall;
		static const long ID_bAddStairsFloor;
		static const long ID_bAddStairsWall;
		static const long ID_bColumnWallBox;
		static const long ID_bColumnWallBox2;
		static const long ID_bCreateCallButtons;
		static const long ID_bAddStairs;
		static const long ID_bAddDoor;
		static const long ID_bAddStairsDoor;
		static const long ID_bAddDirectionalIndicator;
		static const long ID_bAddShaftDoor;
		static const long ID_bAddFloorIndicator;
		static const long ID_bFloorCut;
		static const long ID_bFloorCutAll;
		static const long ID_bAddFillerWalls;
		static const long ID_bFloorAddSound;
		static const long ID_bShaftDoorComponent;
		static const long ID_bFinishShaftDoor;
		static const long ID_bFloorAddModel;
		static const long ID_bAddStairsModel;
		static const long ID_bAddShaftModel;
		static const long ID_bAddShaftStdDoor;
		static const long ID_bElevator;
		static const long ID_bShaft;
		//*)

	private:

		//(*Handlers(CreateObject)
		void On_bLoadTexture_Click(wxCommandEvent& event);
		void On_bFloor_Click(wxCommandEvent& event);
		void On_bElevator_Click(wxCommandEvent& event);
		void On_bShaft_Click(wxCommandEvent& event);
		void On_bAddFloor_Click(wxCommandEvent& event);
		void On_bAddWall_Click(wxCommandEvent& event);
		void On_bAddInterfloorFloorC_lick(wxCommandEvent& event);
		void On_bAddInterfloorFloor_Click(wxCommandEvent& event);
		void On_bAddInterfloorWall_Click(wxCommandEvent& event);
		void On_bAddShaftFloor_Click(wxCommandEvent& event);
		void On_bAddShaftWall_Click(wxCommandEvent& event);
		void On_bAddFloorIndicator_Click(wxCommandEvent& event);
		void On_bAddShaftDoor_Click(wxCommandEvent& event);
		void On_bAddDirectionalIndicator_Click(wxCommandEvent& event);
		void On_bAddStairsDoor_Click(wxCommandEvent& event);
		void On_bAddDoor_Click(wxCommandEvent& event);
		void On_bAddStairs_Click(wxCommandEvent& event);
		void On_bCreateCallButtons_Click(wxCommandEvent& event);
		void On_bColumnWallBox2_Click(wxCommandEvent& event);
		void On_bColumnWallBox_Click(wxCommandEvent& event);
		void On_bAddStairsWall_Click(wxCommandEvent& event);
		void On_bAddStairsFloor_Click(wxCommandEvent& event);
		void On_bFloorCut_Click(wxCommandEvent& event);
		void On_bFloorCutAll_Click(wxCommandEvent& event);
		void On_bAddFillerWalls_Click(wxCommandEvent& event);
		void On_bFloorAddSound_Click(wxCommandEvent& event);
		void On_bShaftDoorComponent_Click(wxCommandEvent& event);
		void On_bFinishShaftDoor_Click(wxCommandEvent& event);
		void On_bFloorAddModel_Click(wxCommandEvent& event);
		void On_bAddStairsModel_Click(wxCommandEvent& event);
		void On_bAddShaftModel_Click(wxCommandEvent& event);
		void On_bAddShaftStdDoor_Click(wxCommandEvent& event);
		//*)
		DebugPanel *panel;

		DECLARE_EVENT_TABLE()
};

}

#endif
