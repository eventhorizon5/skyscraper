/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Edit Elevator Form
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#ifndef EDITELEVATOR_H
#define EDITELEVATOR_H

//(*Headers(editelevator)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/scrolbar.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class editelevator: public wxDialog
{
	friend class Timer;
	public:
		editelevator(wxWindow* parent,wxWindowID id = -1);
		virtual ~editelevator();

		//(*Identifiers(editelevator)
		enum Identifiers
		{
		    ID_tElevator = 0x1000,
		    ID_sNumber,
		    ID_tFloor,
		    ID_sFloor,
		    ID_bDumpFloors,
		    ID_bDumpQueues,
		    ID_bCall,
		    ID_bGo,
		    ID_bOpen,
		    ID_bOpenManual,
		    ID_bStop,
		    ID_bEnqueueUp,
		    ID_bEnqueueDown,
		    ID_bClose,
		    ID_bCloseManual,
		    ID_bAlarm,
		    ID_STATICTEXT3,
		    ID_txtNumber,
		    ID_STATICTEXT5,
		    ID_txtName,
		    ID_bSetName,
		    ID_STATICTEXT4,
		    ID_txtEnabled,
		    ID_STATICTEXT6,
		    ID_txtShaft,
		    ID_STATICTEXT7,
		    ID_txtHeight,
		    ID_STATICTEXT8,
		    ID_txtDoorWidth,
		    ID_STATICTEXT9,
		    ID_txtDoorHeight,
		    ID_STATICTEXT10,
		    ID_txtDoorDirection,
		    ID_STATICTEXT11,
		    ID_txtDoorsOpen,
		    ID_STATICTEXT12,
		    ID_txtBrakes,
		    ID_STATICTEXT13,
		    ID_txtStop,
		    ID_STATICTEXT14,
		    ID_txtFloor,
		    ID_STATICTEXT15,
		    ID_txtPosition,
		    ID_STATICTEXT16,
		    ID_txtOrigin,
		    ID_STATICTEXT17,
		    ID_txtOriginFloor,
		    ID_STATICTEXT18,
		    ID_txtElevStart,
		    ID_STATICTEXT19,
		    ID_txtDoorOrigin,
		    ID_STATICTEXT20,
		    ID_txtShaftDoorOrigin,
		    ID_STATICTEXT38,
		    ID_txtQueueDirection,
		    ID_STATICTEXT39,
		    ID_txtQueuePause,
		    ID_STATICTEXT40,
		    ID_txtQueueLastUp,
		    ID_STATICTEXT41,
		    ID_txtQueueLastDown,
		    ID_STATICTEXT21,
		    ID_txtSpeed,
		    ID_bSetSpeed,
		    ID_STATICTEXT22,
		    ID_txtAcceleration,
		    ID_bSetAcceleration,
		    ID_STATICTEXT23,
		    ID_txtDeceleration,
		    ID_bSetDeceleration,
		    ID_STATICTEXT24,
		    ID_txtOpenSpeed,
		    ID_bSetOpenSpeed,
		    ID_STATICTEXT25,
		    ID_txtDoorAcceleration,
		    ID_bSetDoorAccel,
		    ID_STATICTEXT26,
		    ID_txtRate,
		    ID_STATICTEXT27,
		    ID_txtDirection,
		    ID_STATICTEXT28,
		    ID_txtDoorSpeed,
		    ID_STATICTEXT30,
		    ID_txtDestFloor,
		    ID_STATICTEXT31,
		    ID_txtMoveElevator,
		    ID_STATICTEXT32,
		    ID_txtMoveElevatorFloor,
		    ID_STATICTEXT33,
		    ID_txtDistance,
		    ID_STATICTEXT34,
		    ID_txtDestination,
		    ID_STATICTEXT35,
		    ID_txtStopDistance,
		    ID_STATICTEXT36,
		    ID_txtTempDecel,
		    ID_STATICTEXT37,
		    ID_txtErrorOffset
		};
		//*)

	protected:

		//(*Handlers(editelevator)
		void On_bCall_Click(wxCommandEvent& event);
		void On_bEnqueueUp_Click(wxCommandEvent& event);
		void On_bGo_Click(wxCommandEvent& event);
		void On_bEnqueueDown_Click(wxCommandEvent& event);
		void On_bOpen_Click(wxCommandEvent& event);
		void On_bClose_Click(wxCommandEvent& event);
		void On_bOpenManual_Click(wxCommandEvent& event);
		void On_bCloseManual_Click(wxCommandEvent& event);
		void On_bStop_Click(wxCommandEvent& event);
		void On_bAlarm_Click(wxCommandEvent& event);
		void On_bSetName_Click(wxCommandEvent& event);
		void On_bSetSpeed_Click(wxCommandEvent& event);
		void On_bSetAcceleration_Click(wxCommandEvent& event);
		void On_bSetDeceleration_Click(wxCommandEvent& event);
		void On_bSetOpenSpeed_Click(wxCommandEvent& event);
		void On_bSetDoorAccel_Click(wxCommandEvent& event);
		void On_bDumpFloors_Click(wxCommandEvent& event);
		void On_bDumpQueues_Click(wxCommandEvent& event);
		//*)
        void OnInit();

		//(*Declarations(editelevator)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxBoxSizer* BoxSizer6;
		wxStaticText* tElevator;
		wxScrollBar* sNumber;
		wxStaticText* tFloor;
		wxScrollBar* sFloor;
		wxBoxSizer* BoxSizer7;
		wxButton* bDumpFloors;
		wxButton* bDumpQueues;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxBoxSizer* BoxSizer8;
		wxButton* bCall;
		wxButton* bGo;
		wxButton* bOpen;
		wxButton* bOpenManual;
		wxButton* bStop;
		wxBoxSizer* BoxSizer9;
		wxButton* bEnqueueUp;
		wxButton* bEnqueueDown;
		wxButton* bClose;
		wxButton* bCloseManual;
		wxButton* bAlarm;
		wxBoxSizer* BoxSizer3;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxFlexGridSizer* FlexGridSizer1;
		wxStaticText* StaticText3;
		wxTextCtrl* txtNumber;
		wxStaticText* StaticText5;
		wxTextCtrl* txtName;
		wxButton* bSetName;
		wxStaticText* StaticText4;
		wxTextCtrl* txtEnabled;
		wxStaticText* StaticText6;
		wxTextCtrl* txtShaft;
		wxStaticText* StaticText7;
		wxTextCtrl* txtHeight;
		wxStaticText* StaticText8;
		wxTextCtrl* txtDoorWidth;
		wxStaticText* StaticText9;
		wxTextCtrl* txtDoorHeight;
		wxStaticText* StaticText10;
		wxTextCtrl* txtDoorDirection;
		wxStaticText* StaticText11;
		wxTextCtrl* txtDoorsOpen;
		wxStaticText* StaticText12;
		wxTextCtrl* txtBrakes;
		wxStaticText* StaticText13;
		wxTextCtrl* txtStop;
		wxBoxSizer* BoxSizer10;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxFlexGridSizer* FlexGridSizer2;
		wxStaticText* StaticText14;
		wxTextCtrl* txtFloor;
		wxStaticText* StaticText15;
		wxTextCtrl* txtPosition;
		wxStaticText* StaticText16;
		wxTextCtrl* txtOrigin;
		wxStaticText* StaticText17;
		wxTextCtrl* txtOriginFloor;
		wxStaticText* StaticText18;
		wxTextCtrl* txtElevStart;
		wxStaticText* StaticText19;
		wxTextCtrl* txtDoorOrigin;
		wxStaticText* StaticText20;
		wxTextCtrl* txtShaftDoorOrigin;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxFlexGridSizer* FlexGridSizer3;
		wxStaticText* StaticText38;
		wxTextCtrl* txtQueueDirection;
		wxStaticText* StaticText39;
		wxTextCtrl* txtQueuePause;
		wxStaticText* StaticText40;
		wxTextCtrl* txtQueueLastUp;
		wxStaticText* StaticText41;
		wxTextCtrl* txtQueueLastDown;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxFlexGridSizer* FlexGridSizer6;
		wxStaticText* StaticText21;
		wxTextCtrl* txtSpeed;
		wxButton* bSetSpeed;
		wxStaticText* StaticText22;
		wxTextCtrl* txtAcceleration;
		wxButton* bSetAcceleration;
		wxStaticText* StaticText23;
		wxTextCtrl* txtDeceleration;
		wxButton* bSetDeceleration;
		wxStaticText* StaticText24;
		wxTextCtrl* txtOpenSpeed;
		wxButton* bSetOpenSpeed;
		wxStaticText* StaticText25;
		wxTextCtrl* txtDoorAcceleration;
		wxButton* bSetDoorAccel;
		wxStaticText* StaticText26;
		wxTextCtrl* txtRate;
		wxStaticText* StaticText27;
		wxTextCtrl* txtDirection;
		wxStaticText* StaticText28;
		wxTextCtrl* txtDoorSpeed;
		wxStaticBoxSizer* StaticBoxSizer8;
		wxFlexGridSizer* FlexGridSizer7;
		wxStaticText* StaticText30;
		wxTextCtrl* txtDestFloor;
		wxStaticText* StaticText31;
		wxTextCtrl* txtMoveElevator;
		wxStaticText* StaticText32;
		wxTextCtrl* txtMoveElevatorFloor;
		wxStaticText* StaticText33;
		wxTextCtrl* txtDistance;
		wxStaticText* StaticText34;
		wxTextCtrl* txtDestination;
		wxStaticText* StaticText35;
		wxTextCtrl* txtStopDistance;
		wxStaticText* StaticText36;
		wxTextCtrl* txtTempDecel;
		wxStaticText* StaticText37;
		wxTextCtrl* txtErrorOffset;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
