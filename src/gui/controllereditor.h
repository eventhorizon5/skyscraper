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
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class ControllerEditor: public wxDialog
{
    public:

        ControllerEditor(DebugPanel* parent,wxWindowID id=wxID_ANY);
        virtual ~ControllerEditor();
        void Loop();

        //(*Declarations(ControllerEditor)
        wxButton* bAddElevator;
        wxButton* bCall;
        wxButton* bCallUp;
        wxButton* bRemoveElevator;
        wxButton* bResetArrival;
        wxButton* bSetReprocess;
        wxListBox* lCallStations;
        wxListBox* lControllers;
        wxListBox* lElevators;
        wxListBox* lRoutes;
        wxSlider* sFireService;
        wxStaticLine* StaticLine1;
        wxStaticLine* StaticLine2;
        wxStaticLine* StaticLine3;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* tAddElevator;
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
        static const wxWindowID ID_bRemoveElevator;
        static const wxWindowID ID_bResetArrival;
        static const wxWindowID ID_tAddElevator;
        static const wxWindowID ID_bAddElevator;
        static const wxWindowID ID_STATICLINE1;
        static const wxWindowID ID_STATICLINE2;
        static const wxWindowID ID_STATICLINE3;
        static const wxWindowID ID_bCallUp;
        static const wxWindowID ID_bCall;
        static const wxWindowID ID_lCallStations;
        static const wxWindowID ID_lRoutes;
        //*)

    private:

        //(*Handlers(ControllerEditor)
        void On_bAddElevator_Click(wxCommandEvent& event);
        void On_bRemoveElevator_Click(wxCommandEvent& event);
        void On_bResetArrival_Click(wxCommandEvent& event);
        void On_bCall_Click(wxCommandEvent& event);
        void On_bCallUp_Click(wxCommandEvent& event);
        //*)

        void BuildList(bool restore_selection = false);
        void BuildElevatorList(bool restore_selection = false);
        void BuildStationList(bool restore_selection = false);
        void BuildRouteList(bool restore_selection = false);

        SBS::SBS* Simcore;
        DebugPanel* panel;
        int lastcount;
        int lastcount_elev;
        int lastcount_station;
        int lastcount_route;
        SBS::DispatchController* controller;

        DECLARE_EVENT_TABLE()
};

}

#endif
