/*
	Skyscraper 2.1 - Script Console
	Copyright (C)2003-2024 Ryan Thoryk
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

#include <mutex>
#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "scriptproc.h"
#include "vm.h"
#include "enginecontext.h"
#include "console.h"

//(*InternalHeaders(Console)
#include <wx/intl.h>
#include <wx/string.h>
//*)

namespace Skyscraper {

//(*IdInit(Console)
const long Console::ID_tConsole = wxNewId();
const long Console::ID_tCommand = wxNewId();
const long Console::ID_bSend = wxNewId();
const long Console::ID_bClear = wxNewId();
const long Console::ID_chkEcho = wxNewId();
const long Console::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Console,wxFrame)
	//(*EventTable(Console)
	//*)
END_EVENT_TABLE()

std::mutex mtx;

Console::Console(VM *root, wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Console)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;

	Create(parent, id, _("Console"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxPoint(0,0));
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableRow(0);
	Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	FlexGridSizer3 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer3->AddGrowableCol(0);
	FlexGridSizer3->AddGrowableRow(0);
	tConsole = new wxTextCtrl(Panel1, ID_tConsole, wxEmptyString, wxDefaultPosition, wxSize(600,400), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxVSCROLL|wxHSCROLL, wxDefaultValidator, _T("ID_tConsole"));
	FlexGridSizer3->Add(tConsole, 1, wxALL|wxEXPAND, 5);
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer2->AddGrowableCol(0);
	FlexGridSizer2->AddGrowableRow(0);
	tCommand = new wxTextCtrl(Panel1, ID_tCommand, wxEmptyString, wxDefaultPosition, wxSize(500,100), wxTE_MULTILINE, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer2->Add(tCommand, 1, wxALL|wxEXPAND, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	bSend = new wxButton(Panel1, ID_bSend, _("Send"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSend"));
	BoxSizer1->Add(bSend, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_LEFT, 5);
	bClear = new wxButton(Panel1, ID_bClear, _("Clear"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bClear"));
	BoxSizer1->Add(bClear, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	chkEcho = new wxCheckBox(Panel1, ID_chkEcho, _("Echo"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkEcho"));
	chkEcho->SetValue(true);
	BoxSizer1->Add(chkEcho, 1, wxALL|wxALIGN_LEFT, 5);
	FlexGridSizer2->Add(BoxSizer1, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3->Add(FlexGridSizer2, 1, wxEXPAND, 5);
	Panel1->SetSizer(FlexGridSizer3);
	FlexGridSizer1->Add(Panel1, 1, wxEXPAND, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_bSend,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Console::On_bSend_Click);
	Connect(ID_bClear,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Console::On_bClear_Click);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Console::On_Close);
	//*)
	Simcore = 0;
	this->vm = root;
}

Console::~Console()
{
	//(*Destroy(Console)
	//*)
}

void Console::On_bSend_Click(wxCommandEvent& event)
{
	Simcore = vm->GetActiveSystem();

	if (!Simcore)
		return;

	Simcore->DeleteColliders = true;
	ScriptProcessor *processor = vm->GetActiveScriptProcessor();

	if (!processor)
		return;

	//load new commands into script interpreter, and run
	processor->LoadFromText(std::string(tCommand->GetValue()));
	if (chkEcho->GetValue() == true)
		Write(tCommand->GetValue().ToStdString());
	tCommand->Clear();
}

void Console::On_bClose_Click(wxCommandEvent& event)
{
	this->Close();
}

void Console::On_Close(wxCloseEvent& event)
{
	this->Hide();
}

void Console::Write(const std::string &message)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//due to hangs, don't allow Windows to write to the console from multiple threads
	if (mtx.try_lock() == false)
		return;
#else
	mtx.lock();
#endif
	tConsole->AppendText(message + wxT("\n"));
	tConsole->SetInsertionPointEnd();
	mtx.unlock();
}

void Console::On_bClear_Click(wxCommandEvent& event)
{
	tConsole->Clear();
}

}

