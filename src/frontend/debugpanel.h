/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Debug Panel
	Copyright (C)2004-2017 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/stattext.h>
//*)
#include <wx/timer.h>

namespace Skyscraper {

class MeshControl;
class editelevator;
class Skyscraper;
class CameraControl;
class KeyDialog;
class Stats;
class ObjectInfo;
class Profiler;
class ActionViewer;
class SkyControl;
class EngineManager;
class PeopleManager;
class ParameterViewer;
class CreateObject;
class MoveObject;

class DebugPanel: public wxFrame
{
	friend class Timer;
	public:

		DebugPanel(Skyscraper *root, wxWindow* parent,wxWindowID id = -1);
		virtual ~DebugPanel();

		//(*Identifiers(DebugPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT6;
		static const long ID_t_camerafloor;
		static const long ID_t_floorname;
		static const long ID_t_camerap;
		static const long ID_t_rotation;
		static const long ID_t_elevnumber;
		static const long ID_t_elevfloor;
		static const long ID_t_object;
		static const long ID_t_clickposition;
		static const long ID_t_collision;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT11;
		static const long ID_chkCollisionDetection;
		static const long ID_chkGravity;
		static const long ID_chkFrameLimiter;
		static const long ID_chkProcessElevators;
		static const long ID_chkAutoShafts;
		static const long ID_chkAutoStairs;
		static const long ID_chkRandom;
		static const long ID_CHECKBOX1;
		static const long ID_bFloorList;
		static const long ID_bMeshControl;
		static const long ID_bCameraControl;
		static const long ID_bEditElevator;
		static const long ID_bControlReference;
		static const long ID_bStats;
		static const long ID_bEngineManager;
		static const long ID_bConsole;
		static const long ID_bObjectInfo;
		static const long ID_bActionViewer;
		static const long ID_bPeopleManager;
		static const long ID_bSkyControl;
		static const long ID_bProfiler;
		static const long ID_bKeys;
		static const long ID_bTextures;
		static const long ID_bFloorInfo;
		static const long ID_PANEL1;
		//*)
		void EnableTimer(bool value);
		void Loop();
		Skyscraper* GetRoot() { return skyscraper; }
		SBS::SBS* GetSystem() { return Simcore; }
		void ShowControlReference();
		class Timer : public wxTimer
		{
			public:
			Timer(DebugPanel* parent, SBS::SBS *engine) { dp = parent; Simcore = engine; };
			virtual void Notify();
			SBS::SBS *Simcore;
			DebugPanel* dp;
		};
		Timer *timer;
		SBS::SBS *Simcore;
		Skyscraper* skyscraper;

	protected:

		//(*Handlers(DebugPanel)
		void On_chkCollisionDetection_Click(wxCommandEvent& event);
		void On_chkFrameLimiter_Click(wxCommandEvent& event);
		void On_chkProcessElevators_Click(wxCommandEvent& event);
		void On_chkAutoShafts_Click(wxCommandEvent& event);
		void On_chkFrameSync_Click(wxCommandEvent& event);
		void On_bFloorList_Click(wxCommandEvent& event);
		void On_bMeshControl_Click(wxCommandEvent& event);
		void On_bEditElevator_Click(wxCommandEvent& event);
		void On_chkGravity_Click(wxCommandEvent& event);
		void On_chkAutoStairs_Click(wxCommandEvent& event);
		void On_bCameraControl_Click(wxCommandEvent& event);
		void On_bControlReference_Click(wxCommandEvent& event);
		void On_bStats_Click(wxCommandEvent& event);
		void On_bConsole_Click(wxCommandEvent& event);
		void On_chkVerbose_Click(wxCommandEvent& event);
		void On_bObjectInfo_Click(wxCommandEvent& event);
		void On_chkRandom_Click(wxCommandEvent& event);
		void On_bProfiler_Click(wxCommandEvent& event);
		void On_bActionViewer_Click(wxCommandEvent& event);
		void On_bKeys_Click(wxCommandEvent& event);
		void On_bTextures_Click(wxCommandEvent& event);
		void On_bFloorInfo_Click(wxCommandEvent& event);
		void On_bSkyControl_Click(wxCommandEvent& event);
		void On_bEngineManager_Click(wxCommandEvent& event);
		void On_bPeopleManager_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(DebugPanel)
		wxButton* bEngineManager;
		wxButton* bSkyControl;
		wxPanel* Panel1;
		wxStaticText* t_framerate;
		wxCheckBox* chkCollisionDetection;
		wxButton* bFloorList;
		wxButton* bTextures;
		wxStaticText* t_elevnumber;
		wxButton* bPeopleManager;
		wxStaticText* t_camerap;
		wxStaticText* t_elevfloor;
		wxCheckBox* chkVerbose;
		wxBoxSizer* BoxSizer7;
		wxCheckBox* chkAutoStairs;
		wxCheckBox* chkFrameLimiter;
		wxButton* bFloorInfo;
		wxCheckBox* chkAutoShafts;
		wxButton* bControlReference;
		wxButton* bCameraControl;
		wxBoxSizer* BoxSizer2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
		wxButton* bActionViewer;
		wxStaticText* StaticText3;
		wxButton* bStats;
		wxButton* bProfiler;
		wxButton* bObjectInfo;
		wxStaticText* t_floorname;
		wxStaticText* StaticText8;
		wxBoxSizer* BoxSizer4;
		wxCheckBox* chkGravity;
		wxButton* bKeys;
		wxStaticText* StaticText7;
		wxStaticText* t_collision;
		wxStaticText* StaticText4;
		wxBoxSizer* BoxSizer1;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxStaticText* t_clickposition;
		wxStaticText* t_object;
		wxStaticText* StaticText6;
		wxButton* bEditElevator;
		wxStaticText* t_camerafloor;
		wxStaticText* t_rotation;
		wxCheckBox* chkProcessElevators;
		wxCheckBox* chkRandom;
		wxStaticText* StaticText9;
		wxBoxSizer* BoxSizer6;
		wxButton* bConsole;
		wxStaticText* StaticText11;
		wxBoxSizer* BoxSizer5;
		wxButton* bMeshControl;
		//*)

	private:

		MeshControl *mc;
		editelevator *ee;
		CameraControl *cc;
		KeyDialog *kd;
		Stats *stats;
		ObjectInfo *objectinfo;
		Profiler *profiler;
		ActionViewer *actionviewer;
		SkyControl *skycontrol;
		EngineManager *emanager;
		PeopleManager *pmanager;

		DECLARE_EVENT_TABLE()
};

wxString TruncateNumber(float value, int decimals);
wxString TruncateNumber(double value, int decimals);

bool IsNumeric(const wxString &string);
bool IsNumeric(const wxString &string, int &number);
bool IsNumeric(const wxString &string, float &number);
bool IsNumeric(const wxString &string, double &number);

}

#endif

