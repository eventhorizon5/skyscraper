/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Simulation Frontend
	Copyright (C)2003-2010 Ryan Thoryk
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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <fmod.hpp>
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "debugpanel.h"
#include "revmain.h"

#if defined(__WXGTK__)
   // NOTE: Find the GTK install config with `pkg-config --cflags gtk+-2.0`
   #include "gtk/gtk.h"
   #include "gdk/gdk.h"
   #include "gdk/gdkx.h"
   #include "wx/gtk/win_gtk.h"
   #include "GL/glx.h"
#endif

#if defined(__WXMAC__)
#include <Carbon/Carbon.h>
	WindowRef i_carbonWin;
#endif

IMPLEMENT_APP_NO_MAIN(Skyscraper)
//IMPLEMENT_CLASS(Skyscraper, wxApp)
//BEGIN_EVENT_TABLE(Skyscraper, wxApp)
//END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MainScreen, wxFrame)
  EVT_SHOW(MainScreen::OnShow)
  EVT_ICONIZE(MainScreen::OnIconize)
  EVT_SIZE(MainScreen::OnSize)
  EVT_CLOSE(MainScreen::OnClose)
  EVT_IDLE(MainScreen::OnIdle)
  EVT_PAINT(MainScreen::OnPaint)
END_EVENT_TABLE()

SBS *Simcore;
Skyscraper *skyscraper;
DebugPanel *dpanel;
MainScreen *window;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "uexception.h"
#endif

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	//initialize top-level exception handler
	InitUnhandledExceptionFilter();
#endif

	//main wxWidgets entry point
	wxEntry(argc, argv);

	return 0;
}

bool Skyscraper::OnInit(void)
{
	version = "1.8";
	version_rev = SVN_REVSTR;
	version_state = "Alpha";
	version_frontend = version + ".0." + version_rev;
	skyscraper = this;
	MouseDown = false;
	RenderOnly = false;
	//InputOnly = false;
	canvas_width = 0;
	canvas_height = 0;
	IsRunning = false;
	StartupRunning = false;
	Starting = false;
	Pause = false;
	DisableSound = false;
	DrewButtons = false;
	FullScreen = false;
	Shutdown = false;
	PositionOverride = false;
	Reload = false;
	Shaders = false;
	mRoot = 0;
	mRenderWindow = 0;
	mViewport = 0;
	mSceneMgr = 0;
	canvas = 0;

	//Create main window
	window = new MainScreen(640, 480);
	//AllowResize(false);
	window->ShowWindow();
	window->CenterOnScreen();

	//start and initialize OGRE
	if (!Initialize())
		return ReportError("Error initializing OGRE");

	//autoload a building file if specified
	//BuildingFile = GetConfigString("Skyscraper.Frontend.AutoLoad");
	//if (BuildingFile != "")
		//return Start();

	//show menu
	/*if (GetConfigBool("Skyscraper.Frontend.ShowMenu", true) == true)
	{
		//draw background
		DrawBackground();
		StartupRunning = true;
		StartSound();
	}
	else
	{*/
		//or show building selection window if ShowMenu is false
		if (SelectBuilding() == true)
			return Start();
		else
			return false;
	//}

	return true;
}

int Skyscraper::OnExit()
{
	//clean up

	//cleanup
	printf("Cleaning up...\n");

	UnloadSim();

	//cleanup sound
	if (soundsys)
		soundsys->release();

	//if (sndsource)
		//StopSound();

	//delete wx canvas
	if (canvas)
		canvas->Destroy();
	canvas = 0;

	window->Destroy();
	window = 0;
	skyscraper = 0;

	delete mRoot;
	return wxApp::OnExit();
	//return 0;
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

MainScreen::MainScreen(int width, int height) : wxFrame(0, -1, wxT(""), wxDefaultPosition, wxSize(width, height), wxDEFAULT_FRAME_STYLE)
{
	this->Center();
	wxString title;
	title = wxT("Skyscraper 1.8 Alpha");
	//title = wxT("Skyscraper " + skyscraper->version + " " + skyscraper->version_state);
	this->SetTitle(title);
	//panel = new wxPanel(this, -1, wxPoint(0, 0), this->GetClientSize());
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
	//printf("got show %d\n", (int)event.GetShow());
}

void MainScreen::OnSize(wxSizeEvent& WXUNUSED(event))
{
	//panel->resize(this->GetClientSize());
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
	//panel->Show(true);
}

void MainScreen::OnIdle(wxIdleEvent& event)
{
	if ((skyscraper->IsRunning == true && skyscraper->Pause == false) || skyscraper->StartupRunning == true)
		skyscraper->Loop(); //run simulator loop
	if (skyscraper->Pause == false)
		event.RequestMore(); //request more idles
}

void MainScreen::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	//if (skyscraper->mRenderWindow)
		//skyscraper->mRenderWindow->update(true);
}

