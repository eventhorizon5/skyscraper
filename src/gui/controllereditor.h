/*
    Skyscraper 2.1 - Controller Editor
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

#ifndef CONTROLLEREDITOR_H
#define CONTROLLEREDITOR_H

//(*Headers(ControllerEditor)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class ControllerEditor: public wxDialog
{
    public:

        ControllerEditor(wxWindow* parent,wxWindowID id=wxID_ANY);
        virtual ~ControllerEditor();
        void Loop();

        //(*Declarations(ControllerEditor)
        wxButton* bSetReprocess;
        wxListBox* lCallStations;
        wxListBox* lControllers;
        wxListBox* lElevators;
        wxListBox* lRoutes;
        wxSlider* sFireService;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* tBottomFloor;
        wxTextCtrl* tDD;
        wxTextCtrl* tHybrid;
        wxTextCtrl* tMaxPassengers;
        wxTextCtrl* tRange;
        wxTextCtrl* tRecallFloor;
        wxTextCtrl* tReprocess;
        wxTextCtrl* tTopFloor;
        //*)

    protected:

        //(*Identifiers(ControllerEditor)
        static const wxWindowID ID_lControllers;
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_tDD;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_tHybrid;
        static const wxWindowID ID_STATICTEXT3;
        static const wxWindowID ID_tRange;
        static const wxWindowID ID_STATICTEXT4;
        static const wxWindowID ID_tMaxPassengers;
        static const wxWindowID ID_STATICTEXT5;
        static const wxWindowID ID_tReprocess;
        static const wxWindowID ID_bSetReprocess;
        static const wxWindowID ID_STATICTEXT6;
        static const wxWindowID ID_sFireService;
        static const wxWindowID ID_STATICTEXT7;
        static const wxWindowID ID_tRecallFloor;
        static const wxWindowID ID_STATICTEXT8;
        static const wxWindowID ID_tBottomFloor;
        static const wxWindowID ID_STATICTEXT9;
        static const wxWindowID ID_tTopFloor;
        static const wxWindowID ID_lElevators;
        static const wxWindowID ID_lCallStations;
        static const wxWindowID ID_lRoutes;
        //*)

    private:

        //(*Handlers(ControllerEditor)
        //*)

        void BuildList(bool restore_selection = false);
        void UpdateRunState();

        SBS::SBS* Simcore;
        DebugPanel* panel;
        int lastcount;

        DECLARE_EVENT_TABLE()
};

}

#endif
