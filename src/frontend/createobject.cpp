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

//(*InternalHeaders(CreateObject)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include <wx/app.h>

#include "globals.h"
#include "sbs.h"
#include "parameterviewer.h"
#include "createobject.h"

namespace Skyscraper {

ParameterViewer *viewer;

//(*IdInit(CreateObject)
const long CreateObject::ID_bLoadTexture = wxNewId();
const long CreateObject::ID_bFloor = wxNewId();
const long CreateObject::ID_bAddFloor = wxNewId();
const long CreateObject::ID_bAddWall = wxNewId();
const long CreateObject::ID_bAddInterfloorFloor = wxNewId();
const long CreateObject::ID_bAddInterfloorWall = wxNewId();
const long CreateObject::ID_bAddShaftFloor = wxNewId();
const long CreateObject::ID_bAddShaftWall = wxNewId();
const long CreateObject::ID_bAddStairsFloor = wxNewId();
const long CreateObject::ID_bAddStairsWall = wxNewId();
const long CreateObject::ID_bColumnWallBox = wxNewId();
const long CreateObject::ID_bColumnWallBox2 = wxNewId();
const long CreateObject::ID_bCreateCallButtons = wxNewId();
const long CreateObject::ID_bAddStairs = wxNewId();
const long CreateObject::ID_bAddDoor = wxNewId();
const long CreateObject::ID_bAddStairsDoor = wxNewId();
const long CreateObject::ID_bAddDirectionalIndicator = wxNewId();
const long CreateObject::ID_bAddShaftDoor = wxNewId();
const long CreateObject::ID_bAddFloorIndicator = wxNewId();
const long CreateObject::ID_bFloorCut = wxNewId();
const long CreateObject::ID_bFloorCutAll = wxNewId();
const long CreateObject::ID_bAddFillerWalls = wxNewId();
const long CreateObject::ID_bFloorAddSound = wxNewId();
const long CreateObject::ID_bShaftDoorComponent = wxNewId();
const long CreateObject::ID_bFinishShaftDoor = wxNewId();
const long CreateObject::ID_bFloorAddModel = wxNewId();
const long CreateObject::ID_bAddStairsModel = wxNewId();
const long CreateObject::ID_bAddShaftModel = wxNewId();
const long CreateObject::ID_bAddShaftStdDoor = wxNewId();
const long CreateObject::ID_bElevator = wxNewId();
const long CreateObject::ID_bShaft = wxNewId();
//*)

BEGIN_EVENT_TABLE(CreateObject,wxDialog)
	//(*EventTable(CreateObject)
	//*)
END_EVENT_TABLE()

CreateObject::CreateObject(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(CreateObject)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxGridSizer* GridSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Create Object"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Textures"));
	bLoadTexture = new wxButton(this, ID_bLoadTexture, _("Load Texture"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bLoadTexture"));
	StaticBoxSizer1->Add(bLoadTexture, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Floors/Levels"));
	GridSizer1 = new wxGridSizer(0, 4, 0, 0);
	bFloor = new wxButton(this, ID_bFloor, _("New Level"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloor"));
	GridSizer1->Add(bFloor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddFloor = new wxButton(this, ID_bAddFloor, _("Floor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddFloor"));
	GridSizer1->Add(bAddFloor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddWall = new wxButton(this, ID_bAddWall, _("Wall"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddWall"));
	GridSizer1->Add(bAddWall, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddInterfloorFloor = new wxButton(this, ID_bAddInterfloorFloor, _("Interfloor Floor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddInterfloorFloor"));
	GridSizer1->Add(bAddInterfloorFloor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddInterfloorWall = new wxButton(this, ID_bAddInterfloorWall, _("Interfloor Wall"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddInterfloorWall"));
	GridSizer1->Add(bAddInterfloorWall, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddShaftFloor = new wxButton(this, ID_bAddShaftFloor, _("Shaft Floor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddShaftFloor"));
	GridSizer1->Add(bAddShaftFloor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddShaftWall = new wxButton(this, ID_bAddShaftWall, _("Shaft Wall"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddShaftWall"));
	GridSizer1->Add(bAddShaftWall, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddStairsFloor = new wxButton(this, ID_bAddStairsFloor, _("Stairs Floor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddStairsFloor"));
	GridSizer1->Add(bAddStairsFloor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddStairsWall = new wxButton(this, ID_bAddStairsWall, _("Stairs Wall"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddStairsWall"));
	GridSizer1->Add(bAddStairsWall, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bColumnWallBox = new wxButton(this, ID_bColumnWallBox, _("Column WallBox"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bColumnWallBox"));
	GridSizer1->Add(bColumnWallBox, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bColumnWallBox2 = new wxButton(this, ID_bColumnWallBox2, _("Column WallBox 2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bColumnWallBox2"));
	GridSizer1->Add(bColumnWallBox2, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bCreateCallButtons = new wxButton(this, ID_bCreateCallButtons, _("Call Buttons"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCreateCallButtons"));
	GridSizer1->Add(bCreateCallButtons, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddStairs = new wxButton(this, ID_bAddStairs, _("Stairwell"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddStairs"));
	GridSizer1->Add(bAddStairs, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddDoor = new wxButton(this, ID_bAddDoor, _("Door"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddDoor"));
	GridSizer1->Add(bAddDoor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddStairsDoor = new wxButton(this, ID_bAddStairsDoor, _("Stairs Door"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddStairsDoor"));
	GridSizer1->Add(bAddStairsDoor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddDirectionalIndicator = new wxButton(this, ID_bAddDirectionalIndicator, _("Directional Indicator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddDirectionalIndicator"));
	GridSizer1->Add(bAddDirectionalIndicator, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddShaftDoor = new wxButton(this, ID_bAddShaftDoor, _("Shaft Elevator Door"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddShaftDoor"));
	GridSizer1->Add(bAddShaftDoor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddFloorIndicator = new wxButton(this, ID_bAddFloorIndicator, _("Floor Indicator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddFloorIndicator"));
	GridSizer1->Add(bAddFloorIndicator, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bFloorCut = new wxButton(this, ID_bFloorCut, _("Cut"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloorCut"));
	GridSizer1->Add(bFloorCut, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bFloorCutAll = new wxButton(this, ID_bFloorCutAll, _("CutAll"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloorCutAll"));
	GridSizer1->Add(bFloorCutAll, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddFillerWalls = new wxButton(this, ID_bAddFillerWalls, _("Filler Walls"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddFillerWalls"));
	GridSizer1->Add(bAddFillerWalls, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bFloorAddSound = new wxButton(this, ID_bFloorAddSound, _("AddSound"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloorAddSound"));
	GridSizer1->Add(bFloorAddSound, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bShaftDoorComponent = new wxButton(this, ID_bShaftDoorComponent, _("ShaftDoorComponent"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bShaftDoorComponent"));
	GridSizer1->Add(bShaftDoorComponent, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bFinishShaftDoor = new wxButton(this, ID_bFinishShaftDoor, _("Finish ShaftDoor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFinishShaftDoor"));
	GridSizer1->Add(bFinishShaftDoor, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bFloorAddModel = new wxButton(this, ID_bFloorAddModel, _("Model"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloorAddModel"));
	GridSizer1->Add(bFloorAddModel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddStairsModel = new wxButton(this, ID_bAddStairsModel, _("Stairs Model"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddStairsModel"));
	GridSizer1->Add(bAddStairsModel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddShaftModel = new wxButton(this, ID_bAddShaftModel, _("Shaft Model"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddShaftModel"));
	GridSizer1->Add(bAddShaftModel, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddShaftStdDoor = new wxButton(this, ID_bAddShaftStdDoor, _("Shaft Standard Door"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddShaftStdDoor"));
	GridSizer1->Add(bAddShaftStdDoor, 1, wxALL|wxEXPAND|wxALIGN_TOP, 5);
	StaticBoxSizer2->Add(GridSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Elevators"));
	bElevator = new wxButton(this, ID_bElevator, _("New Elevator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bElevator"));
	bElevator->Disable();
	StaticBoxSizer3->Add(bElevator, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Globals"));
	bShaft = new wxButton(this, ID_bShaft, _("New Shaft"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bShaft"));
	bShaft->Disable();
	StaticBoxSizer4->Add(bShaft, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_bLoadTexture,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bLoadTexture_Click);
	Connect(ID_bFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloor_Click);
	Connect(ID_bAddFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddFloor_Click);
	Connect(ID_bAddWall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddWall_Click);
	Connect(ID_bAddInterfloorFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddInterfloorFloor_Click);
	Connect(ID_bAddInterfloorWall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddInterfloorWall_Click);
	Connect(ID_bAddShaftFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddShaftFloor_Click);
	Connect(ID_bAddShaftWall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddShaftWall_Click);
	Connect(ID_bAddStairsFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddStairsFloor_Click);
	Connect(ID_bAddStairsWall,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddStairsWall_Click);
	Connect(ID_bColumnWallBox,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bColumnWallBox_Click);
	Connect(ID_bColumnWallBox2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bColumnWallBox2_Click);
	Connect(ID_bCreateCallButtons,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bCreateCallButtons_Click);
	Connect(ID_bAddStairs,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddStairs_Click);
	Connect(ID_bAddDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddDoor_Click);
	Connect(ID_bAddStairsDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddStairsDoor_Click);
	Connect(ID_bAddDirectionalIndicator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddDirectionalIndicator_Click);
	Connect(ID_bAddShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddShaftDoor_Click);
	Connect(ID_bAddFloorIndicator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddFloorIndicator_Click);
	Connect(ID_bFloorCut,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloorCut_Click);
	Connect(ID_bFloorCutAll,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloorCutAll_Click);
	Connect(ID_bAddFillerWalls,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddFillerWalls_Click);
	Connect(ID_bFloorAddSound,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloorAddSound_Click);
	Connect(ID_bShaftDoorComponent,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bShaftDoorComponent_Click);
	Connect(ID_bFinishShaftDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFinishShaftDoor_Click);
	Connect(ID_bFloorAddModel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloorAddModel_Click);
	Connect(ID_bAddStairsModel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddStairsModel_Click);
	Connect(ID_bAddShaftModel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddShaftModel_Click);
	Connect(ID_bAddShaftStdDoor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddShaftStdDoor_Click);
	Connect(ID_bElevator,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bElevator_Click);
	Connect(ID_bShaft,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bShaft_Click);
	//*)
}

CreateObject::~CreateObject()
{
	//(*Destroy(CreateObject)
	//*)
	if (viewer)
		viewer->Destroy();
	viewer = 0;
}


void CreateObject::On_bLoadTexture_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("Load"), wxT("Texture"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFloor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("Floor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bElevator_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("Elevator"), wxT("Elevator"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bShaft_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("Shaft"), wxT("Shaft"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddFloor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddFloor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddWall_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddWall"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddInterfloorFloor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddInterfloorFloor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddInterfloorWall_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddInterfloorWall"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddShaftFloor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddShaftFloor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddShaftWall_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddShaftWall"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddFloorIndicator_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddFloorIndicator"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddShaftDoor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddShaftDoor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddDirectionalIndicator_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddDirectionalIndicator"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddStairsDoor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddStairsDoor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddDoor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddDoor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddStairs_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddStairs"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bCreateCallButtons_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("CreateCallButtons"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bColumnWallBox2_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("ColumnWallBox2"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bColumnWallBox_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("ColumnWallBox"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddStairsWall_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddStairsWall"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddStairsFloor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddStairsFloor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFloorCut_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("Cut"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFloorCutAll_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("CutAll"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddFillerWalls_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddFillerWalls"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFloorAddSound_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddSound"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bShaftDoorComponent_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddShaftDoorComponent"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFinishShaftDoor_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("FinishShaftDoor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bFloorAddModel_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddModel"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddStairsModel_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddStairsModel"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddShaftModel_Click(wxCommandEvent& event)
{
	if (viewer)
		delete viewer;
	viewer = 0;
	viewer = new ParameterViewer(this, wxT("AddShaftModel"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bAddShaftStdDoor_Click(wxCommandEvent& event)
{
}

}