void Skyscraper::Render()
{
	if (skyscraper->StartupRunning == false)
	{
		// Tell 3D driver we're going to display 3D things.

		//if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN))
			//return;
	}

#if defined(__WXGTK__)
	if (skyscraper->mRenderWindow)
		skyscraper->mRenderWindow->update(true);
#endif

	// Render to the frame buffer
	mRoot->renderOneFrame();
}

bool Skyscraper::Initialize()
{
	//initialize OGRE

	mRoot = Ogre::Root::getSingletonPtr();
	if(!mRoot)
		mRoot = new Ogre::Root();
	
	if(!mRoot->getRenderSystem())
	{
		//if no render systems are loaded, try to load previous config
		if(!mRoot->restoreConfig())
		{
			//show dialog if load failed
			if(!mRoot->showConfigDialog())
				OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "No RenderSystem chosen", "Skyscraper::Initialize");
		}
	}

	mRoot->initialise(false);
	mRenderWindow = CreateRenderWindow();
	//mRoot->getRenderSystem()->setWaitForVerticalBlank(false); //disable vsync

	//load config file
	configfile.load("skyscraper.ini");

	//load resources
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	std::string secName, typeName, archName;
	while(seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i)
		{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}
	
	//add app's directory to resource manager
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem", "General", true);
	
	//create scene manager
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

	//set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	mCamera = mSceneMgr->createCamera("Main Camera");
	mViewport = mRenderWindow->addViewport(mCamera);
	//mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	//initialize FMOD (sound)
	FMOD_RESULT result = FMOD::System_Create(&soundsys);
	if (result != FMOD_OK)
	{
		printf("Error initializing FMOD\n");
		return false;
	}
	result = soundsys->init(100, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK)
	{
		printf("Error initializing FMOD\n");
		return false;
	}

	return true;
}

