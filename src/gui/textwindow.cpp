/*
	Skyscraper 2.0 Alpha - Text Dialog
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

#include "textwindow.h"

//(*InternalHeaders(TextWindow)
#include <wx/intl.h>
#include <wx/string.h>
//*)

namespace Skyscraper {

//(*IdInit(TextWindow)
const long TextWindow::ID_tMain = wxNewId();
const long TextWindow::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(TextWindow,wxDialog)
	//(*EventTable(TextWindow)
	//*)
END_EVENT_TABLE()

TextWindow::TextWindow(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(TextWindow)
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(2, 1, 0, 0);
	tMain = new wxTextCtrl(this, ID_tMain, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_tMain"));
	tMain->SetMinSize(wxSize(400,400));
	FlexGridSizer1->Add(tMain, 1, wxALL|wxEXPAND, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	FlexGridSizer1->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();

	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TextWindow::On_bOK_Click);
	//*)
}

TextWindow::~TextWindow()
{
	//(*Destroy(TextWindow)
	//*)
}


void TextWindow::On_bOK_Click(wxCommandEvent& event)
{
	this->Close();
}

}
