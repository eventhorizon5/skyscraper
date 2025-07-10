/*
	Skyscraper 2.1 - Escalator Control
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef REVDOORCONTROL_H
#define REVDOORCONTROL_H

//(*Headers(RevolvingDoorControl)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
//*)

namespace Skyscraper
{

class RevolvingDoorControl: public wxDialog
{
    public:

        RevolvingDoorControl(DebugPanel* parent, wxWindowID id=wxID_ANY);
        virtual ~RevolvingDoorControl();
        void Loop();

        //(*Declarations(RevolvingDoorControl)
        wxButton* bOK;
        wxButton* bSet;
        wxListBox* ListBox1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* txtState;
        wxTextCtrl* txtName;
        wxTextCtrl* txtParent;
        wxTextCtrl* txtSpeed;
        wxToggleButton* tRun;
        //*)

    protected:

        //(*Identifiers(RevolvingDoorControl)
        static const wxWindowID ID_LISTBOX1;
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_txtName;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_txtParent;
        static const wxWindowID ID_txtState;
        static const wxWindowID ID_tRun;
        static const wxWindowID ID_STATICTEXT3;
        static const wxWindowID ID_txtSpeed;
        static const wxWindowID ID_bSet;
        static const wxWindowID ID_bOK;
        //*)

    private:

        //(*Handlers(RevolvingDoorControl)
        void On_chkRun_Click(wxCommandEvent& event);
        void On_bOK_Click(wxCommandEvent& event);
        void On_Slider1_Scroll(wxCommandEvent& event);
        void On_tRun_Toggle(wxCommandEvent& event);
        void On_bSet_Click(wxCommandEvent& event);
        //*)
        void BuildList(bool restore_selection = false);
        void UpdateRunState();

		SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;
		SBS::RevolvingDoor *door;

        DECLARE_EVENT_TABLE()
};

}

#endif
