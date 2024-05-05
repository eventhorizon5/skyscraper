/*
	Skyscraper 1.12 Alpha - Main Screen
	Copyright (C)2003-2024 Ryan Thoryk
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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRTShaderSystem.h>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "gui/debugpanel.h"
#include "skyscraper.h"
#include "server.h"
#include "enginecontext.h"
#include "gui/loaddialog.h"
#include "mainscreen.h"

using namespace SBS;

namespace Skyscraper {

BEGIN_EVENT_TABLE(MainScreen, wxFrame)
  EVT_ICONIZE(MainScreen::OnIconize)
  EVT_SIZE(MainScreen::OnSize)
  EVT_CLOSE(MainScreen::OnClose)
  EVT_IDLE(MainScreen::OnIdle)
  EVT_PAINT(MainScreen::OnPaint)
  EVT_ACTIVATE(MainScreen::OnActivate)
END_EVENT_TABLE()

MainScreen::MainScreen(Skyscraper *parent, int width, int height) : wxFrame(0, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	frontend = parent;
	Active = false;
	InLoop = false;
	panel = 0;
	Center();
	wxString title;
	title = wxT("Skyscraper " + frontend->version + " " + frontend->version_state);
	SetTitle(title);
	SetClientSize(width, height);
	SetExtraStyle(wxWS_EX_PROCESS_IDLE);

	//reset input states
	boxes = false;
	colliders = false;
	wireframe = 0;
	strafe_left = false;
	strafe_right = false;
	float_up = false;
	float_down = false;
	spin_up = false;
	spin_down = false;
	turn_left = false;
	turn_right = false;
	look_up = false;
	look_down = false;
	step_forward = false;
	step_backward = false;

	freelook = false;

	key_right = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Right", "D")[0];
	key_left = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Left", "A")[0];
	key_up = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Up", "W")[0];
	key_down = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Down", "S")[0];
	key_lookup = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.LookUp", "P")[0];
	key_lookdown = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.LookDown", "L")[0];
	key_binoculars = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Binoculars", "B")[0];
	key_crouch = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Crouch", "X")[0];
	key_floatup = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.FloatUp", "O")[0];
	key_floatdown = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.FloatDown", "K")[0];
	key_noclip = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.NoClip", "V")[0];
	key_pickup = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.PickUp", "C")[0];
	key_load = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Load", ";")[0];
	key_enter = frontend->GetKeyConfigString("Skyscraper.Frontend.Keyboard.Enter", "E")[0];

	//create panel, rendering is done on this, along with keyboard and mouse events
	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(width, height), wxNO_BORDER);
	panel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(MainScreen::OnKeyDown), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(MainScreen::OnKeyUp), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_LEFT_DOWN, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_LEFT_UP, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_LEFT_DCLICK, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_MIDDLE_DOWN, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_MIDDLE_UP, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_RIGHT_DOWN, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_RIGHT_UP, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_RIGHT_DCLICK, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_MOUSEWHEEL, wxMouseEventHandler(MainScreen::OnMouseButton), NULL, this);
}

void MainScreen::OnIconize(wxIconizeEvent& event)
{
	//pause simulator while minimized

	frontend->Pause = event.IsIconized();

	if (frontend->Pause == true)
		frontend->Report("Pausing simulator...");
	else
		frontend->Report("Resuming simulator...");
}

void MainScreen::OnSize(wxSizeEvent& WXUNUSED(event))
{
	if (panel)
		panel->SetSize(this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight());

	if (frontend->mRenderWindow)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		Real scale = this->GetContentScaleFactor();
		frontend->mRenderWindow->resize(this->GetClientSize().GetWidth() * scale, this->GetClientSize().GetHeight() * scale);
#else
		frontend->mRenderWindow->windowMovedOrResized();
#endif
	}
	if (frontend->mCamera)
		frontend->mCamera->setAspectRatio(Real(frontend->mViewport->getActualWidth()) / Real(frontend->mViewport->getActualHeight()));
}

void MainScreen::OnClose(wxCloseEvent& event)
{
	if (frontend)
	{
		if (frontend->StartupRunning == false)
		{
			int result = wxMessageBox(wxT("Are you sure you want to exit?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
			if (result == wxNO)
				return;
		}
	}

	wxGetApp().Exit();
}

void MainScreen::ShowWindow()
{
	Show(true);
}

void MainScreen::OnIdle(wxIdleEvent& event)
{
	//prevent simultaneous executions - this fixes an issue with wxYield
	if (InLoop == false)
	{
		InLoop = true;

		if (frontend->Pause == false)
		{
			if (this->HasFocus() && !panel->HasFocus())
				panel->SetFocus();

			try {
				bool result = frontend->Loop(); //run simulator loop
				if (!result)
					frontend->Quit();
			}
			catch (Ogre::Exception &e)
			{
				frontend->ReportFatalError("Unhandled OGRE exception:\n\n" + e.getFullDescription() + "\n\nSkyscraper will now exit.  Please report this as a bug.");
				frontend->Quit();
			}

			HandleMouseMovement();
			event.RequestMore(); //request more idles
		}

		InLoop = false;
	}
}

void MainScreen::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	//if (frontend->mRenderWindow)
		//frontend->mRenderWindow->update(true);
}

void MainScreen::OnActivate(wxActivateEvent &event)
{
	Active = event.GetActive();
	event.Skip();
}

void MainScreen::OnKeyDown(wxKeyEvent& event)
{
	//this function is run when a key is pressed

	EngineContext *engine = frontend->GetServer()->GetActiveEngine();

	if (!engine)
		return;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	if (!Simcore)
		return;

	Camera *camera = Simcore->camera;

	if (!camera)
		return;

	int key = event.GetKeyCode();

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE
	//don't process a key down event if the key is actually up, this fixes a "stuck key" issue on some systems
	if (wxGetKeyState((wxKeyCode)key) == false)
		return;
#endif

	if (key == WXK_ESCAPE)
	{
		int result = wxMessageBox(wxT("Exit and return to the main menu?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		if (result == wxYES)
			frontend->Shutdown = true;
		return;
	}

	if (key == WXK_F1)
	{
		if (frontend->dpanel)
			frontend->dpanel->ShowControlReference();
	}

	if (key == WXK_F2)
	{
		Real fps = Simcore->FPS;
		Ogre::RenderSystem *rendersystem = frontend->mRoot->getRenderSystem();

		int batches = (int)rendersystem->_getBatchCount();
		int triangles = (int)rendersystem->_getFaceCount();
		int vertices = (int)rendersystem->_getVertexCount();

		frontend->Report("FPS: " + ToString(Simcore->FPS) + " - Batches: " + ToString(batches) + " - Triangles: " + ToString(triangles) + " - Vertices: " + ToString(vertices));
	}

	//alt modifier
	if (event.AltDown())
	{
		//crash test
		if (event.ControlDown() && key == (wxKeyCode)'C')
			throw;

		if (key == WXK_F4)
			Close();
	}
	else
	{
		if (key == WXK_SPACE)
		{
			if (camera->IsOnGround() == true)
				camera->Jump();
		}

		if (key == (wxKeyCode)key_noclip)
		{
			bool status = camera->GetGravityStatus();

			camera->EnableGravity(!status);
			camera->EnableCollisions(!status);

			if (status == false)
				frontend->Report("Gravity and collision detection on");
			else
				frontend->Report("Gravity and collision detection off");
		}

		if (event.ControlDown() == true && key == (wxKeyCode)'R')
		{
			engine->Reload = true;
			return;
		}

		if (key == WXK_F3)
		{
			//reset rotation/direction and FOV of camera
			camera->ResetView();
			return;
		}

		if (key == WXK_F6)
		{
			//reset camera position and state
			camera->ResetState();
			return;
		}

		if (key == WXK_F7)
		{
			//show colliders
			Simcore->ShowColliders(!colliders);
			colliders = !colliders;
		}

		if (key == WXK_F4)
		{
			//toggle wireframe mode
			if (wireframe == 0)
			{
				frontend->EnableSky(false);
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
				frontend->EnableSky(true);
				camera->SetViewMode(0);
				wireframe = 0;
			}
		}

		if (key == WXK_F11)
		{
			std::string prefix = frontend->GetDataPath();
			frontend->mRenderWindow->writeContentsToTimestampedFile(prefix + "screenshots/skyscraper-", ".jpg");
		}

		if (key == WXK_F12 && !frontend->dpanel)
		{
			//show control panel if closed
			frontend->dpanel = new DebugPanel(frontend, NULL, -1);
			frontend->dpanel->Show(true);
			frontend->dpanel->SetPosition(wxPoint(frontend->GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), frontend->GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}

		if (key == WXK_F5)
		{
			//toggle freelook mode
			EnableFreelook(!camera->Freelook);
		}

		if (key == WXK_F10)
		{
			//toggle fullscreen mode
			frontend->SetFullScreen(!frontend->FullScreen);
		}

		if (key == WXK_F8)
		{
			//show mesh bounding boxes
			Simcore->ShowBoundingBoxes(!boxes);
			boxes = !boxes;
		}

		if (key == WXK_F9)
		{
			//toggle statistics bar
			frontend->ToggleStats();
		}

		if (key == WXK_NUMPAD_SUBTRACT || key == (wxKeyCode)'[')
		{
			//increase FOV angle
			Real angle = camera->GetFOVAngle() + camera->cfg_zoomspeed;
			camera->SetFOVAngle(angle);
		}

		if (key == WXK_NUMPAD_ADD || key == (wxKeyCode)']')
		{
			//decrease FOV angle
			Real angle = camera->GetFOVAngle() - camera->cfg_zoomspeed;
			camera->SetFOVAngle(angle);
		}

		//model pick-up
		if (key == (wxKeyCode)key_pickup)
		{
			if (camera->IsModelAttached() == false)
				camera->PickUpModel();
			else
				camera->DropModel();
		}

		//load a new additional building
		if (key == (wxKeyCode)key_load)
		{
			if (!frontend->loaddialog)
				frontend->loaddialog = new LoadDialog(frontend->dpanel, this, -1);
			frontend->loaddialog->CenterOnScreen();
			frontend->loaddialog->Show();
			return;
		}

		//engine selection
		/*if (key == (wxKeyCode)'1')
		{
			frontend->GetServer()->SetActiveEngine(0);
		}
		else if (key == (wxKeyCode)'2')
		{
			frontend->GetServer()->SetActiveEngine(1);
		}
		else if (key == (wxKeyCode)'3')
		{
			frontend->GetServer()->SetActiveEngine(2);
		}
		else if (key == (wxKeyCode)'4')
		{
			frontend->GetServer()->SetActiveEngine(3);
		}
		else if (key == (wxKeyCode)'5')
		{
			frontend->GetServer()->SetActiveEngine(4);
		}
		else if (key == (wxKeyCode)'6')
		{
			frontend->GetServer()->SetActiveEngine(5);
		}
		else if (key == (wxKeyCode)'7')
		{
			frontend->GetServer()->SetActiveEngine(6);
		}
		else if (key == (wxKeyCode)'8')
		{
			frontend->GetServer()->SetActiveEngine(7);
		}
		else if (key == (wxKeyCode)'9')
		{
			frontend->GetServer()->SetActiveEngine(8);
		}
		else if (key == (wxKeyCode)'0')
		{
			frontend->GetServer()->SetActiveEngine(9);
		}*/

		//enter and exit a vehicle
		if (key == (wxKeyCode)key_enter)
		{
			//enter or exit a vehicle
			camera->AttachToVehicle(!camera->inside_vehicle);
		}
	}

	GetKeyStates(engine, event, true);

	ProcessMovement(engine, event.ControlDown(), event.ShiftDown());
}

