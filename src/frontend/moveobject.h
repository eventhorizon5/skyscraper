/* $Id$ */

/*
	Skyscraper 1.9 Alpha - Move Object Form
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

#ifndef MOVEOBJECT_H
#define MOVEOBJECT_H

//(*Headers(MoveObject)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class MoveObject: public wxDialog
{
	public:

		MoveObject(wxWindow* parent,wxWindowID id = -1, int object_number = 0);
		virtual ~MoveObject();

		//(*Identifiers(MoveObject)
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
		//*)
		void Loop();

	protected:

		//(*Handlers(MoveObject)
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
		void On_bSetSkyMult_Click(wxCommandEvent& event);
		void On_bGotoFloor_Click(wxCommandEvent& event);
		void On_bReportCollisions_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(MoveObject)
		wxStaticText* StaticText24;
		wxButton* bPositionY;
		wxStaticText* StaticText22;
		wxButton* bRotationY;
		wxStaticLine* StaticLine2;
		wxCheckBox* chkHold;
		wxTextCtrl* txtRotationY;
		wxStaticText* StaticText28;
		wxButton* bPositionX;
		wxRadioButton* rPosition;
		wxStaticText* lblRotation;
		wxButton* bZNeg;
		wxButton* bXPlus;
		wxStaticText* lblPosition;
		wxButton* bYNeg;
		wxStaticText* StaticText20;
		wxStaticText* StaticText1;
		wxStaticText* StaticText16;
		wxButton* bRotationZ;
		wxTextCtrl* txtRotationX;
		wxStaticText* StaticText23;
		wxButton* bXNeg;
		wxTextCtrl* txtPositionX;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine3;
		wxButton* bRotationX;
		wxButton* bYPlus;
		wxTextCtrl* txtRotationZ;
		wxTextCtrl* txtPositionZ;
		wxButton* bZPlus;
		wxStaticText* StaticText2;
		wxStaticText* StaticText27;
		wxTextCtrl* txtMoveSpeed;
		wxStaticText* StaticText26;
		wxTextCtrl* txtPositionY;
		wxStaticText* StaticText25;
		wxButton* bPositionZ;
		wxRadioButton* rRotation;
		//*)
		Ogre::Vector3 hold_vector;
		Object *object; //object number

	private:

		DECLARE_EVENT_TABLE()
};

#endif
