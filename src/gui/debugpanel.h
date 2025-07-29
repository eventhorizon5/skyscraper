/*
	Skyscraper 2.1 - Debug Panel
	Copyright (C)2004-2025 Ryan Thoryk
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
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
//*)
#include <wx/timer.h>

namespace Skyscraper {

class MeshControl;
class editelevator;
class VM;
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
class CameraTextureControl;
class SoundManager;
class TextureManager;
class EscalatorControl;
class WalkwayControl;
class RevolvingDoorControl;
class ControllerEditor;
class ScriptDebug;
class ScriptProcessor;
class FloorInfo;

class DebugPanel: public wxFrame
{
	friend class Timer;
	public:

		DebugPanel(VM *root, wxWindow* parent,wxWindowID id = -1);
		virtual ~DebugPanel();

		//(*Identifiers(DebugPanel)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT13;
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
		static const long ID_t_global;
		static const long ID_t_elevnumber;
		static const long ID_t_elevfloor;
		static const long ID_t_object;
		static const long ID_t_clickposition;
		static const long ID_t_collision;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT11;
		static const long ID_chkCollisionDetection;
		static const long ID_chkGravity;
		static const long ID_chkProcessElevators;
		static const long ID_chkAutoShafts;
		static const long ID_chkAutoStairs;
		static const long ID_chkRandom;
		static const long ID_chkMalfunctions;
		static const long ID_chkPower;
		static const long ID_CHECKBOX1;
		static const long ID_bFloorList;
		static const long ID_bMeshControl;
		static const long ID_bCameraControl;
		static const long ID_bEditElevator;
		static const long ID_bControlReference;
		static const long ID_bStats;
		static const long ID_bEngineManager;
		static const long ID_bConsole;
		static const long ID_bCameraTexture;
		static const long ID_bEscalator;
		static const long ID_bRevolvingDoor;
		static const long ID_bObjectInfo;
		static const long ID_bActionViewer;
		static const long ID_bPeopleManager;
		static const long ID_bSkyControl;
		static const long ID_bProfiler;
		static const long ID_bKeys;
		static const long ID_bTextures;
		static const long ID_bFloorInfo;
		static const long ID_bSoundManager;
		static const long ID_bMovingWalkway;
		static const long ID_bControllerEditor;
		static const long ID_PANEL1;
		//*)
		void EnableTimer(bool value);
		void Loop();
		VM* GetRoot() { return vm; }
		SBS::SBS* GetSystem() { return Simcore; }
		ScriptProcessor* GetScriptProcessor();
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
		VM* vm;
		ScriptProcessor *scriptproc;

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
		void On_bCameraTexture_Click(wxCommandEvent& event);
		void On_bLightControl_Click(wxCommandEvent& event);
		void On_bSoundManager_Click(wxCommandEvent& event);
		void On_chkMalfunctions_Click(wxCommandEvent& event);
		void On_bEscalator_Click(wxCommandEvent& event);
		void On_chkPower_Click(wxCommandEvent& event);
		void On_bMovingWalkway_Click(wxCommandEvent& event);
		void On_bRevolvingDoor_Click(wxCommandEvent& event);
		void On_bControllerEditor_Click(wxCommandEvent& event);
		//*)
		void OnInit();

		//(*Declarations(DebugPanel)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxBoxSizer* BoxSizer4;
		wxBoxSizer* BoxSizer5;
		wxBoxSizer* BoxSizer6;
		wxBoxSizer* BoxSizer7;
		wxButton* bActionViewer;
		wxButton* bCameraControl;
		wxButton* bCameraTexture;
		wxButton* bConsole;
		wxButton* bControlReference;
		wxButton* bControllerEditor;
		wxButton* bEditElevator;
		wxButton* bEngineManager;
		wxButton* bEscalator;
		wxButton* bFloorInfo;
		wxButton* bFloorList;
		wxButton* bKeys;
		wxButton* bMeshControl;
		wxButton* bMovingWalkway;
		wxButton* bObjectInfo;
		wxButton* bPeopleManager;
		wxButton* bProfiler;
		wxButton* bRevolvingDoor;
		wxButton* bSkyControl;
		wxButton* bSoundManager;
		wxButton* bStats;
		wxButton* bTextures;
		wxCheckBox* chkAutoShafts;
		wxCheckBox* chkAutoStairs;
		wxCheckBox* chkCollisionDetection;
		wxCheckBox* chkGravity;
		wxCheckBox* chkMalfunctions;
		wxCheckBox* chkPower;
		wxCheckBox* chkProcessElevators;
		wxCheckBox* chkRandom;
		wxCheckBox* chkVerbose;
		wxPanel* Panel1;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxStaticText* t_camerafloor;
		wxStaticText* t_camerap;
		wxStaticText* t_clickposition;
		wxStaticText* t_collision;
		wxStaticText* t_elevfloor;
		wxStaticText* t_elevnumber;
		wxStaticText* t_floorname;
		wxStaticText* t_framerate;
		wxStaticText* t_global;
		wxStaticText* t_object;
		wxStaticText* t_rotation;
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
		CameraTextureControl *camtex;
		SoundManager *smanager;
		TextureManager *tmanager;
		EscalatorControl *esc;
		WalkwayControl *walk;
		RevolvingDoorControl* revdoor;
		ControllerEditor* ceditor;
		FloorInfo* info;

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

