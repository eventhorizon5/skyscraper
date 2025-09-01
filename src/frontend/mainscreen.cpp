/*
	Skyscraper 2.1 - Main Screen
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

#ifdef USING_WX
#include "wx/wx.h"
#include "wx/joystick.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRTShaderSystem.h>
#include <OgreImGuiOverlay.h>
#include "globals.h"
#include "sbs.h"
#include "camera.h"
#include "debugpanel.h"
#include "skyscraper.h"
#include "vm.h"
#include "hal.h"
#include "sky.h"
#include "gui.h"
#include "editor.h"
#include "enginecontext.h"
#include "loaddialog.h"
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
#ifndef __FreeBSD__
  EVT_JOYSTICK_EVENTS(MainScreen::OnJoystickEvent)
#endif
END_EVENT_TABLE()

static ImGuiKey ToImGuiKey(int k)
{
	switch (k)
	{
		case 'A': return ImGuiKey_A; case 'B': return ImGuiKey_B; case 'C': return ImGuiKey_C; case 'D': return ImGuiKey_D;
		case 'E': return ImGuiKey_E; case 'F': return ImGuiKey_F; case 'G': return ImGuiKey_G; case 'H': return ImGuiKey_H;
		case 'I': return ImGuiKey_I; case 'J': return ImGuiKey_J; case 'K': return ImGuiKey_K; case 'L': return ImGuiKey_L;
		case 'M': return ImGuiKey_M; case 'N': return ImGuiKey_N; case 'O': return ImGuiKey_O; case 'P': return ImGuiKey_P;
		case 'Q': return ImGuiKey_Q; case 'R': return ImGuiKey_R; case 'S': return ImGuiKey_S; case 'T': return ImGuiKey_T;
		case 'U': return ImGuiKey_U; case 'V': return ImGuiKey_V; case 'W': return ImGuiKey_W; case 'X': return ImGuiKey_X;
		case 'Y': return ImGuiKey_Y; case 'Z': return ImGuiKey_Z;
		case '0': return ImGuiKey_0; case '1': return ImGuiKey_1; case '2': return ImGuiKey_2; case '3': return ImGuiKey_3;
		case '4': return ImGuiKey_4; case '5': return ImGuiKey_5; case '6': return ImGuiKey_6; case '7': return ImGuiKey_7;
		case '8': return ImGuiKey_8; case '9': return ImGuiKey_9;

		case WXK_F1: return ImGuiKey_F1; case WXK_F2: return ImGuiKey_F2; case WXK_F3: return ImGuiKey_F3; case WXK_F4: return ImGuiKey_F4;
		case WXK_F5: return ImGuiKey_F5; case WXK_F6: return ImGuiKey_F6; case WXK_F7: return ImGuiKey_F7; case WXK_F8: return ImGuiKey_F8;
		case WXK_F9: return ImGuiKey_F9; case WXK_F10: return ImGuiKey_F10; case WXK_F11: return ImGuiKey_F11; case WXK_F12: return ImGuiKey_F12;

		case WXK_LEFT: return ImGuiKey_LeftArrow; case WXK_RIGHT: return ImGuiKey_RightArrow;
		case WXK_UP: return ImGuiKey_UpArrow; case WXK_DOWN: return ImGuiKey_DownArrow;
		case WXK_HOME: return ImGuiKey_Home; case WXK_END: return ImGuiKey_End;
		case WXK_PAGEUP: return ImGuiKey_PageUp; case WXK_PAGEDOWN: return ImGuiKey_PageDown;
		case WXK_INSERT: return ImGuiKey_Insert; case WXK_DELETE: return ImGuiKey_Delete;
		case WXK_BACK: return ImGuiKey_Backspace; case WXK_TAB: return ImGuiKey_Tab;
		case WXK_RETURN: return ImGuiKey_Enter; case WXK_ESCAPE: return ImGuiKey_Escape; case WXK_SPACE: return ImGuiKey_Space;

		case WXK_CONTROL: return ImGuiKey_LeftCtrl;
		case WXK_SHIFT:   return ImGuiKey_LeftShift;
		case WXK_ALT:     return ImGuiKey_LeftAlt;
	#if defined(__APPLE__)
		//case WXK_COMMAND: return ImGuiKey_LeftSuper;
	#else
		case WXK_WINDOWS_MENU: return ImGuiKey_LeftSuper;
	#endif
		default: return ImGuiKey_None;
	}
}

static void UpdateImguiModsFromWx(const wxKeyEvent& e)
{
	ImGuiIO& io = ImGui::GetIO();
	io.AddKeyEvent(ImGuiMod_Ctrl, e.ControlDown());
	io.AddKeyEvent(ImGuiMod_Shift, e.ShiftDown());
	io.AddKeyEvent(ImGuiMod_Alt, e.AltDown());
	io.AddKeyEvent(ImGuiMod_Super, e.MetaDown()); // macOS Command = Meta
}

MainScreen::MainScreen(Skyscraper *parent, int width, int height) : wxFrame(0, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	frontend = parent;
	Active = false;
	InLoop = false;
	panel = 0;
	Center();
	wxString title;
	title = wxT("Skyscraper " + frontend->GetVM()->version + " " + frontend->GetVM()->version_state);
	SetTitle(title);
	SetClientSize(width, height);
	SetExtraStyle(wxWS_EX_PROCESS_IDLE);

#ifndef __FreeBSD__
	joystick = new wxJoystick(wxJOYSTICK1);
	if (joystick->IsOk())
	{
		joy_buttons = joystick->GetNumberButtons();
		joystick->SetCapture(this, 10);
	}
	else
		joy_buttons = -1;
#endif

	HAL *hal = frontend->GetVM()->GetHAL();

	key_right = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Right", "D")[0];
	key_left = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Left", "A")[0];
	key_up = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Up", "W")[0];
	key_down = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Down", "S")[0];
	key_strafeleft = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.StrafeLeft", "Q")[0];
	key_straferight = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.StrafeRight", "E")[0];
	key_lookup = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.LookUp", "P")[0];
	key_lookdown = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.LookDown", "L")[0];
	key_binoculars = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Binoculars", "B")[0];
	key_crouch = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Crouch", "X")[0];
	key_floatup = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.FloatUp", "O")[0];
	key_floatdown = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.FloatDown", "K")[0];
	key_noclip = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.NoClip", "V")[0];
	key_pickup = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.PickUp", "C")[0];
	key_load = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Load", ";")[0];
	key_enter = hal->GetConfigString(hal->keyconfigfile, "Skyscraper.Frontend.Keyboard.Enter", "E")[0];

#ifndef __FreeBSD__
	joy_click = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Click", 0);
	joy_fast = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Fast", 1);
	joy_strafe = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Strafe", 2);
	joy_jump = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Jump", 3);
	joy_turn = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Turn", 0);
	joy_forward = hal->GetConfigInt(hal->joyconfigfile, "Skyscraper.Frontend.Joystick.Forward", 1);
#endif

	//create panel, rendering is done on this, along with keyboard and mouse events
	panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(width, height), wxNO_BORDER);
	panel->Connect(wxID_ANY, wxEVT_KEY_DOWN, wxKeyEventHandler(MainScreen::OnKeyDown), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_KEY_UP, wxKeyEventHandler(MainScreen::OnKeyUp), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_MOTION, wxMouseEventHandler(MainScreen::OnMouseMove), NULL, this);
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
	panel->Connect(wxID_ANY, wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MainScreen::OnMouseLeave), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_CHAR, wxKeyEventHandler(MainScreen::OnChar), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_SET_FOCUS, wxFocusEventHandler(MainScreen::OnFocus), NULL, this);
	panel->Connect(wxID_ANY, wxEVT_KILL_FOCUS, wxFocusEventHandler(MainScreen::OnKillFocus), NULL, this);
}

MainScreen::~MainScreen()
{
#ifndef __FreeBSD__
	joystick->ReleaseCapture();
	delete joystick;
#endif
}

void MainScreen::OnIconize(wxIconizeEvent& event)
{
	//pause simulator while minimized

	VM *vm = frontend->GetVM();
	if (!vm)
		return;
	vm->Pause = event.IsIconized();

	if (vm->Pause == true)
		vm->GetHAL()->Report("Pausing simulator...", "");
	else
		vm->GetHAL()->Report("Resuming simulator...", "");
}

void MainScreen::OnSize(wxSizeEvent& WXUNUSED(event))
{
	VM *vm = frontend->GetVM();
	if (!vm)
		return;
	HAL *hal = vm->GetHAL();

	if (panel)
		panel->SetSize(this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight());

	if (hal->GetRenderWindow())
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		Real scale = this->GetContentScaleFactor();
		hal->GetRenderWindow()->resize(this->GetClientSize().GetWidth() * scale, this->GetClientSize().GetHeight() * scale);
#else
		hal->GetRenderWindow()->windowMovedOrResized();
#endif
	}
	for (size_t i = 0; i < hal->mCameras.size(); i++)
	{
		hal->mCameras[i]->setAspectRatio(Real(hal->mViewports[i]->getActualWidth()) / Real(hal->mViewports[i]->getActualHeight()));
	}
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
	if (!frontend->GetVM())
		return;

	//prevent simultaneous executions - this fixes an issue with wxYield
	if (InLoop == false)
	{
		InLoop = true;

		if (frontend->GetVM()->Pause == false)
		{
			if (this->HasFocus() && !panel->HasFocus())
				panel->SetFocus();

			try {
				Editor *editor = frontend->GetVM()->GetEditor();
				EngineContext *engine = frontend->GetVM()->GetActiveEngine();
				if (editor && engine)
					editor->UpdateFrame(GetClientSize().x, GetClientSize().y, GetContentScaleFactor(), engine->GetSystem()->delta); //update editor frame

				bool result = frontend->Loop(); //run simulator loop
				if (!result)
					frontend->Quit();
			}
			catch (Ogre::Exception &e)
			{
				frontend->GetVM()->GetHAL()->ReportFatalError("Unhandled OGRE exception:\n\n" + e.getFullDescription() + "\n\nSkyscraper will now exit.  Please report this as a bug.", "");
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

	//if (frontend->GetVM()->GetRenderWindow())
		//frontend->GetVM()->GetRenderWindow()->update(true);
}

void MainScreen::OnActivate(wxActivateEvent &event)
{
	Active = event.GetActive();
	event.Skip();
}

void MainScreen::OnKeyDown(wxKeyEvent& event)
{
	//this function is run when a key is pressed

	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		UpdateImguiModsFromWx(event);
		ImGuiKey k = ToImGuiKey(event.GetKeyCode());
		if (k != ImGuiKey_None)
		{
			if (ImGui::GetIO().WantCaptureKeyboard)
				event.Skip(false);
			ImGui::GetIO().AddKeyEvent(k, true);
		}
		else
			event.Skip();
	}

	EngineContext *engine = frontend->GetVM()->GetActiveEngine();

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
			frontend->GetVM()->Shutdown = true;
		return;
	}

	if (key == WXK_F1)
	{
		frontend->GetGUI()->ShowControlReference();
	}

	if (key == WXK_F2)
	{
		Real fps = Simcore->FPS;
		HAL *hal = frontend->GetVM()->GetHAL();
		Ogre::RenderSystem *rendersystem = hal->mRoot->getRenderSystem();

		int batches = (int)rendersystem->_getBatchCount();
		int triangles = (int)rendersystem->_getFaceCount();
		int vertices = (int)rendersystem->_getVertexCount();

		hal->Report("FPS: " + ToString(Simcore->FPS) + " - Batches: " + ToString(batches) + " - Triangles: " + ToString(triangles) + " - Vertices: " + ToString(vertices), "");
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
			//jump (spacebar) action
			if (camera->IsOnGround() == true)
				camera->Jump();
		}

		if (key == (wxKeyCode)key_noclip)
		{
			bool status = camera->GetGravityStatus();

			camera->EnableGravity(!status);
			camera->EnableCollisions(!status);

			if (status == false)
				frontend->GetVM()->GetHAL()->Report("Gravity and collision detection on", "");
			else
				frontend->GetVM()->GetHAL()->Report("Gravity and collision detection off", "");
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
			Simcore->ShowColliders(!frontend->colliders);
			frontend->colliders = !frontend->colliders;
		}

		if (key == WXK_F4)
		{
			//toggle wireframe mode
			if (frontend->wireframe == 0)
			{
				frontend->GetVM()->GetSkySystem()->EnableSky(false);
				camera->SetViewMode(1);
				frontend->wireframe = 1;
			}
			else if (frontend->wireframe == 1)
			{
				camera->SetViewMode(2);
				frontend->wireframe = 2;
			}
			else if (frontend->wireframe == 2)
			{
				frontend->GetVM()->GetSkySystem()->EnableSky(true);
				camera->SetViewMode(0);
				frontend->wireframe = 0;
			}
		}

		if (key == WXK_F11)
		{
			std::string prefix = frontend->GetDataPath();
			frontend->GetVM()->GetHAL()->GetRenderWindow()->writeContentsToTimestampedFile(prefix + "screenshots/skyscraper-", ".jpg");
		}

		if (key == WXK_F12)
		{
			frontend->GetGUI()->ShowDebugPanel();
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
			Simcore->ShowBoundingBoxes(!frontend->boxes);
			frontend->boxes = !frontend->boxes;
		}

		if (key == WXK_F9)
		{
			//toggle statistics bar
			frontend->GetVM()->GetHAL()->ToggleStats();
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

		//editor window
		if (key == (wxKeyCode)'=')
		{
			Editor *editor = frontend->GetVM()->GetEditor();
			editor->Enable(!editor->IsEnabled());
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
			frontend->GetGUI()->ShowLoadDialog();
			return;
		}

		//engine selection
		if (key == (wxKeyCode)'1')
		{
			frontend->GetVM()->SetActiveEngine(0);
		}
		else if (key == (wxKeyCode)'2')
		{
			frontend->GetVM()->SetActiveEngine(1);
		}
		else if (key == (wxKeyCode)'3')
		{
			frontend->GetVM()->SetActiveEngine(2);
		}
		else if (key == (wxKeyCode)'4')
		{
			frontend->GetVM()->SetActiveEngine(3);
		}
		else if (key == (wxKeyCode)'5')
		{
			frontend->GetVM()->SetActiveEngine(4);
		}
		else if (key == (wxKeyCode)'6')
		{
			frontend->GetVM()->SetActiveEngine(5);
		}
		else if (key == (wxKeyCode)'7')
		{
			frontend->GetVM()->SetActiveEngine(6);
		}
		else if (key == (wxKeyCode)'8')
		{
			frontend->GetVM()->SetActiveEngine(7);
		}
		else if (key == (wxKeyCode)'9')
		{
			frontend->GetVM()->SetActiveEngine(8);
		}
		else if (key == (wxKeyCode)'0')
		{
			frontend->GetVM()->SetActiveEngine(9);
		}

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

	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		UpdateImguiModsFromWx(event);
		ImGuiKey k = ToImGuiKey(event.GetKeyCode());
		if (k != ImGuiKey_None)
		{
			if (ImGui::GetIO().WantCaptureKeyboard)
				event.Skip(false);
			ImGui::GetIO().AddKeyEvent(k, false);
		}
			else
				event.Skip();
	}

	EngineContext *engine = frontend->GetVM()->GetActiveEngine();

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
			frontend->strafe_right = down;

		else if (key == WXK_LEFT || key == (wxKeyCode)key_left)
			frontend->strafe_left = down;

		else if (key == WXK_UP || key == (wxKeyCode)key_up)
			frontend->float_up = down;

		else if (key == WXK_DOWN || key == (wxKeyCode)key_down)
			frontend->float_down = down;

		else if (key == WXK_PAGEUP || key == (wxKeyCode)key_lookup)
			frontend->spin_up = down;

		else if (key == WXK_PAGEDOWN || key == (wxKeyCode)key_lookdown)
			frontend->spin_down = down;
	}
	else
	{
		if (camera->Freelook == false)
		{
			if (key == WXK_RIGHT || key == (wxKeyCode)key_right)
				frontend->turn_right = down;

			if (key == WXK_LEFT || key == (wxKeyCode)key_left)
				frontend->turn_left = down;

			if (key == (wxKeyCode)key_straferight)
				frontend->strafe_right = down;

			if (key == (wxKeyCode)key_strafeleft)
				frontend->strafe_left = down;
		}
		else
		{
			if (key == WXK_RIGHT || key == (wxKeyCode)key_right || key == (wxKeyCode)key_straferight)
				frontend->strafe_right = down;

			if (key == WXK_LEFT || key == (wxKeyCode)key_left || key == (wxKeyCode)key_strafeleft)
				frontend->strafe_left = down;
		}

		if (key == WXK_PAGEUP || key == (wxKeyCode)key_lookup)
			frontend->look_up = down;

		if (key == WXK_PAGEDOWN || key == (wxKeyCode)key_lookdown)
			frontend->look_down = down;

		if (key == WXK_UP || key == (wxKeyCode)key_up)
			frontend->step_forward = down;

		if (key == WXK_DOWN || key == (wxKeyCode)key_down)
			frontend->step_backward = down;

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
			frontend->float_up = down;
		if (key == WXK_END || key == (wxKeyCode)key_floatdown)
			frontend->float_down = down;

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

		if (frontend->step_forward == true)
			step += speed_normal;
		if (frontend->step_backward == true)
			step -= speed_normal;

		if (frontend->strafe_left == true)
			strafe -= speed_normal;
		if (frontend->strafe_right == true)
			strafe += speed_normal;

		if (frontend->float_up == true)
			floatval += speed_normal;
		if (frontend->float_down == true)
			floatval -= speed_normal;

		//set camera motion values
		camera->Step(step);
		camera->Strafe(strafe);
		camera->Float(floatval);
	}

	if (frontend->spin_up == true)
		spin += speed_normal;
	if (frontend->spin_down == true)
		spin -= speed_normal;

	if (frontend->turn_left == true)
		turn -= speed_normal;
	if (frontend->turn_right == true)
		turn += speed_normal;

	if (frontend->look_up == true)
		look += speed_normal;
	if (frontend->look_down == true)
		look -= speed_normal;

	//set camera rotation values
	camera->Spin(spin);
	camera->Turn(turn);
	camera->Look(look);
}

void MainScreen::OnMouseButton(wxMouseEvent& event)
{
	//this function is run when a mouse button is pressed

	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
			event.Skip(false);

		//use separate handlers if you prefer; this works for down/up/dclick paths:
		int btn = 0;
		bool down = event.ButtonDown(wxMOUSE_BTN_LEFT) || event.ButtonDClick(wxMOUSE_BTN_LEFT);
		bool up   = event.ButtonUp(wxMOUSE_BTN_LEFT);
		if (!(down || up))
		{
			if (event.ButtonDown(wxMOUSE_BTN_RIGHT) || event.ButtonDClick(wxMOUSE_BTN_RIGHT))
			{
				btn = 1;
				down = true;
			}
			else if (event.ButtonUp(wxMOUSE_BTN_RIGHT))
			{
				btn = 1;
				up = true;
			}
			else if (event.ButtonDown(wxMOUSE_BTN_MIDDLE) || event.ButtonDClick(wxMOUSE_BTN_MIDDLE))
			{
				btn = 2;
				down = true;
			}
			else if (event.ButtonUp(wxMOUSE_BTN_MIDDLE))
			{
				btn = 2;
				up = true;
			}
		}
		if (down)
			io.AddMouseButtonEvent(btn, true);
		if (up)
			io.AddMouseButtonEvent(btn, false);
	}

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

	HAL *hal = frontend->GetVM()->GetHAL();

	if (left == false && right == false && left_dclick == false && right_dclick == false)
	{
		hal->UnclickedObject();
	}
	else
	{
		//apply content scaling factor, fixes issues for example on Retina displays
		Real scale = frontend->window->GetContentScaleFactor();

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		//set scale to 1.0 on MacOS versions earlier than 10.15
		if (frontend->GetVM()->macos_major == 10 && frontend->GetVM()->macos_minor < 15)
			scale = 1.0;
#endif

		hal->ClickedObject(left, wxGetKeyState(WXK_SHIFT), wxGetKeyState(WXK_CONTROL), wxGetKeyState(WXK_ALT), (right || right_dclick), scale, false);
	}
}

void MainScreen::HandleMouseMovement()
{
	EngineContext *engine = frontend->GetVM()->GetActiveEngine();

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
		if (frontend->freelook == false)
		{
			//reset values to prevent movement from getting stuck
			frontend->turn_right = 0;
			frontend->turn_left = 0;

			EnableFreelook(true);
		}

		frontend->freelook = true;

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
		if (frontend->freelook == true)
		{
			//reset values to prevent movement from getting stuck
			frontend->strafe_right = 0;
			frontend->strafe_left = 0;
			if (old_mouse_x != camera->mouse_x || old_mouse_y != camera->mouse_y)
				camera->FreelookMove(Vector3::ZERO);
			ProcessMovement(engine, false, false, true);
		}
		frontend->freelook = false;
	}
}

void MainScreen::EnableFreelook(bool value)
{
	//enable or disable freelook mode

	EngineContext *engine = frontend->GetVM()->GetActiveEngine();

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

#ifndef __FreeBSD__
void MainScreen::OnJoystickEvent(wxJoystickEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (event.IsZMove())
		return;

	EngineContext *engine = frontend->GetVM()->GetActiveEngine();

	if (!engine)
		return;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	Camera *camera = Simcore->camera;

	if (!camera)
		return;

	Real speed_normal = camera->cfg_speed;
	Real speed_fast = camera->cfg_speedfast;
	Real speed_slow = camera->cfg_speedslow;

	Real step = 0, turn = 0, strafe = 0;

	Real speed = speed_normal;

	int MinX = joystick->GetXMin();
	int MaxX = joystick->GetXMax();
	int MinY = joystick->GetYMin();
	int MaxY = joystick->GetYMax();

	int CenterX = (MaxX + MinX) / 2;
	int CenterY = (MaxY + MinY) / 2;
	int range = (MaxX - MinX) / 8;

	if (joystick->GetButtonState(joy_fast))
		speed = speed_fast;

	if (joystick->GetPosition(joy_forward) < (CenterY - range))
		step += speed;
	if (joystick->GetPosition(joy_forward) > (CenterY + range))
		step -= speed;

	if (joystick->GetButtonState(joy_strafe))
	{
		if (joystick->GetPosition(joy_turn) > (CenterX + range))
			strafe += speed;
		if (joystick->GetPosition(joy_turn) < (CenterX - range))
			strafe -= speed;
	}
	else
	{
		if (joystick->GetPosition(joy_turn) > (CenterX + range))
			turn += speed;
		if (joystick->GetPosition(joy_turn) < (CenterX - range))
			turn -= speed;
	}

	camera->Step(step);
	camera->Turn(turn);
	camera->Strafe(strafe);

	if (joystick->GetButtonState(joy_click))
		engine->GetVM()->GetHAL()->ClickedObject(true, false, false, false, false, 0.0, true);

	if (joystick->GetButtonState(joy_jump) && camera->IsOnGround() == true)
		camera->Jump();
}
#endif

void MainScreen::OnMouseMove(wxMouseEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		if (ImGui::GetIO().WantCaptureMouse)
			event.Skip(false);
		auto p = event.GetPosition();
		ImGui::GetIO().AddMousePosEvent((float)p.x, (float)p.y);
	}
}

void MainScreen::OnMouseLeave(wxMouseEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		ImGui::GetIO().AddMousePosEvent(-FLT_MAX, -FLT_MAX);
		event.Skip();
	}
}

void MainScreen::OnChar(wxKeyEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		const int uni = event.GetUnicodeKey();
		if (uni > 0 && uni != WXK_NONE && uni != WXK_DELETE)
		{
			if (ImGui::GetIO().WantTextInput)
				event.Skip(false);
			ImGui::GetIO().AddInputCharacter((unsigned int)uni);
			return;
		}
		event.Skip();
	}
}

void MainScreen::OnFocus(wxFocusEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		ImGui::GetIO().AddFocusEvent(true);
		event.Skip();
	}
}

void MainScreen::OnKillFocus(wxFocusEvent &event)
{
	if (!frontend->GetVM())
		return;

	if (frontend->GetVM()->GetEditor()->IsInitialized() == true && frontend->GetVM()->GetHAL()->IsVREnabled() == false)
	{
		ImGui::GetIO().AddFocusEvent(false);
		event.Skip();

	}
}

}

#endif
