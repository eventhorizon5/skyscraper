#include "parameterviewer.h"

//(*InternalHeaders(ParameterViewer)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "fileio.h"
#include "skyscraper.h"
#include "objectinfo.h"

extern SBS *Simcore; //external pointer to the SBS engine
extern Skyscraper *skyscraper;
extern ObjectInfo *objectinfo;

//(*IdInit(ParameterViewer)
const long ParameterViewer::ID_tDesc = wxNewId();
const long ParameterViewer::ID_tVariables = wxNewId();
const long ParameterViewer::ID_STATICTEXT1 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL1 = wxNewId();
const long ParameterViewer::ID_STATICTEXT2 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL2 = wxNewId();
const long ParameterViewer::ID_STATICTEXT3 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL3 = wxNewId();
const long ParameterViewer::ID_STATICTEXT4 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL4 = wxNewId();
const long ParameterViewer::ID_STATICTEXT5 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL5 = wxNewId();
const long ParameterViewer::ID_STATICTEXT6 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL6 = wxNewId();
const long ParameterViewer::ID_STATICTEXT7 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL7 = wxNewId();
const long ParameterViewer::ID_STATICTEXT8 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL8 = wxNewId();
const long ParameterViewer::ID_STATICTEXT9 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL9 = wxNewId();
const long ParameterViewer::ID_STATICTEXT10 = wxNewId();
const long ParameterViewer::ID_TEXTCTRL10 = wxNewId();
const long ParameterViewer::ID_l11 = wxNewId();
const long ParameterViewer::ID_t11 = wxNewId();
const long ParameterViewer::ID_l12 = wxNewId();
const long ParameterViewer::ID_t12 = wxNewId();
const long ParameterViewer::ID_l13 = wxNewId();
const long ParameterViewer::ID_t13 = wxNewId();
const long ParameterViewer::ID_l14 = wxNewId();
const long ParameterViewer::ID_t14 = wxNewId();
const long ParameterViewer::ID_l15 = wxNewId();
const long ParameterViewer::ID_t15 = wxNewId();
const long ParameterViewer::ID_l16 = wxNewId();
const long ParameterViewer::ID_t16 = wxNewId();
const long ParameterViewer::ID_l17 = wxNewId();
const long ParameterViewer::ID_t17 = wxNewId();
const long ParameterViewer::ID_l18 = wxNewId();
const long ParameterViewer::ID_t18 = wxNewId();
const long ParameterViewer::ID_l19 = wxNewId();
const long ParameterViewer::ID_t19 = wxNewId();
const long ParameterViewer::ID_l20 = wxNewId();
const long ParameterViewer::ID_t20 = wxNewId();
const long ParameterViewer::ID_STATICTEXT11 = wxNewId();
const long ParameterViewer::ID_tCommand = wxNewId();
const long ParameterViewer::ID_bCancel = wxNewId();
const long ParameterViewer::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(ParameterViewer,wxDialog)
	//(*EventTable(ParameterViewer)
	//*)
END_EVENT_TABLE()

