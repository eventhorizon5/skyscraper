/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Debug Panel
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

#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

//(*Headers(DebugPanel)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/scrolbar.h>
#include <wx/settings.h>
#include <wx/stattext.h>
//*)
#include <wx/timer.h>

class Timer : public wxTimer
{
public:
	Timer() { };
	virtual void Notify();

private:
	char intbuffer[65];
	char buffer[20];

};

class DebugPanel: public wxDialog
{
	friend class Timer;
	public:

		DebugPanel(wxWindow* parent,wxWindowID id = -1);
		virtual ~DebugPanel();

		//(*Identifiers(DebugPanel)
		enum Identifiers
		{
		    ID_bListAltitudes = 0x1000,
		    st4c,
		    st5c,
		    st6c,
		    st7c,
		    st8c,
		    st9c,
		    st10c,
		    st11c,
		    ID_t_camerafloor,
		    ID_t_camerap,
		    ID_t_elevnumber,
		    ID_t_elevfloor,
		    ID_t_gotofloor,
		    ID_t_disttodest,
		    ID_t_rate,
		    ID_t_object,
		    st22c,
		    ID_chkCollisionDetection,
		    ID_bMeshControl,
		    ID_bInitRealtime,
		    ID_t_number,
		    ID_t_floor,
		    st44c,
		    ID_elevFloor,
		    ID_bEditElevator,
		    ID_bCallElevator,
		    ID_bGo,
		    ID_bOpen,
		    ID_bOpenManual,
		    ID_bEnqueueUp,
		    ID_bEnqueueDown,
		    ID_bClose,
		    ID_bCloseManual,
		    st55c,
		    ID_s_ElevFloor,
		    ID_s_ElevNum,
		    ID_chkFrameSync,
		    ID_chkAutoShafts,
		    ID_chkMainProcessing,
		    ID_chkFrameLimiter,
		    ID_bStop
		};
		//*)

	protected:

		//(*Handlers(DebugPanel)
		void On_bGo_Click(wxCommandEvent& event);
        void On_bCallElevator_Click(wxCommandEvent& event);
        void On_bOpen_Click(wxCommandEvent& event);
        void On_bOpenManual_Click(wxCommandEvent& event);
        void On_bEnqueueUp_Click(wxCommandEvent& event);
        void On_bEnqueueDown_Click(wxCommandEvent& event);
        void On_bClose_Click(wxCommandEvent& event);
        void On_bCloseManual_Click(wxCommandEvent& event);
        void On_bListAltitudes_Click(wxCommandEvent& event);
        void On_bMeshControl_Click(wxCommandEvent& event);
        void On_bInitRealtime_Click(wxCommandEvent& event);
        void On_chkCollisionDetection_Change(wxCommandEvent& event);
        void On_chkFrameLimiter_Change(wxCommandEvent& event);
        void On_bStop_Click(wxCommandEvent& event);
		void On_chkFrameSync_Change(wxCommandEvent& event);
        //*)
        void OnInit();

		//(*Declarations(DebugPanel)
		wxButton* bListAltitudes;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* t_camerafloor;
		wxStaticText* t_camerap;
		wxStaticText* t_elevnumber;
		wxStaticText* t_elevfloor;
		wxStaticText* t_gotofloor;
		wxStaticText* t_disttodest;
		wxStaticText* t_rate;
		wxStaticText* t_object;
		wxStaticText* StaticText17;
		wxCheckBox* chkCollisionDetection;
		wxButton* bMeshControl;
		wxButton* bInitRealtime;
		wxStaticText* t_number;
		wxStaticText* t_floor;
		wxStaticText* StaticText25;
		wxStaticText* elevFloor;
		wxButton* bEditElevator;
		wxButton* bCallElevator;
		wxButton* bGo;
		wxButton* bOpen;
		wxButton* bOpenManual;
		wxButton* bEnqueueUp;
		wxButton* bEnqueueDown;
		wxButton* bClose;
		wxButton* bCloseManual;
		wxStaticText* StaticText27;
		wxScrollBar* s_ElevFloor;
		wxScrollBar* s_ElevNum;
		wxCheckBox* chkFrameSync;
		wxCheckBox* chkAutoShafts;
		wxCheckBox* chkMainProcessing;
		wxCheckBox* chkFrameLimiter;
		wxButton* bStop;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
