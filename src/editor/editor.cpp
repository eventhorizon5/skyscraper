/*
	Skyscraper 2.1 - Editor
	Copyright (C)2003-2025 Ryan Thoryk
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

#include <OgreRoot.h>
#include <OgreImGuiOverlay.h>
#include <OgreImGuiInputListener.h>

//ImGuizmo system
//#include "ImGuizmo.h"

#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "object.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "editor.h"

#include <unordered_map>

struct TRS {
	float pos[3]  = {0,0,0}; // x,y,z
	float rot[3]  = {0,0,0}; // pitch(Y), yaw(Z), roll(X)
	float scl[3]  = {1,1,1}; // sx,sy,sz
	bool  hasInit = false;
};

static std::unordered_map<int, TRS> s_trsById;

//helper: clamp angles to [-180,180] just to keep UI tidy
static inline float WrapAngle180(float a)
{
	while (a > 180.f)
		a -= 360.f;
	while (a < -180.f)
		a += 360.f;
	return a;
}

namespace Skyscraper {

Editor::Editor(VM *parent)
{
	this->vm = parent;
	enabled = false;
	initialized = false;
}

Editor::~Editor()
{

}

bool Editor::Initialize()
{
	HAL *hal = vm->GetHAL();

	//initialize ImGui Overlay
	if (vm->GetHAL()->IsVREnabled() == false)
	{
		try
		{
			imgui = new Ogre::ImGuiOverlay();
			imgui->setZOrder(300);
			imgui->show();
			Ogre::OverlayManager::getSingleton().addOverlay(imgui);
		}
		catch(Ogre::Exception &e)
		{
			return hal->ReportFatalError("Error initializing ImGui overlay\nDetails: " + e.getDescription(), "editor:");
		}
		initialized = true;
	}

	return true;
}

bool Editor::Run()
{
	//process imgui

	if (!imgui || !initialized)
		return false;

	Ogre::ImGuiOverlay::NewFrame();
	//ImGuizmo::BeginFrame();

	if (!enabled)
		return true;

	Process();

	return true;
}

void Editor::Process()
{
	//process ImGui

	//ImGui::ShowDemoWindow();
	ShowTransformGizmoWindow();
}

void Editor::Unload()
{
	//remove imgui overlay

	if (!imgui || !initialized)
		return;

	imgui->hide();
	Ogre::OverlayManager::getSingleton().destroy(imgui);
	//delete imgui;
	imgui = 0;
	initialized = false;
}

void Editor::Enable(bool value)
{
	enabled = value;
}

bool Editor::IsEnabled()
{
	return enabled;
}

bool Editor::IsInitialized()
{
	return initialized;
}

void Editor::UpdateFrame(Real size_x, Real size_y, Real scale, Real delta)
{
	//update ImGui frame data

	if (!imgui || !initialized)
		return;

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)size_x, (float)size_y);
	io.DisplayFramebufferScale = ImVec2(scale, scale);
	io.DeltaTime = delta > 0.f ? delta : (1.0f / 60.0f);
}

void Editor::ShowTransformGizmoWindow()
{
	if (!vm->GetActiveSystem())
		return;

	::SBS::SBS *Simcore = vm->GetActiveSystem();
	if (!Simcore->camera)
		return;

	ImGui::Begin("Transform Gizmo", &enabled);

	static int objId = vm->GetActiveSystem()->camera->GetClickedObjectNumber();
	SBS::Object *object = Simcore->GetObject(objId);

	// --- Object selector ---
	ImGui::InputInt("Object ID", &objId);
	TRS& st = s_trsById[objId];

	// --- Space & pivot options (UI only; wire to your backend as needed) ---
	static bool localSpace = true;
	ImGui::Checkbox("Local space", &localSpace);
	ImGui::SameLine();
	static bool uniformScale = true;
	ImGui::Checkbox("Uniform scale", &uniformScale);

	// --- Snap & step config ---
	ImGui::Separator();
	static bool snap = false;
	ImGui::Checkbox("Snap", &snap);
	ImGui::SameLine();
	static float moveStep = 0.1f;
	ImGui::SetNextItemWidth(100); ImGui::InputFloat("Move step", &moveStep);
	ImGui::SameLine();
	static float rotStepDeg = 5.0f;
	ImGui::SetNextItemWidth(100); ImGui::InputFloat("Rot step°", &rotStepDeg);
	ImGui::SameLine();
	static float scaleStep = 0.1f;
	ImGui::SetNextItemWidth(100); ImGui::InputFloat("Scale step", &scaleStep);

	//pull current transform from engine
	if (!st.hasInit && object)
	{
		Vector3 pos = object->GetPosition();
		st.pos[0] = pos.x;
		st.pos[1] = pos.y;
		st.pos[2] = pos.z;
		Vector3 rot = object->GetRotation();
		st.rot[0] = rot.x;
		st.rot[1] = rot.y;
		st.rot[2] = rot.z;
		st.scl[0] = 1.0;
		st.scl[1] = 1.0;
		st.scl[2] = 1.0;
		st.hasInit = true;
	}

	// --- Position ---
	ImGui::SeparatorText("Position");
	if (ImGui::DragFloat3("P (X,Y,Z)", st.pos, 0.01f))
	{
		if (snap)
		{
			for (int i=0;i<3;i++)
			{
				st.pos[i] = std::round(st.pos[i] / moveStep) * moveStep;
			}
		}
	}
	if (ImGui::Button("Apply Position"))
	{
		Simcore->MoveObject(object, Vector3(st.pos[0], st.pos[1], st.pos[2]), localSpace, true, true, true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Nudge -X")) { float d=-moveStep; /*MoveObjectDelta(objId, d,0,0, localSpace);*/ st.pos[0]+=d; }
	ImGui::SameLine();
	if (ImGui::Button("+X"))       { float d= moveStep; /*MoveObjectDelta(objId, d,0,0, localSpace);*/ st.pos[0]+=d; }
	if (ImGui::Button("Nudge -Y")) { float d=-moveStep; /*MoveObjectDelta(objId, 0,d,0, localSpace);*/ st.pos[1]+=d; }
	ImGui::SameLine();
	if (ImGui::Button("+Y"))       { float d= moveStep; /*MoveObjectDelta(objId, 0,d,0, localSpace);*/ st.pos[1]+=d; }
	if (ImGui::Button("Nudge -Z")) { float d=-moveStep; /*MoveObjectDelta(objId, 0,0,d, localSpace);*/ st.pos[2]+=d; }
	ImGui::SameLine();
	if (ImGui::Button("+Z"))       { float d= moveStep; /*MoveObjectDelta(objId, 0,0,d, localSpace);*/ st.pos[2]+=d; }

	// --- Rotation (degrees) ---
	ImGui::SeparatorText("Rotation (deg)");
	bool rotChanged = false;
	rotChanged |= ImGui::DragFloat("Pitch (Y)", &st.rot[0], 0.1f, -9999.f, 9999.f);
	rotChanged |= ImGui::DragFloat("Yaw   (Z)", &st.rot[1], 0.1f, -9999.f, 9999.f);
	rotChanged |= ImGui::DragFloat("Roll  (X)", &st.rot[2], 0.1f, -9999.f, 9999.f);
	if (rotChanged && snap)
	{
		st.rot[0] = std::round(st.rot[0] / rotStepDeg) * rotStepDeg;
		st.rot[1] = std::round(st.rot[1] / rotStepDeg) * rotStepDeg;
		st.rot[2] = std::round(st.rot[2] / rotStepDeg) * rotStepDeg;
	}
	// keep UI neat
	st.rot[0] = WrapAngle180(st.rot[0]);
	st.rot[1] = WrapAngle180(st.rot[1]);
	st.rot[2] = WrapAngle180(st.rot[2]);

	if (ImGui::Button("Apply Rotation"))
	{
		Simcore->RotateObject(object, Vector3(st.rot[0], st.rot[1], st.rot[2]), 1.0, localSpace, true, true, true);
	}
	ImGui::SameLine();
	if (ImGui::Button("-Pitch"))   { /*RotateObjectDeltaEulerDeg(objId, -rotStepDeg, 0, 0, localSpace);*/ st.rot[0] = WrapAngle180(st.rot[0]-rotStepDeg); }
	ImGui::SameLine();
	if (ImGui::Button("+Pitch"))   { /*RotateObjectDeltaEulerDeg(objId,  rotStepDeg, 0, 0, localSpace);*/ st.rot[0] = WrapAngle180(st.rot[0]+rotStepDeg); }
	if (ImGui::Button("-Yaw"))     { /*RotateObjectDeltaEulerDeg(objId, 0,-rotStepDeg, 0, localSpace);*/ st.rot[1] = WrapAngle180(st.rot[1]-rotStepDeg); }
	ImGui::SameLine();
	if (ImGui::Button("+Yaw"))     { /*RotateObjectDeltaEulerDeg(objId, 0, rotStepDeg, 0, localSpace);*/ st.rot[1] = WrapAngle180(st.rot[1]+rotStepDeg); }
	if (ImGui::Button("-Roll"))    { /*RotateObjectDeltaEulerDeg(objId, 0, 0,-rotStepDeg, localSpace);*/ st.rot[2] = WrapAngle180(st.rot[2]-rotStepDeg); }
	ImGui::SameLine();
	if (ImGui::Button("+Roll"))    { /*RotateObjectDeltaEulerDeg(objId, 0, 0, rotStepDeg, localSpace);*/ st.rot[2] = WrapAngle180(st.rot[2]+rotStepDeg); }

	// --- Scale ---
	ImGui::SeparatorText("Scale");
	if (uniformScale)
	{
		float s = st.scl[0];
		if (ImGui::DragFloat("S (uniform)", &s, 0.01f, 0.001f, 1000.f))
		{
			if (snap) s = std::round(s / scaleStep) * scaleStep;
			st.scl[0]=st.scl[1]=st.scl[2]=s;
		}
	}
	else
	{
		if (ImGui::DragFloat3("Sx/Sy/Sz", st.scl, 0.01f, 0.001f, 1000.f))
		{
			if (snap)
				for (int i=0;i<3;i++)
					st.scl[i] = std::round(st.scl[i] / scaleStep) * scaleStep;
		}
	}
	if (ImGui::Button("Apply Scale"))
	{
		// Absolute set:
		// ScaleObjectAbs(objId, st.scl[0], st.scl[1], st.scl[2]);
	}
	ImGui::SameLine();
	if (ImGui::Button("-S")) { float d = -scaleStep; /*ScaleObjectDelta(objId, d,d,d);*/ st.scl[0]+=d; st.scl[1]+=d; st.scl[2]+=d; }
	ImGui::SameLine();
	if (ImGui::Button("+S")) { float d =  scaleStep; /*ScaleObjectDelta(objId, d,d,d);*/ st.scl[0]+=d; st.scl[1]+=d; st.scl[2]+=d; }

	// --- Quick actions ---
	ImGui::Separator();
	if (ImGui::Button("Apply All (P/R/S)"))
	{
		// MoveObjectAbs(objId, st.pos[0], st.pos[1], st.pos[2], localSpace);
		// RotateObjectAbsEulerDeg(objId, st.rot[0], st.rot[1], st.rot[2], localSpace);
		// ScaleObjectAbs(objId, st.scl[0], st.scl[1], st.scl[2]);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset TRS"))
	{
		st.pos[0]=st.pos[1]=st.pos[2]=0;
		st.rot[0]=st.rot[1]=st.rot[2]=0;
		st.scl[0]=st.scl[1]=st.scl[2]=1;
		// Optionally also push to engine immediately.
	}

	ImGui::End();
}

}
