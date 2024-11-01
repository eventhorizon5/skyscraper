/*
	Skyscraper 2.1 - Escalator Control
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef ESCALATORCONTROL_H
#define ESCALATORCONTROL_H

//(*Headers(EscalatorControl)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper
{

class EscalatorControl: public wxDialog
{
    public:

        EscalatorControl(DebugPanel* parent, wxWindowID id=wxID_ANY);
        virtual ~EscalatorControl();
        void Loop();

        //(*Declarations(EscalatorControl)
        wxButton* bOK;
        wxListBox* ListBox1;
        wxSlider* Slider1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* txtRun;
        wxTextCtrl* txtName;
        wxTextCtrl* txtParent;
        //*)

    protected:

        //(*Identifiers(EscalatorControl)
        static const long ID_LISTBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_txtName;
        static const long ID_STATICTEXT2;
        static const long ID_txtParent;
        static const long ID_txtRun;
        static const long ID_SLIDER1;
        static const long ID_bOK;
        //*)

    private:

        //(*Handlers(EscalatorControl)
        void On_chkRun_Click(wxCommandEvent& event);
        void On_bOK_Click(wxCommandEvent& event);
        void On_Slider1_Scroll(wxCommandEvent& event);
        //*)
        void BuildList(bool restore_selection = false);
        void UpdateRunState();

		SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;
		SBS::Escalator *escalator;

        DECLARE_EVENT_TABLE()
};

}

#endif
