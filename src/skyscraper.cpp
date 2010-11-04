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
#include "globals.h"
#include "Ogre.h"
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
	//BuildingFile = confman->GetStr("Skyscraper.Frontend.AutoLoad");
	//if (BuildingFile != "")
		//return Start();

	//show menu
	/*if (confman->GetBool("Skyscraper.Frontend.ShowMenu", true) == true)
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
	/*if (skyscraper->StartupRunning == false)
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
	}*/

	if (skyscraper->mRenderWindow)
		skyscraper->mRenderWindow->update(true);

	// Render to the frame buffer
	mRoot->renderOneFrame();
}

bool Skyscraper::Initialize()
{
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
				OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "No RenderSystem chosen", "wxOgre::createOgreRenderWindow");
		}
	}

	mRoot->initialise(false);
	mRenderWindow = CreateRenderWindow();

	//load resources
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
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
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	mCamera = mSceneMgr->createCamera("Main Camera");
	mViewport = mRenderWindow->addViewport(mCamera);
	//mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	/*
	object_reg = csInitializer::CreateEnvironment(argc, argv);
	if (!object_reg) return false;

	//first initialize VFS
	vfs = csInitializer::SetupVFS(object_reg);
	if (!vfs)
		return ReportError("Error loading the VFS plugin");
	
	//mount app's directory in VFS
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
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

#if CS_VERSION_NUM_MAJOR == 1 && CS_VERSION_NUM_MINOR == 4
	csRef<iBase> plug = csLoadPluginAlways (plugin_mgr, "crystalspace.utilities.bugplug");
#endif
#if CS_VERSION_NUM_MAJOR == 1 && CS_VERSION_NUM_MINOR == 9
	csRef<iComponent> plug = csLoadPluginAlways (plugin_mgr, "crystalspace.utilities.bugplug");
#endif
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
	canvas_width = canvas->size().GetWidth();
	canvas_height = canvas->size().GetHeight();

	font = g2d->GetFontServer()->LoadFont(CSFONT_LARGE);
	*/

	//Platform = OGRE_PLATFORM;

	/*Ogre::String renderLoop;
	if (confman->GetBool("Skyscraper.Frontend.Shaders", false) == true)
	{
		renderLoop = "diffuse";
		Shaders = true;
	}
	if (confman->GetBool("Skyscraper.Frontend.ShaderShadows", false) == true && Shaders == true)
		renderLoop = "shadowed";*/

	/*
	// Open the main system. This will open all the previously loaded plug-ins.
	if (!csInitializer::OpenApplication (object_reg))
		return ReportError ("Error opening system!");

	shaderman = csQueryRegistry<iShaderManager> (object_reg);
	if (!shaderman)	return ReportError("Failed to locate shader manager");

	if (!loader->LoadShader("/shader/light.xml"))
		return false;
	if (!loader->LoadShader("/shader/light_bumpmap.xml"))
		return false;
	if (!loader->LoadShader("/shader/ambient.xml"))
		return false;
	if (!loader->LoadShader("/shader/fullbright.xml"))
		return false;
	if (!loader->LoadShader("/shader/reflectsphere.xml"))
		return false;
	if (!loader->LoadShader("/shader/parallax/parallax.xml"))
		return false;
	if (!loader->LoadShader("/shader/parallaxAtt/parallaxAtt.xml"))
		return false;
	if (!loader->LoadShader("/shader/specular/light_spec_bumpmap.xml"))
		return false;

	//initialize frame printer
	printer.AttachNew(new FramePrinter(object_reg));

	//initialize event queue
	equeue = csQueryRegistry<iEventQueue> (object_reg);

	//enable ambient light
	//engine->SetAmbientLight(csColor(0.5, 0.5, 0.5));
	engine->SetAmbientLight(csColor(1, 1, 1));

	//set up viewport
	view = csPtr<iView>(new csView (engine, g3d));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//workaround for a canvas quirk on Mac
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	canvas->resize(canvas_width, canvas_height + 1);
	canvas->resize(canvas_width, canvas_height);
#endif

	if (!renderLoop.IsEmpty ())
	{
		iRenderLoopManager* rloopmgr = engine->GetRenderLoopManager();
		Ogre::String rl = "/shader/std_rloop_";
		rl += renderLoop;
		rl += ".xml";
		csRef<iRenderLoop> rloop = rloopmgr->Load(rl);
		if (!rloop)
			return ReportError("Bad renderloop '%s'", (const char*)renderLoop);
		if (!engine->SetCurrentDefaultRenderloop (rloop))
			return ReportError ("Couldn't set renderloop in engine!");
	}
	*/
	return true;
}