void MainScreen::OnKeyUp(wxKeyEvent& event)
{
	//this function is run when a key is released

	EngineContext *engine = frontend->GetServer()->GetActiveEngine();

	if (!engine)
		return;

	GetKeyStates(engine, event, false);

	ProcessMovement(engine, event.ControlDown(), event.ShiftDown());
}

void MainScreen::GetKeyStates(EngineContext *engine, wxKeyEvent& event, bool down)
{
	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	int key = event.GetKeyCode();

	//alt modifier
	if (event.AltDown())
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
	else
	{
		if (camera->Freelook == false)
		{
			if (key == WXK_RIGHT || key == (wxKeyCode)key_right)
				turn_right = down;

			if (key == WXK_LEFT || key == (wxKeyCode)key_left)
				turn_left = down;
		}
		else
		{
			if (key == WXK_RIGHT || key == (wxKeyCode)key_right)
				strafe_right = down;

			if (key == WXK_LEFT || key == (wxKeyCode)key_left)
				strafe_left = down;
		}

		if (key == WXK_PAGEUP || key == (wxKeyCode)key_lookup)
			look_up = down;

		if (key == WXK_PAGEDOWN || key == (wxKeyCode)key_lookdown)
			look_down = down;

		if (key == WXK_UP || key == (wxKeyCode)key_up)
			step_forward = down;

		if (key == WXK_DOWN || key == (wxKeyCode)key_down)
			step_backward = down;

		//binoculars
		if (key == (wxKeyCode)key_binoculars)
		{
			camera->Binoculars(down);
		}

		//crouch
		if (key == (wxKeyCode)key_crouch)
		{
			camera->Crouch(down);
		}

		//values from old version
		if (key == WXK_HOME || key == (wxKeyCode)key_floatup)
			float_up = down;
		if (key == WXK_END || key == (wxKeyCode)key_floatdown)
			float_down = down;

		//drive functions, when user is inside a vehicle
		if (camera->Freelook == true && camera->inside_vehicle == true)
		{
			if (key == WXK_LEFT || key == (wxKeyCode)key_left)
				engine->GetSystem()->camera->Drive(true, false, false, false, down);
			if (key == WXK_RIGHT || key == (wxKeyCode)key_right)
				engine->GetSystem()->camera->Drive(false, true, false, false, down);
			if (key == WXK_DOWN || key == (wxKeyCode)key_down)
				engine->GetSystem()->camera->Drive(false, false, true, false, down);
			if (key == WXK_UP || key == (wxKeyCode)key_up)
				engine->GetSystem()->camera->Drive(false, false, false, true, down);
		}
	}
}

