/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Edit Elevator Form
	Copyright (C)2005-2008 Ryan Thoryk
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
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/scrolbar.h>
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
		static const long ID_tElevator;
		static const long ID_sNumber;
		static const long ID_tFloor;
		static const long ID_sFloor;
		static const long ID_bDumpFloors;
		static const long ID_bDumpQueues;
		static const long ID_bCall;
		static const long ID_bGo;
		static const long ID_bOpen;
		static const long ID_bOpenManual;
		static const long ID_bStop;
		static const long ID_bEnqueueUp;
		static const long ID_bEnqueueDown;
		static const long ID_bClose;
		static const long ID_bCloseManual;
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
		static const long ID_STATICTEXT26;
		static const long ID_txtRate;
		static const long ID_STATICTEXT27;
		static const long ID_txtDirection;
		static const long ID_STATICTEXT28;
		static const long ID_txtDoorSpeed;
		static const long ID_STATICTEXT30;
		static const long ID_txtDestFloor;
		static const long ID_STATICTEXT31;
		static const long ID_txtMoveElevator;
		static const long ID_STATICTEXT32;
		static const long ID_txtMoveElevatorFloor;
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
		//*)
		void OnInit();

		//(*Declarations(editelevator)
		wxBoxSizer*  BoxSizer1;
		wxBoxSizer*  BoxSizer2;
		wxBoxSizer*  BoxSizer6;
		wxStaticText*  tElevator;
		wxScrollBar*  sNumber;
		wxStaticText*  tFloor;
		wxScrollBar*  sFloor;
		wxBoxSizer*  BoxSizer7;
		wxButton*  bDumpFloors;
		wxButton*  bDumpQueues;
		wxStaticBoxSizer*  StaticBoxSizer1;
		wxBoxSizer*  BoxSizer8;
		wxButton*  bCall;
		wxButton*  bGo;
		wxButton*  bOpen;
		wxButton*  bOpenManual;
		wxButton*  bStop;
		wxBoxSizer*  BoxSizer9;
		wxButton*  bEnqueueUp;
		wxButton*  bEnqueueDown;
		wxButton*  bClose;
		wxButton*  bCloseManual;
		wxButton*  bAlarm;
		wxBoxSizer*  BoxSizer3;
		wxStaticBoxSizer*  StaticBoxSizer2;
		wxFlexGridSizer*  FlexGridSizer1;
		wxStaticText*  StaticText3;
		wxTextCtrl*  txtNumber;
		wxStaticText*  StaticText5;
		wxTextCtrl*  txtName;
		wxButton*  bSetName;
		wxStaticText*  StaticText4;
		wxTextCtrl*  txtEnabled;
		wxStaticText*  StaticText6;
		wxTextCtrl*  txtShaft;
		wxStaticText*  StaticText7;
		wxTextCtrl*  txtHeight;
		wxStaticText*  StaticText8;
		wxTextCtrl*  txtDoorWidth;
		wxStaticText*  StaticText9;
		wxTextCtrl*  txtDoorHeight;
		wxStaticText*  StaticText10;
		wxTextCtrl*  txtDoorDirection;
		wxStaticText*  StaticText11;
		wxTextCtrl*  txtDoorsOpen;
		wxStaticText*  StaticText12;
		wxTextCtrl*  txtBrakes;
		wxStaticText*  StaticText13;
		wxTextCtrl*  txtStop;
		wxBoxSizer*  BoxSizer10;
		wxStaticBoxSizer*  StaticBoxSizer5;
		wxFlexGridSizer*  FlexGridSizer2;
		wxStaticText*  StaticText14;
		wxTextCtrl*  txtFloor;
		wxStaticText*  StaticText15;
		wxTextCtrl*  txtPosition;
		wxStaticText*  StaticText16;
		wxTextCtrl*  txtOrigin;
		wxStaticText*  StaticText17;
		wxTextCtrl*  txtOriginFloor;
		wxStaticText*  StaticText18;
		wxTextCtrl*  txtElevStart;
		wxStaticText*  StaticText19;
		wxTextCtrl*  txtDoorOrigin;
		wxStaticText*  StaticText20;
		wxTextCtrl*  txtShaftDoorOrigin;
		wxStaticBoxSizer*  StaticBoxSizer3;
		wxFlexGridSizer*  FlexGridSizer3;
		wxStaticText*  StaticText38;
		wxTextCtrl*  txtQueueDirection;
		wxStaticText*  StaticText39;
		wxTextCtrl*  txtQueuePause;
		wxStaticText*  StaticText40;
		wxTextCtrl*  txtQueueLastUp;
		wxStaticText*  StaticText41;
		wxTextCtrl*  txtQueueLastDown;
		wxStaticBoxSizer*  StaticBoxSizer7;
		wxFlexGridSizer*  FlexGridSizer6;
		wxStaticText*  StaticText21;
		wxTextCtrl*  txtSpeed;
		wxButton*  bSetSpeed;
		wxStaticText*  StaticText22;
		wxTextCtrl*  txtAcceleration;
		wxButton*  bSetAcceleration;
		wxStaticText*  StaticText23;
		wxTextCtrl*  txtDeceleration;
		wxButton*  bSetDeceleration;
		wxStaticText*  StaticText24;
		wxTextCtrl*  txtOpenSpeed;
		wxButton*  bSetOpenSpeed;
		wxStaticText*  StaticText25;
		wxTextCtrl*  txtDoorAcceleration;
		wxButton*  bSetDoorAccel;
		wxStaticText*  StaticText26;
		wxTextCtrl*  txtRate;
		wxStaticText*  StaticText27;
		wxTextCtrl*  txtDirection;
		wxStaticText*  StaticText28;
		wxTextCtrl*  txtDoorSpeed;
		wxStaticBoxSizer*  StaticBoxSizer8;
		wxFlexGridSizer*  FlexGridSizer7;
		wxStaticText*  StaticText30;
		wxTextCtrl*  txtDestFloor;
		wxStaticText*  StaticText31;
		wxTextCtrl*  txtMoveElevator;
		wxStaticText*  StaticText32;
		wxTextCtrl*  txtMoveElevatorFloor;
		wxStaticText*  StaticText33;
		wxTextCtrl*  txtDistance;
		wxStaticText*  StaticText34;
		wxTextCtrl*  txtDestination;
		wxStaticText*  StaticText35;
		wxTextCtrl*  txtStopDistance;
		wxStaticText*  StaticText36;
		wxTextCtrl*  txtTempDecel;
		wxStaticText*  StaticText37;
		wxTextCtrl*  txtErrorOffset;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
