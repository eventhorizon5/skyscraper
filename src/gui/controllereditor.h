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
#include <wx/tglbtn.h>
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
        wxButton* bPressDown;
        wxButton* bPressUp;
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
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText12;
        wxStaticText* StaticText13;
        wxStaticText* StaticText14;
        wxStaticText* StaticText15;
        wxStaticText* StaticText16;
        wxStaticText* StaticText17;
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
        wxTextCtrl* tDestFloor;
        wxTextCtrl* tDestination;
        wxTextCtrl* tDirection;
        wxTextCtrl* tDownStatus;
        wxTextCtrl* tMaxPassengers;
        wxTextCtrl* tProcessed;
        wxTextCtrl* tRange;
        wxTextCtrl* tRecallFloor;
        wxTextCtrl* tReprocess;
        wxTextCtrl* tRequests;
        wxTextCtrl* tRouteElevator;
        wxTextCtrl* tStartingFloor;
        wxTextCtrl* tStationFloor;
        wxTextCtrl* tTopFloor;
        wxTextCtrl* tUpStatus;
        wxToggleButton* tDD;
        wxToggleButton* tHybrid;
        //*)

    protected:

        //(*Identifiers(ControllerEditor)
        static const long ID_lControllers;
        static const long ID_tDD;
        static const long ID_tHybrid;
        static const long ID_STATICTEXT3;
        static const long ID_tRange;
        static const long ID_STATICTEXT4;
        static const long ID_tMaxPassengers;
        static const long ID_STATICTEXT5;
        static const long ID_tReprocess;
        static const long ID_bSetReprocess;
        static const long ID_STATICTEXT6;
        static const long ID_sFireService;
        static const long ID_STATICTEXT7;
        static const long ID_tRecallFloor;
        static const long ID_STATICTEXT8;
        static const long ID_tBottomFloor;
        static const long ID_STATICTEXT9;
        static const long ID_tTopFloor;
        static const long ID_lElevators;
        static const long ID_bRemoveElevator;
        static const long ID_bResetArrival;
        static const long ID_tAddElevator;
        static const long ID_bAddElevator;
        static const long ID_STATICLINE1;
        static const long ID_STATICLINE2;
        static const long ID_STATICLINE3;
        static const long ID_bCallUp;
        static const long ID_bCall;
        static const long ID_lCallStations;
        static const long ID_STATICTEXT1;
        static const long ID_tStationFloor;
        static const long ID_STATICTEXT2;
        static const long ID_tUpStatus;
        static const long ID_STATICTEXT17;
        static const long ID_tDownStatus;
        static const long ID_bPressUp;
        static const long ID_bPressDown;
        static const long ID_lRoutes;
        static const long ID_STATICTEXT10;
        static const long ID_tStartingFloor;
        static const long ID_STATICTEXT11;
        static const long ID_tDestFloor;
        static const long ID_STATICTEXT12;
        static const long ID_tDirection;
        static const long ID_STATICTEXT13;
        static const long ID_tRequests;
        static const long ID_STATICTEXT14;
        static const long ID_tProcessed;
        static const long ID_STATICTEXT15;
        static const long ID_tRouteElevator;
        static const long ID_STATICTEXT16;
        static const long ID_tDestination;
        //*)

    private:

        //(*Handlers(ControllerEditor)
        void On_bAddElevator_Click(wxCommandEvent& event);
        void On_bRemoveElevator_Click(wxCommandEvent& event);
        void On_bResetArrival_Click(wxCommandEvent& event);
        void On_bCall_Click(wxCommandEvent& event);
        void On_bCallUp_Click(wxCommandEvent& event);
        void On_bPressUp_Click(wxCommandEvent& event);
        void On_bPressDown_Click(wxCommandEvent& event);
        void On_tDD_Toggle(wxCommandEvent& event);
        void On_tHybrid_Toggle(wxCommandEvent& event);
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
        bool full_update;

        DECLARE_EVENT_TABLE()
};

}

#endif
