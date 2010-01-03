/* $Id$ */

/*
	Skyscraper 1.6 Alpha - Simulation Frontend
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

#define CS_IMPLEMENT_PLATFORM_APPLICATION
#define CS_NO_MALLOC_OVERRIDE

#include <wx/wx.h>
#include <crystalspace.h>
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "loader.h"

CS_IMPLEMENT_APPLICATION
IMPLEMENT_APP_NO_MAIN(Skyscraper)

BEGIN_EVENT_TABLE(MainScreen, wxFrame)
  EVT_SHOW(MainScreen::OnShow)
  EVT_ICONIZE(MainScreen::OnIconize)
  EVT_SIZE(MainScreen::OnSize)
  EVT_CLOSE(MainScreen::OnClose)
  EVT_IDLE(MainScreen::OnIdle)
END_EVENT_TABLE()

SBS *Simcore;
Skyscraper *skyscraper;
DebugPanel *dpanel;
MainScreen *window;
iObjectRegistry* object_reg;

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
	wxEntry(argc, argv);

	csInitializer::DestroyApplication (object_reg);
	object_reg = 0;

	return 0;
}

bool Skyscraper::OnInit(void)
{
	skyscraper = this;
	MouseDown = false;
	RenderOnly = false;
	InputOnly = false;
	canvas_width = 0;
	canvas_height = 0;
	IsRunning = false;
	StartupRunning = false;
	Starting = false;
	Pause = false;
	DisableSound = false;
	DrewButtons = false;
	FullScreen = false;
	bool Shutdown = false;
	PositionOverride = false;
	Reload = false;

	//Create main window
	window = new MainScreen(640, 480);
	//AllowResize(false);
	window->ShowWindow();

	//start and initialize CS
	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	char** csargv;
	csargv = (char**)cs_malloc(sizeof(char*) * argc);
	for (int i = 0; i < argc; i++)
		csargv[i] = strdup(wxString(argv[i]).mb_str().data());
	if (!Initialize(argc, csargv, window->panel))
	#else
	if (!Initialize(argc, argv, window->panel))
	#endif
		return ReportError("Error initializing Crystal Space");

	//autoload a building file if specified
	BuildingFile = confman->GetStr("Skyscraper.Frontend.AutoLoad");
	if (BuildingFile != "")
		return Start();

	//show menu
	if (confman->GetBool("Skyscraper.Frontend.ShowMenu", true) == true)
	{
		//draw background
		DrawBackground();
		StartupRunning = true;
		StartSound();
	}
	else
	{
		//or show building selection window if ShowMenu is false
		if (SelectBuilding() == true)
			return Start();
		else
			return false;
	}

	return true;
}

int Skyscraper::OnExit()
{
	//clean up

	UnloadSim();

	//delete frame printer object
	printer.Invalidate();

	//cleanup sound
	if (sndsource)
		StopSound();

	//delete wx canvas
	if (canvas)
		canvas->Destroy();
	canvas = 0;

	window->Destroy();
	window = 0;
	skyscraper = 0;

	//cleanup
	csPrintf ("Cleaning up...\n");

	return 0;
}

void Skyscraper::UnloadSim()
{
	//delete control panel object
	if(dpanel)
	{
		if(dpanel->timer)
			dpanel->timer->Stop();
		dpanel->Destroy();
	}
	dpanel = 0;
	//delete simulator object
	delete Simcore;
	Simcore = 0;
}

MainScreen::MainScreen(int width, int height) : wxFrame(0, -1, wxT("Skyscraper 1.6 Alpha"), wxDefaultPosition, wxSize(width, height), wxDEFAULT_FRAME_STYLE)
{
	this->Center();
	panel = new wxPanel(this, -1, wxPoint(0, 0), this->GetClientSize());
	//this->SetTitle(wxString::FromAscii(windowtitle));
}

MainScreen::~MainScreen()
{

}

void MainScreen::OnIconize(wxIconizeEvent& event)
{
	//pause simulator while minimized
	if (skyscraper->IsRunning == false)
		return;

	skyscraper->Pause = event.Iconized();

	if (skyscraper->Pause == true)
		skyscraper->Report("Pausing simulator...");
	else
		skyscraper->Report("Resuming simulator...");
}

void MainScreen::OnShow(wxShowEvent& event)
{
	//csPrintf("got show %d\n", (int)event.GetShow());
}

void MainScreen::OnSize(wxSizeEvent& WXUNUSED(event))
{
	panel->SetSize(this->GetClientSize());
}

void MainScreen::OnClose(wxCloseEvent& event)
{
	if (skyscraper->StartupRunning == false)
	{
		int result = wxMessageBox(wxT("Are you sure you want to exit?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		if (result == wxNO)
			return;
	}

	if(dpanel)
	{
		if(dpanel->timer)
			dpanel->timer->Stop();
	}
	wxGetApp().Exit();
}

void MainScreen::ShowWindow()
{
	Show(true);
	panel->Show(true);
}

void MainScreen::OnIdle(wxIdleEvent& event)
{
	if ((skyscraper->IsRunning == true && skyscraper->Pause == false) || skyscraper->StartupRunning == true)
		skyscraper->PushFrame(); //run simulator loop
	if (skyscraper->Pause == false)
		event.RequestMore(); //request more idles
}

void Skyscraper::Render()
{
	if (skyscraper->StartupRunning == false)
	{
		// Tell 3D driver we're going to display 3D things.
		if (Simcore->IsSkyboxEnabled == false)
		{
			//clear screen if the skybox is off. This will keep the background black - otherwise it'll streak.
			if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN))
				return;
		}
		else
		{
			//if the skybox is on, don't clear the screen, for improved performance
			if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER))
				return;
		}
	}

	// Tell the camera to render into the frame buffer.
	view->Draw();
}

void Skyscraper::SetupFrame()
{
	//Main simulator loop
	if (IsRunning == false)
	{
		DrawBackground();
		GetMenuInput();
		return;
	}

	//resize canvas if needed
	if (canvas->GetSize().GetWidth() != canvas_width || canvas->GetSize().GetHeight() != canvas_height)
	{
		//update canvas size values
		canvas_width = canvas->GetSize().GetWidth();
		canvas_height = canvas->GetSize().GetHeight();

		//resize viewport
		wxwin->GetWindow()->SetSize(canvas->GetSize());
	}

	RenderOnly = confman->GetBool("Skyscraper.Frontend.RenderOnly", false);
	InputOnly = confman->GetBool("Skyscraper.Frontend.InputOnly", false);

	Simcore->RenderOnly = RenderOnly;
	Simcore->InputOnly = InputOnly;

	//run SBS main loop
	Simcore->MainLoop();

	//get input
	if (RenderOnly == false)
		GetInput();

	//process camera loop
	Simcore->camera->Loop();

	Render();

	//exit if shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		//if showmenu is true, unload simulator and return to main menu
		if (confman->GetBool("Skyscraper.Frontend.ShowMenu", true) == true)
			Unload();
		//otherwise exit app
		else
			Quit();
	}

	//reload building if requested
	if (Reload == true)
	{
		PositionOverride = true;
		override_position = Simcore->camera->GetPosition();
		override_rotation = Simcore->camera->GetRotation();
		IsRunning = false;
		Starting = false;
		Pause = false;
		UnloadSim();
		mouse->Reset();
		Start();
	}
}

bool Skyscraper::HandleEvent(iEvent& Event)
{
	//Event handler
	if (Event.Name == Frame)
	{
		if (IsRunning == true)
			Simcore->CalculateFrameRate();
		SetupFrame();
		return true;
	}
	return false;
}

static bool SkyscraperEventHandler(iEvent& Event)
{
	if (skyscraper)
		return skyscraper->HandleEvent(Event);
	else
		return false;
}

bool Skyscraper::Initialize(int argc, const char* const argv[], wxPanel* RenderObject)
{
	object_reg = csInitializer::CreateEnvironment(argc, argv);
	if (!object_reg) return false;

	//first initialize VFS
	vfs = csInitializer::SetupVFS(object_reg);
	if (!vfs)
		return ReportError("Error loading the VFS plugin");
	
	//mount app's directory in VFS
        #ifndef CS_PLATFORM_WIN32
                dir_char = "/";
        #else
                dir_char = "\\";
        #endif
        root_dir = csInstallationPathsHelper::GetAppDir(argv[0]) + dir_char;
        vfs->Mount("/root/", root_dir);

	//load options from config file
	//if (!csInitializer::SetupConfigManager(object_reg, "/root/data/config/skyscraper.cfg", "Skyscraper.Frontend"))
    if (!csInitializer::SetupConfigManager(object_reg, "/root/skyscraper.ini", "Skyscraper.Frontend"))
		return ReportError("Error reading config file skyscraper.ini");

	if (!csInitializer::RequestPlugins(object_reg,
		CS_REQUEST_FONTSERVER,
		//CS_REQUEST_PLUGIN("crystalspace.font.server.freetype2", iFontServer),
		CS_REQUEST_PLUGIN("crystalspace.graphics2d.wxgl", iGraphics2D),
		CS_REQUEST_OPENGL3D,
		CS_REQUEST_ENGINE,
		CS_REQUEST_IMAGELOADER,
		CS_REQUEST_LEVELLOADER,
		CS_REQUEST_CONSOLEOUT,
		CS_REQUEST_REPORTER,
		CS_REQUEST_REPORTERLISTENER,
		CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode", iCollideSystem),
		CS_REQUEST_PLUGIN("crystalspace.sndsys.element.loader", iSndSysLoader),
		CS_REQUEST_PLUGIN("crystalspace.sndsys.renderer.software", iSndSysRenderer),
		CS_REQUEST_END))
		return ReportError("Couldn't init app!");

	FocusGained = csevFocusGained(object_reg);
	FocusLost = csevFocusLost(object_reg);
	KeyboardDown = csevKeyboardDown(object_reg);

	if (!csInitializer::SetupEventHandler(object_reg, SkyscraperEventHandler))
		return ReportError("Couldn't initialize event handler!");

	CS_INITIALIZE_EVENT_SHORTCUTS(object_reg);

	// Check for commandline help.
	if (csCommandLineHelper::CheckHelp(object_reg))
	{
		csCommandLineHelper::Help(object_reg);
		return false;
	}

	csRef<iPluginManager> plugin_mgr = csQueryRegistry<iPluginManager> (object_reg);

	vc = csQueryRegistry<iVirtualClock> (object_reg);
	if (!vc) return ReportError("Failed to locate Virtual Clock");
	kbd = csQueryRegistry<iKeyboardDriver> (object_reg);
	if (!kbd) return ReportError ("Failed to locate Keyboard Driver");
	engine = csQueryRegistry<iEngine> (object_reg);
	if (!engine) return ReportError ("Failed to locate 3D engine");
	loader = csQueryRegistry<iLoader> (object_reg);
	if (!loader) return ReportError ("Failed to locate Loader");
	g3d = csQueryRegistry<iGraphics3D> (object_reg);
	if (!g3d) return ReportError ("Failed to locate 3D renderer");
	imageio = csQueryRegistry<iImageIO> (object_reg);
	if (!imageio) return ReportError ("Failed to locate image loader");
	console = csQueryRegistry<iConsoleOutput> (object_reg);
	if (!console) return ReportError ("Failed to locate console output driver");
	confman = csQueryRegistry<iConfigManager> (object_reg);
	if (!confman) return ReportError ("Failed to locate configuration manager");
	mouse = csQueryRegistry<iMouseDriver> (object_reg);
	if (!mouse) return ReportError("Failed to locate mouse driver");
	collision_sys = csQueryRegistry<iCollideSystem> (object_reg);
	if (!collision_sys) return ReportError("Failed to locate collision detection driver");
	sndrenderer = csQueryRegistry<iSndSysRenderer> (object_reg);
	if (!sndrenderer)
	{
		ReportError("Failed to locate sound renderer");
		DisableSound = true;
	}
	sndloader = csQueryRegistry<iSndSysLoader> (object_reg);
	if (!sndloader)
	{
		ReportError("Failed to locate sound loader");
		DisableSound = true;
	}
	sndmanager = csQueryRegistry<iSndSysManager> (object_reg);
	if (!sndmanager)
	{
		ReportError("Failed to locate sound manager");
		DisableSound = true;
	}
	csRef<iBase> plug = csLoadPluginAlways (plugin_mgr, "crystalspace.utilities.bugplug");
	if (!plug) return ReportError ("Failed to locate BugPlug!");
	plug->IncRef ();

	//load bugplug reference
	bugplug = csQueryPluginClass<iBugPlug> (plugin_mgr, "crystalspace.utilities.bugplug");
	if (confman->GetBool("Skyscraper.Frontend.DisplayFPS", false) == false)
		bugplug->ExecCommand("fps"); //turn off FPS display
	
	rep = csQueryRegistry<iReporter> (object_reg);
	if (!rep) return ReportError("Failed to locate reporter driver");
	stdrep = csQueryRegistry<iStandardReporterListener> (object_reg);
	if (!stdrep) return ReportError ("Failed to locate stdrep plugin!");
	stdrep->SetDebugFile ("/tmp/skyscraper_report.txt");
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_BUG, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_ERROR, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_WARNING, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_NOTIFY, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_DEBUG, true, false, false, false, true, false);

	g2d = g3d->GetDriver2D();
	wxwin = scfQueryInterface<iWxWindow> (g2d);
	if(!wxwin) return ReportError("Canvas is no iWxWindow plugin!");
	wxwin->SetParent(RenderObject);
	canvas = RenderObject;
	canvas_width = canvas->GetSize().GetWidth();
	canvas_height = canvas->GetSize().GetHeight();

	font = g2d->GetFontServer()->LoadFont(CSFONT_LARGE);

	// Open the main system. This will open all the previously loaded plug-ins.
	if (!csInitializer::OpenApplication (object_reg))
		return ReportError ("Error opening system!");

	//initialize frame printer
	printer.AttachNew(new FramePrinter(object_reg));

	//initialize event queue
	equeue = csQueryRegistry<iEventQueue> (object_reg);

	//disable the lighting cache
	engine->SetLightingCacheMode(0);
	engine->SetAmbientLight(csColor(0.5, 0.5, 0.5));

	//set up viewport
	view = csPtr<iView>(new csView (engine, g3d));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//workaround for a canvas quirk on Mac
#ifdef CS_PLATFORM_MACOSX
	canvas->SetSize(canvas_width, canvas_height + 1);
	canvas->SetSize(canvas_width, canvas_height);
#endif

	return true;
}

void Skyscraper::GetInput()
{
	//quit if main window isn't selected
	if (window->IsActive() == false)
		return;

	static bool wireframe;
	static bool wait, waitcheck;
	static csTicks old_time;
	static int old_mouse_x, old_mouse_y;

	// First get elapsed time from the virtual clock.
	elapsed_time = vc->GetElapsedTicks ();
	current_time = vc->GetCurrentTicks ();

	//speed limit certain keys
	if (wait == true)
	{
		if (waitcheck == false)
		{
			old_time = current_time;
			waitcheck = true;
		}
		if (current_time > old_time + 200)
		{
			waitcheck = false;
			wait = false;
		}
	}

	// Now rotate the camera according to keyboard state
	//float speed = elapsed_time / 1000.0f;

	//fix for the camera velocities due to the non-event driven key system
	Simcore->camera->desired_velocity.Set(0, 0, 0);
	Simcore->camera->desired_angle_velocity.Set(0, 0, 0);

	//get old mouse coordinates
	old_mouse_x = Simcore->mouse_x;
	old_mouse_y = Simcore->mouse_y;

	//get mouse pointer coordinates
	Simcore->mouse_x = mouse->GetLastX();
	Simcore->mouse_y = mouse->GetLastY();

	//if mouse coordinates changed, and we're in freelook mode, rotate camera
	if (Simcore->camera->Freelook == true && (old_mouse_x != Simcore->mouse_x || old_mouse_y != Simcore->mouse_y))
	{
		canvas->WarpPointer(g2d->GetWidth() / 2, g2d->GetHeight() / 2);
		csVector3 rotational (Simcore->camera->Freelook_speed * (-((float)(Simcore->mouse_y - (g2d->GetHeight() / 2))) / (g2d->GetHeight() * 2)), Simcore->camera->Freelook_speed * (-((g2d->GetWidth() / 2) - (float)Simcore->mouse_x) / (g2d->GetWidth() * 2)), 0);
		Simcore->camera->Rotate(rotational, 1);
	}

	//check if the user clicked on an object, and process it
	if (mouse->GetLastButton(0) == true && MouseDown == false)
	{
		MouseDown = true;
		Simcore->camera->MouseDown = MouseDown;
		Simcore->camera->ClickedObject(wxGetKeyState(WXK_SHIFT), wxGetKeyState(WXK_CONTROL), wxGetKeyState(WXK_ALT));
	}

	//reset mouse state
	if (mouse->GetLastButton(0) == false)
	{
		MouseDown = false;
		Simcore->camera->MouseDown = MouseDown;
	}

	if (wxGetKeyState(WXK_ESCAPE))
	{
		int result = wxMessageBox(wxT("Exit and return to the main menu?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		if (result == wxYES)
			Shutdown = true;
	}

	if (wxGetKeyState(WXK_F2) && wait == false)
	{
		Report(wxVariant(Simcore->FPS).GetString().ToAscii());
		wait = true;
	}

	Simcore->camera->speed = 1;
	float speed_normal = Simcore->camera->cfg_speed;
	float speed_fast = Simcore->camera->cfg_speedfast;
	float speed_slow = Simcore->camera->cfg_speedslow;

	if (wxGetKeyState(WXK_CONTROL))
	{
		if (wxGetKeyState((wxKeyCode)'r'))
		{
			Reload = true;
			return;
		}
		if (wxGetKeyState((wxKeyCode)'d')) //exit if bugplug key is pressed
			return;
		Simcore->camera->speed = speed_slow;
	}
	else if (wxGetKeyState(WXK_SHIFT))
		Simcore->camera->speed = speed_fast;

	if (wxGetKeyState(WXK_ALT))
	{
		//strafe movement
		if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'d'))
			Simcore->camera->Strafe(speed_normal);
		if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'a'))
			Simcore->camera->Strafe(-speed_normal);
		if (wxGetKeyState(WXK_UP) || wxGetKeyState((wxKeyCode)'w'))
			Simcore->camera->Float(speed_normal);
		if (wxGetKeyState(WXK_DOWN) || wxGetKeyState((wxKeyCode)'s'))
			Simcore->camera->Float(-speed_normal);
		if (wxGetKeyState(WXK_PAGEUP))
			Simcore->camera->Spin(speed_normal);
		if (wxGetKeyState(WXK_PAGEDOWN))
			Simcore->camera->Spin(-speed_normal);
	}
	else
	{
		if (Simcore->camera->Freelook == false)
		{
			if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'d'))
				Simcore->camera->Turn(speed_normal);
			if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'a'))
				Simcore->camera->Turn(-speed_normal);
		}
		else
		{
			if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'d'))
				Simcore->camera->Strafe(speed_normal);
			if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'a'))
				Simcore->camera->Strafe(-speed_normal);
		}
		if (wxGetKeyState(WXK_PAGEUP))
			Simcore->camera->Look(speed_normal);
		if (wxGetKeyState(WXK_PAGEDOWN))
			Simcore->camera->Look(-speed_normal);
		if (wxGetKeyState(WXK_UP) || wxGetKeyState((wxKeyCode)'w'))
			Simcore->camera->Step(speed_normal);
		if (wxGetKeyState(WXK_DOWN) || wxGetKeyState((wxKeyCode)'s'))
			Simcore->camera->Step(-speed_normal);
		if (wxGetKeyState(WXK_SPACE))
			Simcore->camera->Jump();
		if (wxGetKeyState(WXK_F3))
		{
			//reset view
			Simcore->camera->SetToStartDirection();
			Simcore->camera->SetToStartRotation();
			Simcore->camera->SetToDefaultFOV();
		}
		if (wxGetKeyState(WXK_F6))
		{
			//reset camera position and state
			Simcore->GetFloor(Simcore->camera->CurrentFloor)->Enabled(false);
			Simcore->GetFloor(Simcore->camera->CurrentFloor)->EnableGroup(false);
			Simcore->camera->EnableGravity(true);
			Simcore->camera->EnableCollisions = true;
			Simcore->camera->SetToStartPosition();
			Simcore->camera->SetToStartRotation();
			Simcore->camera->SetToDefaultFOV();
			Simcore->GetFloor(Simcore->camera->StartFloor)->Enabled(true);
			Simcore->GetFloor(Simcore->camera->StartFloor)->EnableGroup(true);
		}

		if (wxGetKeyState(WXK_F4) && wait == false)
		{
			//enable/disable wireframe mode
			if (wireframe == false)
			{
				bugplug->ExecCommand("edges");
				bugplug->ExecCommand("clear");
				Simcore->EnableSkybox(false);
				wireframe = true;
			}
			else
			{
				bugplug->ExecCommand("edges");
				Simcore->EnableSkybox(true);
				wireframe = false;
			}
			wait = true;
		}
		if (wxGetKeyState(WXK_F11) && wait == false)
		{
			bugplug->ExecCommand("scrshot");
			wait = true;
		}
		if (wxGetKeyState(WXK_F12) && !dpanel)
		{
			//show control panel if closed
			dpanel = new DebugPanel(NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(10, 10));
		}
		if (wxGetKeyState(WXK_F5) && wait == false)
		{
			//enable/disable freelook mode
			Simcore->camera->Freelook = !Simcore->camera->Freelook;
			if (Simcore->camera->Freelook == true)
				canvas->SetCursor(wxCURSOR_BLANK);
			else
				canvas->SetCursor(wxNullCursor);
			wait = true;
		}
		if (wxGetKeyState(WXK_F10) && wait == false)
		{
			//enable/disable fullscreen mode
			FullScreen = !FullScreen;
			window->ShowFullScreen(FullScreen);
			window->ToggleWindowStyle(wxSTAY_ON_TOP);
			window->Refresh();
			wait = true;
		}
		if (wxGetKeyState(WXK_NUMPAD_SUBTRACT))
		{
			//increase FOV angle
			float angle = Simcore->camera->GetFOVAngle() + Simcore->camera->cfg_zoomspeed;
			Simcore->camera->SetFOVAngle(angle);
		}
		if (wxGetKeyState(WXK_NUMPAD_ADD))
		{
			//decrease FOV angle
			float angle = Simcore->camera->GetFOVAngle() - Simcore->camera->cfg_zoomspeed;
			Simcore->camera->SetFOVAngle(angle);
		}

		//values from old version
		if (wxGetKeyState(WXK_HOME))
			Simcore->camera->Float(speed_normal);
		if (wxGetKeyState(WXK_END))
			Simcore->camera->Float(-speed_normal);
	}
}

void Skyscraper::Report (const char* msg, ...)
{
	va_list arg;
	va_start (arg, msg);
	if (rep)
		rep->ReportV (CS_REPORTER_SEVERITY_NOTIFY, "skyscraper", msg, arg);
	else
	{
		csPrintfV (msg, arg);
		csPrintf ("\n");
		fflush (stdout);
	}
	va_end (arg);
}

bool Skyscraper::ReportError (const char* msg, ...)
{
	va_list arg;
	va_start (arg, msg);
	if (rep)
		rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "skyscraper", msg, arg);
	else
	{
		csPrintfV (msg, arg);
		csPrintf ("\n");
		fflush (stdout);
	}
	va_end (arg);
	return false;
}

void Skyscraper::PushFrame()
{
	if (!equeue)
		return ;

	if (vc)
		vc->Advance();

	equeue->Process();
}

void Skyscraper::DrawBackground()
{
	//draw menu background

	int w = g2d->GetWidth();
	int h = g2d->GetHeight();
	if (!g3d->BeginDraw(CSDRAW_2DGRAPHICS))
		return;
	g2d->Clear(0);
	g2d->SetClipRect(0, 0, w, h);

	DrawImage("/root/data/menu.png", 0, 0, 1, false);

	if (DrewButtons == false)
	{
		DrawImage("/root/data/button_triton.png", &button1, 0, -20, true, "/root/data/button_triton_selected.png", "/root/data/button_triton_pressed.png");
		DrawImage("/root/data/button_searstower.png", &button2, 0, 30, true, "/root/data/button_searstower_selected.png", "/root/data/button_searstower_pressed.png");
		DrawImage("/root/data/button_glasstower.png", &button3, 0, 80, true, "/root/data/button_glasstower_selected.png", "/root/data/button_glasstower_pressed.png");
		DrawImage("/root/data/button_simple.png", &button4, 0, 130, true, "/root/data/button_simple_selected.png", "/root/data/button_simple_pressed.png");
		DrawImage("/root/data/button_other.png", &button5, 0, 180, true, "/root/data/button_other_selected.png", "/root/data/button_other_pressed.png");
		DrewButtons = true;
	}
}

void Skyscraper::DrawImage(const char *filename, buttondata *button, int x, int y, bool center, const char *filename_selected, const char *filename_pressed)
{
	int w2, h2;
	int w = g2d->GetWidth();
	int h = g2d->GetHeight();

	image = 0;

	//preload stored image data
	if (button)
	{
		if (button->filename == filename && button->button_image.IsValid())
			image = button->button_image;
		if (button->filename_selected == filename && button->selected_image.IsValid())
			image = button->selected_image;
		if (button->filename_pressed == filename && button->pressed_image.IsValid())
			image = button->pressed_image;
	}
	else if (background_image.IsValid())
		image = background_image;

	//load image data from file, if not already preloaded
	if (!image)
	{
		csRef<iFile> imagefile = vfs->Open(filename, VFS_FILE_READ);
		if (imagefile.IsValid())
		{
			csRef<iDataBuffer> filedata = imagefile->GetAllData();
			image = imageio->Load(filedata, CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);
		}
		imagefile = 0;
	}

	//set values and draw button
	if (image.IsValid())
	{
		w2 = image->GetWidth();
		h2 = image->GetHeight();
		if (button)
		{
			//store general button data
			if (filename_selected)
			{
				button->filename = filename;
				button->filename_selected = filename_selected;
				button->drawn_selected = false;
				button->drawn_pressed = false;
			}
			if (filename_pressed)
				button->filename_pressed = filename_pressed;
			button->offset_x = x;
			button->offset_y = y;
			if (center == true)
			{
				button->x = (w / 2) - (w2 / 2) + x;
				button->y = (h / 2) - (h2 / 2) + y;
			}
			else
			{
				button->x = x;
				button->y = y;
			}
			button->size_x = w2;
			button->size_y = h2;

			//then store image data if not done yet
			if (button->filename == filename && !button->button_image)
				button->button_image = image;
			if (button->filename_selected == filename && !button->selected_image)
				button->selected_image = image;
			if (button->filename_pressed == filename && !button->pressed_image)
				button->pressed_image = image;
		}
		else
			background_image = image;

		if (center == true)
		{
			x += (w / 2) - (w2 / 2);
			y += (h / 2) - (h2 / 2);
		}

		//draw image data
		g2d->Blit(x, y, w2, h2, (unsigned char*)image->GetImageData());
	}
	image = 0;
}

void Skyscraper::GetMenuInput()
{
	//input handler for main menu

	//exit if simulator is starting
	if (Starting == true)
		return;

	//get mouse coordinates
	int mouse_x = mouse->GetLastX();
	int mouse_y = mouse->GetLastY();

	for (int i = 1; i <= 5; i++)
	{
		buttondata *button;
		if (i == 1)
			button = &button1;
		if (i == 2)
			button = &button2;
		if (i == 3)
			button = &button3;
		if (i == 4)
			button = &button4;
		if (i == 5)
			button = &button5;

		//draw buttons
		if (button->drawn_pressed == true)
			DrawImage(button->filename_pressed, button, button->offset_x, button->offset_y, true);
		else if (button->drawn_selected == true)
			DrawImage(button->filename_selected, button, button->offset_x, button->offset_y, true);
		else
			DrawImage(button->filename, button, button->offset_x, button->offset_y, true);

	    //only process buttons if main window is selected
        if (window->IsActive() != false)
        {
        	//change button status based on mouse position and button press status
        	if (mouse_x > button->x && mouse_x < button->x + button->size_x && mouse_y > button->y && mouse_y < button->y + button->size_y)
        	{
        		if (button->drawn_selected == false && mouse->GetLastButton(0) == false)
        		{
        			if (button->drawn_pressed == true)
        			{
        				//user clicked on button
        				Click(i);
        				return;
        			}
        			button->drawn_selected = true;
        		}
        		if (button->drawn_pressed == false && mouse->GetLastButton(0) == true)
        		{
        			button->drawn_pressed = true;
        			button->drawn_selected = false;
        		}
        	}
        	else if (button->drawn_selected == true || button->drawn_pressed == true)
        	{
        		button->drawn_selected = false;
        		button->drawn_pressed = false;
        	}
        }
	}

}

void Skyscraper::Click(int index)
{
	//user clicked a button

	if (index == 5)
	{
		//show file selection dialog
		if (SelectBuilding() == true)
			Start();
		return;
	}

	if (index == 1)
		BuildingFile = "Triton Center.bld";
	if (index == 2)
		BuildingFile = "Sears Tower.bld";
	if (index == 3)
		BuildingFile = "Glass Tower.bld";
	if (index == 4)
		BuildingFile = "Simple.bld";
	if (index > 0 && BuildingFile != "")
		Start();
}

void Skyscraper::StartSound()
{
	//load and start background music

	if (DisableSound == true)
		return;

	if (confman->GetBool("Skyscraper.Frontend.IntroMusic", true) == false)
	{
		DisableSound = true;
		return;
	}

	//load new sound
	csRef<iDataBuffer> sndbuffer = vfs->ReadFile("/root/data/intro.ogg");
	if (!sndbuffer)
	{
		ReportError("Can't load file intro.ogg");
		return;
	}

	csRef<iSndSysData> snddata = sndloader->LoadSound(sndbuffer);
	if (!snddata)
	{
		ReportError("Can't load sound intro.ogg");
		return;
	}

	sndstream = sndrenderer->CreateStream(snddata, CS_SND3D_DISABLE);
	if (!sndstream)
	{
		ReportError("Can't create stream for intro.ogg");
		return;
	}

	sndsource = sndrenderer->CreateSource(sndstream);
	if (!sndsource)
	{
		ReportError("Can't create source for intro.ogg");
		return;
	}

	sndstream->SetLoopState(true);
	sndsource->SetVolume(1.0f);
	sndstream->Unpause();
}

void Skyscraper::StopSound()
{
	//stop and unload sound
	if (sndstream)
		sndstream->Pause();
	sndrenderer->RemoveSource(sndsource);
	sndrenderer->RemoveStream(sndstream);
	sndsource = 0;
	sndstream = 0;
}

bool Skyscraper::SelectBuilding()
{
	//choose a building from a script file
	BuildingFile = "";
	srand (time (0));

	//set building file
	wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings"), _(""), _("Building files (*.bld)|*.bld"), wxOPEN);
	int result = Selector->ShowModal();
	if (result == wxID_CANCEL)
	{
		//delete dialog
		delete Selector;
		Selector = 0;
		//quit
		return false;
	}

	wxSleep(1);

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	BuildingFile = Selector->GetFilename().mb_str().data();
	#else
	BuildingFile = Selector->GetFilename();
	#endif

	//delete dialog
	delete Selector;
	Selector = 0;

	return true;
}

bool Skyscraper::Start()
{
	//start simulator

	//clear screen
	g2d->Clear(0);
	g2d->FinishDraw();
	g2d->Print(0);

	//resize main window
	window->SetBackgroundColour(*wxBLACK);
	window->SetSize(confman->GetInt("Skyscraper.Frontend.ScreenWidth", 640), confman->GetInt("Skyscraper.Frontend.ScreenHeight", 480));
	window->Center();

	//switch to fullscreen mode if specified
	if (confman->GetBool("Skyscraper.Frontend.FullScreen", false) == true)
	{
		FullScreen = true;
		window->ShowFullScreen(FullScreen);
	}

	Starting = true;

	//Create new simulator object
	Simcore = new SBS();

	//initialize SBS
	Simcore->Initialize(iSCF::SCF, object_reg, view, root_dir, dir_char);

	//load building data file
	Simcore->Report("\nLoading building data from " + BuildingFile + "...\n");

	//Pause for 1 second
	csSleep(1000);

	if (Reload == false)
		BuildingFile.Insert(0, "/root/buildings/");

	//load script processor object and load building
	bool loaderror = false;
	processor = new ScriptProcessor();
	if (!processor->LoadDataFile(BuildingFile))
	{
		loaderror = true;
		ReportError("Error loading building file\n");
	}
	if (loaderror == false)
	{
		if (!processor->LoadBuilding())
		{
			loaderror = true;
			ReportError("Error processing building\n");
		}
	}

	//unload script processor
	delete processor;
	processor = 0;

	if (loaderror == true)
	{
		Unload();
		return false;
	}

	//the sky needs to be created before Prepare() is called
	Simcore->CreateSky(Simcore->SkyName);

	//have CS process textures, bounding boxes and lighting
	Prepare();

	//start simulation
	if (!Simcore->Start())
		return ReportError("Error starting simulator\n");

	g2d->Clear(0);
	g2d->FinishDraw();
	g2d->Print(0);

	//set to saved position if reloading building
	if (PositionOverride == true)
	{
		PositionOverride = false;
		Simcore->camera->SetPosition(override_position);
		Simcore->camera->SetRotation(override_rotation);
	}

	//load control panel
	if (confman->GetBool("Skyscraper.Frontend.ShowControlPanel", true) == true)
	{
		dpanel = new DebugPanel(NULL, -1);
		dpanel->Show(true);
		dpanel->SetPosition(wxPoint(10, 10));
	}

	window->Raise();

	//show main window
	window->ShowWindow();

	//turn on window resizing, if specified
	//AllowResize(confman->GetBool("Skyscraper.Frontend.AllowResize", true));

	//run simulation
	Simcore->Report("Running simulation...");
	StopSound();
	Simcore->IsRunning = true;
	IsRunning = true;
	Starting = false;
	StartupRunning = false;
	Reload = false;
	return true;
}

void Skyscraper::AllowResize(bool value)
{
	//changes the window style to either allow or disallow resizing
	
	if (value)
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE | wxMAXIMIZE);
	else
		window->SetWindowStyleFlag(wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE);
	window->Refresh();
}

void Skyscraper::Prepare()
{
	//prepare CS engine objects
	engine->Prepare();
}

void Skyscraper::Unload()
{
	//unload sim and return to the main menu
	BuildingFile = "";
	IsRunning = false;
	Starting = false;
	Pause = false;
	UnloadSim();
	DrawBackground();
	StartSound();
	StartupRunning = true;
	mouse->Reset();
}

void Skyscraper::Quit()
{
	//exit app
	if(dpanel)
	{
		if(dpanel->timer)
			dpanel->timer->Stop();
	}
	wxGetApp().Exit();
}
