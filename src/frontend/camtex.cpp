
//(*InternalHeaders(CameraTextureControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "cameratexture.h"
#include "camtex.h"

namespace Skyscraper {

//(*IdInit(CameraTextureControl)
const long CameraTextureControl::ID_CameraList = wxNewId();
const long CameraTextureControl::ID_chkEnabled = wxNewId();
const long CameraTextureControl::ID_lblPositionX = wxNewId();
const long CameraTextureControl::ID_lblPositionY = wxNewId();
const long CameraTextureControl::ID_lblPositionZ = wxNewId();
const long CameraTextureControl::ID_txtPositionX = wxNewId();
const long CameraTextureControl::ID_txtPositionY = wxNewId();
const long CameraTextureControl::ID_txtPositionZ = wxNewId();
const long CameraTextureControl::ID_bSetPosition = wxNewId();
const long CameraTextureControl::ID_lblRotationX = wxNewId();
const long CameraTextureControl::ID_lblRotationY = wxNewId();
const long CameraTextureControl::ID_lblRotationZ = wxNewId();
const long CameraTextureControl::ID_txtRotationX = wxNewId();
const long CameraTextureControl::ID_txtRotationY = wxNewId();
const long CameraTextureControl::ID_txtRotationZ = wxNewId();
const long CameraTextureControl::ID_bSetRotation = wxNewId();
const long CameraTextureControl::ID_bSetLookAt = wxNewId();
const long CameraTextureControl::ID_lblFOV = wxNewId();
const long CameraTextureControl::ID_txtFOV = wxNewId();
const long CameraTextureControl::ID_bSetFOV = wxNewId();
const long CameraTextureControl::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(CameraTextureControl,wxDialog)
	//(*EventTable(CameraTextureControl)
	//*)
END_EVENT_TABLE()

CameraTextureControl::CameraTextureControl(DebugPanel* parent,wxWindowID id)
{
	//(*Initialize(CameraTextureControl)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer7;
	wxFlexGridSizer* FlexGridSizer8;
	wxFlexGridSizer* FlexGridSizer9;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;

	Create(parent, id, _("CameraTexture Control"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	CameraList = new wxListBox(this, ID_CameraList, wxDefaultPosition, wxSize(200,150), 0, 0, 0, wxDefaultValidator, _T("ID_CameraList"));
	BoxSizer2->Add(CameraList, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 1, 0, 0);
	chkEnabled = new wxCheckBox(this, ID_chkEnabled, _("Enabled"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkEnabled"));
	chkEnabled->SetValue(false);
	FlexGridSizer3->Add(chkEnabled, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Position"));
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	lblPositionX = new wxStaticText(this, ID_lblPositionX, _("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblPositionX"));
	FlexGridSizer5->Add(lblPositionX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblPositionY = new wxStaticText(this, ID_lblPositionY, _("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblPositionY"));
	FlexGridSizer5->Add(lblPositionY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblPositionZ = new wxStaticText(this, ID_lblPositionZ, _("Z"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblPositionZ"));
	FlexGridSizer5->Add(lblPositionZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionX = new wxTextCtrl(this, ID_txtPositionX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtPositionX"));
	txtPositionX->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(txtPositionX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionY = new wxTextCtrl(this, ID_txtPositionY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtPositionY"));
	txtPositionY->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(txtPositionY, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtPositionZ = new wxTextCtrl(this, ID_txtPositionZ, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtPositionZ"));
	txtPositionZ->SetMinSize(wxSize(90,-1));
	FlexGridSizer5->Add(txtPositionZ, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(FlexGridSizer5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetPosition = new wxButton(this, ID_bSetPosition, _("Set Position"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetPosition"));
	FlexGridSizer4->Add(bSetPosition, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer1->Add(FlexGridSizer4, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(StaticBoxSizer1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Rotation"));
	FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
	lblRotationX = new wxStaticText(this, ID_lblRotationX, _("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblRotationX"));
	FlexGridSizer7->Add(lblRotationX, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblRotationY = new wxStaticText(this, ID_lblRotationY, _("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblRotationY"));
	FlexGridSizer7->Add(lblRotationY, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	lblRotationZ = new wxStaticText(this, ID_lblRotationZ, _("Z"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblRotationZ"));
	FlexGridSizer7->Add(lblRotationZ, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationX = new wxTextCtrl(this, ID_txtRotationX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtRotationX"));
	txtRotationX->SetMinSize(wxSize(90,-1));
	FlexGridSizer7->Add(txtRotationX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationY = new wxTextCtrl(this, ID_txtRotationY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtRotationY"));
	txtRotationY->SetMinSize(wxSize(90,-1));
	FlexGridSizer7->Add(txtRotationY, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtRotationZ = new wxTextCtrl(this, ID_txtRotationZ, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtRotationZ"));
	txtRotationZ->SetMinSize(wxSize(90,-1));
	FlexGridSizer7->Add(txtRotationZ, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(FlexGridSizer7, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bSetRotation = new wxButton(this, ID_bSetRotation, _("Set Rotation"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetRotation"));
	BoxSizer3->Add(bSetRotation, 1, wxBOTTOM|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetLookAt = new wxButton(this, ID_bSetLookAt, _("LookAt"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetLookAt"));
	BoxSizer3->Add(bSetLookAt, 1, wxBOTTOM|wxLEFT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer6->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer2->Add(FlexGridSizer6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(StaticBoxSizer2, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("FOV"));
	FlexGridSizer8 = new wxFlexGridSizer(0, 1, 0, 0);
	lblFOV = new wxStaticText(this, ID_lblFOV, _("FOV"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_lblFOV"));
	FlexGridSizer8->Add(lblFOV, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer9 = new wxFlexGridSizer(0, 3, 0, 0);
	txtFOV = new wxTextCtrl(this, ID_txtFOV, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE, wxDefaultValidator, _T("ID_txtFOV"));
	txtFOV->SetMinSize(wxSize(90,-1));
	FlexGridSizer9->Add(txtFOV, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer8->Add(FlexGridSizer9, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSetFOV = new wxButton(this, ID_bSetFOV, _("Set FOV"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSetFOV"));
	FlexGridSizer8->Add(bSetFOV, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer3->Add(FlexGridSizer8, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(StaticBoxSizer3, 1, wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer1->Add(bOK, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_chkEnabled,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_chkEnabled_Click);
	Connect(ID_bSetPosition,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_bSetPosition_Click);
	Connect(ID_bSetRotation,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_bSetRotation_Click);
	Connect(ID_bSetLookAt,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_bSetLookAt_Click);
	Connect(ID_bSetFOV,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_bSetFOV_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CameraTextureControl::On_bOK_Click);
	//*)

	lastcount = 0;
	Simcore = 0;
	panel = parent;
	camera = 0;
}

CameraTextureControl::~CameraTextureControl()
{
	//(*Destroy(CameraTextureControl)
	//*)
}

void CameraTextureControl::Loop()
{
	if (Simcore != panel->GetSystem())
	{
		//if active engine has changed, refresh values
		Simcore = panel->GetSystem();
	}

	if (!Simcore)
		return;

	BuildList();

	int selection = CameraList->GetSelection();

	if (selection >= 0)
	{
		SBS::CameraTexture *newcamera = Simcore->GetCameraTexture(selection);

		//if a new camera has been selected, update values
		if (newcamera && camera != newcamera)
		{
			camera = Simcore->GetCameraTexture(selection);

			txtPositionX->SetValue(SBS::ToString(camera->GetPosition().x));
			txtPositionY->SetValue(SBS::ToString(camera->GetPosition().y));
			txtPositionZ->SetValue(SBS::ToString(camera->GetPosition().z));

			txtRotationX->SetValue(SBS::ToString(camera->GetRotation().x));
			txtRotationY->SetValue(SBS::ToString(camera->GetRotation().y));
			txtRotationZ->SetValue(SBS::ToString(camera->GetRotation().z));

			txtFOV->SetValue(SBS::ToString(camera->GetFOVAngle()));
		}
	}
	else
		camera = 0;

	if (!camera)
		return;

	chkEnabled->SetValue(camera->IsEnabled());
}

void CameraTextureControl::BuildList(bool restore_selection)
{
	int count = Simcore->GetCameraTextureCount();

	if (count != lastcount)
	{
		lastcount = count;
		int old_selection = CameraList->GetSelection();
		CameraList->Clear();

		for (int i = 0; i < count; i++)
		{
			::SBS::CameraTexture *cam = Simcore->GetCameraTexture(i);
			CameraList->Append(SBS::ToString(i + 1) + wxT(": ") + cam->GetName() + wxT(" (") + cam->GetParent()->GetName() + wxT(")"));
		}

		if (count > 0)
		{
			if (restore_selection == false)
				CameraList->SetSelection(0);
			else
				CameraList->SetSelection(old_selection);
		}
		else
		{
			//clear values
			txtPositionX->SetValue(wxT(""));
			txtPositionY->SetValue(wxT(""));
			txtPositionZ->SetValue(wxT(""));
			txtRotationX->SetValue(wxT(""));
			txtRotationY->SetValue(wxT(""));
			txtRotationZ->SetValue(wxT(""));
			txtFOV->SetValue(wxT(""));
			chkEnabled->SetValue(false);
		}
	}
}

void CameraTextureControl::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

void CameraTextureControl::On_chkEnabled_Click(wxCommandEvent& event)
{
	if (camera)
		camera->Enabled(chkEnabled->GetValue());
}

void CameraTextureControl::On_bSetPosition_Click(wxCommandEvent& event)
{
	if (camera)
	{
		camera->SetPosition(atof(txtPositionX->GetValue()), atof(txtPositionY->GetValue()), atof(txtPositionZ->GetValue()));
	}
}

void CameraTextureControl::On_bSetRotation_Click(wxCommandEvent& event)
{
	if (camera)
	{
		camera->SetRotation(Vector3(atof(txtRotationX->GetValue()), atof(txtRotationY->GetValue()), atof(txtRotationZ->GetValue())));
	}
}

void CameraTextureControl::On_bSetFOV_Click(wxCommandEvent& event)
{
	if (camera)
		camera->SetFOVAngle(atof(txtFOV->GetValue()));
}

void CameraTextureControl::On_bSetLookAt_Click(wxCommandEvent& event)
{
	if (camera)
	{
		camera->LookAt(Vector3(atof(txtRotationX->GetValue()), atof(txtRotationY->GetValue()), atof(txtRotationZ->GetValue())));
	}
}

}
