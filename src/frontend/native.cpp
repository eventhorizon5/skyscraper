/*
	Skyscraper 2.1 - Simulation Frontend (Native)
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

#ifndef USING_WX

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__

#include <mach-o/dyld.h>
#include <climits>

#elif
#include <unistd.h>
#endif

#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "camera.h"
#include "sky.h"
#include "skyscraper.h"

using namespace SBS;

namespace Skyscraper {

std::filesystem::path Skyscraper::GetExeDirectory()
{
	//return the executable path

#ifdef _WIN32
    // Windows specific
    wchar_t szPath[MAX_PATH];
    GetModuleFileNameW( NULL, szPath, MAX_PATH );
#elif __APPLE__
	// Mac specific
    char szPath[PATH_MAX];
    uint32_t bufsize = PATH_MAX;
    if (!_NSGetExecutablePath(szPath, &bufsize))
        return std::filesystem::path{szPath}.parent_path() / ""; // to finish the folder path with (back)slash
    return {};  // some error
#else
    // Linux specific
    char szPath[PATH_MAX];
    ssize_t count = readlink( "/proc/self/exe", szPath, PATH_MAX );
    if( count < 0 || count >= PATH_MAX )
        return {}; // some error
    szPath[count] = '\0';
#endif
}

bool Skyscraper::SetCWD()
{
	//set working directory
	std::string path = GetExeDirectory();
	#ifdef __APPLE__
	{
		path = path +  "../../../";
		vm->data_path = settingsPath() + "/Skyscraper/"; //Application Support folder
	}
	#endif
	#ifdef __LINUX__
		path = path + "../";
	#endif

	std::filesystem::current_path(path);

	return true;
}

bool Skyscraper::keyPressed(const OgreBites::KeyboardEvent& evt)
{
	//this function is run when a key is pressed

	EngineContext *engine = vm->GetActiveEngine();

	if (!engine)
		return false;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	if (!Simcore)
		return false;

	Camera *camera = Simcore->camera;

	if (!camera)
		return false;

	OgreBites::Keycode key = evt.keysym.sym;

    if (key == OgreBites::SDLK_ESCAPE)
    {
        //vm->Shutdown = true;
		Quit();
    }
	else if (key == OgreBites::SDLK_F2)
	{
		Real fps = Simcore->FPS;
		HAL *hal = vm->GetHAL();
		Ogre::RenderSystem *rendersystem = hal->mRoot->getRenderSystem();

		int batches = (int)rendersystem->_getBatchCount();
		int triangles = (int)rendersystem->_getFaceCount();
		int vertices = (int)rendersystem->_getVertexCount();

		hal->Report("FPS: " + ToString(Simcore->FPS) + " - Batches: " + ToString(batches) + " - Triangles: " + ToString(triangles) + " - Vertices: " + ToString(vertices), "");
	}
	else if (key == OgreBites::SDLK_SPACE)
	{
		//jump (spacebar) action
		if (camera->IsOnGround() == true)
			camera->Jump();
	}
	/*else if (key == OgreBites::v)
	{
		bool status = camera->GetGravityStatus();

		camera->EnableGravity(!status);
		camera->EnableCollisions(!status);

		if (status == false)
			vm->GetHAL()->Report("Gravity and collision detection on", "");
		else
			vm->GetHAL()->Report("Gravity and collision detection off", "");
	}*/
	/*else if (key == OgreBites::ctrl-r)
	{
		engine->Reload = true;
		return true;
	}*/
	else if (key == OgreBites::SDLK_F3)
	{
		//reset rotation/direction and FOV of camera
		camera->ResetView();
		return true;
	}
	else if (key == OgreBites::SDLK_F6)
	{
		//reset camera position and state
		camera->ResetState();
		return true;
	}
	else if (key == OgreBites::SDLK_F7)
	{
		//show colliders
		Simcore->ShowColliders(!colliders);
		colliders = !colliders;
	}
	else if (key == OgreBites::SDLK_F4)
	{
		//toggle wireframe mode
		if (wireframe == 0)
		{
			vm->GetSkySystem()->EnableSky(false);
			camera->SetViewMode(1);
			wireframe = 1;
		}
		else if (wireframe == 1)
		{
			camera->SetViewMode(2);
			wireframe = 2;
		}
		else if (wireframe == 2)
		{
			vm->GetSkySystem()->EnableSky(true);
			camera->SetViewMode(0);
			wireframe = 0;
		}
	}
	else if (key == OgreBites::SDLK_F11)
	{
		std::string prefix = GetDataPath();
		vm->GetHAL()->GetRenderWindow()->writeContentsToTimestampedFile(prefix + "screenshots/skyscraper-", ".jpg");
	}
	else if (key == OgreBites::SDLK_F5)
	{
		//toggle freelook mode
		EnableFreelook(!camera->Freelook);
	}
	else if (key == OgreBites::SDLK_F10)
	{
		//toggle fullscreen mode
		SetFullScreen(!FullScreen);
	}
	else if (key == OgreBites::SDLK_F8)
	{
		//show mesh bounding boxes
		Simcore->ShowBoundingBoxes(!boxes);
		boxes = !boxes;
	}
	else if (key == OgreBites::SDLK_F9)
	{
		//toggle statistics bar
		vm->GetHAL()->ToggleStats();
	}
	else if (key == OgreBites::SDLK_KP_MINUS)
	{
		//increase FOV angle
		Real angle = camera->GetFOVAngle() + camera->cfg_zoomspeed;
		camera->SetFOVAngle(angle);
	}
	else if (key == OgreBites::SDLK_KP_PLUS)
	{
		//decrease FOV angle
		Real angle = camera->GetFOVAngle() - camera->cfg_zoomspeed;
		camera->SetFOVAngle(angle);
	}
	//model pick-up
	/*else if (key == (wxKeyCode)key_pickup)
	{
		if (camera->IsModelAttached() == false)
			camera->PickUpModel();
		else
			camera->DropModel();
	}*/
	//load a new additional building
	/*else if (key == (wxKeyCode)key_load)
	{
		frontend->GetGUI()->ShowLoadDialog();
		return;
	}*/
	else if (key == OgreBites::SDLK_KP_1)
	{
		vm->SetActiveEngine(0);
	}
	else if (key == OgreBites::SDLK_KP_2)
	{
		vm->SetActiveEngine(1);
	}
	else if (key == OgreBites::SDLK_KP_3)
	{
		vm->SetActiveEngine(2);
	}
	else if (key == OgreBites::SDLK_KP_4)
	{
		vm->SetActiveEngine(3);
	}
	else if (key == OgreBites::SDLK_KP_5)
	{
		vm->SetActiveEngine(4);
	}
	else if (key == OgreBites::SDLK_KP_6)
	{
		vm->SetActiveEngine(5);
	}
	else if (key == OgreBites::SDLK_KP_7)
	{
		vm->SetActiveEngine(6);
	}
	else if (key == OgreBites::SDLK_KP_8)
	{
		vm->SetActiveEngine(7);
	}
	else if (key == OgreBites::SDLK_KP_9)
	{
		vm->SetActiveEngine(8);
	}
	else if (key == OgreBites::SDLK_KP_0)
	{
		vm->SetActiveEngine(9);
	}
	else if (key == OgreBites::SDLK_RETURN)
	{
		//enter or exit a vehicle
		camera->AttachToVehicle(!camera->inside_vehicle);
	}

	GetKeyStates(engine, key, true);

	ProcessMovement(engine, false, false);

	return true;
}

