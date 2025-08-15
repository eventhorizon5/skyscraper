/*
	Skyscraper 2.1 - Floor Information Dialog
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

#ifndef FLOORINFO_H
#define FLOORINFO_H

//(*Headers(FloorInfo)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class FloorInfo: public wxDialog
{
    public:

        FloorInfo(DebugPanel* parent,wxWindowID id=wxID_ANY);
        virtual ~FloorInfo();
        void Loop();

        //(*Declarations(FloorInfo)
        wxButton* bOK;
        wxListBox* lstFloors;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* txtAltitude;
        wxTextCtrl* txtDescription;
        wxTextCtrl* txtFloorType;
        wxTextCtrl* txtHeight;
        wxTextCtrl* txtID;
        wxTextCtrl* txtIntHeight;
        wxTextCtrl* txtName;
        wxTextCtrl* txtNumber;
        wxTextCtrl* txtNumberID;
        //*)

    protected:

        //(*Identifiers(FloorInfo)
        static const long ID_lstFloors;
        static const long ID_STATICTEXT1;
        static const long ID_txtNumber;
        static const long ID_STATICTEXT2;
        static const long ID_txtName;
        static const long ID_STATICTEXT3;
        static const long ID_txtID;
        static const long ID_STATICTEXT4;
        static const long ID_txtNumberID;
        static const long ID_STATICTEXT5;
        static const long ID_txtFloorType;
        static const long ID_STATICTEXT6;
        static const long ID_txtDescription;
        static const long ID_STATICTEXT7;
        static const long ID_txtAltitude;
        static const long ID_STATICTEXT8;
        static const long ID_txtHeight;
        static const long ID_STATICTEXT9;
        static const long ID_txtIntHeight;
        static const long ID_bOK;
        //*)

    private:

        //(*Handlers(FloorInfo)
        void On_bOK_Click(wxCommandEvent& event);
        //*)
        void BuildList(bool restore_selection = false);

        SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;
		SBS::Floor *floor;

        DECLARE_EVENT_TABLE()
};

}

#endif
