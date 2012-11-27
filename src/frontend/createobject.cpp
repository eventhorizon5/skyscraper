#include "createobject.h"

//(*InternalHeaders(CreateObject)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "parameterviewer.h"

extern SBS *Simcore; //external pointer to the SBS engine
ParameterViewer *viewer;

//(*IdInit(CreateObject)
const long CreateObject::ID_bLoadTexture = wxNewId();
const long CreateObject::ID_bFloor = wxNewId();
const long CreateObject::ID_bAddFloor = wxNewId();
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
	wxFlexGridSizer* FlexGridSizer1;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer1;
	
	Create(parent, id, _("Create Object"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Textures"));
	bLoadTexture = new wxButton(this, ID_bLoadTexture, _("Load Texture"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bLoadTexture"));
	StaticBoxSizer1->Add(bLoadTexture, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Floors"));
	bFloor = new wxButton(this, ID_bFloor, _("New Floor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bFloor"));
	StaticBoxSizer2->Add(bFloor, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bAddFloor = new wxButton(this, ID_bAddFloor, _("AddFloor"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bAddFloor"));
	StaticBoxSizer2->Add(bAddFloor, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	FlexGridSizer1->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Elevators"));
	bElevator = new wxButton(this, ID_bElevator, _("New Elevator"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bElevator"));
	StaticBoxSizer3->Add(bElevator, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Globals"));
	bShaft = new wxButton(this, ID_bShaft, _("New Shaft"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bShaft"));
	StaticBoxSizer4->Add(bShaft, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	FlexGridSizer1->Add(StaticBoxSizer4, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_bLoadTexture,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bLoadTexture_Click);
	Connect(ID_bFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bFloor_Click);
	Connect(ID_bAddFloor,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CreateObject::On_bAddFloor_Click);
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
}

void CreateObject::On_bFloor_Click(wxCommandEvent& event)
{
	if (!viewer)
		viewer = new ParameterViewer(this, wxT("Floor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}

void CreateObject::On_bElevator_Click(wxCommandEvent& event)
{
}

void CreateObject::On_bShaft_Click(wxCommandEvent& event)
{
}

void CreateObject::On_bAddFloor_Click(wxCommandEvent& event)
{
	if (!viewer)
		viewer = new ParameterViewer(this, wxT("AddFloor"), wxT("Floor"), true, -1);
	viewer->Show();
	this->Hide();
}