bool Skyscraper::keyReleased(const OgreBites::KeyboardEvent& evt)
{
	EngineContext *engine = vm->GetActiveEngine();

	if (!engine)
		return false;

	OgreBites::Keycode key = evt.keysym.sym;

	GetKeyStates(engine, key, false);

	ProcessMovement(engine, false, false);

	return true;
}

bool Skyscraper::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
	//this function runs when the mouse is moved

	if (!vm->GetActiveEngine())
		return false;

	//get SBS instance
	::SBS::SBS *Simcore = vm->GetActiveEngine()->GetSystem();

	if (!Simcore)
		return false;

	Camera *camera = Simcore->camera;

	if (!camera)
		return false;

	//get old mouse coordinates
	int old_mouse_x = camera->mouse_x;
	int old_mouse_y = camera->mouse_y;

	//get mouse pointer coordinates
	camera->mouse_x = evt.x;
	camera->mouse_y = evt.y;

	//freelook mode
	if (camera->Freelook == true)
	{
		if (freelook == false)
		{
			//reset values to prevent movement from getting stuck
			turn_right = 0;
			turn_left = 0;
		}

		freelook = true;

		//get window dimensions
		Real width = mRenderWindow->getWidth();
		Real height = mRenderWindow->getHeight();

		//if mouse coordinates changed, and we're in freelook mode, rotate camera
		if (old_mouse_x != camera->mouse_x || old_mouse_y != camera->mouse_y)
		{
			//WarpPointer(width * 0.5, height * 0.5);

			Vector3 rotational;
			rotational.x = -(((Real)camera->mouse_y - (height / 2))) / (height * 2);
			rotational.y = -((width / 2) - (Real)camera->mouse_x) / (width * 2);
			rotational.z = 0;

			Real fps_adjust = Simcore->FPS / 60;
			rotational *= fps_adjust;

			//apply freelook rotation
			camera->FreelookMove(rotational);
		}
		else
		{
			//reset rotation by reprocessing keyboard-based rotation
			ProcessMovement(vm->GetActiveEngine(), false, false, true);
		}
	}
	else
	{
		if (freelook == true)
		{
			//reset values to prevent movement from getting stuck
			strafe_right = 0;
			strafe_left = 0;
			if (old_mouse_x != camera->mouse_x || old_mouse_y != camera->mouse_y)
				camera->FreelookMove(Vector3::ZERO);
			ProcessMovement(vm->GetActiveEngine(), false, false, true);
		}
		freelook = false;
	}

	HandleMouseMovement();
	return true;
}

