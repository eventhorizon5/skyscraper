/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Camera Control Form
	Copyright (C)2004-2015 Ryan Thoryk
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

#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

//(*Headers(CameraControl)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class CameraControl: public wxDialog
{
	public:

		CameraControl(wxWindow* parent,wxWindowID id = -1);
		virtual ~CameraControl();

		//(*Identifiers(CameraControl)
		static const long ID_STATICTEXT3;
		static const long ID_txtDirectionFront;
		static const long ID_STATICTEXT15;
		static const long ID_txtDirectionTop;
		static const long ID_STATICTEXT4;
		static const long ID_txtCurrentFloor;
		static const long ID_STATICTEXT5;
		static const long ID_txtLastMesh;
		static const long ID_STATICTEXT6;
		static const long ID_txtDesiredVel;
		static const long ID_STATICTEXT7;
		static const long ID_txtVelocity;
		static const long ID_STATICTEXT8;
		static const long ID_txtDesiredAngle;
		static const long ID_STATICTEXT9;
		static const long ID_txtAngle;
		static const long ID_STATICTEXT10;
		static const long ID_txtSpeed;
		static const long ID_STATICTEXT11;
		static const long ID_txtMouseDown;
		static const long ID_STATICTEXT12;
		static const long ID_txtStartFloor;
		static const long ID_STATICTEXT13;
		static const long ID_txtStartPosition;
		static const long ID_STATICTEXT29;
		static const long ID_txtFOV;
		static const long ID_STATICTEXT14;
		static const long ID_txtGravity;
		static const long ID_bGravity;
		static const long ID_lblGravityEnabled;
		static const long ID_txtGravityEnabled;
		static const long ID_bGravityEnabled;
		static const long ID_STATICTEXT17;
		static const long ID_txtCollisions;
		static const long ID_bCollisions;
		static const long ID_STATICTEXT32;
		static const long ID_txtReportCollisions;
		static const long ID_bReportCollisions;
		static const long ID_STATICTEXT19;
		static const long ID_txtFreelook;
		static const long ID_bFreelook;
		static const long ID_STATICTEXT20;
		static const long ID_txtFreelookSpeed;
		static const long ID_bFreelookSpeed;
		static const long ID_STATICTEXT33;
		static const long ID_txtBinocularsFOV;
		static const long ID_bBinocularsFOV;
		static const long ID_STATICTEXT18;
		static const long ID_txtGotoFloor;
		static const long ID_bGotoFloor;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_lblPosition;
		static const long ID_lblRotation;
		static const long ID_STATICLINE2;
		static const long ID_rPosition;
		static const long ID_rRotation;
		static const long ID_STATICTEXT28;
		static const long ID_txtMoveSpeed;
		static const long ID_bZPlus;
		static const long ID_bYPlus;
		static const long ID_bXNeg;
		static const long ID_chkHold;
		static const long ID_bXPlus;
		static const long ID_bZNeg;
		static const long ID_bYNeg;
		static const long ID_bStartPosition;
		static const long ID_bStartRotation;
		static const long ID_bStartDirection;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT21;
		static const long ID_txtPositionX;
		static const long ID_bPositionX;
		static const long ID_STATICTEXT22;
		static const long ID_txtPositionY;
		static const long ID_bPositionY;
		static const long ID_STATICTEXT23;
		static const long ID_txtPositionZ;
		static const long ID_bPositionZ;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT27;
		static const long ID_STATICTEXT24;
		static const long ID_txtRotationX;
		static const long ID_bRotationX;
		static const long ID_STATICTEXT25;
		static const long ID_txtRotationY;
		static const long ID_bRotationY;
		static const long ID_STATICTEXT26;
		static const long ID_txtRotationZ;
		static const long ID_bRotationZ;
		static const long ID_STATICLINE3;
		static const long ID_STATICTEXT30;
		static const long ID_txtSetFOV;
		static const long ID_bSetFOV;
		static const long ID_bResetFOV;
		//*)
		void Loop();

	protected:

		//(*Handlers(CameraControl)
		void OnInit(wxInitDialogEvent& event);
		void On_rPosition_Select(wxCommandEvent& event);
		void On_rRotation_Select(wxCommandEvent& event);
		void On_bZPlus_Click(wxCommandEvent& event);
		void On_bYPlus_Click(wxCommandEvent& event);
		void On_bXNeg_Click(wxCommandEvent& event);
		void On_bXPlus_Click(wxCommandEvent& event);
		void On_bZNeg_Click(wxCommandEvent& event);
		void On_bYNeg_Click(wxCommandEvent& event);
		void On_bStartPosition_Click(wxCommandEvent& event);
		void On_bStartRotation_Click(wxCommandEvent& event);
		void On_bStartDirection_Click(wxCommandEvent& event);
		void On_bGravity_Click(wxCommandEvent& event);
		void On_bGravityEnabled_Click(wxCommandEvent& event);
		void On_bCollisions_Click(wxCommandEvent& event);
		void On_bFreelook_Click(wxCommandEvent& event);
		void On_bFreelookSpeed_Click(wxCommandEvent& event);
		void On_bPositionX_Click(wxCommandEvent& event);
		void On_bPositionY_Click(wxCommandEvent& event);
		void On_bPositionZ_Click(wxCommandEvent& event);
		void On_bRotationX_Click(wxCommandEvent& event);
		void On_bRotationY_Click(wxCommandEvent& event);
		void On_bRotationZ_Click(wxCommandEvent& event);
		void On_chkHold_Click(wxCommandEvent& event);
		void On_bSetFOV_Click(wxCommandEvent& event);
		void On_bResetFOV_Click(wxCommandEvent& event);
		void On_bGotoFloor_Click(wxCommandEvent& event);
		void On_bReportCollisions_Click(wxCommandEvent& event);
		void On_bBinocularsFOV_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(CameraControl)
		wxStaticText* StaticText24;
		wxButton* bPositionY;
		wxTextCtrl* txtDirectionTop;
		wxStaticText* lblGravityEnabled;
		wxStaticText* StaticText22;
		wxTextCtrl* txtGravityEnabled;
		wxButton* bGravity;
		wxTextCtrl* txtStartFloor;
		wxStaticText* StaticText21;
		wxButton* bRotationY;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText15;
		wxButton* bGravityEnabled;
		wxTextCtrl* txtSetFOV;
		wxStaticLine* StaticLine2;
		wxTextCtrl* txtFOV;
		wxCheckBox* chkHold;
		wxStaticText* StaticText32;
		wxStaticText* StaticText17;
		wxTextCtrl* txtGotoFloor;
		wxTextCtrl* txtRotationY;
		wxStaticText* StaticText28;
		wxTextCtrl* txtVelocity;
		wxButton* bPositionX;
		wxTextCtrl* txtReportCollisions;
		wxTextCtrl* txtFreelookSpeed;
		wxTextCtrl* txtDirectionFront;
		wxTextCtrl* txtMouseDown;
		wxTextCtrl* txtDesiredAngle;
		wxRadioButton* rPosition;
		wxStaticText* lblRotation;
		wxTextCtrl* txtLastMesh;
		wxButton* bZNeg;
		wxButton* bXPlus;
		wxStaticText* lblPosition;
		wxButton* bYNeg;
		wxTextCtrl* txtCurrentFloor;
		wxTextCtrl* txtFreelook;
		wxStaticText* StaticText20;
		wxButton* bSetFOV;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxTextCtrl* txtBinocularsFOV;
		wxTextCtrl* txtCollisions;
		wxStaticText* StaticText10;
		wxStaticText* StaticText16;
		wxButton* bRotationZ;
		wxStaticText* StaticText3;
		wxTextCtrl* txtRotationX;
		wxStaticText* StaticText23;
		wxButton* bStartRotation;
		wxTextCtrl* txtDesiredVel;
		wxButton* bXNeg;
		wxTextCtrl* txtPositionX;
		wxStaticLine* StaticLine1;
		wxButton* bReportCollisions;
		wxStaticText* StaticText8;
		wxStaticText* StaticText12;
		wxTextCtrl* txtStartPosition;
		wxStaticLine* StaticLine3;
		wxButton* bRotationX;
		wxButton* bYPlus;
		wxStaticText* StaticText7;
		wxTextCtrl* txtAngle;
		wxTextCtrl* txtSpeed;
		wxTextCtrl* txtRotationZ;
		wxTextCtrl* txtPositionZ;
		wxButton* bZPlus;
		wxStaticText* StaticText4;
		wxButton* bGotoFloor;
		wxButton* bCollisions;
		wxStaticText* StaticText5;
		wxStaticText* StaticText30;
		wxButton* bStartDirection;
		wxStaticText* StaticText2;
		wxStaticText* StaticText27;
		wxTextCtrl* txtMoveSpeed;
		wxButton* bFreelookSpeed;
		wxTextCtrl* txtGravity;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxButton* bBinocularsFOV;
		wxButton* bResetFOV;
		wxStaticText* StaticText19;
		wxStaticText* StaticText29;
		wxTextCtrl* txtPositionY;
		wxButton* bStartPosition;
		wxStaticText* StaticText9;
		wxStaticText* StaticText33;
		wxButton* bFreelook;
		wxStaticText* StaticText11;
		wxStaticText* StaticText25;
		wxButton* bPositionZ;
		wxRadioButton* rRotation;
		//*)
		Ogre::Vector3 hold_vector;

	private:

		DECLARE_EVENT_TABLE()
};

}

#endif
