/* $Id$ */

/*
	Skyscraper 1.6 Alpha - Edit Elevator Form
	Copyright (C)2004-2010 Ryan Thoryk
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
		static const long ID_bUp;
		static const long ID_bGoToggle;
		static const long ID_bDown;
		static const long ID_tDoor;
		static const long ID_sDoor;
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
		static const long ID_bUpIndicator;
		static const long ID_bEnqueueUp;
		static const long ID_bEnqueueDown;
		static const long ID_bClose;
		static const long ID_bCloseManual;
		static const long ID_bCloseShaftDoor;
		static const long ID_bAlarm;
		static const long ID_bStopDoors;
		static const long ID_bDownIndicator;
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
		static const long ID_STATICTEXT52;
		static const long ID_txtAlarm;
		static const long ID_STATICTEXT28;
		static const long ID_txtWaitForDoors;
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
		static const long ID_STATICTEXT50;
		static const long ID_txtOnFloor;
		static const long ID_STATICTEXT25;
		static const long ID_txtMotor;
		static const long ID_STATICTEXT54;
		static const long ID_txtCameraOffset;
		static const long ID_STATICTEXT38;
		static const long ID_txtQueueDirection;
		static const long ID_STATICTEXT40;
		static const long ID_txtQueueLastUp;
		static const long ID_STATICTEXT41;
		static const long ID_txtQueueLastDown;
		static const long ID_STATICTEXT51;
		static const long ID_txtQueueLastDirection;
		static const long ID_bResetQueues;
		static const long ID_STATICTEXT21;
		static const long ID_txtSpeed;
		static const long ID_bSetSpeed;
		static const long ID_STATICTEXT22;
		static const long ID_txtAcceleration;
		static const long ID_bSetAcceleration;
		static const long ID_STATICTEXT23;
		static const long ID_txtDeceleration;
		static const long ID_bSetDeceleration;
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
		static const long ID_STATICTEXT2;
		static const long ID_txtJerkRate;
		static const long ID_STATICTEXT49;
		static const long ID_txtDoorStopped;
		static const long ID_STATICTEXT53;
		static const long ID_txtIsIdle;
		static const long ID_STATICTEXT24;
		static const long ID_txtManualGo;
		static const long ID_STATICTEXT55;
		static const long ID_txtLeveling;
		static const long ID_STATICTEXT56;
		static const long ID_txtLevelingSpeed;
		static const long ID_bSetLevelingSpeed;
		static const long ID_STATICTEXT57;
		static const long ID_txtLevelingOffset;
		static const long ID_bSetLevelingOffset;
		static const long ID_STATICTEXT58;
		static const long ID_txtLevelingOpen;
		static const long ID_bSetLevelingOpen;
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
		static const long ID_STATICTEXT39;
		static const long ID_txtParkingFloor;
		static const long ID_bSetParkingFloor;
		static const long ID_STATICTEXT48;
		static const long ID_txtParkingDelay;
		static const long ID_bSetParkingDelay;
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
		void On_bStopDoors_Click(wxCommandEvent& event);
		void On_bUp_Toggle(wxCommandEvent& event);
		void On_bGoToggle_Toggle(wxCommandEvent& event);
		void On_bDown_Toggle(wxCommandEvent& event);
		void On_bUpIndicator_Click(wxCommandEvent& event);
		void On_bDownIndicator_Click(wxCommandEvent& event);
		void On_bResetQueues_Click(wxCommandEvent& event);
		void On_bSetParkingFloor_Click(wxCommandEvent& event);
		void On_bSetParkingDelay_Click(wxCommandEvent& event);
		void On_bSetLevelingSpeed_Click(wxCommandEvent& event);
		void On_bSetLevelingOffset_Click(wxCommandEvent& event);
		void On_bSetLevelingOpen_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(editelevator)
		wxButton* bCloseShaftDoor;
		wxTextCtrl* txtDecelJerk;
		wxRadioButton* Fire1Off;
		wxButton* bSetLevelingOpen;
		wxTextCtrl* txtBrakes;
		wxScrollBar* sFloor;
		wxButton* bSetDecelJerk;
		wxButton* bCall;
		wxStaticText* StaticText32;
		wxStaticText* StaticText36;
		wxButton* bUpIndicator;
		wxTextCtrl* txtDoorOrigin;
		wxTextCtrl* txtDoorHeight;
		wxStaticText* StaticText40;
		wxTextCtrl* txtName;
		wxTextCtrl* txtLevelingOffset;
		wxTextCtrl* txtHeight;
		wxStaticText* StaticText27;
		wxRadioButton* Fire2Off;
		wxButton* bDumpQueues;
		wxButton* bSetRecallFloor;
		wxBoxSizer* BoxSizer2;
		wxButton* bSetDeceleration;
		wxStaticText* StaticText41;
		wxToggleButton* bACPMode;
		wxStaticText* StaticText50;
		wxButton* bDumpFloors;
		wxRadioButton* Fire2Hold;
		wxStaticText* StaticText46;
		wxButton* bEnqueueDown;
		wxTextCtrl* txtShaftDoorOrigin;
		wxTextCtrl* txtDestination;
		wxTextCtrl* txtDistance;
		wxBoxSizer* BoxSizer9;
		wxButton* bSetParkingDelay;
		wxStaticText* StaticText42;
		wxStaticText* StaticText48;
		wxStaticText* StaticText5;
		wxStaticText* StaticText25;
		wxTextCtrl* txtCameraOffset;
		wxStaticText* StaticText6;
		wxTextCtrl* txtStop;
		wxStaticText* StaticText52;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxTextCtrl* txtQueueLastDown;
		wxStaticText* tDoor;
		wxTextCtrl* txtParkingDelay;
		wxCheckBox* chkVisible;
		wxTextCtrl* txtElevStart;
		wxTextCtrl* txtRecallAlternate;
		wxStaticText* StaticText19;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxTextCtrl* txtLeveling;
		wxTextCtrl* txtIsIdle;
		wxToggleButton* bUp;
		wxButton* bOpenManual;
		wxStaticText* StaticText39;
		wxFlexGridSizer* FlexGridSizer3;
		wxBoxSizer* BoxSizer1;
		wxToggleButton* bUpPeak;
		wxStaticBoxSizer* StaticBoxSizer8;
		wxStaticText* StaticText10;
		wxTextCtrl* txtErrorOffset;
		wxStaticText* StaticText35;
		wxBoxSizer* BoxSizer6;
		wxButton* bEnqueueUp;
		wxTextCtrl* txtLevelingSpeed;
		wxToggleButton* bDownPeak;
		wxStaticText* StaticText23;
		wxStaticText* StaticText33;
		wxTextCtrl* txtDeceleration;
		wxStaticText* StaticText12;
		wxTextCtrl* txtQueueDirection;
		wxStaticText* StaticText22;
		wxTextCtrl* txtACPFloor;
		wxStaticText* StaticText56;
		wxToggleButton* bDown;
		wxTextCtrl* txtNumber;
		wxTextCtrl* txtMotor;
		wxTextCtrl* txtDoorsOpen;
		wxButton* bSetAccelJerk;
		wxButton* bSetAcceleration;
		wxStaticText* StaticText49;
		wxStaticText* StaticText55;
		wxRadioButton* Fire1On;
		wxToggleButton* bInsService;
		wxStaticText* StaticText7;
		wxStaticText* tFloor;
		wxButton* bChime;
		wxTextCtrl* txtTempDecel;
		wxTextCtrl* txtAccelJerk;
		wxStaticText* StaticText16;
		wxTextCtrl* txtOriginFloor;
		wxTextCtrl* txtStopDistance;
		wxButton* bRefresh;
		wxStaticText* StaticText57;
		wxStaticText* StaticText47;
		wxButton* bCloseManual;
		wxStaticText* StaticText43;
		wxStaticText* StaticText24;
		wxTextCtrl* txtDoorWidth;
		wxTextCtrl* txtSpeed;
		wxTextCtrl* txtEnabled;
		wxTextCtrl* txtFloor;
		wxButton* bSetRecallAlternate;
		wxStaticText* StaticText14;
		wxStaticText* StaticText34;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxTextCtrl* txtJerkRate;
		wxStaticText* StaticText18;
		wxTextCtrl* txtAlarm;
		wxButton* bSetParkingFloor;
		wxStaticText* StaticText37;
		wxStaticText* StaticText13;
		wxBoxSizer* BoxSizer10;
		wxStaticText* StaticText51;
		wxStaticText* StaticText53;
		wxTextCtrl* txtParkingFloor;
		wxStaticText* StaticText54;
		wxStaticText* StaticText20;
		wxButton* bSetLevelingSpeed;
		wxStaticText* StaticText31;
		wxTextCtrl* txtMoveElevator;
		wxScrollBar* sNumber;
		wxButton* bStop;
		wxToggleButton* bGoToggle;
		wxButton* bSetACPFloor;
		wxToggleButton* bIndService;
		wxButton* bClose;
		wxTextCtrl* txtWaitForDoors;
		wxButton* bDownIndicator;
		wxTextCtrl* txtOrigin;
		wxButton* bSetDoorTimer;
		wxTextCtrl* txtSkipFloorText;
		wxTextCtrl* txtAcceleration;
		wxButton* bStopDoors;
		wxStaticText* StaticText45;
		wxButton* bAlarm;
		wxStaticText* StaticText21;
		wxStaticText* StaticText4;
		wxButton* bGo;
		wxButton* bSetLevelingOffset;
		wxRadioButton* Fire1Bypass;
		wxTextCtrl* txtUseSkipFloor;
		wxBoxSizer* BoxSizer8;
		wxScrollBar* sDoor;
		wxTextCtrl* txtDirection;
		wxStaticText* tElevator;
		wxButton* bResetQueues;
		wxTextCtrl* txtPosition;
		wxRadioButton* Fire2On;
		wxStaticText* StaticText15;
		wxStaticText* StaticText26;
		wxBoxSizer* BoxSizer3;
		wxStaticText* StaticText8;
		wxButton* bSetSkipFloorText;
		wxStaticText* StaticText28;
		wxStaticText* StaticText30;
		wxStaticText* StaticText29;
		wxStaticText* StaticText58;
		wxButton* bSetName;
		wxTextCtrl* txtShaft;
		wxButton* bOpen;
		wxTextCtrl* txtIsMoving;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxTextCtrl* txtManualGo;
		wxTextCtrl* txtRate;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxTextCtrl* txtQueueLastUp;
		wxTextCtrl* txtQueueLastDirection;
		wxTextCtrl* txtDestFloor;
		wxStaticText* StaticText38;
		wxStaticText* StaticText17;
		wxStaticText* StaticText11;
		wxTextCtrl* txtDoorTimer;
		wxFlexGridSizer* FlexGridSizer6;
		wxFlexGridSizer* FlexGridSizer7;
		wxTextCtrl* txtLevelingOpen;
		wxTextCtrl* txtRecallFloor;
		wxTextCtrl* txtDoorStopped;
		wxTextCtrl* txtOnFloor;
		wxStaticText* StaticText9;
		wxFlexGridSizer* FlexGridSizer1;
		wxButton* bSetSpeed;
		wxFlexGridSizer* FlexGridSizer2;
		wxTextCtrl* txtDoorDirection;
		wxButton* bOpenShaftDoor;
		wxStaticText* StaticText44;
		wxStaticText* StaticText3;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
