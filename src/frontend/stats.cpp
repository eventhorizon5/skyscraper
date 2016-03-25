/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Statistics Dialog
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

//(*InternalHeaders(Stats)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "texture.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "stats.h"

using namespace SBS;

namespace Skyscraper {

//(*IdInit(Stats)
const long Stats::ID_STATICTEXT18 = wxNewId();
const long Stats::ID_tName = wxNewId();
const long Stats::ID_STATICTEXT17 = wxNewId();
const long Stats::ID_tDesigner = wxNewId();
const long Stats::ID_STATICTEXT16 = wxNewId();
const long Stats::ID_tLocation = wxNewId();
const long Stats::ID_STATICTEXT15 = wxNewId();
const long Stats::ID_tDescription = wxNewId();
const long Stats::ID_STATICTEXT14 = wxNewId();
const long Stats::ID_tVersion = wxNewId();
const long Stats::ID_STATICTEXT5 = wxNewId();
const long Stats::ID_tFloors = wxNewId();
const long Stats::ID_STATICTEXT6 = wxNewId();
const long Stats::ID_tBasements = wxNewId();
const long Stats::ID_STATICTEXT7 = wxNewId();
const long Stats::ID_tElevators = wxNewId();
const long Stats::ID_STATICTEXT8 = wxNewId();
const long Stats::ID_tShafts = wxNewId();
const long Stats::ID_STATICTEXT9 = wxNewId();
const long Stats::ID_tStairs = wxNewId();
const long Stats::ID_STATICTEXT1 = wxNewId();
const long Stats::ID_tMeshes = wxNewId();
const long Stats::ID_STATICTEXT3 = wxNewId();
const long Stats::ID_tTextures = wxNewId();
const long Stats::ID_STATICTEXT4 = wxNewId();
const long Stats::ID_tActions = wxNewId();
const long Stats::ID_STATICTEXT21 = wxNewId();
const long Stats::ID_tSounds = wxNewId();
const long Stats::ID_STATICTEXT22 = wxNewId();
const long Stats::ID_tObjects = wxNewId();
const long Stats::ID_STATICTEXT24 = wxNewId();
const long Stats::ID_tWalls = wxNewId();
const long Stats::ID_STATICTEXT25 = wxNewId();
const long Stats::ID_tPolygons = wxNewId();
const long Stats::ID_STATICTEXT10 = wxNewId();
const long Stats::ID_tInStairwell = wxNewId();
const long Stats::ID_STATICTEXT11 = wxNewId();
const long Stats::ID_tInElevator = wxNewId();
const long Stats::ID_STATICTEXT12 = wxNewId();
const long Stats::ID_tInShaft = wxNewId();
const long Stats::ID_STATICTEXT13 = wxNewId();
const long Stats::ID_tRunningTime = wxNewId();
const long Stats::ID_STATICTEXT20 = wxNewId();
const long Stats::ID_tFrontendVersion = wxNewId();
const long Stats::ID_STATICTEXT23 = wxNewId();
const long Stats::ID_tSBSVersion = wxNewId();
const long Stats::ID_STATICTEXT26 = wxNewId();
const long Stats::ID_tPlatform = wxNewId();
const long Stats::ID_STATICTEXT2 = wxNewId();
const long Stats::ID_tRenderer = wxNewId();
const long Stats::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(Stats,wxDialog)
	//(*EventTable(Stats)
	//*)
END_EVENT_TABLE()

Stats::Stats(DebugPanel* parent,wxWindowID id)
{
	//(*Initialize(Stats)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxStaticBoxSizer* StaticBoxSizer1;

	Create(parent, wxID_ANY, _("Simulator Statistics"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Building Information"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText18 = new wxStaticText(this, ID_STATICTEXT18, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT18"));
	FlexGridSizer3->Add(StaticText18, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_tName"));
	tName->SetMinSize(wxSize(175,50));
	FlexGridSizer3->Add(tName, 1, wxLEFT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText17 = new wxStaticText(this, ID_STATICTEXT17, _("Designer:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT17"));
	FlexGridSizer3->Add(StaticText17, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tDesigner = new wxTextCtrl(this, ID_tDesigner, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_tDesigner"));
	tDesigner->SetMinSize(wxSize(-1,50));
	FlexGridSizer3->Add(tDesigner, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Location:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer3->Add(StaticText16, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tLocation = new wxTextCtrl(this, ID_tLocation, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_tLocation"));
	tLocation->SetMinSize(wxSize(-1,50));
	FlexGridSizer3->Add(tLocation, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText15 = new wxStaticText(this, ID_STATICTEXT15, _("Description:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT15"));
	FlexGridSizer3->Add(StaticText15, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tDescription = new wxTextCtrl(this, ID_tDescription, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_tDescription"));
	tDescription->SetMinSize(wxSize(-1,50));
	FlexGridSizer3->Add(tDescription, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText14 = new wxStaticText(this, ID_STATICTEXT14, _("Version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT14"));
	FlexGridSizer3->Add(StaticText14, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tVersion = new wxTextCtrl(this, ID_tVersion, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tVersion"));
	FlexGridSizer3->Add(tVersion, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Floors:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer3->Add(StaticText5, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tFloors = new wxTextCtrl(this, ID_tFloors, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tFloors"));
	FlexGridSizer3->Add(tFloors, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Basements:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer3->Add(StaticText6, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tBasements = new wxTextCtrl(this, ID_tBasements, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tBasements"));
	FlexGridSizer3->Add(tBasements, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Elevators:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer3->Add(StaticText7, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tElevators = new wxTextCtrl(this, ID_tElevators, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tElevators"));
	FlexGridSizer3->Add(tElevators, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Shafts:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer3->Add(StaticText8, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tShafts = new wxTextCtrl(this, ID_tShafts, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tShafts"));
	FlexGridSizer3->Add(tShafts, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Stairs:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer3->Add(StaticText9, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tStairs = new wxTextCtrl(this, ID_tStairs, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tStairs"));
	FlexGridSizer3->Add(tStairs, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("General Information"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Meshes:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tMeshes = new wxTextCtrl(this, ID_tMeshes, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tMeshes"));
	tMeshes->SetMinSize(wxSize(150,-1));
	FlexGridSizer2->Add(tMeshes, 1, wxLEFT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Textures:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(StaticText3, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tTextures = new wxTextCtrl(this, ID_tTextures, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tTextures"));
	FlexGridSizer2->Add(tTextures, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Actions:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(StaticText4, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tActions = new wxTextCtrl(this, ID_tActions, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tActions"));
	FlexGridSizer2->Add(tActions, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText21 = new wxStaticText(this, ID_STATICTEXT21, _("Sounds:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer2->Add(StaticText21, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tSounds = new wxTextCtrl(this, ID_tSounds, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tSounds"));
	FlexGridSizer2->Add(tSounds, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText22 = new wxStaticText(this, ID_STATICTEXT22, _("Objects:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	FlexGridSizer2->Add(StaticText22, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tObjects = new wxTextCtrl(this, ID_tObjects, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tObjects"));
	FlexGridSizer2->Add(tObjects, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText24 = new wxStaticText(this, ID_STATICTEXT24, _("Walls:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer2->Add(StaticText24, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tWalls = new wxTextCtrl(this, ID_tWalls, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tWalls"));
	FlexGridSizer2->Add(tWalls, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText25 = new wxStaticText(this, ID_STATICTEXT25, _("Polygons:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer2->Add(StaticText25, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tPolygons = new wxTextCtrl(this, ID_tPolygons, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPolygons"));
	FlexGridSizer2->Add(tPolygons, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("InStairwell:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(StaticText10, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tInStairwell = new wxTextCtrl(this, ID_tInStairwell, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tInStairwell"));
	FlexGridSizer2->Add(tInStairwell, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("InElevator:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer2->Add(StaticText11, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tInElevator = new wxTextCtrl(this, ID_tInElevator, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tInElevator"));
	FlexGridSizer2->Add(tInElevator, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("InShaft:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer2->Add(StaticText12, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tInShaft = new wxTextCtrl(this, ID_tInShaft, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tInShaft"));
	FlexGridSizer2->Add(tInShaft, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("Running Time:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer2->Add(StaticText13, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tRunningTime = new wxTextCtrl(this, ID_tRunningTime, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tRunningTime"));
	FlexGridSizer2->Add(tRunningTime, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText20 = new wxStaticText(this, ID_STATICTEXT20, _("Frontend Ver:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
	FlexGridSizer2->Add(StaticText20, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tFrontendVersion = new wxTextCtrl(this, ID_tFrontendVersion, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tFrontendVersion"));
	FlexGridSizer2->Add(tFrontendVersion, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText23 = new wxStaticText(this, ID_STATICTEXT23, _("SBS Version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	FlexGridSizer2->Add(StaticText23, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tSBSVersion = new wxTextCtrl(this, ID_tSBSVersion, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tSBSVersion"));
	FlexGridSizer2->Add(tSBSVersion, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText26 = new wxStaticText(this, ID_STATICTEXT26, _("Platform:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer2->Add(StaticText26, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tPlatform = new wxTextCtrl(this, ID_tPlatform, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPlatform"));
	FlexGridSizer2->Add(tPlatform, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Renderer:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	tRenderer = new wxTextCtrl(this, ID_tRenderer, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tRenderer"));
	FlexGridSizer2->Add(tRenderer, 1, wxLEFT|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	FlexGridSizer1->Add(bOK, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Stats::On_bOK_Click);
	//*)
	Simcore = 0;
	panel = parent;
	OnInit();
}

Stats::~Stats()
{
	//(*Destroy(Stats)
	//*)
}


void Stats::On_bOK_Click(wxCommandEvent& event)
{
	this->Hide();
}

void Stats::OnInit()
{
	Simcore = panel->GetSystem();

	tFrontendVersion->SetValue(panel->GetRoot()->version_frontend);
	tSBSVersion->SetValue(Simcore->version);
	tPlatform->SetValue(panel->GetRoot()->Platform);
	tRenderer->SetValue(panel->GetRoot()->Renderer);

	tName->SetValue(Simcore->BuildingName);
	tDesigner->SetValue(Simcore->BuildingDesigner);
	tLocation->SetValue(Simcore->BuildingLocation);
	tDescription->SetValue(Simcore->BuildingDescription);
	tVersion->SetValue(Simcore->BuildingVersion);
}

void Stats::Loop()
{
	//if active engine has changed, refresh values
	if (Simcore != panel->GetSystem())
		OnInit();

	if (!Simcore)
		return;

	tMeshes->SetValue(ToString(Simcore->GetMeshCount()));
	tTextures->SetValue(ToString(Simcore->GetTextureManager()->GetMaterialCount()));
	tActions->SetValue(ToString(Simcore->GetActionCount()));
	tSounds->SetValue(ToString(Simcore->GetSoundCount()));
	tInStairwell->SetValue(BoolToString(Simcore->InStairwell));
	tInElevator->SetValue(BoolToString(Simcore->InElevator));
	tInShaft->SetValue(BoolToString(Simcore->InShaft));
	tRunningTime->SetValue(TruncateNumber(Simcore->running_time, 2));
	tObjects->SetValue(ToString(Simcore->GetObjectCount()));
	tWalls->SetValue(ToString(Simcore->GetWallCount()));
	tPolygons->SetValue(ToString(Simcore->GetPolygonCount()));
	tFloors->SetValue(ToString(Simcore->Floors));
	tBasements->SetValue(ToString(Simcore->Basements));
	tElevators->SetValue(ToString(Simcore->GetElevatorCount()));
	tShafts->SetValue(ToString(Simcore->GetShaftCount()));
	tStairs->SetValue(ToString(Simcore->GetStairsCount()));
}

}

