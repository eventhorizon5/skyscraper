/* $Id$ */

/*
	Skyscraper 1.4 Alpha - Edit Elevator Form
	Copyright (C)2005-2009 Ryan Thoryk
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

#ifndef EDITELEVATOR_H
#define EDITELEVATOR_H

//(*Headers(editelevator)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/tglbtn.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/scrolbar.h>
#include <wx/dialog.h>
//*)

class editelevator: public wxDialog
{
	friend class Timer;
	public:
		editelevator(wxWindow* parent,wxWindowID id = -1);
		virtual ~editelevator();

		//(*Identifiers(editelevator)
		static const long ID_tElevator;
		static const long ID_sNumber;
		static const long ID_tFloor;
		static const long ID_sFloor;
		static const long ID_bACPMode;
		static const long ID_bUpPeak;
		static const long ID_bDownPeak;
		static const long ID_bIndService;
		static const long ID_bInsService;
		static const long ID_Fire1Off;
		static const long ID_Fire1On;
		static const long ID_Fire1Bypass;
		static const long ID_Fire2Off;
		static const long ID_Fire2On;
		static const long ID_Fire2Hold;
		static const long ID_bRefresh;
		static const long ID_bDumpFloors;
		static const long ID_bDumpQueues;
		static const long ID_CHECKBOX1;
		static const long ID_bCall;
		static const long ID_bGo;
		static const long ID_bOpen;
		static const long ID_bOpenManual;
		static const long ID_bOpenShaftDoor;
		static const long ID_bStop;
		static const long ID_bChime;
		static const long ID_bEnqueueUp;
		static const long ID_bEnqueueDown;
		static const long ID_bClose;
		static const long ID_bCloseManual;
		static const long ID_bCloseShaftDoor;
		static const long ID_bAlarm;
		static const long ID_STATICTEXT3;
		static const long ID_txtNumber;
		static const long ID_STATICTEXT5;
		static const long ID_txtName;
		static const long ID_bSetName;
		static const long ID_STATICTEXT4;
		static const long ID_txtEnabled;
		static const long ID_STATICTEXT6;
		static const long ID_txtShaft;
		static const long ID_STATICTEXT7;
		static const long ID_txtHeight;
		static const long ID_STATICTEXT8;
		static const long ID_txtDoorWidth;
		static const long ID_STATICTEXT9;
		static const long ID_txtDoorHeight;
		static const long ID_STATICTEXT10;
		static const long ID_txtDoorDirection;
		static const long ID_STATICTEXT11;
		static const long ID_txtDoorsOpen;
		static const long ID_STATICTEXT12;
		static const long ID_txtBrakes;
		static const long ID_STATICTEXT13;
		static const long ID_txtStop;
		static const long ID_STATICTEXT42;
		static const long ID_txtDoorTimer;
		static const long ID_bSetDoorTimer;
		static const long ID_STATICTEXT32;
		static const long ID_txtUseSkipFloor;
		static const long ID_STATICTEXT44;
		static const long ID_txtSkipFloorText;
		static const long ID_bSetSkipFloorText;
		static const long ID_STATICTEXT14;
		static const long ID_txtFloor;
		static const long ID_STATICTEXT15;
		static const long ID_txtPosition;
		static const long ID_STATICTEXT16;
		static const long ID_txtOrigin;
		static const long ID_STATICTEXT17;
		static const long ID_txtOriginFloor;
		static const long ID_STATICTEXT18;
		static const long ID_txtElevStart;
		static const long ID_STATICTEXT19;
		static const long ID_txtDoorOrigin;
		static const long ID_STATICTEXT20;
		static const long ID_txtShaftDoorOrigin;
		static const long ID_STATICTEXT38;
		static const long ID_txtQueueDirection;
		static const long ID_STATICTEXT39;
		static const long ID_txtQueuePause;
		static const long ID_STATICTEXT40;
		static const long ID_txtQueueLastUp;
		static const long ID_STATICTEXT41;
		static const long ID_txtQueueLastDown;
		static const long ID_STATICTEXT21;
		static const long ID_txtSpeed;
		static const long ID_bSetSpeed;
		static const long ID_STATICTEXT22;
		static const long ID_txtAcceleration;
		static const long ID_bSetAcceleration;
		static const long ID_STATICTEXT23;
		static const long ID_txtDeceleration;
		static const long ID_bSetDeceleration;
		static const long ID_STATICTEXT24;
		static const long ID_txtOpenSpeed;
		static const long ID_bSetOpenSpeed;
		static const long ID_STATICTEXT25;
		static const long ID_txtDoorAcceleration;
		static const long ID_bSetDoorAccel;
		static const long ID_STATICTEXT1;
		static const long ID_txtAccelJerk;
		static const long ID_bSetAccelJerk;
		static const long ID_STATICTEXT29;
		static const long ID_txtDecelJerk;
		static const long ID_bSetDecelJerk;
		static const long ID_STATICTEXT26;
		static const long ID_txtRate;
		static const long ID_STATICTEXT27;
		static const long ID_txtDirection;
		static const long ID_STATICTEXT28;
		static const long ID_txtDoorSpeed;
		static const long ID_STATICTEXT2;
		static const long ID_txtJerkRate;
		static const long ID_STATICTEXT30;
		static const long ID_txtDestFloor;
		static const long ID_STATICTEXT43;
		static const long ID_txtIsMoving;
		static const long ID_STATICTEXT31;
		static const long ID_txtMoveElevator;
		static const long ID_STATICTEXT33;
		static const long ID_txtDistance;
		static const long ID_STATICTEXT34;
		static const long ID_txtDestination;
		static const long ID_STATICTEXT35;
		static const long ID_txtStopDistance;
		static const long ID_STATICTEXT36;
		static const long ID_txtTempDecel;
		static const long ID_STATICTEXT37;
		static const long ID_txtErrorOffset;
		static const long ID_STATICTEXT45;
		static const long ID_txtACPFloor;
		static const long ID_bSetACPFloor;
		static const long ID_STATICTEXT46;
		static const long ID_txtRecallFloor;
		static const long ID_bSetRecallFloor;
		static const long ID_STATICTEXT47;
		static const long ID_txtRecallAlternate;
		static const long ID_bSetRecallAlternate;
		static const long ID_STATICTEXT48;
		static const long ID_txtMovePending;
		//*)
		void Loop();
		void SetMainValues();

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
		void OnchkVisibleClick(wxCommandEvent& event);
		void On_chkVisible_Click(wxCommandEvent& event);
		void On_bSetJerk_Click(wxCommandEvent& event);
		void On_bSetDecelJerk_Click(wxCommandEvent& event);
		void On_bSetAccelJerk_Click(wxCommandEvent& event);
		void On_bOpenShaftDoor_Click(wxCommandEvent& event);
		void On_bCloseShaftDoor_Click(wxCommandEvent& event);
		void On_bSetDoorTimer_Click(wxCommandEvent& event);
		void On_bChime_Click(wxCommandEvent& event);
		void On_bACPMode_Toggle(wxCommandEvent& event);
		void On_bUpPeak_Toggle(wxCommandEvent& event);
		void On_bDownPeak_Toggle(wxCommandEvent& event);
		void On_bIndService_Toggle(wxCommandEvent& event);
		void On_bInsService_Toggle(wxCommandEvent& event);
		void On_bRefresh_Click(wxCommandEvent& event);
		void On_bSetSkipFloorText_Click(wxCommandEvent& event);
		void On_bSetACPFloor_Click(wxCommandEvent& event);
		void On_bSetRecallFloor_Click(wxCommandEvent& event);
		void On_bSetRecallAlternate_Click(wxCommandEvent& event);
		void On_Fire1Off_Select(wxCommandEvent& event);
		void On_Fire1On_Select(wxCommandEvent& event);
		void On_Fire1Bypass_Select(wxCommandEvent& event);
		void On_Fire2Off_Select(wxCommandEvent& event);
		void On_Fire2On_Select(wxCommandEvent& event);
		void On_Fire2Hold_Select(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(editelevator)
		wxStaticText* StaticText10;
		wxButton* bDumpFloors;
		wxStaticText* tFloor;
		wxStaticText* StaticText22;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxStaticText* StaticText9;
		wxBoxSizer* BoxSizer6;
		wxTextCtrl* txtQueuePause;
		wxButton* bOpenShaftDoor;
		wxTextCtrl* txtDoorSpeed;
		wxStaticText* StaticText20;
		wxTextCtrl* txtRecallAlternate;
		wxStaticText* StaticText45;
		wxButton* bAlarm;
		wxButton* bEnqueueDown;
		wxTextCtrl* txtOrigin;
		wxTextCtrl* txtErrorOffset;
		wxTextCtrl* txtOpenSpeed;
		wxButton* bSetOpenSpeed;
		wxRadioButton* Fire1Off;
		wxTextCtrl* txtDoorWidth;
		wxTextCtrl* txtJerkRate;
		wxButton* bSetSpeed;
		wxTextCtrl* txtNumber;
		wxStaticText* StaticText29;
		wxBoxSizer* BoxSizer10;
		wxTextCtrl* txtQueueLastUp;
		wxBoxSizer* BoxSizer8;
		wxStaticText* StaticText37;
		wxTextCtrl* txtDeceleration;
		wxButton* bSetSkipFloorText;
		wxScrollBar* sNumber;
		wxStaticText* StaticText33;
		wxTextCtrl* txtDoorTimer;
		wxStaticText* StaticText13;
		wxStaticText* StaticText2;
		wxStaticText* StaticText30;
		wxStaticText* StaticText14;
		wxTextCtrl* txtDestFloor;
		wxButton* bSetRecallAlternate;
		wxButton* bRefresh;
		wxFlexGridSizer* FlexGridSizer3;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxScrollBar* sFloor;
		wxTextCtrl* txtAccelJerk;
		wxButton* bSetDoorTimer;
		wxTextCtrl* txtSkipFloorText;
		wxTextCtrl* txtRecallFloor;
		wxTextCtrl* txtACPFloor;
		wxButton* bSetName;
		wxStaticText* StaticText40;
		wxButton* bOpen;
		wxButton* bCall;
		wxTextCtrl* txtOriginFloor;
		wxStaticText* StaticText19;
		wxStaticText* StaticText32;
		wxStaticText* StaticText42;
		wxStaticText* StaticText38;
		wxTextCtrl* txtFloor;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxTextCtrl* txtMoveElevator;
		wxButton* bCloseShaftDoor;
		wxStaticText* StaticText18;
		wxTextCtrl* txtRate;
		wxButton* bCloseManual;
		wxTextCtrl* txtStopDistance;
		wxStaticText* StaticText31;
		wxButton* bSetDecelJerk;
		wxFlexGridSizer* FlexGridSizer2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText27;
		wxRadioButton* Fire2On;
		wxBoxSizer* BoxSizer2;
		wxStaticText* StaticText3;
		wxButton* bGo;
		wxButton* bStop;
		wxFlexGridSizer* FlexGridSizer7;
		wxTextCtrl* txtBrakes;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxTextCtrl* txtPosition;
		wxStaticText* StaticText44;
		wxStaticText* StaticText21;
		wxStaticText* StaticText39;
		wxTextCtrl* txtIsMoving;
		wxStaticText* StaticText23;
		wxStaticText* StaticText24;
		wxToggleButton* bInsService;
		wxButton* bEnqueueUp;
		wxTextCtrl* txtDoorAcceleration;
		wxRadioButton* Fire2Off;
		wxTextCtrl* txtDoorDirection;
		wxStaticBoxSizer* StaticBoxSizer8;
		wxTextCtrl* txtUseSkipFloor;
		wxButton* bSetAcceleration;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxTextCtrl* txtQueueDirection;
		wxTextCtrl* txtStop;
		wxTextCtrl* txtEnabled;
		wxButton* bSetDeceleration;
		wxTextCtrl* txtShaftDoorOrigin;
		wxStaticText* StaticText34;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxButton* bChime;
		wxButton* bClose;
		wxButton* bDumpQueues;
		wxButton* bSetAccelJerk;
		wxTextCtrl* txtQueueLastDown;
		wxTextCtrl* txtDirection;
		wxStaticText* StaticText47;
		wxRadioButton* Fire1Bypass;
		wxTextCtrl* txtDoorsOpen;
		wxTextCtrl* txtDoorHeight;
		wxStaticText* StaticText28;
		wxBoxSizer* BoxSizer1;
		wxStaticText* StaticText43;
		wxStaticText* StaticText41;
		wxTextCtrl* txtName;
		wxStaticText* StaticText15;
		wxStaticText* StaticText12;
		wxBoxSizer* BoxSizer9;
		wxStaticText* StaticText35;
		wxTextCtrl* txtHeight;
		wxTextCtrl* txtShaft;
		wxFlexGridSizer* FlexGridSizer6;
		wxStaticText* tElevator;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxFlexGridSizer* FlexGridSizer1;
		wxTextCtrl* txtAcceleration;
		wxTextCtrl* txtDoorOrigin;
		wxButton* bSetDoorAccel;
		wxStaticText* StaticText25;
		wxTextCtrl* txtElevStart;
		wxToggleButton* bDownPeak;
		wxBoxSizer* BoxSizer3;
		wxButton* bSetACPFloor;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxButton* bOpenManual;
		wxStaticText* StaticText36;
		wxStaticText* StaticText17;
		wxStaticText* StaticText4;
		wxRadioButton* Fire2Hold;
		wxStaticText* StaticText48;
		wxRadioButton* Fire1On;
		wxToggleButton* bIndService;
		wxToggleButton* bACPMode;
		wxTextCtrl* txtTempDecel;
		wxTextCtrl* txtDecelJerk;
		wxButton* bSetRecallFloor;
		wxCheckBox* chkVisible;
		wxToggleButton* bUpPeak;
		wxStaticText* StaticText16;
		wxTextCtrl* txtDestination;
		wxTextCtrl* txtDistance;
		wxStaticText* StaticText46;
		wxTextCtrl* txtMovePending;
		wxTextCtrl* txtSpeed;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
