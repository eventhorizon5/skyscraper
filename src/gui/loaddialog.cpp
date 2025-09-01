/*
	Skyscraper 2.1 - Load Building Dialog
	Copyright (C)2003-2025 Ryan Thoryk
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

//(*InternalHeaders(LoadDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "gui.h"
#include "debugpanel.h"
#include "loaddialog.h"

namespace Skyscraper {

//(*IdInit(LoadDialog)
const wxWindowID LoadDialog::ID_tFilename = wxNewId();
const wxWindowID LoadDialog::ID_bSelect = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT11 = wxNewId();
const wxWindowID LoadDialog::ID_txtParent = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT1 = wxNewId();
const wxWindowID LoadDialog::ID_PosX = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT4 = wxNewId();
const wxWindowID LoadDialog::ID_tPosY = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT5 = wxNewId();
const wxWindowID LoadDialog::ID_tPosZ = wxNewId();
const wxWindowID LoadDialog::ID_STATICLINE3 = wxNewId();
const wxWindowID LoadDialog::ID_STATICLINE4 = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT2 = wxNewId();
const wxWindowID LoadDialog::ID_tRotation = wxNewId();
const wxWindowID LoadDialog::ID_chkCutLandscape = wxNewId();
const wxWindowID LoadDialog::ID_chkCutBuildings = wxNewId();
const wxWindowID LoadDialog::ID_chkCutExternal = wxNewId();
const wxWindowID LoadDialog::ID_chkCutFloors = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT3 = wxNewId();
const wxWindowID LoadDialog::ID_tMinX = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT6 = wxNewId();
const wxWindowID LoadDialog::ID_tMinY = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT7 = wxNewId();
const wxWindowID LoadDialog::ID_tMinZ = wxNewId();
const wxWindowID LoadDialog::ID_STATICLINE1 = wxNewId();
const wxWindowID LoadDialog::ID_STATICLINE2 = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT8 = wxNewId();
const wxWindowID LoadDialog::ID_tMaxX = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT9 = wxNewId();
const wxWindowID LoadDialog::ID_tMaxY = wxNewId();
const wxWindowID LoadDialog::ID_STATICTEXT10 = wxNewId();
const wxWindowID LoadDialog::ID_tMaxZ = wxNewId();
const wxWindowID LoadDialog::ID_bLoad = wxNewId();
//*)

BEGIN_EVENT_TABLE(LoadDialog,wxDialog)
	//(*EventTable(LoadDialog)
	//*)
END_EVENT_TABLE()

LoadDialog::LoadDialog(DebugPanel *root, wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(LoadDialog)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer10;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;

	Create(parent, wxID_ANY, _("Load Building"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Filename"));
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	tFilename = new wxTextCtrl(this, ID_tFilename, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tFilename"));
	tFilename->SetMinSize(wxSize(150,-1));
	FlexGridSizer4->Add(tFilename, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSelect = new wxButton(this, ID_bSelect, _("Select"), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT, wxDefaultValidator, _T("ID_bSelect"));
	FlexGridSizer4->Add(bSelect, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer7 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Parent Engine:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer7->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtParent = new wxTextCtrl(this, ID_txtParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtParent"));
	FlexGridSizer7->Add(txtParent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(StaticBoxSizer2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
	FlexGridSizer9 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Position and Rotation"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Center X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosX = new wxTextCtrl(this, ID_PosX, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_PosX"));
	tPosX->SetMinSize(wxSize(90,-1));
	FlexGridSizer3->Add(tPosX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Center Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer3->Add(StaticText4, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosY = new wxTextCtrl(this, ID_tPosY, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tPosY"));
	tPosY->SetMinSize(wxSize(90,-1));
	FlexGridSizer3->Add(tPosY, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Center Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer3->Add(StaticText5, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPosZ = new wxTextCtrl(this, ID_tPosZ, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tPosZ"));
	tPosZ->SetMinSize(wxSize(90,-1));
	FlexGridSizer3->Add(tPosZ, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine3 = new wxStaticLine(this, ID_STATICLINE3, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE3"));
	FlexGridSizer3->Add(StaticLine3, 1, wxLEFT|wxEXPAND, 5);
	StaticLine4 = new wxStaticLine(this, ID_STATICLINE4, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE4"));
	FlexGridSizer3->Add(StaticLine4, 1, wxRIGHT|wxEXPAND, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Rotation:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tRotation = new wxTextCtrl(this, ID_tRotation, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tRotation"));
	tRotation->SetMinSize(wxSize(90,-1));
	FlexGridSizer3->Add(tRotation, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer3, 1, wxEXPAND, 5);
	FlexGridSizer9->Add(StaticBoxSizer1, 1, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Options"));
	FlexGridSizer10 = new wxFlexGridSizer(0, 1, 0, 0);
	chkCutLandscape = new wxCheckBox(this, ID_chkCutLandscape, _("Cut Landscape"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCutLandscape"));
	chkCutLandscape->SetValue(false);
	FlexGridSizer10->Add(chkCutLandscape, 1, wxEXPAND, 5);
	chkCutBuildings = new wxCheckBox(this, ID_chkCutBuildings, _("Cut Buildings"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCutBuildings"));
	chkCutBuildings->SetValue(false);
	FlexGridSizer10->Add(chkCutBuildings, 1, wxEXPAND, 5);
	chkCutExternal = new wxCheckBox(this, ID_chkCutExternal, _("Cut External"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCutExternal"));
	chkCutExternal->SetValue(false);
	FlexGridSizer10->Add(chkCutExternal, 1, wxEXPAND, 5);
	chkCutFloors = new wxCheckBox(this, ID_chkCutFloors, _("Cut Floors"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCutFloors"));
	chkCutFloors->SetValue(false);
	FlexGridSizer10->Add(chkCutFloors, 1, wxEXPAND, 5);
	StaticBoxSizer4->Add(FlexGridSizer10, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer9->Add(StaticBoxSizer4, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer6->Add(FlexGridSizer9, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Bounds"));
	FlexGridSizer5 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Min X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer5->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMinX = new wxTextCtrl(this, ID_tMinX, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMinX"));
	tMinX->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMinX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Min Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer5->Add(StaticText6, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMinY = new wxTextCtrl(this, ID_tMinY, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMinY"));
	tMinY->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMinY, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Min Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer5->Add(StaticText7, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMinZ = new wxTextCtrl(this, ID_tMinZ, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMinZ"));
	tMinZ->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMinZ, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer5->Add(StaticLine1, 1, wxLEFT|wxEXPAND, 5);
	StaticLine2 = new wxStaticLine(this, ID_STATICLINE2, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE2"));
	FlexGridSizer5->Add(StaticLine2, 1, wxRIGHT|wxEXPAND, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Max X:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer5->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMaxX = new wxTextCtrl(this, ID_tMaxX, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMaxX"));
	tMaxX->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMaxX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Max Y:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer5->Add(StaticText9, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMaxY = new wxTextCtrl(this, ID_tMaxY, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMaxY"));
	tMaxY->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMaxY, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Max Z:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer5->Add(StaticText10, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tMaxZ = new wxTextCtrl(this, ID_tMaxZ, _T("0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_tMaxZ"));
	tMaxZ->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(tMaxZ, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer2->Add(FlexGridSizer6, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bLoad = new wxButton(this, ID_bLoad, _("Load"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bLoad"));
	BoxSizer1->Add(bLoad, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxBOTTOM|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_bSelect, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&LoadDialog::On_bSelect_Click);
	Connect(ID_bLoad, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&LoadDialog::On_bLoad_Click);
	//*)
	panel = root;

	//set cut options
	VM *vm = panel->GetRoot();
	chkCutLandscape->SetValue(vm->CutLandscape);
	chkCutBuildings->SetValue(vm->CutBuildings);
	chkCutExternal->SetValue(vm->CutExternal);
	chkCutFloors->SetValue(vm->CutFloors);
}

LoadDialog::~LoadDialog()
{
	//(*Destroy(LoadDialog)
	//*)
}

void LoadDialog::On_bSelect_Click(wxCommandEvent& event)
{
	std::string filename = panel->GetRoot()->GetGUI()->SelectBuilding(panel->GetRoot()->data_path);

	tFilename->SetValue(filename);
}

void LoadDialog::On_bLoad_Click(wxCommandEvent& event)
{
	EngineContext *parent_engine = panel->GetRoot()->GetEngine(SBS::ToInt(txtParent->GetValue().ToStdString()));
	if (!parent_engine)
		return;

	Vector3 position, min, max;
	Real rotation;

	//get position values
	if (!IsNumeric(tPosX->GetValue(), position.x))
		return;

	if (!IsNumeric(tPosY->GetValue(), position.y))
		return;

	if (!IsNumeric(tPosZ->GetValue(), position.z))
		return;

	//get rotation
	if (!IsNumeric(tRotation->GetValue(), rotation))
		return;

	if (!IsNumeric(tMinX->GetValue(), min.x))
		return;

	//get min bounds
	if (!IsNumeric(tMinY->GetValue(), min.y))
		return;

	if (!IsNumeric(tMinZ->GetValue(), min.z))
		return;

	//get max bounds
	if (!IsNumeric(tMaxX->GetValue(), max.x))
		return;

	if (!IsNumeric(tMaxY->GetValue(), max.y))
		return;

	if (!IsNumeric(tMaxZ->GetValue(), max.z))
		return;

	std::string filename;
	filename = tFilename->GetValue();

	VM *vm = panel->GetRoot();

	//set cut options
	vm->CutLandscape = chkCutLandscape->GetValue();
	vm->CutBuildings = chkCutBuildings->GetValue();
	vm->CutExternal = chkCutExternal->GetValue();
	vm->CutFloors = chkCutFloors->GetValue();

	Vector3 rot (0, rotation, 0);
	vm->Load(false, false, filename, parent_engine, position, rot, min, max);

	this->Close();
}

void LoadDialog::SetEngineParent(EngineContext *parent)
{
	if (parent)
		txtParent->SetValue(SBS::ToString(parent->GetNumber()));
}

}

