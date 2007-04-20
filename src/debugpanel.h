/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Debug Panel
	Copyright (C)2005-2007 Ryan Thoryk
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
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/frame.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
//*)
#include <wx/timer.h>
#include <wx/variant.h>

class Timer : public wxTimer
{
public:
	Timer() { };
	virtual void Notify();
};

class DebugPanel: public wxFrame
{
	friend class Timer;
	public:

		DebugPanel(wxWindow* parent,wxWindowID id = -1);
		virtual ~DebugPanel();

		//(*Identifiers(DebugPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_t_camerafloor;
		static const long ID_t_camerap;
		static const long ID_t_elevnumber;
		static const long ID_t_elevfloor;
		static const long ID_t_object;
		static const long ID_STATICTEXT11;
		static const long ID_chkCollisionDetection;
		static const long ID_chkFrameLimiter;
		static const long ID_chkMainProcessing;
		static const long ID_chkAutoShafts;
		static const long ID_chkFrameSync;
		static const long ID_bListAltitudes;
		static const long ID_bMeshControl;
		static const long ID_bInitRealtime;
		static const long ID_bEditElevator;
		//*)
		wxTimer *timer;

	protected:

		//(*Handlers(DebugPanel)
		void On_chkCollisionDetection_Click(wxCommandEvent& event);
		void On_chkFrameLimiter_Click(wxCommandEvent& event);
		void On_chkMainProcessing_Click(wxCommandEvent& event);
		void On_chkAutoShafts_Click(wxCommandEvent& event);
		void On_chkFrameSync_Click(wxCommandEvent& event);
		void On_bListAltitudes_Click(wxCommandEvent& event);
		void On_bMeshControl_Click(wxCommandEvent& event);
		void On_bInitRealtime_Click(wxCommandEvent& event);
		void On_bEditElevator_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(DebugPanel)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxBoxSizer* BoxSizer4;
		wxBoxSizer* BoxSizer6;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxBoxSizer* BoxSizer7;
		wxStaticText* t_camerafloor;
		wxStaticText* t_camerap;
		wxStaticText* t_elevnumber;
		wxStaticText* t_elevfloor;
		wxStaticText* t_object;
		wxBoxSizer* BoxSizer5;
		wxStaticText* StaticText11;
		wxCheckBox* chkCollisionDetection;
		wxCheckBox* chkFrameLimiter;
		wxCheckBox* chkMainProcessing;
		wxCheckBox* chkAutoShafts;
		wxCheckBox* chkFrameSync;
		wxBoxSizer* BoxSizer3;
		wxButton* bListAltitudes;
		wxButton* bMeshControl;
		wxButton* bInitRealtime;
		wxButton* bEditElevator;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
