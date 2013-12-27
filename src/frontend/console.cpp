/* $Id$ */

/*
	Skyscraper 1.9 Alpha - Script Console
	Copyright (C)2003-2013 Ryan Thoryk
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

#include "console.h"
#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "fileio.h"
#include "skyscraper.h"

extern SBS *Simcore;
extern Skyscraper *skyscraper;
extern Console *console;

//(*InternalHeaders(Console)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(Console)
const long Console::ID_tConsole = wxNewId();
const long Console::ID_tCommand = wxNewId();
const long Console::ID_bSend = wxNewId();
const long Console::ID_chkEcho = wxNewId();
//*)

BEGIN_EVENT_TABLE(Console,wxFrame)
	//(*EventTable(Console)
	//*)
END_EVENT_TABLE()

Console::Console(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Console)
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer1;

	Create(parent, id, _("Console"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxPoint(0,0));
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	FlexGridSizer1->AddGrowableCol(1);
	FlexGridSizer1->AddGrowableRow(1);
	tConsole = new wxTextCtrl(this, ID_tConsole, wxEmptyString, wxDefaultPosition, wxSize(600,400), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tConsole"));
	FlexGridSizer1->Add(tConsole, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer2 = new wxFlexGridSizer(1, 2, 0, 0);
	FlexGridSizer2->AddGrowableRow(1);
	tCommand = new wxTextCtrl(this, ID_tCommand, wxEmptyString, wxDefaultPosition, wxSize(500,100), wxTE_MULTILINE, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer2->Add(tCommand, 1, wxALL|wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	bSend = new wxButton(this, ID_bSend, _("Send"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSend"));
	BoxSizer1->Add(bSend, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	chkEcho = new wxCheckBox(this, ID_chkEcho, _("Echo"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkEcho"));
	chkEcho->SetValue(false);
	BoxSizer1->Add(chkEcho, 1, wxALL|wxALIGN_TOP|wxALIGN_BOTTOM, 5);
	FlexGridSizer2->Add(BoxSizer1, 1, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(FlexGridSizer2, 1, wxEXPAND|wxALIGN_LEFT|wxALIGN_TOP, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);

	Connect(ID_bSend,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Console::On_bSend_Click);
	Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&Console::On_Close);
	//*)
}

Console::~Console()
{
	//(*Destroy(Console)
	//*)
}

void Console::On_bSend_Click(wxCommandEvent& event)
{
	if (!Simcore)
		return;

	Simcore->DeleteColliders = true;
	ScriptProcessor *processor = skyscraper->GetScriptProcessor();

	if (!processor)
		return;

	//load new commands into script interpreter, and run
	processor->LoadFromText(tCommand->GetValue().ToAscii());
	if (chkEcho->GetValue() == true)
		tConsole->AppendText(tCommand->GetValue());
	processor->Run();
	Simcore->Prepare(false);
	Simcore->DeleteColliders = false;
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

