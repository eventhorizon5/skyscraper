/*
	Skyscraper 1.11 Alpha - Profiler GUI
	Copyright (C)2003-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#include "globals.h"
#include "sbs.h"
#include "profilergui.h"
#include "profiler.h"
#include "debugpanel.h"
#include "skyscraper.h"

//(*InternalHeaders(Profiler)
#include <wx/intl.h>
#include <wx/string.h>
//*)

namespace Skyscraper {

//(*IdInit(Profiler)
const long Profiler::ID_chkAdvanced = wxNewId();
const long Profiler::ID_chkCapture = wxNewId();
const long Profiler::ID_txtMain = wxNewId();
//*)

BEGIN_EVENT_TABLE(Profiler,wxDialog)
	//(*EventTable(Profiler)
	//*)
END_EVENT_TABLE()

Profiler::Profiler(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(Profiler)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, wxID_ANY, _("Profiler"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	chkAdvanced = new wxCheckBox(this, ID_chkAdvanced, _("Advanced Profiling"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkAdvanced"));
	chkAdvanced->SetValue(false);
	BoxSizer2->Add(chkAdvanced, 1, wxALIGN_TOP, 5);
	chkCapture = new wxCheckBox(this, ID_chkCapture, _("Capture"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_chkCapture"));
	chkCapture->SetValue(true);
	BoxSizer2->Add(chkCapture, 1, wxALIGN_TOP, 5);
	FlexGridSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
	txtMain = new wxTextCtrl(this, ID_txtMain, wxEmptyString, wxDefaultPosition, wxSize(640,480), wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL, wxDefaultValidator, _T("ID_txtMain"));
	FlexGridSizer1->Add(txtMain, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(FlexGridSizer1, 1, wxALIGN_TOP, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	//*)
	advanced = false;
	count = 0;
}

Profiler::~Profiler()
{
	SBS::ProfileManager::CleanupMemory();

	//(*Destroy(Profiler)
	//*)
}

void Profiler::Loop()
{
	if (advanced != chkAdvanced->GetValue())
		SBS::ProfileManager::CleanupMemory();

	advanced = chkAdvanced->GetValue();
	SBS::enable_advanced_profiling = advanced;

	if (count == 0)
	{
		std::string output;

		/*SBS::ProfileIterator* profileIterator = SBS::ProfileManager::Get_Iterator();
		SBS::ProfileManager::dumpRecursive(output, profileIterator, 0);
		SBS::ProfileManager::Release_Iterator(profileIterator);*/

		if (chkCapture->GetValue() == true)
		{
			SBS::ProfileManager::dumpAll(output);
			txtMain->SetValue(output);
		}
	}
	count++;

	if (count == 10)
		count = 0;
}

void Profiler::OnResize(wxSizeEvent& event)
{
}

}