bool Skyscraper::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
	//this function is run when a mouse button is pressed

	//check if the user clicked on an object, and process it
	unsigned char button = evt.button;
	bool left = (button == '\x01');
	bool right = (button != '\x01');

	HAL *hal = vm->GetHAL();

	//apply content scaling factor, fixes issues for example on Retina displays
	Real scale = 1.0; //leave at 1.0 due to no wxWidgets available

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	//set scale to 1.0 on MacOS versions earlier than 10.15
	if (vm->macos_major == 10 && vm->macos_minor < 15)
		scale = 1.0;
#endif

	hal->ClickedObject(left, false, false, false, right, scale, false);

	return true;
}

bool Skyscraper::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
	bool left = (evt.button == '\x01');
	bool right = (evt.button != '\x02');

	if (left == true || right == true)
	{
		vm->GetHAL()->UnclickedObject();
	}
	else
		vm->GetHAL()->ClickedObject(!left, false, false, false, !right, 1.0, false);

	return true;
}

bool Skyscraper::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
	//enter or exit freelook mode using mouse scroll wheel
	if (evt.y > 0)
	{
		EnableFreelook(true);
		return true;
	}
	else if (evt.y < 0)
	{
		EnableFreelook(false);
		return true;
	}

	return false;
}

bool Skyscraper::touchMoved(const OgreBites::TouchFingerEvent &evt)
{
	return true;
}

bool Skyscraper::touchPressed(const OgreBites::TouchFingerEvent &evt)
{
	return true;
}

bool Skyscraper::touchReleased(const OgreBites::TouchFingerEvent &evt)
{
	return true;
}

