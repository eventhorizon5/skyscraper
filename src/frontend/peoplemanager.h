/* $Id$ */

/*
	Skyscraper 1.10 Alpha - People Manager Dialog
	Copyright (C)2003-2015 Ryan Thoryk
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

#ifndef PEOPLEMANAGER_H
#define PEOPLEMANAGER_H

//(*Headers(PeopleManager)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
//*)

namespace Skyscraper {

class PeopleManager: public wxDialog
{
	public:

		PeopleManager(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~PeopleManager();

		//(*Declarations(PeopleManager)
		wxButton* bGo;
		wxStaticLine* StaticLine2;
		wxButton* bSetName;
		wxCheckBox* chkService;
		wxButton* bStop;
		wxButton* bNew;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* bSetFloor;
		wxStaticLine* StaticLine1;
		wxTextCtrl* tRouteActive;
		wxStaticLine* StaticLine3;
		wxTextCtrl* tDestination;
		wxTextCtrl* tName;
		wxButton* bDelete;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxButton* bOK;
		wxTextCtrl* tFloor;
		wxListBox* PersonList;
		wxCheckBox* chkRandom;
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
		int lastcount;
		SBS::Person *person;
		int floor;
		int dest_floor;
		std::string status;

		DECLARE_EVENT_TABLE()
};

}

#endif

