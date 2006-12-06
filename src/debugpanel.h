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
		    st11c,
		    ID_t_camerafloor,
		    ID_t_camerap,
		    ID_t_elevnumber,
		    ID_t_elevfloor,
		    ID_t_object,
		    st22c,
		    ID_chkCollisionDetection,
		    ID_bMeshControl,
		    ID_bInitRealtime,
		    ID_bEditElevator,
		    st55c,
		    ID_chkFrameSync,
		    ID_chkAutoShafts,
		    ID_chkMainProcessing,
		    ID_chkFrameLimiter
		};
		//*)

	protected:

		//(*Handlers(DebugPanel)
		void On_bListAltitudes_Click(wxCommandEvent& event);
		void On_bMeshControl_Click(wxCommandEvent& event);
		void On_bInitRealtime_Click(wxCommandEvent& event);
		void On_chkCollisionDetection_Change(wxCommandEvent& event);
		void On_chkFrameLimiter_Change(wxCommandEvent& event);
		void On_chkFrameSync_Change(wxCommandEvent& event);
		void On_chkAutoShafts_Change(wxCommandEvent& event);
		void On_bEditElevator_Click(wxCommandEvent& event);
		//*)
        void OnInit();

		//(*Declarations(DebugPanel)
		wxButton* bListAltitudes;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText8;
		wxStaticText* t_camerafloor;
		wxStaticText* t_camerap;
		wxStaticText* t_elevnumber;
		wxStaticText* t_elevfloor;
		wxStaticText* t_object;
		wxStaticText* StaticText17;
		wxCheckBox* chkCollisionDetection;
		wxButton* bMeshControl;
		wxButton* bInitRealtime;
		wxButton* bEditElevator;
		wxStaticText* StaticText27;
		wxCheckBox* chkFrameSync;
		wxCheckBox* chkAutoShafts;
		wxCheckBox* chkMainProcessing;
		wxCheckBox* chkFrameLimiter;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
