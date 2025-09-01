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
#include "section.h"
#include "scriptdebug.h"

namespace Skyscraper {

//(*IdInit(ScriptDebug)
const long ScriptDebug::ID_lstScript = wxNewId();
const long ScriptDebug::ID_STATICTEXT1 = wxNewId();
const long ScriptDebug::ID_txtFilename = wxNewId();
const long ScriptDebug::ID_STATICTEXT2 = wxNewId();
const long ScriptDebug::ID_txtLine = wxNewId();
const long ScriptDebug::ID_STATICTEXT3 = wxNewId();
const long ScriptDebug::ID_txtSection = wxNewId();
const long ScriptDebug::ID_lstFunctions = wxNewId();
const long ScriptDebug::ID_bGoto = wxNewId();
const long ScriptDebug::ID_bRun = wxNewId();
const long ScriptDebug::ID_bStart = wxNewId();
const long ScriptDebug::ID_bStop = wxNewId();
const long ScriptDebug::ID_bReset = wxNewId();
const long ScriptDebug::ID_lstVariables = wxNewId();
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
    wxFlexGridSizer* FlexGridSizer6;
    wxFlexGridSizer* FlexGridSizer7;
    wxStaticBoxSizer* StaticBoxSizer1;
    wxStaticBoxSizer* StaticBoxSizer2;
    wxStaticBoxSizer* StaticBoxSizer3;

    Create(parent, wxID_ANY, _("Script Debugger"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
    FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Script"));
    lstScript = new wxListBox(this, ID_lstScript, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_HSCROLL, wxDefaultValidator, _T("ID_lstScript"));
    lstScript->SetMinSize(wxSize(450,400));
    StaticBoxSizer1->Add(lstScript, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer6 = new wxFlexGridSizer(0, 1, 0, 0);
    FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Filename:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    FlexGridSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtFilename = new wxTextCtrl(this, ID_txtFilename, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtFilename"));
    FlexGridSizer3->Add(txtFilename, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Line:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtLine = new wxTextCtrl(this, ID_txtLine, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtLine"));
    FlexGridSizer3->Add(txtLine, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Section:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    FlexGridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    txtSection = new wxTextCtrl(this, ID_txtSection, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY, wxDefaultValidator, _T("ID_txtSection"));
    FlexGridSizer3->Add(txtSection, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer6->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer7 = new wxFlexGridSizer(0, 3, 0, 0);
    StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Functions"));
    lstFunctions = new wxListBox(this, ID_lstFunctions, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_lstFunctions"));
    lstFunctions->SetMinSize(wxSize(200,200));
    StaticBoxSizer3->Add(lstFunctions, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer7->Add(StaticBoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer6->Add(FlexGridSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Variables"));
    lstVariables = new wxListBox(this, ID_lstVariables, wxDefaultPosition, wxDefaultSize, 0, 0, wxLB_HSCROLL, wxDefaultValidator, _T("ID_lstVariables"));
    lstVariables->SetMinSize(wxSize(200,200));
    StaticBoxSizer2->Add(lstVariables, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer5->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(FlexGridSizer1);
    FlexGridSizer1->SetSizeHints(this);

    Connect(ID_bGoto,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScriptDebug::On_bGoto_Click);
    Connect(ID_bRun,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScriptDebug::On_bRun_Click);
    Connect(ID_bStart,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScriptDebug::On_bStart_Click);
    Connect(ID_bStop,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScriptDebug::On_bStop_Click);
    Connect(ID_bReset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ScriptDebug::On_bReset_Click);
    //*)
    this->panel = root;
    variable_count = 0;
    function_count = 0;
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
    scriptproc = panel->GetScriptProcessor();

    if (!Simcore || !scriptproc)
		return;

    std::vector<std::string> *data = scriptproc->GetBuildingData();

    for (int i = 0; i < data->size(); i++)
    {
        lstScript->Append(SBS::ToString(i + 1) + ": " + data->at(i));
    }
}

void ScriptDebug::Loop()
{
	//if active engine has changed, refresh values
	if (Simcore != panel->GetSystem())
		OnInit();

    if (!Simcore || !scriptproc)
		return;

    txtLine->SetValue(SBS::ToString(scriptproc->line + 1));
    txtFilename->SetValue(Simcore->BuildingFilename);

    if (scriptproc->GetConfigHandler()->SectionNum == SECTION_NONE)
        txtSection->SetValue("None");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_GLOBAL)
        txtSection->SetValue("Global");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_FLOOR)
        txtSection->SetValue("Floor");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_BUILDINGS)
        txtSection->SetValue("Buildings");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_ELEVATOR)
        txtSection->SetValue("Elevator");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_TEXTURE)
        txtSection->SetValue("Texture");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_ELEVATORCAR)
        txtSection->SetValue("Elevator Car");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_VEHICLE)
        txtSection->SetValue("Vehicle");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_CONTROLLER)
        txtSection->SetValue("Controller");
    else if (scriptproc->GetConfigHandler()->SectionNum == SECTION_CALLSTATION)
        txtSection->SetValue("CallStation");
    else
        txtSection->SetValue("Error");

    size_t temp_count = scriptproc->variables.size();
    if (variable_count != temp_count)
    {
        variable_count = temp_count;
        for (size_t i = 0; i < variable_count; i++)
        {
            std::string name = scriptproc->variables[i].name;
            std::string value = scriptproc->variables[i].value;
            lstVariables->Append(SBS::ToString(i + 1) + ": " + name + " - " + value);
        }
    }

    temp_count = scriptproc->GetFunctionCount();
    if (function_count != temp_count)
    {
        function_count = temp_count;
        for (size_t i = 0; i < function_count; i++)
        {
            ScriptProcessor::FunctionInfo info = scriptproc->GetFunctionInfo(i);
            lstFunctions->Append(SBS::ToString(i + 1) + ": " + info.name + " - line " + SBS::ToString(info.line + 1));
        }
    }
}

void ScriptDebug::On_bGoto_Click(wxCommandEvent& event)
{
    int selection = lstScript->GetSelection();
    if (selection == wxNOT_FOUND)
        return;

    if (scriptproc)
        scriptproc->line = selection;
}

void ScriptDebug::On_bRun_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->Reset(false);
}

void ScriptDebug::On_bStart_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->Start();
}

void ScriptDebug::On_bStop_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->line = -1;
}

void ScriptDebug::On_bReset_Click(wxCommandEvent& event)
{
    if (scriptproc)
        scriptproc->Reset();
}

}