void Skyscraper::GetKeyStates(EngineContext *engine, OgreBites::Keycode& key, bool down)
{
	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	//alt modifier
	/*if (event.AltDown())
	{
		//strafe movement
		if (key == WXK_RIGHT || key == (wxKeyCode)key_right)
			strafe_right = down;

		else if (key == WXK_LEFT || key == (wxKeyCode)key_left)
			strafe_left = down;

		else if (key == WXK_UP || key == (wxKeyCode)key_up)
			float_up = down;

		else if (key == WXK_DOWN || key == (wxKeyCode)key_down)
			float_down = down;

		else if (key == WXK_PAGEUP || key == (wxKeyCode)key_lookup)
			spin_up = down;

		else if (key == WXK_PAGEDOWN || key == (wxKeyCode)key_lookdown)
			spin_down = down;
	}
	else*/
	{
		if (camera->Freelook == false)
		{
			if (key == OgreBites::SDLK_RIGHT)
				turn_right = down;

			if (key == OgreBites::SDLK_LEFT)
				turn_left = down;

			//if (key == (wxKeyCode)key_straferight)
				//strafe_right = down;

			//if (key == (wxKeyCode)key_strafeleft)
				//strafe_left = down;
		}
		else
		{
			if (key == OgreBites::SDLK_RIGHT)
				strafe_right = down;

			if (key == OgreBites::SDLK_LEFT)
				strafe_left = down;
		}

		if (key == OgreBites::SDLK_PAGEUP)
			look_up = down;

		if (key == OgreBites::SDLK_PAGEDOWN)
			look_down = down;

		if (key == OgreBites::SDLK_UP)
			step_forward = down;

		if (key == OgreBites::SDLK_DOWN)
			step_backward = down;

		//binoculars
		/*if (key == (wxKeyCode)key_binoculars)
		{
			camera->Binoculars(down);
		}*/

		//crouch
		/*if (key == (wxKeyCode)key_crouch)
		{
			camera->Crouch(down);
		}*/

		//values from old version
		if (key == OgreBites::SDLK_HOME)
			float_up = down;
		if (key == OgreBites::SDLK_END)
			float_down = down;

		//drive functions, when user is inside a vehicle
		if (camera->Freelook == true && camera->inside_vehicle == true)
		{
			if (key == OgreBites::SDLK_LEFT)
				engine->GetSystem()->camera->Drive(true, false, false, false, down);
			if (key == OgreBites::SDLK_RIGHT)
				engine->GetSystem()->camera->Drive(false, true, false, false, down);
			if (key == OgreBites::SDLK_DOWN)
				engine->GetSystem()->camera->Drive(false, false, true, false, down);
			if (key == OgreBites::SDLK_UP)
				engine->GetSystem()->camera->Drive(false, false, false, true, down);
		}
	}
}

void Skyscraper::ProcessMovement(EngineContext *engine, bool control, bool shift, bool angle_only)
{
	//process movement

	Real strafe = 0, floatval = 0, spin = 0, turn = 0, look = 0, step = 0;

	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	Real speed_normal = camera->cfg_speed;
	Real speed_fast = camera->cfg_speedfast;
	Real speed_slow = camera->cfg_speedslow;

	if (angle_only == false)
	{
		camera->speed = 1;

		if (control == true)
			camera->speed = speed_slow;
		else if (shift == true)
			camera->speed = speed_fast;

		if (step_forward == true)
			step += speed_normal;
		if (step_backward == true)
			step -= speed_normal;

		if (strafe_left == true)
			strafe -= speed_normal;
		if (strafe_right == true)
			strafe += speed_normal;

		if (float_up == true)
			floatval += speed_normal;
		if (float_down == true)
			floatval -= speed_normal;

		//set camera motion values
		camera->Step(step);
		camera->Strafe(strafe);
		camera->Float(floatval);
	}

	if (spin_up == true)
		spin += speed_normal;
	if (spin_down == true)
		spin -= speed_normal;

	if (turn_left == true)
		turn -= speed_normal;
	if (turn_right == true)
		turn += speed_normal;

	if (look_up == true)
		look += speed_normal;
	if (look_down == true)
		look -= speed_normal;

	//set camera rotation values
	camera->Spin(spin);
	camera->Turn(turn);
	camera->Look(look);
}

void Skyscraper::EnableFreelook(bool value)
{
	//enable or disable freelook mode

	EngineContext *engine = vm->GetActiveEngine();

	if (!engine)
		return;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	Camera *camera = Simcore->camera;

	if (!camera)
		return;

	camera->Freelook = value;

#if defined(__WXMSW__)
	if (value == true)
		SetCursor(wxCURSOR_CROSS);
	else
		SetCursor(wxNullCursor);
#else

#ifdef USING_WX
	//detect for Wayland on Linux
	bool wayland = false;
	const char * val = std::getenv("WAYLAND_DISPLAY");
	if (val != 0)
		wayland = true;

	if (value == true)
	{
		if (wayland == false)
			wxSetCursor(wxCURSOR_CROSS);
		else
			wxSetCursor(wxCURSOR_BLANK); //set to blank for FreeLook to work on Wayland
	}
	else
		wxSetCursor(wxNullCursor);
#endif
#endif
}

void Skyscraper::HandleMouseMovement()
{
	EngineContext *engine = vm->GetActiveEngine();

	//if (!engine || IsActive() == false)
	if (!engine)
		return;

	if (engine->IsRunning() == false)
		return;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	if (!Simcore)
		return;

	Camera *camera = Simcore->camera;

	if (!camera)
		return;
}

}

#endif