void Skyscraper::GetInput()
{
	//quit if main window isn't selected
	if (window->IsActive() == false)
		return;

	static int wireframe;
	static bool wait, waitcheck, colliders;
	static unsigned int old_time;
	static int old_mouse_x, old_mouse_y;

	// First get elapsed time from the virtual clock.
	elapsed_time = Simcore->GetElapsedTime();
	current_time = Simcore->GetRunTime();

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
	Simcore->camera->desired_velocity = Ogre::Vector3(0, 0, 0);
	Simcore->camera->desired_angle_velocity = Ogre::Vector3(0, 0, 0);

	//get old mouse coordinates
	old_mouse_x = Simcore->mouse_x;
	old_mouse_y = Simcore->mouse_y;

	//get mouse pointer coordinates
	Simcore->mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	Simcore->mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

	//if mouse coordinates changed, and we're in freelook mode, rotate camera
	if (Simcore->camera->Freelook == true && (old_mouse_x != Simcore->mouse_x || old_mouse_y != Simcore->mouse_y))
	{
		window->WarpPointer(window->GetClientSize().GetWidth() / 2, window->GetClientSize().GetHeight() / 2);
		Ogre::Vector3 rotational;
		rotational.x = Ogre::Math::DegreesToRadians(Simcore->camera->Freelook_speed * -((float)(Simcore->mouse_y - (window->GetClientSize().GetHeight() / 2))) / (window->GetClientSize().GetHeight() * 2));
		rotational.y = Ogre::Math::DegreesToRadians(Simcore->camera->Freelook_speed * -((window->GetClientSize().GetWidth() / 2) - (float)Simcore->mouse_x) / (window->GetClientSize().GetWidth() * 2));
		rotational.z = 0;
		Simcore->camera->RotateLocal(rotational, 1);
	}

	//check if the user clicked on an object, and process it
	if (wxGetMouseState().LeftDown() == true && MouseDown == false)
	{
		MouseDown = true;
		Simcore->camera->MouseDown = MouseDown;
		Simcore->camera->ClickedObject(wxGetKeyState(WXK_SHIFT), wxGetKeyState(WXK_CONTROL), wxGetKeyState(WXK_ALT));
	}

	//reset mouse state
	if (wxGetMouseState().LeftDown() == false)
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
		Report(std::string(wxVariant(Simcore->FPS).GetString().ToAscii()));
		wait = true;
	}

	Simcore->camera->speed = 1;
	float speed_normal = Simcore->camera->cfg_speed;
	float speed_fast = Simcore->camera->cfg_speedfast;
	float speed_slow = Simcore->camera->cfg_speedslow;

	//crash test
	if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_ALT) && wxGetKeyState((wxKeyCode)'c'))
		throw;

	if (wxGetKeyState(WXK_CONTROL))
	{
		if (wxGetKeyState((wxKeyCode)'r'))
		{
			Reload = true;
			return;
		}
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
		if (wxGetKeyState((wxKeyCode)'v') && wait == false)
		{
			if (Simcore->camera->GetGravityStatus() == false)
			{
				Simcore->camera->EnableGravity(true);
				Simcore->camera->EnableCollisions(true);
				Report("Gravity and collision detection on");
			}
			else
			{
				Simcore->camera->EnableGravity(false);
				Simcore->camera->EnableCollisions(false);
				Report("Gravity and collision detection off");
			}
			wait = true;
		}
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
		if (wxGetKeyState(WXK_SPACE) && wait == false)
		{
			if (Simcore->camera->IsOnGround() == true)
			{
				Simcore->camera->Jump();
				wait = true;
			}
		}
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
			Simcore->camera->EnableCollisions(true);
			Simcore->camera->SetToStartPosition();
			Simcore->camera->SetToStartRotation();
			Simcore->camera->SetToDefaultFOV();
			Simcore->GetFloor(Simcore->camera->StartFloor)->Enabled(true);
			Simcore->GetFloor(Simcore->camera->StartFloor)->EnableGroup(true);
		}

		if (wxGetKeyState(WXK_F7) && wait == false)
		{
			//show colliders
			Simcore->ShowColliders(!colliders);
			colliders = !colliders;
			wait = true;
		}

		if (wxGetKeyState(WXK_F4) && wait == false)
		{
			//enable/disable wireframe mode
			if (wireframe == 0)
			{
				Simcore->EnableSkybox(false);
				Simcore->camera->SetViewMode(1);
				wireframe = 1;
			}
			else if (wireframe == 1)
			{
				Simcore->camera->SetViewMode(2);
				wireframe = 2;
			}
			else if (wireframe == 2)
			{
				Simcore->EnableSkybox(true);
				Simcore->camera->SetViewMode(0);
				wireframe = 0;
			}
			wait = true;
		}
		if (wxGetKeyState(WXK_F11) && wait == false)
		{
			//bugplug->ExecCommand("scrshot");
			wait = true;
		}
		if (wxGetKeyState(WXK_F12) && !dpanel)
		{
			//show control panel if closed
			dpanel = new DebugPanel(NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}
		if (wxGetKeyState(WXK_F5) && wait == false)
		{
			//enable/disable freelook mode
			Simcore->camera->Freelook = !Simcore->camera->Freelook;
			if (Simcore->camera->Freelook == true)
				window->SetCursor(wxCURSOR_BLANK);
			else
				window->SetCursor(wxNullCursor);
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

void Skyscraper::Report(std::string message, ...)
{
	ReplaceAll(message, "%", "%%"); //allow percent signs

	printf(message.c_str());
	printf("\n");
	fflush(stdout);
}

bool Skyscraper::ReportError(std::string message, ...)
{
	ReplaceAll(message, "%", "%%"); //allow percent signs

	printf(message.c_str());
	printf("\n");
	fflush(stdout);

	return false;
}

void Skyscraper::Loop()
{
	//Main simulator loop

	//main menu routine
	if (IsRunning == false)
	{
		DrawBackground();
		GetMenuInput();
		return;
	}

	if (!Simcore)
		return;

	Simcore->AdvanceClock();
	if (IsRunning == true)
		Simcore->CalculateFrameRate();

	//resize canvas if needed
	/*if (canvas->GetSize().GetWidth() != canvas_width || canvas->GetSize().GetHeight() != canvas_height)
	{
		//update canvas size values
		canvas_width = canvas->GetSize().GetWidth();
		canvas_height = canvas->GetSize().GetHeight();

		//resize viewport
		//wxwin->GetWindow()->resize(canvas->size());
	}*/

	//RenderOnly = GetConfigBool("Skyscraper.Frontend.RenderOnly", false);
	//InputOnly = GetConfigBool("Skyscraper.Frontend.InputOnly", false);

	Simcore->RenderOnly = RenderOnly;
	//Simcore->InputOnly = this->InputOnly;

	//run SBS main loop
	Simcore->MainLoop();

	//get input
	if (RenderOnly == false)
		GetInput();

	//process camera loop
	Simcore->camera->Loop();

	//render graphics
	Render();

	//exit if shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		//if showmenu is true, unload simulator and return to main menu
		//if (GetConfigBool("Skyscraper.Frontend.ShowMenu", true) == true)
			//Unload();
		//otherwise exit app
		//else
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
		//mouse->Reset();
		Start();
	}
}

void Skyscraper::DrawBackground()
{
	//draw menu background

	int w = mRenderWindow->getWidth();
	int h = mRenderWindow->getHeight();
	/*if (!g3d->BeginDraw(CSDRAW_2DGRAPHICS))
		return;
	g2d->Clear(0);
	g2d->SetClipRect(0, 0, w, h);

	DrawImage("data/menu.png", 0, 0, 1, false);

	if (DrewButtons == false)
	{
		DrawImage("data/button_triton.png", &button1, 0, -20, true, "/root/data/button_triton_selected.png", "/root/data/button_triton_pressed.png");
		DrawImage("data/button_searstower.png", &button2, 0, 30, true, "/root/data/button_searstower_selected.png", "/root/data/button_searstower_pressed.png");
		DrawImage("data/button_glasstower.png", &button3, 0, 80, true, "/root/data/button_glasstower_selected.png", "/root/data/button_glasstower_pressed.png");
		DrawImage("data/button_simple.png", &button4, 0, 130, true, "/root/data/button_simple_selected.png", "/root/data/button_simple_pressed.png");
		DrawImage("data/button_other.png", &button5, 0, 180, true, "/root/data/button_other_selected.png", "/root/data/button_other_pressed.png");
		DrewButtons = true;
	}*/
}

void Skyscraper::DrawImage(const char *filename, buttondata *button, int x, int y, bool center, const char *filename_selected, const char *filename_pressed)
{
	int w2, h2;
	int w = mRenderWindow->getWidth();
	int h = mRenderWindow->getHeight();

	/*
	image = 0;

	if (!filename)
		return;

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
	image = 0;*/
}

void Skyscraper::GetMenuInput()
{
	//input handler for main menu

	//exit if simulator is starting
	if (Starting == true)
		return;

	//get mouse coordinates
	int mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	int mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

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
			DrawImage(button->filename_pressed.c_str(), button, button->offset_x, button->offset_y, true);
		else if (button->drawn_selected == true)
			DrawImage(button->filename_selected.c_str(), button, button->offset_x, button->offset_y, true);
		else
			DrawImage(button->filename.c_str(), button, button->offset_x, button->offset_y, true);

	    //only process buttons if main window is selected
        if (window->IsActive() != false)
        {
        	//change button status based on mouse position and button press status
        	if (mouse_x > button->x && mouse_x < button->x + button->size_x && mouse_y > button->y && mouse_y < button->y + button->size_y)
        	{
        		if (button->drawn_selected == false && wxGetMouseState().LeftDown() == true == false)
        		{
        			if (button->drawn_pressed == true)
        			{
        				//user clicked on button
        				Click(i);
	        			button->drawn_selected = true;
        				return;
        			}
        			button->drawn_selected = true;
        		}
        		if (button->drawn_pressed == false && wxGetMouseState().LeftDown() == true == true)
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

	if (GetConfigBool("Skyscraper.Frontend.IntroMusic", true) == false)
	{
		DisableSound = true;
		return;
	}

	std::string filename = GetConfigString("Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	std::string filename_full = "data/" + filename;

	//load new sound
	FMOD_RESULT result = soundsys->createSound(filename_full.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &sound);
	if (result != FMOD_OK)
	{
		ReportError("Can't load file '" + filename_full + "'");
		return;
	}

	FMOD::Channel *channel;
	result = soundsys->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
	if (result != FMOD_OK)
	{
		ReportError("Error playing " + filename);
		return;
	}

	channel->setLoopCount(-1);
	channel->setVolume(1.0f);
	channel->setPaused(false);
}

void Skyscraper::StopSound()
{
	//stop and unload sound
	if (sound)
		sound->release();
	sound = 0;
}

bool Skyscraper::SelectBuilding()
{
	//choose a building from a script file
	BuildingFile = "";
	srand (time (0));

	//set building file
	//wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings/"), _(""), _("Building files (*.bld;*.txt)|*.bld;*.txt"), wxFD_OPEN);
	wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings/"), _(""), _("Building files (*.bld)|*.bld"), wxFD_OPEN);
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

	//resize main window
	window->SetBackgroundColour(*wxBLACK);
	//window->resize(GetConfigInt("Skyscraper.Frontend.ScreenWidth", 640), GetConfigInt("Skyscraper.Frontend.ScreenHeight", 480));
	window->Center();

	//switch to fullscreen mode if specified
	if (GetConfigBool("Skyscraper.Frontend.FullScreen", false) == true)
	{
		FullScreen = true;
		window->ShowFullScreen(FullScreen);
	}

	Starting = true;

	//Create new simulator object
	Simcore = new SBS();

	//initialize SBS
	Simcore->Initialize(mRenderWindow, mSceneMgr, mCamera, root_dir.c_str(), dir_char.c_str(), soundsys);
	Simcore->Shaders = Shaders;

	//load building data file
	Simcore->Report("\nLoading building data from " + BuildingFile + "...\n");
	Simcore->BuildingFilename = BuildingFile;

	//Pause for 1 second
	wxSleep(1);

	if (Reload == false)
		BuildingFile.insert(0, "buildings/");

	//load script processor object and load building
	bool loaderror = false;
	processor = new ScriptProcessor();
	if (!processor->LoadDataFile(BuildingFile.c_str()))
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

	//report on missing files, if any
	if (loaderror == false)
		processor->ReportMissingFiles();

	//unload script processor
	delete processor;
	processor = 0;

	if (loaderror == true)
	{
		Unload();
		return false;
	}

	//the sky needs to be created before Prepare() is called
	//Simcore->CreateSky(Simcore->SkyName.c_str());

	//have SBS prepare objects for use (upload geometry data to graphics card, etc)
	Simcore->Prepare();

	//start simulation
	if (!Simcore->Start())
		return ReportError("Error starting simulator\n");

	//g2d->Clear(0);
	//g2d->FinishDraw();
	//g2d->Print(0);

	//set to saved position if reloading building
	if (PositionOverride == true)
	{
		PositionOverride = false;
		Simcore->camera->SetPosition(override_position);
		Simcore->camera->SetRotation(override_rotation);
	}

	//load control panel
	if (GetConfigBool("Skyscraper.Frontend.ShowControlPanel", true) == true)
	{
		dpanel = new DebugPanel(NULL, -1);
		dpanel->Show(true);
		dpanel->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
	}

	window->Raise();

	//show main window
	window->ShowWindow();

	//turn on window resizing, if specified
	AllowResize(GetConfigBool("Skyscraper.Frontend.AllowResize", true));

	//run simulation
	Simcore->Report("Running simulation...");
	//StopSound();
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

void Skyscraper::Unload()
{
	//unload sim
	BuildingFile = "";
	IsRunning = false;
	Starting = false;
	Pause = false;
	UnloadSim();

	//cleanup sound
	//if (sndsource)
		//StopSound();

	//return to main menu
	DrawBackground();
	StartSound();
	StartupRunning = true;
	//mouse->Reset();
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

Ogre::RenderWindow* Skyscraper::CreateRenderWindow(const Ogre::NameValuePairList* miscParams, const std::string& windowName)
{
	std::string name = windowName;
	//if (windowName.empty())
		//name = std::string("wxOgreRenderWindow");

	//do not clear background
	//window->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	int width, height;
	window->GetClientSize(&width, &height);

	Ogre::NameValuePairList params;
	if (miscParams)
		params = *miscParams;

	params["vsync"] = "true";
	params["vsyncInterval"] = "1";
	params["externalWindowHandle"] = getOgreHandle();

	//create the render window
	mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(name, width, height, false, &params);
	mRenderWindow->setActive(true);
	
	return mRenderWindow;
}

void Skyscraper::destroyRenderWindow()
{
	if (mRenderWindow)
	   Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);

	mRenderWindow = 0;

	//restore background
	//window->SetBackgroundStyle(wxBG_STYLE_SYSTEM);

#if defined(__WXMAC__)
   DisposeWindow(i_carbonWin);
#endif
}

const std::string Skyscraper::getOgreHandle() const
{
#if defined(__WXMSW__)
   // Handle for Windows systems
   return Ogre::StringConverter::toString((size_t)((HWND)window->GetHandle()));
#elif defined(__WXGTK__)
   // Handle for GTK-based systems

   // wxWidgets uses several internal GtkWidgets, the GetHandle method
   // returns a different one than this, but wxWidgets's GLCanvas uses this
   // one to interact with GLX, so we do the same.
   // NOTE: this method relies on implementation details in wxGTK and could
   //      change without any notification from the developers.
   GtkWidget* privHandle = window->m_wxwindow;

   // prevents flickering
   gtk_widget_set_double_buffered(privHandle, false);

   gtk_widget_realize(privHandle);

   // grab the window object
   GdkWindow* gdkWin = GTK_PIZZA(privHandle)->bin_window;
   Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
   Window wid = GDK_WINDOW_XWINDOW(gdkWin);

   // screen (returns "display.screen")
   std::string screenStr = DisplayString(display);
   screenStr = screenStr.substr(screenStr.find(".") + 1, screenStr.size());

   std::stringstream handleStream;
   handleStream << (unsigned long)display << ':' << screenStr << ':' << wid;

   // retrieve XVisualInfo
   // NOTE: '-lGL' linker flag must be specified.
   int attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };
   XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);
   handleStream << ':' << (unsigned long)vi;

   return std::string(handleStream.str());

#elif defined(__WXMAC__)
   Rect rect;
   wxPoint position;
   wxSize size;
   position = window->GetScreenPosition();
   size      = window->GetSize();
   rect.left   = position.x;
   rect.top   = position.y;
   rect.right   = position.x + size.GetWidth();
   rect.bottom   = position.y + size.GetHeight();

   CreateNewWindow(kFloatingWindowClass, kWindowNoTitleBarAttribute, &rect, &i_carbonWin);
   ShowWindow(i_carbonWin);
   return Ogre::StringConverter::toString((unsigned long)(HIViewGetRoot(i_carbonWin)));

#else
   #error Not supported on this platform!
#endif
}

int Skyscraper::GetConfigInt(const char *key, int default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseInt(result);
}

std::string Skyscraper::GetConfigString(const char *key, const char *default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return result;
}

bool Skyscraper::GetConfigBool(const char *key, bool default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseBool(result);
}

float Skyscraper::GetConfigFloat(const char *key, float default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseReal(result);
}
