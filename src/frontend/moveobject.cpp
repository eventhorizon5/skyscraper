/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Move Object Form
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

//(*InternalHeaders(MoveObject)
#include <wx/string.h>
#include <wx/intl.h>
//*)
#include "debugpanel.h"
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "skyscraper.h"
#include "unix.h"
#include "moveobject.h"

namespace Skyscraper {

//(*IdInit(MoveObject)
const long MoveObject::ID_STATICTEXT1 = wxNewId();
const long MoveObject::ID_lblPosition = wxNewId();
const long MoveObject::ID_STATICTEXT3 = wxNewId();
const long MoveObject::ID_STATICTEXT4 = wxNewId();
const long MoveObject::ID_STATICTEXT2 = wxNewId();
const long MoveObject::ID_lblRotation = wxNewId();
const long MoveObject::ID_STATICLINE2 = wxNewId();
const long MoveObject::ID_rPosition = wxNewId();
const long MoveObject::ID_rRotation = wxNewId();
const long MoveObject::ID_STATICTEXT28 = wxNewId();
const long MoveObject::ID_txtMoveSpeed = wxNewId();
const long MoveObject::ID_bZPlus = wxNewId();
const long MoveObject::ID_bYPlus = wxNewId();
const long MoveObject::ID_bXNeg = wxNewId();
const long MoveObject::ID_chkHold = wxNewId();
const long MoveObject::ID_bXPlus = wxNewId();
const long MoveObject::ID_bZNeg = wxNewId();
const long MoveObject::ID_bYNeg = wxNewId();
const long MoveObject::ID_STATICTEXT16 = wxNewId();
const long MoveObject::ID_STATICTEXT21 = wxNewId();
const long MoveObject::ID_txtPositionX = wxNewId();
const long MoveObject::ID_bPositionX = wxNewId();
const long MoveObject::ID_STATICTEXT22 = wxNewId();
const long MoveObject::ID_txtPositionY = wxNewId();
const long MoveObject::ID_bPositionY = wxNewId();
const long MoveObject::ID_STATICTEXT23 = wxNewId();
const long MoveObject::ID_txtPositionZ = wxNewId();
const long MoveObject::ID_bPositionZ = wxNewId();
const long MoveObject::ID_STATICLINE1 = wxNewId();
const long MoveObject::ID_STATICTEXT27 = wxNewId();
const long MoveObject::ID_STATICTEXT24 = wxNewId();
const long MoveObject::ID_txtRotationX = wxNewId();
const long MoveObject::ID_bRotationX = wxNewId();
const long MoveObject::ID_STATICTEXT25 = wxNewId();
const long MoveObject::ID_txtRotationY = wxNewId();
const long MoveObject::ID_bRotationY = wxNewId();
const long MoveObject::ID_STATICTEXT26 = wxNewId();
const long MoveObject::ID_txtRotationZ = wxNewId();
const long MoveObject::ID_bRotationZ = wxNewId();
const long MoveObject::ID_STATICLINE3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(MoveObject,wxDialog)
//(*EventTable(MoveObject)
//*)
END_EVENT_TABLE()

MoveObject::MoveObject(DebugPanel* root, wxWindow* parent,wxWindowID id, int object_number)
{
	//(*Initialize(MoveObject)
	wxStaticBoxSizer* StaticBoxSizer2;
	wxGridSizer* GridSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer11;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer9;
	wxFlexGridSizer* FlexGridSizer14;
	wxFlexGridSizer* FlexGridSizer6;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxFlexGridSizer* FlexGridSizer12;
	wxFlexGridSizer* FlexGridSizer5;

	Create(parent, wxID_ANY, _("Move Object"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(2, 2, 0, 0);
	FlexGridSizer9 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Control"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 1, 0, 0);
	GridSizer1 = new wxGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Position:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT1"));
	GridSizer1->Add(StaticText1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblPosition = new wxStaticText(this, ID_lblPosition, _("0, 0, 0"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_lblPosition"));
	GridSizer1->Add(lblPosition, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Relative Position:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT3"));
	GridSizer1->Add(StaticText3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblRelPosition = new wxStaticText(this, ID_STATICTEXT4, _("0, 0, 0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT4"));
	GridSizer1->Add(lblRelPosition, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Rotation:"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_STATICTEXT2"));
	GridSizer1->Add(StaticText2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblRotation = new wxStaticText(this, ID_lblRotation, _("0, 0, 0"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_CENTRE, _T("ID_lblRotation"));
	GridSizer1->Add(lblRotation, 1, wxALL|wxEXPAND|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(GridSizer1, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer5->Add(StaticLine2, 1, wxBOTTOM|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer11 = new wxFlexGridSizer(0, 2, 0, 0);
	rPosition = new wxRadioButton(this, ID_rPosition, _("Position"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_rPosition"));
	rPosition->SetValue(true);
	FlexGridSizer11->Add(rPosition, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	rRotation = new wxRadioButton(this, ID_rRotation, _("Rotation"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_rRotation"));
	FlexGridSizer11->Add(rRotation, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer11, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer14 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText20 = new wxStaticText(this, ID_STATICTEXT28, _("Speed"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT28"));
	FlexGridSizer14->Add(StaticText20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtMoveSpeed = new wxTextCtrl(this, ID_txtMoveSpeed, _("1.0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtMoveSpeed"));
	FlexGridSizer14->Add(txtMoveSpeed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5->Add(FlexGridSizer14, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bZPlus = new wxButton(this, ID_bZPlus, _("Z+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bZPlus"));
	FlexGridSizer12->Add(bZPlus, 1, wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	bYPlus = new wxButton(this, ID_bYPlus, _("Y+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bYPlus"));
	FlexGridSizer12->Add(bYPlus, 1, wxBOTTOM|wxLEFT|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
	bXNeg = new wxButton(this, ID_bXNeg, _("X-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bXNeg"));
	FlexGridSizer12->Add(bXNeg, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	chkHold = new wxCheckBox(this, ID_chkHold, _("Hold"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkHold"));
	chkHold->SetValue(false);
	FlexGridSizer12->Add(chkHold, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bXPlus = new wxButton(this, ID_bXPlus, _("X+"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bXPlus"));
	FlexGridSizer12->Add(bXPlus, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer12->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bZNeg = new wxButton(this, ID_bZNeg, _("Z-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bZNeg"));
	FlexGridSizer12->Add(bZNeg, 1, wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	bYNeg = new wxButton(this, ID_bYNeg, _("Y-"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bYNeg"));
	FlexGridSizer12->Add(bYNeg, 1, wxTOP|wxLEFT|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer5->Add(FlexGridSizer12, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(StaticBoxSizer2, 1, wxTOP|wxRIGHT|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Set"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticText16 = new wxStaticText(this, ID_STATICTEXT16, _("Position"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT16"));
	FlexGridSizer6->Add(StaticText16, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText22 = new wxStaticText(this, ID_STATICTEXT21, _("X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT21"));
	FlexGridSizer4->Add(StaticText22, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionX = new wxTextCtrl(this, ID_txtPositionX, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionX"));
	FlexGridSizer4->Add(txtPositionX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionX = new wxButton(this, ID_bPositionX, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionX"));
	FlexGridSizer4->Add(bPositionX, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText23 = new wxStaticText(this, ID_STATICTEXT22, _("Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT22"));
	FlexGridSizer4->Add(StaticText23, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionY = new wxTextCtrl(this, ID_txtPositionY, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionY"));
	FlexGridSizer4->Add(txtPositionY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionY = new wxButton(this, ID_bPositionY, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionY"));
	FlexGridSizer4->Add(bPositionY, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText24 = new wxStaticText(this, ID_STATICTEXT23, _("Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT23"));
	FlexGridSizer4->Add(StaticText24, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionZ = new wxTextCtrl(this, ID_txtPositionZ, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtPositionZ"));
	FlexGridSizer4->Add(txtPositionZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bPositionZ = new wxButton(this, ID_bPositionZ, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bPositionZ"));
	FlexGridSizer4->Add(bPositionZ, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer4, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer6->Add(StaticLine1, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText28 = new wxStaticText(this, ID_STATICTEXT27, _("Rotation"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT27"));
	FlexGridSizer6->Add(StaticText28, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText25 = new wxStaticText(this, ID_STATICTEXT24, _("X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT24"));
	FlexGridSizer7->Add(StaticText25, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationX = new wxTextCtrl(this, ID_txtRotationX, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationX"));
	FlexGridSizer7->Add(txtRotationX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationX = new wxButton(this, ID_bRotationX, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationX"));
	FlexGridSizer7->Add(bRotationX, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText26 = new wxStaticText(this, ID_STATICTEXT25, _("Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT25"));
	FlexGridSizer7->Add(StaticText26, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationY = new wxTextCtrl(this, ID_txtRotationY, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationY"));
	FlexGridSizer7->Add(txtRotationY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationY = new wxButton(this, ID_bRotationY, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationY"));
	FlexGridSizer7->Add(bRotationY, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText27 = new wxStaticText(this, ID_STATICTEXT26, _("Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT26"));
	FlexGridSizer7->Add(StaticText27, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationZ = new wxTextCtrl(this, ID_txtRotationZ, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtRotationZ"));
	FlexGridSizer7->Add(txtRotationZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bRotationZ = new wxButton(this, ID_bRotationZ, _("Set"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bRotationZ"));
	FlexGridSizer7->Add(bRotationZ, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer7, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	FlexGridSizer6->Add(StaticLine3, 1, wxTOP|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer4->Add(FlexGridSizer6, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9->Add(StaticBoxSizer4, 1, wxTOP|wxBOTTOM|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer1->Add(FlexGridSizer9, 1, wxALIGN_RIGHT|wxALIGN_TOP, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();

	Connect(ID_rPosition,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MoveObject::On_rPosition_Select);
	Connect(ID_rRotation,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&MoveObject::On_rRotation_Select);
	Connect(ID_bZPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bZPlus_Click);
	Connect(ID_bYPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bYPlus_Click);
	Connect(ID_bXNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bXNeg_Click);
	Connect(ID_chkHold,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&MoveObject::On_chkHold_Click);
	Connect(ID_bXPlus,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bXPlus_Click);
	Connect(ID_bZNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bZNeg_Click);
	Connect(ID_bYNeg,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bYNeg_Click);
	Connect(ID_bPositionX,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bPositionX_Click);
	Connect(ID_bPositionY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bPositionY_Click);
	Connect(ID_bPositionZ,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bPositionZ_Click);
	Connect(ID_bRotationX,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bRotationX_Click);
	Connect(ID_bRotationY,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bRotationY_Click);
	Connect(ID_bRotationZ,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MoveObject::On_bRotationZ_Click);
	//*)
	object_num = object_number;
	object = 0;
	Simcore = 0;
	panel = root;
	OnInit();
}

MoveObject::~MoveObject()
{

}

void MoveObject::OnInit()
{
	Simcore = panel->GetSystem();
	hold_vector = Ogre::Vector3(0, 0, 0);
}

void MoveObject::Loop()
{
	if (!panel->GetRoot()->GetActiveEngine())
		return;

	if (panel->GetRoot()->IsValidSystem(Simcore) == false)
		OnInit();

	object = Simcore->GetObject(object_num);

	if (object)
	{
		Ogre::Vector3 pos = object->GetPosition();
		Ogre::Vector3 relpos = object->GetPosition(true);
		Ogre::Vector3 rot = object->GetRotation();
		lblPosition->SetLabel(TruncateNumber(pos.x, 2) + wxT(", ") + TruncateNumber(pos.y, 2) + wxT(", ") + TruncateNumber(pos.z, 2));
		lblRelPosition->SetLabel(TruncateNumber(relpos.x, 2) + wxT(", ") + TruncateNumber(relpos.y, 2) + wxT(", ") + TruncateNumber(relpos.z, 2));
		lblRotation->SetLabel(TruncateNumber(rot.x, 2) + wxT(", ") + TruncateNumber(rot.y, 2) + wxT(", ") + TruncateNumber(rot.z, 2));

		//move if hold vector is not zero
		if (hold_vector != Ogre::Vector3(0, 0, 0))
		{
			if (rPosition->GetValue() == true)
				Simcore->MoveObject(object, hold_vector, true, true, true, true);
			else
				Simcore->RotateObject(object, hold_vector, 1, true, true, true, true);
		}
	}
	else
		Close();
}

void MoveObject::On_rPosition_Select(wxCommandEvent& event)
{

}

void MoveObject::On_rRotation_Select(wxCommandEvent& event)
{

}

void MoveObject::On_bZPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += Ogre::Vector3(0, 0, atof(txtMoveSpeed->GetValue().ToAscii()));
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(0, 0, 1) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(0, 0, 1), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bYPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += Ogre::Vector3(0, atof(txtMoveSpeed->GetValue().ToAscii()), 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(0, 1, 0) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(0, 1, 0), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bXNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= Ogre::Vector3(atof(txtMoveSpeed->GetValue().ToAscii()), 0, 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(-1, 0, 0) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(-1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bXPlus_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector += Ogre::Vector3(atof(txtMoveSpeed->GetValue().ToAscii()), 0, 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(1, 0, 0) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(1, 0, 0), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bZNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= Ogre::Vector3(0, 0, atof(txtMoveSpeed->GetValue().ToAscii()));
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(0, 0, -1) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(0, 0, -1), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bYNeg_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == true)
	{
		hold_vector -= Ogre::Vector3(0, atof(txtMoveSpeed->GetValue().ToAscii()), 0);
		return;
	}

	if (rPosition->GetValue() == true)
		Simcore->MoveObject(object, Ogre::Vector3(0, -1, 0) * atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
	else
		Simcore->RotateObject(object, Ogre::Vector3(0, -1, 0), atof(txtMoveSpeed->GetValue().ToAscii()), true, true, true, true);
}

void MoveObject::On_bPositionX_Click(wxCommandEvent& event)
{
	Simcore->MoveObject(object, Ogre::Vector3(atof(txtPositionX->GetValue().ToAscii()), 0.0, 0.0), false, true, false, false);
}

void MoveObject::On_bPositionY_Click(wxCommandEvent& event)
{
	Simcore->MoveObject(object, Ogre::Vector3(0.0, atof(txtPositionY->GetValue().ToAscii()), 0.0), false, false, true, false);
}

void MoveObject::On_bPositionZ_Click(wxCommandEvent& event)
{
	Simcore->MoveObject(object, Ogre::Vector3(0.0, 0.0, atof(txtPositionZ->GetValue().ToAscii())), false, false, false, true);
}

void MoveObject::On_bRotationX_Click(wxCommandEvent& event)
{
	Simcore->RotateObject(object, Ogre::Vector3(atof(txtRotationX->GetValue().ToAscii()), 0.0, 0.0), 0, false, true, false, false);
}

void MoveObject::On_bRotationY_Click(wxCommandEvent& event)
{
	Simcore->RotateObject(object, Ogre::Vector3(0.0, atof(txtRotationY->GetValue().ToAscii()), 0.0), 0, false, false, true, false);
}

void MoveObject::On_bRotationZ_Click(wxCommandEvent& event)
{
	Simcore->RotateObject(object, Ogre::Vector3(0.0, 0.0, atof(txtRotationZ->GetValue().ToAscii())), 0, false, false, false, true);
}

void MoveObject::On_chkHold_Click(wxCommandEvent& event)
{
	if (chkHold->GetValue() == false)
		hold_vector = Ogre::Vector3(0, 0, 0);
}

}
