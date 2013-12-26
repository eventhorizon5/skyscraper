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

//(*InternalHeaders(Console)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(Console)
const long Console::ID_tCommand = wxNewId();
const long Console::ID_bSend = wxNewId();
const long Console::ID_bClose = wxNewId();
//*)

BEGIN_EVENT_TABLE(Console,wxDialog)
	//(*EventTable(Console)
	//*)
END_EVENT_TABLE()

Console::Console(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Console)
	wxFlexGridSizer* FlexGridSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	
	Create(parent, id, _("Script Console"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	FlexGridSizer1->AddGrowableCol(0);
	FlexGridSizer1->AddGrowableCol(1);
	FlexGridSizer1->AddGrowableRow(0);
	tCommand = new wxTextCtrl(this, ID_tCommand, wxEmptyString, wxDefaultPosition, wxSize(400,100), wxTE_MULTILINE|wxHSCROLL|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_tCommand"));
	FlexGridSizer1->Add(tCommand, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxVERTICAL);
	bSend = new wxButton(this, ID_bSend, _("Send"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSend"));
	BoxSizer2->Add(bSend, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bClose = new wxButton(this, ID_bClose, _("Close"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bClose"));
	BoxSizer2->Add(bClose, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_bSend,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Console::On_bSend_Click);
	Connect(ID_bClose,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Console::On_bClose_Click);
	//*)
}

Console::~Console()
{
	//(*Destroy(Console)
	//*)
}


void Console::On_bSend_Click(wxCommandEvent& event)
{
	Simcore->RecreateColliders = true;
	ScriptProcessor *processor = skyscraper->GetScriptProcessor();

	if (!processor)
		return;

	//load new commands into script interpreter, and run
	processor->LoadFromText(tCommand->GetValue().ToAscii());
	processor->Run();

	Simcore->Prepare();
	Simcore->RecreateColliders = false;
}

void Console::On_bClose_Click(wxCommandEvent& event)
{
	this->Close();
}
