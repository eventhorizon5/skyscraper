#include "escalatorcontrol.h"

//(*InternalHeaders(EscalatorControl)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(EscalatorControl)
const long EscalatorControl::ID_LISTBOX1 = wxNewId();
const long EscalatorControl::ID_STATICTEXT1 = wxNewId();
const long EscalatorControl::ID_txtName = wxNewId();
const long EscalatorControl::ID_STATICTEXT2 = wxNewId();
const long EscalatorControl::ID_txtParent = wxNewId();
const long EscalatorControl::ID_chkRun = wxNewId();
const long EscalatorControl::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(EscalatorControl,wxDialog)
    //(*EventTable(EscalatorControl)
    //*)
END_EVENT_TABLE()

EscalatorControl::EscalatorControl(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(EscalatorControl)
    wxBoxSizer* BoxSizer1;
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    ListBox1 = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxSize(150,300), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    FlexGridSizer2->Add(ListBox1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtName"));
    FlexGridSizer3->Add(txtName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtParent = new wxTextCtrl(this, ID_txtParent, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtParent"));
    FlexGridSizer3->Add(txtParent, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
    chkRun = new wxCheckBox(this, ID_chkRun, _("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkRun"));
    chkRun->SetValue(false);
    FlexGridSizer5->Add(chkRun, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
    FlexGridSizer4->Add(BoxSizer1, 1, wxALL|wxEXPAND, 5);
    bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
    FlexGridSizer4->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_chkRun,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&EscalatorControl::On_chkRun_Click);
    Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EscalatorControl::On_bOK_Click);
    //*)
}

EscalatorControl::~EscalatorControl()
{
    //(*Destroy(EscalatorControl)
    //*)
}


void EscalatorControl::On_chkRun_Click(wxCommandEvent& event)
{
}


void EscalatorControl::On_bOK_Click(wxCommandEvent& event)
{
}
