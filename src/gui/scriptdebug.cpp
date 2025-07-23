/*
    Skyscraper 2.1 - Script Debugger
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

//(*InternalHeaders(ScriptDebug)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "gui.h"
#include "debugpanel.h"
#include "scriptproc.h"
#include "scriptdebug.h"

namespace Skyscraper {

//(*IdInit(ScriptDebug)
const wxWindowID ScriptDebug::ID_lstScript = wxNewId();
const wxWindowID ScriptDebug::ID_STATICTEXT1 = wxNewId();
const wxWindowID ScriptDebug::ID_txtFilename = wxNewId();
const wxWindowID ScriptDebug::ID_STATICTEXT2 = wxNewId();
const wxWindowID ScriptDebug::ID_txtLine = wxNewId();
const wxWindowID ScriptDebug::ID_STATICTEXT3 = wxNewId();
const wxWindowID ScriptDebug::ID_bGoto = wxNewId();
const wxWindowID ScriptDebug::ID_bRun = wxNewId();
const wxWindowID ScriptDebug::ID_bStart = wxNewId();
const wxWindowID ScriptDebug::ID_bStop = wxNewId();
const wxWindowID ScriptDebug::ID_bReset = wxNewId();
const wxWindowID ScriptDebug::ID_lstVariables = wxNewId();
//*)

BEGIN_EVENT_TABLE(ScriptDebug,wxDialog)
    //(*EventTable(ScriptDebug)
    //*)
END_EVENT_TABLE()

ScriptDebug::ScriptDebug(DebugPanel* root, wxWindow* parent)
{
    //(*Initialize(ScriptDebug)
    wxFlexGridSizer* FlexGridSizer1;
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer3;
    wxFlexGridSizer* FlexGridSizer4;
    wxFlexGridSizer* FlexGridSizer5;

    Create(parent, wxID_ANY, _("Script Debugger"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    lstScript = new wxEditableListBox(this, ID_lstScript, _("Script Code"), wxDefaultPosition, wxDefaultSize, wxEL_ALLOW_NEW|wxEL_ALLOW_EDIT|wxEL_ALLOW_DELETE, _T("ID_lstScript"));
    FlexGridSizer2->Add(lstScript, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtFilename = new wxTextCtrl(this, ID_txtFilename, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtFilename"));
    FlexGridSizer3->Add(txtFilename, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Line:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtLine = new wxTextCtrl(this, ID_txtLine, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtLine"));
    FlexGridSizer3->Add(txtLine, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Label"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
    bGoto = new wxButton(this, ID_bGoto, _("Goto Line"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bGoto"));
    FlexGridSizer4->Add(bGoto, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bRun = new wxButton(this, ID_bRun, _("Run"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bRun"));
    FlexGridSizer4->Add(bRun, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bStart = new wxButton(this, ID_bStart, _("Start"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStart"));
    FlexGridSizer4->Add(bStart, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bStop = new wxButton(this, ID_bStop, _("Stop"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bStop"));
    FlexGridSizer4->Add(bStop, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bReset = new wxButton(this, ID_bReset, _("Reset"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReset"));
    FlexGridSizer4->Add(bReset, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
    lstVariables = new wxEditableListBox(this, ID_lstVariables, _("Variables"), wxDefaultPosition, wxDefaultSize, wxEL_ALLOW_NEW|wxEL_ALLOW_EDIT|wxEL_ALLOW_DELETE, _T("ID_lstVariables"));
    FlexGridSizer5->Add(lstVariables, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_bGoto, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScriptDebug::On_bGoto_Click);
    Connect(ID_bRun, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScriptDebug::On_bRun_Click);
    Connect(ID_bStart, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScriptDebug::On_bStart_Click);
    Connect(ID_bStop, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScriptDebug::On_bStop_Click);
    Connect(ID_bReset, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&ScriptDebug::On_bReset_Click);
    //*)
    this->panel = root;
    OnInit();
}

ScriptDebug::~ScriptDebug()
{
    //(*Destroy(ScriptDebug)
    //*)
}

void ScriptDebug::OnInit()
{
    Simcore = panel->GetSystem();
}

void ScriptDebug::Loop()
{
	//if active engine has changed, refresh values
	if (Simcore != panel->GetSystem())
		OnInit();

    scriptproc = panel->GetScriptProcessor();

    if (!Simcore || !scriptproc)
		return;

}

void ScriptDebug::On_bGoto_Click(wxCommandEvent& event)
{
}

void ScriptDebug::On_bRun_Click(wxCommandEvent& event)
{

}

void ScriptDebug::On_bStart_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->Start();
}

void ScriptDebug::On_bStop_Click(wxCommandEvent& event)
{

}

void ScriptDebug::On_bReset_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->Reset();
}

}
