/* $Id$ */

/*
	Skyscraper 1.5 Alpha - Debug Panel
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

#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

//(*Headers(DebugPanel)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
//*)
#include <wx/timer.h>
#include <wx/variant.h>

class DebugPanel: public wxFrame
{
	friend class Timer;
	public:

		DebugPanel(wxWindow* parent,wxWindowID id = -1);
		virtual ~DebugPanel();

		//(*Identifiers(DebugPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT6;
		static const long ID_t_camerafloor;
		static const long ID_t_camerap;
		static const long ID_t_rotation;
		static const long ID_t_elevnumber;
		static const long ID_t_elevfloor;
		static const long ID_t_object;
		static const long ID_t_collision;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT11;
		static const long ID_chkCollisionDetection;
		static const long ID_chkGravity;
		static const long ID_chkFrameLimiter;
		static const long ID_chkProcessElevators;
		static const long ID_chkAutoShafts;
		static const long ID_chkAutoStairs;
		static const long ID_chkFrameSync;
		static const long ID_bListAltitudes;
		static const long ID_bMeshControl;
		static const long ID_bCameraControl;
		static const long ID_bEditElevator;
		static const long ID_bControlReference;
		static const long ID_PANEL1;
		//*)
		class Timer : public wxTimer
		{
			public:
			Timer() { };
			virtual void Notify();
		};
		Timer *timer;

	protected:

		//(*Handlers(DebugPanel)
		void On_chkCollisionDetection_Click(wxCommandEvent& event);
		void On_chkFrameLimiter_Click(wxCommandEvent& event);
		void On_chkProcessElevators_Click(wxCommandEvent& event);
		void On_chkAutoShafts_Click(wxCommandEvent& event);
		void On_chkFrameSync_Click(wxCommandEvent& event);
		void On_bListAltitudes_Click(wxCommandEvent& event);
		void On_bMeshControl_Click(wxCommandEvent& event);
		void On_bEditElevator_Click(wxCommandEvent& event);
		void On_chkGravity_Click(wxCommandEvent& event);
		void On_chkAutoStairs_Click(wxCommandEvent& event);
		void On_bCameraControl_Click(wxCommandEvent& event);
		void On_bControlReference_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(DebugPanel)
		wxBoxSizer* BoxSizer4;
		wxBoxSizer* BoxSizer6;
		wxCheckBox* chkGravity;
		wxBoxSizer* BoxSizer5;
		wxBoxSizer* BoxSizer7;
		wxStaticText* t_elevfloor;
		wxStaticText* StaticText2;
		wxButton* bListAltitudes;
		wxCheckBox* chkProcessElevators;
		wxStaticText* t_rotation;
		wxStaticText* StaticText6;
		wxButton* bEditElevator;
		wxButton* bMeshControl;
		wxStaticText* StaticText8;
		wxStaticText* StaticText11;
		wxStaticText* t_framerate;
		wxPanel* Panel1;
		wxButton* bCameraControl;
		wxStaticText* StaticText1;
		wxBoxSizer* BoxSizer2;
		wxStaticText* StaticText3;
		wxCheckBox* chkCollisionDetection;
		wxStaticText* t_elevnumber;
		wxButton* bControlReference;
		wxStaticText* t_object;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxCheckBox* chkFrameSync;
		wxBoxSizer* BoxSizer1;
		wxStaticText* t_camerap;
		wxStaticText* t_camerafloor;
		wxStaticText* t_collision;
		wxBoxSizer* BoxSizer3;
		wxCheckBox* chkAutoStairs;
		wxCheckBox* chkAutoShafts;
		wxStaticText* StaticText4;
		wxCheckBox* chkFrameLimiter;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

wxString TruncateNumber(double value, int decimals);
wxString TruncateNumber(float value, int decimals);

#endif