void MainScreen::ProcessMovement(EngineContext *engine, bool control, bool shift, bool angle_only)
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

void MainScreen::OnMouseButton(wxMouseEvent& event)
{
	//this function is run when a mouse button is pressed

	EngineContext *engine = frontend->GetServer()->GetActiveEngine();

	if (!engine)
		return;

	//get SBS camera
	Camera *camera = engine->GetSystem()->camera;

	if (!camera)
		return;

	//enter or exit freelook mode using mouse scroll wheel
	if (event.GetWheelRotation() > 0)
	{
		EnableFreelook(true);
		return;
	}
	else if (event.GetWheelRotation() < 0)
	{
		EnableFreelook(false);
		return;
	}

	//check if the user clicked on an object, and process it
	bool left = event.LeftDown();
	bool left_dclick = event.LeftDClick();
	bool right = event.RightDown();
	bool right_dclick = event.RightDClick();

	if (left == false && right == false && left_dclick == false && right_dclick == false)
	{
		camera->UnclickedObject();
		camera->MouseLeftDown = false;
		camera->MouseRightDown = false;
	}
	else
	{
		if (left == true)
			camera->MouseLeftDown = true;
		else
			camera->MouseRightDown = true;

		//apply content scaling factor, fixes issues for example on Retina displays
		Real scale = frontend->window->GetContentScaleFactor();

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		//set scale to 1.0 on MacOS versions earlier than 10.15
		if (frontend->macos_major == 10 && frontend->macos_minor < 15)
			scale = 1.0;
#endif

		camera->ClickedObject(wxGetKeyState(WXK_SHIFT), wxGetKeyState(WXK_CONTROL), wxGetKeyState(WXK_ALT), (right || right_dclick), scale);
	}
}

void MainScreen::HandleMouseMovement()
{
	EngineContext *engine = frontend->GetServer()->GetActiveEngine();

	if (!engine || IsActive() == false)
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

	//get old mouse coordinates
	int old_mouse_x = camera->mouse_x;
	int old_mouse_y = camera->mouse_y;

	//get mouse pointer coordinates
	camera->mouse_x = ScreenToClient(wxGetMousePosition()).x;
	camera->mouse_y = ScreenToClient(wxGetMousePosition()).y;

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
		Real width = GetClientSize().GetWidth();
		Real height = GetClientSize().GetHeight();

		//if mouse coordinates changed, and we're in freelook mode, rotate camera
		if (old_mouse_x != camera->mouse_x || old_mouse_y != camera->mouse_y)
		{
			WarpPointer(width * 0.5, height * 0.5);

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
			ProcessMovement(engine, false, false, true);
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
			ProcessMovement(engine, false, false, true);
		}
		freelook = false;
	}
}

void MainScreen::EnableFreelook(bool value)
{
	//enable or disable freelook mode

	EngineContext *engine = frontend->GetServer()->GetActiveEngine();

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
}

}
