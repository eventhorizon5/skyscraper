/*
	Skyscraper 1.12 Alpha - People Manager Dialog
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

#ifndef PEOPLEMANAGER_H
#define PEOPLEMANAGER_H

//(*Headers(PeopleManager)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class PeopleManager: public wxDialog
{
	public:

		PeopleManager(DebugPanel* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PeopleManager();

		//(*Declarations(PeopleManager)
		wxButton* bDelete;
		wxButton* bGo;
		wxButton* bNew;
		wxButton* bOK;
		wxButton* bSetFloor;
		wxButton* bSetName;
		wxButton* bStop;
		wxCheckBox* chkRandom;
		wxCheckBox* chkService;
		wxListBox* PersonList;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText5;
		wxTextCtrl* tDestination;
		wxTextCtrl* tFloor;
		wxTextCtrl* tName;
		wxTextCtrl* tRouteActive;
		wxTextCtrl* tStatus;
		//*)
		void Loop();

	protected:

		//(*Identifiers(PeopleManager)
		static const long ID_PersonList;
		static const long ID_STATICTEXT1;
		static const long ID_tName;
		static const long ID_bSetName;
		static const long ID_STATICTEXT2;
		static const long ID_tFloor;
		static const long ID_bSetFloor;
		static const long ID_STATICTEXT3;
		static const long ID_tDestination;
		static const long ID_bGo;
		static const long ID_STATICTEXT5;
		static const long ID_tRouteActive;
		static const long ID_bStop;
		static const long ID_STATICLINE1;
		static const long ID_STATICLINE2;
		static const long ID_STATICLINE3;
		static const long ID_tStatus;
		static const long ID_chkRandom;
		static const long ID_chkService;
		static const long ID_bNew;
		static const long ID_bDelete;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(PeopleManager)
		void On_bSetName_Click(wxCommandEvent& event);
		void On_bSetFloor_Click(wxCommandEvent& event);
		void On_bGo_Click(wxCommandEvent& event);
		void On_bNew_Click(wxCommandEvent& event);
		void On_bDelete_Click(wxCommandEvent& event);
		void On_bOK_Click(wxCommandEvent& event);
		void On_chkRandom_Click(wxCommandEvent& event);
		void On_chkService_Click(wxCommandEvent& event);
		void On_bStop_Click(wxCommandEvent& event);
		//*)
		void BuildList(bool restore_selection = false);
		SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;
		SBS::Person *person;
		int floor;
		int dest_floor;
		std::string status;

		DECLARE_EVENT_TABLE()
};

}

#endif

