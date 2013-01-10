/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Edit Elevator Form
	Copyright (C)2004-2013 Ryan Thoryk
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
#include <wx/checkbox.h>
#include <wx/scrolbar.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/tglbtn.h>
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
		static const long ID_chkRun;
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
		static const long ID_STATICTEXT61;
		static const long ID_txtQuickClose;
		static const long ID_bSetQuickClose;
		static const long ID_STATICTEXT32;
		static const long ID_txtUseSkipFloor;
		static const long ID_STATICTEXT44;
		static const long ID_txtSkipFloorText;
		static const long ID_bSetSkipFloorText;
		static const long ID_STATICTEXT52;
		static const long ID_txtAlarm;
		static const long ID_STATICTEXT28;
		static const long ID_txtWaitForDoors;
		static const long ID_STATICTEXT67;
		static const long ID_txtNudgeMode;
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
		static const long ID_STATICTEXT60;
		static const long ID_txtParking;
		static const long ID_STATICTEXT63;
		static const long ID_txtSlowSpeed;
		static const long ID_bSetSlowSpeed;
		static const long ID_STATICTEXT64;
		static const long ID_txtManualSpeed;
		static const long ID_bSetManualSpeed;
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
		static const long ID_STATICTEXT59;
		static const long ID_txtNotifyEarly;
		static const long ID_bNotifyEarly;
		static const long ID_STATICTEXT65;
		static const long ID_txtDepartureDelay;
		static const long ID_bSetDepartureDelay;
		static const long ID_STATICTEXT66;
		static const long ID_txtArrivalDelay;
		static const long ID_bSetArrivalDelay;
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
		static const long ID_STATICTEXT62;
		static const long ID_txtNudgeTimer;
		static const long ID_bSetNudgeTimer;
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
		void On_bNotifyEarly_Click(wxCommandEvent& event);
		void On_chkRun_Click(wxCommandEvent& event);
		void On_bSetQuickClose_Click(wxCommandEvent& event);
		void On_bSetNudgeTimer_Click(wxCommandEvent& event);
		void On_bSetSlowSpeed_Click(wxCommandEvent& event);
		void On_bSetManualSpeed_Click(wxCommandEvent& event);
		void On_bSetDepartureDelay_Click(wxCommandEvent& event);
		void On_bSetArrivalDelay_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(editelevator)
		wxButton* bSetManualSpeed;
		wxButton* bAlarm;
		wxFlexGridSizer* FlexGridSizer7;
		wxStaticText* StaticText1;
		wxStaticText* StaticText23;
		wxRadioButton* Fire2Hold;
		wxTextCtrl* txtDestFloor;
		wxStaticText* StaticText40;
		wxStaticText* StaticText52;
		wxTextCtrl* txtACPFloor;
		wxStaticText* StaticText13;
		wxButton* bStopDoors;
		wxTextCtrl* txtDirection;
		wxToggleButton* bGoToggle;
		wxStaticText* StaticText46;
		wxButton* bSetACPFloor;
		wxTextCtrl* txtMotor;
		wxTextCtrl* txtMoveElevator;
		wxButton* bOpenShaftDoor;
		wxBoxSizer* BoxSizer10;
		wxTextCtrl* txtOriginFloor;
		wxBoxSizer* BoxSizer2;
		wxButton* bStop;
		wxStaticText* StaticText32;
		wxCheckBox* chkRun;
		wxTextCtrl* txtSlowSpeed;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxCheckBox* chkVisible;
		wxToggleButton* bUpPeak;
		wxStaticText* StaticText20;
		wxButton* bUpIndicator;
		wxStaticText* StaticText42;
		wxTextCtrl* txtDecelJerk;
		wxTextCtrl* txtNudgeTimer;
		wxTextCtrl* txtErrorOffset;
		wxTextCtrl* txtSkipFloorText;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxTextCtrl* txtQueueLastUp;
		wxStaticText* StaticText6;
		wxBoxSizer* BoxSizer8;
		wxTextCtrl* txtShaftDoorOrigin;
		wxStaticText* StaticText45;
		wxTextCtrl* txtDoorHeight;
		wxStaticText* StaticText56;
		wxStaticText* StaticText18;
		wxTextCtrl* txtQuickClose;
		wxButton* bSetDeceleration;
		wxStaticText* StaticText17;
		wxStaticText* StaticText66;
		wxStaticText* StaticText24;
		wxStaticText* StaticText30;
		wxBoxSizer* BoxSizer9;
		wxButton* bSetDecelJerk;
		wxStaticText* StaticText15;
		wxStaticText* StaticText43;
		wxButton* bCloseManual;
		wxStaticText* StaticText35;
		wxButton* bSetLevelingOpen;
		wxToggleButton* bUp;
		wxTextCtrl* txtDoorTimer;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxStaticText* StaticText50;
		wxToggleButton* bInsService;
		wxTextCtrl* txtAlarm;
		wxStaticText* tFloor;
		wxStaticText* StaticText22;
		wxButton* bSetAccelJerk;
		wxButton* bGo;
		wxTextCtrl* txtQueueLastDirection;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxStaticText* StaticText61;
		wxButton* bSetQuickClose;
		wxStaticText* StaticText48;
		wxTextCtrl* txtLevelingOffset;
		wxStaticText* StaticText3;
		wxFlexGridSizer* FlexGridSizer6;
		wxButton* bSetNudgeTimer;
		wxTextCtrl* txtElevStart;
		wxStaticText* StaticText57;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxStaticText* StaticText2;
		wxTextCtrl* txtPosition;
		wxTextCtrl* txtDoorDirection;
		wxTextCtrl* txtUseSkipFloor;
		wxButton* bSetLevelingSpeed;
		wxTextCtrl* txtFloor;
		wxStaticText* StaticText27;
		wxTextCtrl* txtDistance;
		wxStaticBoxSizer* StaticBoxSizer8;
		wxRadioButton* Fire2Off;
		wxButton* bSetParkingFloor;
		wxButton* bEnqueueUp;
		wxTextCtrl* txtShaft;
		wxStaticText* StaticText51;
		wxButton* bDownIndicator;
		wxTextCtrl* txtRecallAlternate;
		wxButton* bClose;
		wxFlexGridSizer* FlexGridSizer2;
		wxTextCtrl* txtRate;
		wxStaticText* tElevator;
		wxRadioButton* Fire1Bypass;
		wxStaticText* StaticText55;
		wxTextCtrl* txtLeveling;
		wxTextCtrl* txtIsMoving;
		wxScrollBar* sDoor;
		wxTextCtrl* txtNotifyEarly;
		wxFlexGridSizer* FlexGridSizer1;
		wxButton* bSetSpeed;
		wxRadioButton* Fire2On;
		wxTextCtrl* txtSpeed;
		wxFlexGridSizer* FlexGridSizer3;
		wxStaticText* StaticText44;
		wxStaticText* StaticText64;
		wxButton* bSetName;
		wxTextCtrl* txtDepartureDelay;
		wxStaticText* StaticText16;
		wxStaticText* StaticText39;
		wxTextCtrl* txtEnabled;
		wxButton* bEnqueueDown;
		wxTextCtrl* txtName;
		wxTextCtrl* txtAcceleration;
		wxStaticText* StaticText63;
		wxStaticText* StaticText34;
		wxTextCtrl* txtManualSpeed;
		wxButton* bSetAcceleration;
		wxTextCtrl* txtOnFloor;
		wxStaticText* StaticText41;
		wxTextCtrl* txtStop;
		wxRadioButton* Fire1Off;
		wxBoxSizer* BoxSizer6;
		wxStaticText* StaticText25;
		wxStaticText* StaticText26;
		wxRadioButton* Fire1On;
		wxTextCtrl* txtRecallFloor;
		wxStaticText* StaticText31;
		wxScrollBar* sNumber;
		wxButton* bChime;
		wxButton* bSetArrivalDelay;
		wxTextCtrl* txtParking;
		wxButton* bSetDepartureDelay;
		wxStaticText* StaticText54;
		wxStaticText* StaticText60;
		wxButton* bCall;
		wxButton* bCloseShaftDoor;
		wxStaticText* StaticText14;
		wxStaticText* StaticText4;
		wxTextCtrl* txtOrigin;
		wxStaticText* StaticText8;
		wxTextCtrl* txtDoorStopped;
		wxStaticText* StaticText47;
		wxStaticText* StaticText49;
		wxStaticText* tDoor;
		wxTextCtrl* txtDoorsOpen;
		wxButton* bOpenManual;
		wxButton* bRefresh;
		wxTextCtrl* txtParkingFloor;
		wxStaticText* StaticText9;
		wxStaticText* StaticText21;
		wxTextCtrl* txtDoorOrigin;
		wxTextCtrl* txtArrivalDelay;
		wxButton* bDumpQueues;
		wxStaticText* StaticText29;
		wxTextCtrl* txtHeight;
		wxTextCtrl* txtNudgeMode;
		wxStaticText* StaticText58;
		wxStaticText* StaticText19;
		wxStaticText* StaticText38;
		wxTextCtrl* txtAccelJerk;
		wxBoxSizer* BoxSizer3;
		wxStaticText* StaticText37;
		wxTextCtrl* txtJerkRate;
		wxTextCtrl* txtDoorWidth;
		wxScrollBar* sFloor;
		wxStaticText* StaticText53;
		wxButton* bNotifyEarly;
		wxStaticText* StaticText28;
		wxStaticText* StaticText33;
		wxTextCtrl* txtQueueLastDown;
		wxButton* bSetParkingDelay;
		wxStaticText* StaticText7;
		wxStaticText* StaticText11;
		wxButton* bSetLevelingOffset;
		wxTextCtrl* txtDestination;
		wxToggleButton* bDown;
		wxTextCtrl* txtIsIdle;
		wxButton* bSetRecallAlternate;
		wxTextCtrl* txtLevelingSpeed;
		wxStaticText* StaticText62;
		wxStaticText* StaticText59;
		wxStaticText* StaticText12;
		wxButton* bSetSkipFloorText;
		wxButton* bSetDoorTimer;
		wxBoxSizer* BoxSizer1;
		wxButton* bResetQueues;
		wxButton* bSetRecallFloor;
		wxButton* bOpen;
		wxToggleButton* bIndService;
		wxStaticText* StaticText10;
		wxButton* bSetSlowSpeed;
		wxStaticText* StaticText67;
		wxStaticText* StaticText5;
		wxTextCtrl* txtNumber;
		wxTextCtrl* txtLevelingOpen;
		wxTextCtrl* txtQueueDirection;
		wxToggleButton* bDownPeak;
		wxTextCtrl* txtParkingDelay;
		wxStaticText* StaticText36;
		wxTextCtrl* txtStopDistance;
		wxTextCtrl* txtWaitForDoors;
		wxButton* bDumpFloors;
		wxTextCtrl* txtTempDecel;
		wxTextCtrl* txtManualGo;
		wxTextCtrl* txtDeceleration;
		wxTextCtrl* txtBrakes;
		wxStaticText* StaticText65;
		wxToggleButton* bACPMode;
		wxTextCtrl* txtCameraOffset;
		//*)
		int last_elevator;

	private:

		DECLARE_EVENT_TABLE()
};

#endif