void Skyscraper::GetInput()
{
	//quit if main window isn't selected
	if (window->IsActive() == false)
		return;

	static bool wireframe;
	static bool wait, waitcheck;
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
	//Simcore->mouse_x = mouse->GetLastX();
	//Simcore->mouse_y = mouse->GetLastY();

	//if mouse coordinates changed, and we're in freelook mode, rotate camera
	/*if (Simcore->camera->Freelook == true && (old_mouse_x != Simcore->mouse_x || old_mouse_y != Simcore->mouse_y))
	{
		canvas->WarpPointer(g2d->GetWidth() / 2, g2d->GetHeight() / 2);
		Ogre::Vector3 rotational (Simcore->camera->Freelook_speed * (-((float)(Simcore->mouse_y - (g2d->GetHeight() / 2))) / (g2d->GetHeight() * 2)), Simcore->camera->Freelook_speed * (-((g2d->GetWidth() / 2) - (float)Simcore->mouse_x) / (g2d->GetWidth() * 2)), 0);
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
	}*/

	if (wxGetKeyState(WXK_ESCAPE))
	{
		int result = wxMessageBox(wxT("Exit and return to the main menu?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		if (result == wxYES)
			Shutdown = true;
	}

	if (wxGetKeyState(WXK_F2) && wait == false)
	{
		//Report(wxVariant(Simcore->FPS).GetString().ToAscii());
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
		if (wxGetKeyState((wxKeyCode)'v') && wait == false)
		{
			if (Simcore->camera->GetGravityStatus() == false)
			{
				Simcore->camera->EnableGravity(true);
	                        Simcore->camera->EnableCollisions = true;
				Report("Gravity and collision detection on");
			}
			else
			{
				Simcore->camera->EnableGravity(false);
                                Simcore->camera->EnableCollisions = false;
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
				//bugplug->ExecCommand("edges");
				//bugplug->ExecCommand("clear");
				Simcore->EnableSkybox(false);
				wireframe = true;
			}
			else
			{
				//bugplug->ExecCommand("edges");
				Simcore->EnableSkybox(true);
				wireframe = false;
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
			//dpanel->SetPosition(wxPoint(confman->GetInt("Skyscraper.Frontend.ControlPanelX", 10), confman->GetInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}
		if (wxGetKeyState(WXK_F5) && wait == false)
		{
			//enable/disable freelook mode
			Simcore->camera->Freelook = !Simcore->camera->Freelook;
			//if (Simcore->camera->Freelook == true)
				//canvas->SetCursor(wxCURSOR_BLANK);
			//else
				//canvas->SetCursor(wxNullCursor);
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

	//RenderOnly = confman->GetBool("Skyscraper.Frontend.RenderOnly", false);
	//InputOnly = confman->GetBool("Skyscraper.Frontend.InputOnly", false);

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
		//if (confman->GetBool("Skyscraper.Frontend.ShowMenu", true) == true)
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

	/*
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
	        			button->drawn_selected = true;
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
	}*/

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

	/*if (DisableSound == true)
		return;

	if (confman->GetBool("Skyscraper.Frontend.IntroMusic", true) == false)
	{
		DisableSound = true;
		return;
	}

	Ogre::String filename = confman->GetStr("Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	Ogre::String filename_full = "data/" + filename;

	//load new sound
	csRef<iDataBuffer> sndbuffer = vfs->ReadFile(filename_full);
	if (!sndbuffer)
	{
		ReportError("Can't load file " + filename);
		return;
	}

	csRef<iSndSysData> snddata = sndloader->LoadSound(sndbuffer);
	if (!snddata)
	{
		ReportError("Can't load sound " + filename);
		return;
	}

	sndstream = sndrenderer->CreateStream(snddata, CS_SND3D_DISABLE);
	if (!sndstream)
	{
		ReportError("Can't create stream for " + filename);
		return;
	}

	sndsource = sndrenderer->CreateSource(sndstream);
	if (!sndsource)
	{
		ReportError("Can't create source for " + filename);
		return;
	}

	sndstream->SetLoopState(true);
	sndsource->SetVolume(1.0f);
	sndstream->Unpause();*/
}

void Skyscraper::StopSound()
{
	//stop and unload sound
	/*if (sndstream)
		sndstream->Pause();
	if (sndrenderer)
	{
		sndrenderer->RemoveSource(sndsource);
		sndrenderer->RemoveStream(sndstream);
	}
	sndsource = 0;
	sndstream = 0;*/
}

bool Skyscraper::SelectBuilding()
{
	//choose a building from a script file
	BuildingFile = "";
	srand (time (0));

	//set building file
	//wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings/"), _(""), _("Building files (*.bld;*.txt)|*.bld;*.txt"), wxOPEN);
	wxFileDialog *Selector = new wxFileDialog(0, _("Select a Building"), _("buildings/"), _(""), _("Building files (*.bld)|*.bld"), wxOPEN);
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
	/*g2d->Clear(0);
	g2d->FinishDraw();
	g2d->Print(0);
	g2d->ClearAll(0);*/

	//resize main window
	window->SetBackgroundColour(*wxBLACK);
	//window->resize(confman->GetInt("Skyscraper.Frontend.ScreenWidth", 640), confman->GetInt("Skyscraper.Frontend.ScreenHeight", 480));
	window->Center();

	//switch to fullscreen mode if specified
	/*if (confman->GetBool("Skyscraper.Frontend.FullScreen", false) == true)
	{
		FullScreen = true;
		window->ShowFullScreen(FullScreen);
	}*/

	Starting = true;

	//Create new simulator object
	Simcore = new SBS();

	//initialize SBS
	Simcore->Initialize(mRenderWindow, mSceneMgr, mCamera, root_dir.c_str(), dir_char.c_str());
	Simcore->Shaders = Shaders;

	//load building data file
	Simcore->Report("\nLoading building data from " + BuildingFile + "...\n");
	Simcore->BuildingFilename = BuildingFile;

	//Pause for 1 second
	//sleep(1000);

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
	Simcore->CreateSky(Simcore->SkyName.c_str());

	//have CS process textures, bounding boxes and lighting
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
	//if (confman->GetBool("Skyscraper.Frontend.ShowControlPanel", true) == true)
	//{
		dpanel = new DebugPanel(NULL, -1);
		dpanel->Show(true);
		//dpanel->SetPosition(wxPoint(confman->GetInt("Skyscraper.Frontend.ControlPanelX", 10), confman->GetInt("Skyscraper.Frontend.ControlPanelY", 25)));
	//}

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

Ogre::RenderWindow* Skyscraper::CreateRenderWindow(const Ogre::NameValuePairList* miscParams, const Ogre::String& windowName)
{
	Ogre::String name = windowName;
	if (windowName.empty())
		name = Ogre::String("wxOgreRenderWindow");

	//do not clear background
	//window->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	int width, height;
	window->GetClientSize(&width, &height);

	Ogre::NameValuePairList params;
	if (miscParams)
		params = *miscParams;

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

const Ogre::String Skyscraper::getOgreHandle() const
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

   return Ogre::String(handleStream.str());

#elif defined(__WXMAC__)
   Rect rect;
   wxPoint position;
   wxSize size;
   position = GetScreenPosition();
   size      = GetSize();
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