ParameterViewer::ParameterViewer(wxWindow* parent, wxString object_type, wxString object_parent, bool createobject, wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ParameterViewer)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("Parameter Viewer"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	tDesc = new wxStaticText(this, ID_tDesc, _("Desc"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tDesc"));
	FlexGridSizer1->Add(tDesc, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	tVariables = new wxStaticText(this, ID_tVariables, _("Variables"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE, _T("ID_tVariables"));
	FlexGridSizer1->Add(tVariables, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	l1 = new wxStaticText(this, ID_STATICTEXT1, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(l1, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	t1->SetMinSize(wxSize(200,-1));
	FlexGridSizer2->Add(t1, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l2 = new wxStaticText(this, ID_STATICTEXT2, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(l2, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t2 = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer2->Add(t2, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l3 = new wxStaticText(this, ID_STATICTEXT3, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(l3, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t3 = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	FlexGridSizer2->Add(t3, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l4 = new wxStaticText(this, ID_STATICTEXT4, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(l4, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t4 = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	FlexGridSizer2->Add(t4, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l5 = new wxStaticText(this, ID_STATICTEXT5, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(l5, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t5 = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	FlexGridSizer2->Add(t5, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l6 = new wxStaticText(this, ID_STATICTEXT6, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer2->Add(l6, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t6 = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	FlexGridSizer2->Add(t6, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l7 = new wxStaticText(this, ID_STATICTEXT7, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer2->Add(l7, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t7 = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	FlexGridSizer2->Add(t7, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l8 = new wxStaticText(this, ID_STATICTEXT8, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer2->Add(l8, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t8 = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	FlexGridSizer2->Add(t8, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l9 = new wxStaticText(this, ID_STATICTEXT9, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer2->Add(l9, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t9 = new wxTextCtrl(this, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	FlexGridSizer2->Add(t9, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	l10 = new wxStaticText(this, ID_STATICTEXT10, _("1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer2->Add(l10, 1, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t10 = new wxTextCtrl(this, ID_TEXTCTRL10, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	FlexGridSizer2->Add(t10, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2->Add(FlexGridSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	l11 = new wxStaticText(this, ID_l11, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l11"));
	FlexGridSizer3->Add(l11, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t11 = new wxTextCtrl(this, ID_t11, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t11"));
	t11->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t11, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l12 = new wxStaticText(this, ID_l12, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l12"));
	FlexGridSizer3->Add(l12, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t12 = new wxTextCtrl(this, ID_t12, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t12"));
	t12->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t12, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l13 = new wxStaticText(this, ID_l13, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l13"));
	FlexGridSizer3->Add(l13, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t13 = new wxTextCtrl(this, ID_t13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t13"));
	t13->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t13, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l14 = new wxStaticText(this, ID_l14, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l14"));
	FlexGridSizer3->Add(l14, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t14 = new wxTextCtrl(this, ID_t14, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t14"));
	t14->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t14, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l15 = new wxStaticText(this, ID_l15, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l15"));
	FlexGridSizer3->Add(l15, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t15 = new wxTextCtrl(this, ID_t15, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t15"));
	t15->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t15, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l16 = new wxStaticText(this, ID_l16, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l16"));
	FlexGridSizer3->Add(l16, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t16 = new wxTextCtrl(this, ID_t16, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t16"));
	t16->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t16, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l17 = new wxStaticText(this, ID_l17, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l17"));
	FlexGridSizer3->Add(l17, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t17 = new wxTextCtrl(this, ID_t17, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t17"));
	t17->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t17, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l18 = new wxStaticText(this, ID_l18, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l18"));
	FlexGridSizer3->Add(l18, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t18 = new wxTextCtrl(this, ID_t18, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t18"));
	t18->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t18, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l19 = new wxStaticText(this, ID_l19, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l19"));
	FlexGridSizer3->Add(l19, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t19 = new wxTextCtrl(this, ID_t19, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t19"));
	t19->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t19, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	l20 = new wxStaticText(this, ID_l20, _("1"), wxDefaultPosition, wxSize(150,-1), 0, _T("ID_l20"));
	FlexGridSizer3->Add(l20, 1, wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
	t20 = new wxTextCtrl(this, ID_t20, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_t20"));
	t20->SetMinSize(wxSize(200,-1));
	FlexGridSizer3->Add(t20, 1, wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer2->Add(FlexGridSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT11, _("Script Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	tCommand = new wxTextCtrl(this, ID_tCommand, _("Command"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer1->Add(tCommand, 1, wxALL|wxEXPAND|wxALIGN_TOP|wxALIGN_BOTTOM|wxALIGN_CENTER_HORIZONTAL, 5);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	bCancel = new wxButton(this, ID_bCancel, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCancel"));
	BoxSizer1->Add(bCancel, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer1->Add(bOK, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer1, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL7,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL8,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL9,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_TEXTCTRL10,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t11,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t12,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t13,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t14,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t15,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t16,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t17,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t18,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t19,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_t20,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&ParameterViewer::ChangeParam);
	Connect(ID_bCancel,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParameterViewer::On_bCancel_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ParameterViewer::On_bOK_Click);
	//*)
	objecttype = object_type;
	objectparent = object_parent;
	create = createobject;
	Setup();
}

ParameterViewer::~ParameterViewer()
{
	//(*Destroy(ParameterViewer)
	//*)
}

bool ParameterViewer::Setup()
{
	//set up parameter dialog

	if (create == true)
	{
		if (objecttype == wxT("Floor") && objectparent == wxT("Floor"))
		{
			SetTitle(wxT("Create Floor"));

			t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			wxString newlabel = wxT("Create a New Floor\nFloors above ground start with 0 (so a 15-story building would have floors 0-14).\nAlso, floors must be made in the proper order: basement levels must be made first in decending order (-1, -2, -3 etc),\nand then above-ground floors in ascending order (0, 1, 2, etc)\nItems with an asterisk (*) are required.\nMove mouse over parameter name for help");
			newlabel = newlabel +wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Number*:"));
			l1->SetToolTip(wxT("Number of new floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name*:"));
			l2->SetToolTip(wxT("the name of the current floor, required\nExample: Name = Floor %floor%"));

			l3->SetLabel(wxT("ID*:"));
			l3->SetToolTip(wxT("the floor indicator name for the current floor, such as L (for Lobby), LL (lower level), M (Mezzanine), etc. This is also used to determine what texture should be loaded for the elevator floor indicators and floor signs. The texture name would be 'Button[ID]' - so if the ID is 10, the texture name would be 'Button10'.\nExample: ID = %floor%"));

			l4->SetLabel(wxT("Type*:"));
			l4->SetToolTip(wxT("the type of floor the current floor is. The types are still being defined, but the currently used ones are Basement, Lobby, Mezzanine, Conference, Office, Service, Skylobby, Hotel, Apartment, Condominium, Restaurant, Observatory, Recreation, Ballroom, Communications, and Roof. (Required)\nExample: Type = Office"));

			l5->SetLabel(wxT("Description:"));
			l5->SetToolTip(wxT("description of the current floor, optional\nExample: Description = Offices"));

			l6->SetLabel(wxT("Height*:"));
			l6->SetToolTip(wxT("the floor-to-ceiling height of the current floor, required\nExample: Height = 9.5"));

			l7->SetLabel(wxT("InterfloorHeight*:"));
			l7->SetToolTip(wxT(" the height in feet of the space between floors (below each floor), starting at the floor's altitude, and ending right below the level's floor; required.\nExample: InterfloorHeight = 2.24"));

			l8->SetLabel(wxT("Altitude:"));
			l8->SetToolTip(wxT("this parameter is optional and is only recommended if the first level has an interfloor area that needs to be below ground. If this parameter is not used, the altitude will be calculated automatically.\nExample: Altitude = -2.24"));

			l9->SetLabel(wxT("Group:"));
			l9->SetToolTip(wxT("group floors together. This is a list of comma-separated floor numbers (or a range specified with the - symbol) that should be enabled along with this floor when the user arrives at this floor. For example, if a 2-story room has a balcony, and the room base and balcony are separate floors, you would specify the other floor's number in this parameter.\nExamples:\nGroup = 5\nGroup = 4, 5\nGroup = 4 - 10"));
		}

		if (objecttype == wxT("AddFloor") && objectparent == wxT("Floor"))
		{
			//t1->SetValue(wxVariant((int)Simcore->Floors).GetString());

			SetTitle(wxT("Floor - AddFloor"));

			wxString newlabel = wxT("Adds a textured floor with the specified dimensions to the current floor/level\n");
			newlabel = newlabel + wxT("Items with an asterisk (*) are required.\nMove mouse over parameter name for help");

			tDesc->SetLabel(newlabel);
			tVariables->SetLabel(wxT("Parameters:\n%floor% - contains the current floor number\n%height% - contains the current floor's ceiling height\n%interfloorheight% - contains the current floor's interfloor height (spacing between floors)\n%fullheight% - contains the current floor's total height, including the interfloor height\n%base% - if the InterfloorOnTop parameter in the Globals section is set to 'false' (the default),\n    then Base refers to the floor's altitude plus interfloor height; otherwise it's the floor's altitude\n"));

			l1->SetLabel(wxT("Number:"));
			l1->SetToolTip(wxT("Number of floor (such as 1), or a range of floors (such as 5 to 10)"));

			l2->SetLabel(wxT("Name:"));

			l3->SetLabel(wxT("Texture Name:"));

			l4->SetLabel(wxT("Thickness:"));

			l5->SetLabel(wxT("X1:"));

			l6->SetLabel(wxT("Z1:"));

			l7->SetLabel(wxT("X2:"));

			l8->SetLabel(wxT("Z2:"));

			l9->SetLabel(wxT("Vertical Offset1:"));
			l9->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l10->SetLabel(wxT("Vertical Offset2:"));
			l10->SetToolTip(wxT("Voffset1 and voffset2 are the height in feet above the current floor's altitude"));

			l11->SetLabel(wxT("Texture Width:"));
			l11->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t11->SetValue(wxT("0"));

			l12->SetLabel(wxT("Texture Height:"));
			l12->SetToolTip(wxT("tw and th are to size/tile the texture (0 lets the app autosize them), and isexternal determines if the floor is part of the building's external framework, or is part of the current floor (is either True or False)"));
			t12->SetValue(wxT("0"));

			l13->SetLabel(wxT("Is External:"));
			l13->SetToolTip(wxT("isexternal determines if the wall is part of the building's external framework (true) or if it's part of the current floor (false)"));
			t13->SetValue(wxT("false"));

		}

		if (l1->GetLabel() == wxT("1"))
		{
			l1->Hide();
			t1->Hide();
		}
		if (l2->GetLabel() == wxT("1"))
		{
			l2->Hide();
			t2->Hide();
		}
		if (l3->GetLabel() == wxT("1"))
		{
			l3->Hide();
			t3->Hide();
		}
		if (l4->GetLabel() == wxT("1"))
		{
			l4->Hide();
			t4->Hide();
		}
		if (l5->GetLabel() == wxT("1"))
		{
			l5->Hide();
			t5->Hide();
		}
		if (l6->GetLabel() == wxT("1"))
		{
			l6->Hide();
			t6->Hide();
		}
		if (l7->GetLabel() == wxT("1"))
		{
			l7->Hide();
			t7->Hide();
		}
		if (l8->GetLabel() == wxT("1"))
		{
			l8->Hide();
			t8->Hide();
		}
		if (l9->GetLabel() == wxT("1"))
		{
			l9->Hide();
			t9->Hide();
		}
		if (l10->GetLabel() == wxT("1"))
		{
			l10->Hide();
			t10->Hide();
		}
		if (l11->GetLabel() == wxT("1"))
		{
			l11->Hide();
			t11->Hide();
		}
		if (l12->GetLabel() == wxT("1"))
		{
			l12->Hide();
			t12->Hide();
		}
		if (l13->GetLabel() == wxT("1"))
		{
			l13->Hide();
			t13->Hide();
		}
		if (l14->GetLabel() == wxT("1"))
		{
			l14->Hide();
			t14->Hide();
		}
		if (l15->GetLabel() == wxT("1"))
		{
			l15->Hide();
			t15->Hide();
		}
		if (l16->GetLabel() == wxT("1"))
		{
			l16->Hide();
			t16->Hide();
		}
		if (l17->GetLabel() == wxT("1"))
		{
			l17->Hide();
			t17->Hide();
		}
		if (l18->GetLabel() == wxT("1"))
		{
			l18->Hide();
			t18->Hide();
		}
		if (l19->GetLabel() == wxT("1"))
		{
			l19->Hide();
			t19->Hide();
		}
		if (l20->GetLabel() == wxT("1"))
		{
			l20->Hide();
			t20->Hide();
		}

		this->Fit();
		this->CentreOnScreen();
	}
}

void ParameterViewer::On_bCancel_Click(wxCommandEvent& event)
{
	this->Hide();
}

void ParameterViewer::On_bOK_Click(wxCommandEvent& event)
{
	Simcore->RecreateColliders = true;
	ScriptProcessor *processor = new ScriptProcessor();
	processor->LoadFromText(command.ToAscii());
	processor->LoadBuilding();
	delete processor;
	processor = 0;
	Simcore->Prepare();
	Simcore->RecreateColliders = false;
	objectinfo->PopulateTree();
	this->Hide();
}

void ParameterViewer::ChangeParam(wxCommandEvent& event)
{
	//command constructor

	if (objecttype == wxT("Floor") && objectparent == wxT("Floor"))
	{
		wxString temp = t1->GetValue();
		temp.LowerCase();
		bool multiple = false;
		if (temp.Find(wxT("to")) > 0)
			multiple = true;

		if (multiple == true)
			command = wxT("<Floors ") + wxString(t1->GetValue()) + wxT(">\n");
		else
			command = wxT("<Floor ") + wxString(t1->GetValue()) + wxT(">\n");
		command = command + wxT("Name = ") + wxString(t2->GetValue()) + wxT("\n");
		command = command + wxT("ID = ") + wxString(t3->GetValue()) + wxT("\n");
		command = command + wxT("Type = ") + wxString(t4->GetValue()) + wxT("\n");
		if (t5->IsEmpty() == false)
			command = command + wxT("Description = ") + wxString(t5->GetValue()) + wxT("\n");
		command = command + wxT("Height = ") + wxString(t6->GetValue()) + wxT("\n");
		command = command + wxT("InterfloorHeight = ") + wxString(t7->GetValue()) + wxT("\n");
		if (t8->IsEmpty() == false)
			command = command + wxT("Altitude = ") + wxString(t8->GetValue()) + wxT("\n");
		if (t9->IsEmpty() == false)
			command = command + wxT("Group = ") + wxString(t9->GetValue()) + wxT("\n");

		if (multiple == true)
			command = command + wxT("<EndFloors>\n");
		else
			command = command + wxT("<EndFloor>\n");
	}

	if (objecttype == wxT("AddFloor") && objectparent == wxT("Floor"))
	{
		wxString temp = t1->GetValue();
		temp.LowerCase();
		bool multiple = false;
		if (temp.Find(wxT("to")) > 0)
			multiple = true;

		if (multiple == true)
			command = wxT("<Floors ") + wxString(t1->GetValue()) + wxT(">\n");
		else
			command = wxT("<Floor ") + wxString(t1->GetValue()) + wxT(">\n");

		command = command + wxT("AddFloor ") + wxString(t2->GetValue()) + wxT(", ");
		command = command + wxString(t3->GetValue()) + wxT(", ");
		command = command + wxString(t4->GetValue()) + wxT(", ");
		command = command + wxString(t5->GetValue()) + wxT(", ");
		command = command + wxString(t6->GetValue()) + wxT(", ");
		command = command + wxString(t7->GetValue()) + wxT(", ");
		command = command + wxString(t8->GetValue()) + wxT(", ");
		command = command + wxString(t9->GetValue()) + wxT(", ");
		command = command + wxString(t10->GetValue()) + wxT(", ");
		command = command + wxString(t11->GetValue()) + wxT(", ");
		command = command + wxString(t12->GetValue()) + wxT(", ");
		command = command + wxString(t13->GetValue()) + wxT("\n");

		if (multiple == true)
			command = command + wxT("<EndFloors>\n");
		else
			command = command + wxT("<EndFloor>\n");
	}

	tCommand->SetValue(command);
	this->Fit();
	this->CentreOnScreen();
}
