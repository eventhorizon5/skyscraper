/*
	Skyscraper 1.12 Alpha - Move Object Form
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef MOVEOBJECT_H
#define MOVEOBJECT_H

//(*Headers(MoveObject)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class MoveObject: public wxDialog
{
	public:

		MoveObject(DebugPanel* root, wxWindow* parent,wxWindowID id = -1, EngineContext *engine = 0, int object_number = 0);
		virtual ~MoveObject();

		//(*Identifiers(MoveObject)
		static const long ID_STATICTEXT1;
		static const long ID_lblPosition;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT2;
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
		wxButton* bPositionX;
		wxButton* bPositionY;
		wxButton* bPositionZ;
		wxButton* bRotationX;
		wxButton* bRotationY;
		wxButton* bRotationZ;
		wxButton* bXNeg;
		wxButton* bXPlus;
		wxButton* bYNeg;
		wxButton* bYPlus;
		wxButton* bZNeg;
		wxButton* bZPlus;
		wxCheckBox* chkHold;
		wxRadioButton* rPosition;
		wxRadioButton* rRotation;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticText* StaticText16;
		wxStaticText* StaticText1;
		wxStaticText* StaticText20;
		wxStaticText* StaticText22;
		wxStaticText* StaticText23;
		wxStaticText* StaticText24;
		wxStaticText* StaticText25;
		wxStaticText* StaticText26;
		wxStaticText* StaticText27;
		wxStaticText* StaticText28;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* lblPosition;
		wxStaticText* lblRelPosition;
		wxStaticText* lblRotation;
		wxTextCtrl* txtMoveSpeed;
		wxTextCtrl* txtPositionX;
		wxTextCtrl* txtPositionY;
		wxTextCtrl* txtPositionZ;
		wxTextCtrl* txtRotationX;
		wxTextCtrl* txtRotationY;
		wxTextCtrl* txtRotationZ;
		//*)
		Vector3 hold_vector;
		SBS::Object *object;
		int object_num; //object number

	private:

		SBS::SBS *Simcore;
		EngineContext *engine;
		DebugPanel *panel;

		DECLARE_EVENT_TABLE()
};

}

#endif
