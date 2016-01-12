/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Simulation Frontend
	Copyright (C)2003-2016 Ryan Thoryk
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
#include "wx/dir.h"
#endif
#include <locale>
#include "globals.h"
#include "sbs.h"
#include "debugpanel.h"
#include "skyscraper.h"
#include "unix.h"
#include "revmain.h"

#if defined(__WXGTK__)
   // NOTE: Find the GTK install config with `pkg-config --cflags gtk+-2.0`
   #include "gtk/gtk.h"
   #include "gdk/gdk.h"
   #include "gdk/gdkx.h"
   #include "GL/glx.h"
#endif

using namespace SBS;

namespace Skyscraper {

IMPLEMENT_APP_NO_MAIN(Skyscraper)

BEGIN_EVENT_TABLE(MainScreen, wxFrame)
  EVT_ICONIZE(MainScreen::OnIconize)
  EVT_SIZE(MainScreen::OnSize)
  EVT_CLOSE(MainScreen::OnClose)
  EVT_IDLE(MainScreen::OnIdle)
  EVT_PAINT(MainScreen::OnPaint)
  EVT_ACTIVATE(MainScreen::OnActivate)
  EVT_ENTER_WINDOW(MainScreen::OnEnterWindow)
  EVT_LEAVE_WINDOW(MainScreen::OnLeaveWindow)
END_EVENT_TABLE()

}

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
	Skyscraper::InitUnhandledExceptionFilter();
#endif

	//main wxWidgets entry point
	wxEntry(argc, argv);

	return 0;
}

namespace Skyscraper {

bool Skyscraper::OnInit(void)
{
	version = "1.10";
	version_rev = SVN_REVSTR;
	version_state = "Alpha";
	version_frontend = version + ".0." + version_rev;
	MouseDown = false;
	StartupRunning = false;
	Pause = false;
	FullScreen = false;
	Shutdown = false;
#if OGRE_VERSION >= 0x00010900
	mOverlaySystem = 0;
#endif
	mRoot = 0;
	mRenderWindow = 0;
	mViewport = 0;
	mSceneMgr = 0;
	mCamera = 0;
	sound = 0;
	channel = 0;
	SkyMult = 0;
	mCaelumSystem = 0;
	buttons = 0;
	buttoncount = 0;
	logger = 0;
	console = 0;
	soundsys = 0;
	progdialog = 0;
	dpanel = 0;
	window = 0;
	console = 0;
	new_location = false;
	new_time = false;
	latitude = 0.0f;
	longitude = 0.0f;
	datetime = 0.0;
	active_engine = 0;
	ConcurrentLoads = false;
	RenderOnStartup = false;
	CutLandscape = true;
	CutBuildings = true;
	CutExternal = false;
	CutFloors = false;
	loaddialog = 0;

	wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	UnhandledExceptionSetRoot(this);
#endif

	//set locale to default for conversion functions
#ifdef OGRE_DEFAULT_LOCALE
	setlocale(LC_ALL, OGRE_DEFAULT_LOCALE);
#else
	setlocale(LC_ALL, "C");
#endif

	//load config file
	try
	{
		configfile.load("skyscraper.ini");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading skyscraper.ini file\nDetails: " + e.getDescription());
	}

	showconsole = GetConfigBool("Skyscraper.Frontend.ShowConsole", true);

	//create console window
	if (showconsole == true)
		ShowConsole(false);

	//Create main window and set size from INI file defaults
	window = new MainScreen(this, GetConfigInt("Skyscraper.Frontend.MenuWidth", 640), GetConfigInt("Skyscraper.Frontend.MenuHeight", 480));
	//AllowResize(false);
	window->ShowWindow();
	window->CenterOnScreen();

	//start and initialize OGRE
	if (!Initialize())
		return ReportError("Error initializing frontend");

	//set sky name
	SkyName = GetConfigString("Skyscraper.Frontend.SkyName", "DefaultSky");

	//autoload a building file if specified
	std::string filename = GetConfigString("Skyscraper.Frontend.AutoLoad", "");
	if (filename != "")
		return Load(filename);

	//show menu
	if (GetConfigBool("Skyscraper.Frontend.Menu.Show", true) == true)
	{
		StartupRunning = true;
		StartSound();
	}
	else
	{
		//or show building selection window if ShowMenu is false
		return Load(SelectBuilding());
	}

	return true;
}

int Skyscraper::OnExit()
{
	//clean up

	//cleanup
	Report("Cleaning up...");

	if (loaddialog)
		loaddialog->Destroy();
	loaddialog = 0;

	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	UnloadSim();

	//delete Caelum
	if (mCaelumSystem)
		delete mCaelumSystem;

	//cleanup sound
	StopSound();
	if (soundsys)
		soundsys->release();

	//delete console window
	if (console)
		console->Destroy();
	console = 0;

	CloseProgressDialog();

	if (window)
		window->Destroy();
	window = 0;

#if OGRE_VERSION >= 0x00010900
	delete mOverlaySystem;
#endif
	mRoot->shutdown(); //shutdown root instead of delete, to fix a crash on certain systems
	//delete mRoot;
	delete logger;
	return wxApp::OnExit();
}

void Skyscraper::UnloadSim()
{
	//delete control panel object
	if(dpanel)
		delete dpanel;
	dpanel = 0;

	if (mCaelumSystem)
	{
		mCaelumSystem->clear();
		mCaelumSystem->detachAllViewports();
		delete mCaelumSystem;
		mCaelumSystem = 0;
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("Caelum");
	}

	if (console)
		console->bSend->Enable(false);

	DeleteEngines();

	//do a full clear of Ogre objects

	//remove all meshes
	Ogre::MeshManager::getSingleton().removeAll();

	//remove all materials
	Ogre::MaterialManager::getSingleton().removeAll();
	Ogre::MaterialManager::getSingleton().initialise();  //restore default materials

	//remove all fonts
	Ogre::FontManager::getSingleton().removeAll();

	//remove all textures
	Ogre::TextureManager::getSingleton().removeAll();

	//clear scene manager
	mSceneMgr->clearScene();
}

MainScreen::MainScreen(Skyscraper *parent, int width, int height) : wxFrame(0, -1, wxT(""), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
	frontend = parent;
	Active = false;
	InLoop = false;
	Center();
	wxString title;
	title = wxT("Skyscraper 1.10 Alpha");
	//title = wxT("Skyscraper " + frontend->version + " " + frontend->version_state);
	SetTitle(title);
	SetClientSize(width, height);
	SetExtraStyle(wxWS_EX_PROCESS_IDLE);
}

MainScreen::~MainScreen()
{

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
	if (frontend->mRenderWindow)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		frontend->mRenderWindow->resize(this->GetClientSize().GetWidth(), this->GetClientSize().GetHeight());
#endif
		frontend->mRenderWindow->windowMovedOrResized();
	}
	if (frontend->mCamera)
		frontend->mCamera->setAspectRatio(Ogre::Real(frontend->mViewport->getActualWidth()) / Ogre::Real(frontend->mViewport->getActualHeight()));
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
			frontend->Loop(); //run simulator loop
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

void MainScreen::OnEnterWindow(wxMouseEvent& event)
{

}

void MainScreen::OnLeaveWindow(wxMouseEvent& event)
{

}

void Skyscraper::Render()
{
	SBS_PROFILE_MAIN("Render");

	// Render to the frame buffer
	mRoot->renderOneFrame();

#if defined(__WXGTK__)
	if (mRenderWindow)
		mRenderWindow->update(true);
#endif
}

bool Skyscraper::Initialize()
{
	//initialize OGRE
	try
	{
		mRoot = Ogre::Root::getSingletonPtr();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error during initial OGRE check\nDetails: " + e.getDescription());
	}

	if(!mRoot)
	{
		try
		{
			//set up custom logger
			if (!logger)
			{
				logger = new Ogre::LogManager();
				Ogre::Log *log = logger->createLog("skyscraper.log", true, !showconsole, false);
				log->addListener(this);
			}

			//load OGRE
			mRoot = new Ogre::Root();
		}
		catch (Ogre::Exception &e)
		{
			return ReportFatalError("Error initializing OGRE\nDetails: " + e.getDescription());
		}
	}

	//set up overlay system
#if OGRE_VERSION >= 0x00010900
	try
	{
		mOverlaySystem = new Ogre::OverlaySystem();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating overlay system\nDetails: " + e.getDescription());
	}
#endif

	//configure render system
	try
	{
		if(!mRoot->getRenderSystem())
		{
			//if no render systems are loaded, try to load previous config
			if(!mRoot->restoreConfig())
			{
				//show dialog if load failed
				mRoot->showConfigDialog();
			}
		}
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error configuring render system\nDetails: " + e.getDescription());
	}

	//if using DirectX, prevent it from switching into single-point floating point mode
	Ogre::RenderSystem *rendersystem = mRoot->getRenderSystem();
	if (rendersystem)
	{
		Ogre::ConfigOptionMap& CurrentRendererOptions = mRoot->getRenderSystem()->getConfigOptions();
		Ogre::ConfigOptionMap::iterator configItr = CurrentRendererOptions.begin();

		for (; configItr != CurrentRendererOptions.end(); configItr++)
		{
			if ((configItr)->first == "Floating-point mode")
			{
				rendersystem->setConfigOption("Floating-point mode", "Consistent");
				break;
			}
		}
	}

	//initialize render window
	try
	{
		mRoot->initialise(false);
		mRenderWindow = CreateRenderWindow();
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing render window\nDetails: " + e.getDescription());
	}

	//get renderer info
	Renderer = mRoot->getRenderSystem()->getCapabilities()->getRenderSystemName();

	//shorten name
	int loc = Renderer.find("Rendering Subsystem");
	Renderer = Renderer.substr(0, loc - 1);

	//load resource configuration
	Ogre::ConfigFile cf;
	try
	{
		cf.load("resources.cfg");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading resources.cfg\nDetails: " + e.getDescription());
	}
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	//add resource locations
	try
	{
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

		//add materials group, and autoload
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/materials", "FileSystem", "Materials", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Materials");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing resources\nDetails: " + e.getDescription());
	}

	//create scene manager
	try
	{
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating scene manager\nDetails: " + e.getDescription());
	}

#if OGRE_VERSION >= 0x00010900
	mSceneMgr->addRenderQueueListener(mOverlaySystem);
#endif

	//set ambient light
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	try
	{
		mCamera = mSceneMgr->createCamera("Main Camera");
		mViewport = mRenderWindow->addViewport(mCamera);
		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating camera and viewport\nDetails: " + e.getDescription());
	}

	//setup texture filtering
	int filtermode = GetConfigInt("Skyscraper.Frontend.TextureFilter", 3);
	int maxanisotropy = GetConfigInt("Skyscraper.Frontend.MaxAnisotropy", 8);

	if (filtermode < 0 || filtermode > 3)
		filtermode = 3;

	if (filtermode < 3)
		maxanisotropy = 1;

	Ogre::TextureFilterOptions filter;
	if (filtermode == 0)
		filter = Ogre::TFO_NONE;
	if (filtermode == 1)
		filter = Ogre::TFO_BILINEAR;
	if (filtermode == 2)
		filter = Ogre::TFO_TRILINEAR;
	if (filtermode == 3)
		filter = Ogre::TFO_ANISOTROPIC;

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(filter);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(maxanisotropy);

	//initialize FMOD (sound)
	DisableSound = GetConfigBool("Skyscraper.Frontend.DisableSound", false);
	if (DisableSound == false)
	{
		Report("\n FMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2016\n");

		FMOD_RESULT result = FMOD::System_Create(&soundsys);
		if (result != FMOD_OK)
		{
			ReportFatalError("Error initializing sound");
			DisableSound = true;
		}
		else
		{
			result = soundsys->init(100, FMOD_INIT_NORMAL, 0);
			if (result != FMOD_OK)
			{
				ReportFatalError("Error initializing sound");
				DisableSound = true;
			}
			else
			{
				//get FMOD version information
				unsigned int version;
				soundsys->getVersion(&version);
				int major = version >> 16;
				int minor = (version >> 8) & 255;
				int rev = version & 255;

				std::string name;
				if (major == 1)
					name = "FMOD Studio";
				else
					name = "FMOD Ex";
				Report("Sound initialized: " + name + " version " + ToString(major) + "." + ToString(minor) + "." + ToString(rev));
			}
		}
	}
	else
		Report("Sound Disabled");

	//load Caelum plugin
	if (GetConfigBool("Skyscraper.Frontend.Caelum", true) == true)
	{
		try
		{
			if (!Caelum::CaelumPlugin::getSingletonPtr())
				mRoot->installPlugin(new Caelum::CaelumPlugin());
		}
		catch (Ogre::Exception &e)
		{
			return ReportFatalError("Error initializing Caelum plugin:\nDetails: " + e.getDescription());
		}
	}

	//set platform name
	std::string arch;

#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_32
	arch = "32-bit";
#endif
#if OGRE_ARCH_TYPE == OGRE_ARCHITECTURE_64
	arch = "64-bit";
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	Platform = "Windows " + arch;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
	Platform = "Linux " + arch;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	Platform = "MacOS " + arch;
#endif

	return true;
}

void Skyscraper::GetInput(EngineContext *engine)
{
	SBS_PROFILE_MAIN("GetInput");

	if (!engine)
		return;

	//quit if main window isn't selected
	if (window->Active == false)
		return;

	static int wireframe;
	static bool wait, waitcheck, colliders, b_down, boxes;
	static unsigned int old_time;
	static int old_mouse_x, old_mouse_y;

	//get SBS instance
	::SBS::SBS *Simcore = engine->GetSystem();

	// First get elapsed time from the virtual clock.
	unsigned int current_time = Simcore->GetRunTime();

	Camera *camera = Simcore->camera;

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

	//fix for the camera velocities due to the non-event driven key system
	camera->desired_velocity = Ogre::Vector3(0, 0, 0);
	camera->desired_angle_velocity = Ogre::Vector3(0, 0, 0);

	//get old mouse coordinates
	old_mouse_x = Simcore->mouse_x;
	old_mouse_y = Simcore->mouse_y;

	//get mouse pointer coordinates
	Simcore->mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	Simcore->mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

	//get window dimensions
	float width = window->GetClientSize().GetWidth();
	float height = window->GetClientSize().GetHeight();

	//if mouse coordinates changed, and we're in freelook mode, rotate camera
	if (camera->Freelook == true && (old_mouse_x != Simcore->mouse_x || old_mouse_y != Simcore->mouse_y))
	{
		window->WarpPointer(width / 2, height / 2);
		Ogre::Vector3 rotational;
		rotational.x = camera->Freelook_speed * -((float)(Simcore->mouse_y - (height / 2))) / (height * 2);
		rotational.y = camera->Freelook_speed * -((width / 2) - (float)Simcore->mouse_x) / (width * 2);
		rotational.z = 0;
		camera->desired_angle_velocity = rotational;
		camera->angle_velocity = rotational;
	}

	//check if the user clicked on an object, and process it
	bool left = wxGetMouseState().LeftIsDown();
	bool right = wxGetMouseState().RightIsDown();
	if ((left == true || right == true) && MouseDown == false)
	{
		MouseDown = true;
		camera->MouseDown = MouseDown;
		camera->ClickedObject(wxGetKeyState(WXK_SHIFT), wxGetKeyState(WXK_CONTROL), wxGetKeyState(WXK_ALT), right);
	}

	//reset mouse state
	if (left == false && right == false)
	{
		MouseDown = false;
		camera->MouseDown = MouseDown;
	}

	if (wxGetKeyState(WXK_ESCAPE))
	{
		int result = wxMessageBox(wxT("Exit and return to the main menu?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		if (result == wxYES)
			Shutdown = true;
	}

	if (wxGetKeyState(WXK_F2) && wait == false)
	{
		float fps = Simcore->FPS;
		int batches = (int)mRoot->getRenderSystem()->_getBatchCount();
		int faces = (int)mRoot->getRenderSystem()->_getFaceCount();
		int vertices = (int)mRoot->getRenderSystem()->_getVertexCount();

		Report("FPS: " + ToString(Simcore->FPS) + " - Batches: " + ToString(batches) + " - Faces: " + ToString(faces) + " - Vertices: " + ToString(vertices));
		wait = true;
	}

	camera->speed = 1;
	float speed_normal = camera->cfg_speed;
	float speed_fast = camera->cfg_speedfast;
	float speed_slow = camera->cfg_speedslow;

	//crash test
	if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_ALT) && wxGetKeyState((wxKeyCode)'C'))
		throw;

	if (wxGetKeyState(WXK_CONTROL))
	{
		if (wxGetKeyState((wxKeyCode)'R'))
		{
			engine->Reload = true;
			return;
		}
		camera->speed = speed_slow;
	}
	else if (wxGetKeyState(WXK_SHIFT))
		camera->speed = speed_fast;

	if (wxGetKeyState(WXK_ALT))
	{
		//strafe movement
		if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'D'))
			camera->Strafe(speed_normal);
		if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'A'))
			camera->Strafe(-speed_normal);
		if (wxGetKeyState(WXK_UP) || wxGetKeyState((wxKeyCode)'W'))
			camera->Float(speed_normal);
		if (wxGetKeyState(WXK_DOWN) || wxGetKeyState((wxKeyCode)'S'))
			camera->Float(-speed_normal);
		if (wxGetKeyState(WXK_PAGEUP) || wxGetKeyState((wxKeyCode)'P'))
			camera->Spin(speed_normal);
		if (wxGetKeyState(WXK_PAGEDOWN) || wxGetKeyState((wxKeyCode)'L'))
			camera->Spin(-speed_normal);
	}
	else
	{
		if (wxGetKeyState((wxKeyCode)'V') && wait == false)
		{
			if (camera->GetGravityStatus() == false)
			{
				camera->EnableGravity(true);
				camera->EnableCollisions(true);
				Report("Gravity and collision detection on");
			}
			else
			{
				camera->EnableGravity(false);
				camera->EnableCollisions(false);
				Report("Gravity and collision detection off");
			}
			wait = true;
		}
		if (camera->Freelook == false)
		{
			if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'D'))
				camera->Turn(speed_normal);
			if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'A'))
				camera->Turn(-speed_normal);
		}
		else
		{
			if (wxGetKeyState(WXK_RIGHT) || wxGetKeyState((wxKeyCode)'D'))
				camera->Strafe(speed_normal);
			if (wxGetKeyState(WXK_LEFT) || wxGetKeyState((wxKeyCode)'A'))
				camera->Strafe(-speed_normal);
		}
		if (wxGetKeyState(WXK_PAGEUP) || wxGetKeyState((wxKeyCode)'P'))
			camera->Look(speed_normal);
		if (wxGetKeyState(WXK_PAGEDOWN) || wxGetKeyState((wxKeyCode)'L'))
			camera->Look(-speed_normal);
		if (wxGetKeyState(WXK_UP) || wxGetKeyState((wxKeyCode)'W'))
			camera->Step(speed_normal);
		if (wxGetKeyState(WXK_DOWN) || wxGetKeyState((wxKeyCode)'S'))
			camera->Step(-speed_normal);
		if (wxGetKeyState(WXK_SPACE) && wait == false)
		{
			if (camera->IsOnGround() == true)
			{
				camera->Jump();
				wait = true;
			}
		}
		if (wxGetKeyState(WXK_F3))
		{
			//reset rotation/direction and FOV of camera
			camera->ResetView();
		}
		if (wxGetKeyState(WXK_F6))
		{
			//reset camera position and state
			camera->ResetState();
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
			//toggle wireframe mode
			if (wireframe == 0)
			{
				Simcore->EnableSkybox(false);
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
				Simcore->EnableSkybox(true);
				camera->SetViewMode(0);
				wireframe = 0;
			}
			wait = true;
		}
		if (wxGetKeyState(WXK_F11) && wait == false)
		{
			mRenderWindow->writeContentsToTimestampedFile("screenshots/skyscraper-", ".jpg");
			wait = true;
		}
		if (wxGetKeyState(WXK_F12) && !dpanel)
		{
			//show control panel if closed
			dpanel = new DebugPanel(this, NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}
		if (wxGetKeyState(WXK_F5) && wait == false)
		{
			//toggle freelook mode
			camera->Freelook = !camera->Freelook;
			if (camera->Freelook == true)
				window->SetCursor(wxCURSOR_CROSS);
			else
				window->SetCursor(wxNullCursor);
			wait = true;
		}
		if (wxGetKeyState(WXK_F10) && wait == false)
		{
			//toggle fullscreen mode
			SetFullScreen(!FullScreen);
			wait = true;
		}
		if (wxGetKeyState(WXK_F8) && wait == false)
		{
			//show mesh bounding boxes
			Simcore->ShowBoundingBoxes(!boxes);
			boxes = !boxes;
			wait = true;
		}

		if (wxGetKeyState(WXK_NUMPAD_SUBTRACT) || wxGetKeyState((wxKeyCode)'['))
		{
			//increase FOV angle
			float angle = camera->GetFOVAngle() + camera->cfg_zoomspeed;
			camera->SetFOVAngle(angle);
		}
		if (wxGetKeyState(WXK_NUMPAD_ADD) || wxGetKeyState((wxKeyCode)']'))
		{
			//decrease FOV angle
			float angle = camera->GetFOVAngle() - camera->cfg_zoomspeed;
			camera->SetFOVAngle(angle);
		}

		//binoculars
		if (wxGetKeyState((wxKeyCode)'B') && b_down == false)
		{
			camera->Binoculars(true);
			b_down = true;
		}
		else if (!wxGetKeyState((wxKeyCode)'B') && b_down == true)
		{
			camera->Binoculars(false);
			b_down = false;
		}

		//model pick-up
		if (wxGetKeyState((wxKeyCode)'C') && wait == false)
		{
			if (camera->IsModelAttached() == false)
				camera->PickUpModel();
			else
				camera->DropModel();
			wait = true;
		}

		//load a new additional building
		if (wxGetKeyState((wxKeyCode)';') && wait == false)
		{
			if (!loaddialog)
				loaddialog = new LoadDialog(dpanel, window, -1);
			loaddialog->CenterOnScreen();
			loaddialog->Show();
			wait = true;
			return;
		}

		//temporary engine selection test
		if (wxGetKeyState((wxKeyCode)'1'))
		{
			SetActiveEngine(0);
		}
		if (wxGetKeyState((wxKeyCode)'2'))
		{
			SetActiveEngine(1);
		}
		if (wxGetKeyState((wxKeyCode)'3'))
		{
			SetActiveEngine(2);
		}
		if (wxGetKeyState((wxKeyCode)'4'))
		{
			SetActiveEngine(3);
		}
		if (wxGetKeyState((wxKeyCode)'5'))
		{
			SetActiveEngine(4);
		}
		if (wxGetKeyState((wxKeyCode)'6'))
		{
			SetActiveEngine(5);
		}
		if (wxGetKeyState((wxKeyCode)'7'))
		{
			SetActiveEngine(6);
		}
		if (wxGetKeyState((wxKeyCode)'8'))
		{
			SetActiveEngine(7);
		}
		if (wxGetKeyState((wxKeyCode)'9'))
		{
			SetActiveEngine(8);
		}
		if (wxGetKeyState((wxKeyCode)'0'))
		{
			SetActiveEngine(9);
		}

		//values from old version
		if (wxGetKeyState(WXK_HOME) || wxGetKeyState((wxKeyCode)'O'))
			camera->Float(speed_normal);
		if (wxGetKeyState(WXK_END) || wxGetKeyState((wxKeyCode)'K'))
			camera->Float(-speed_normal);
	}
}

void Skyscraper::Report(const std::string &message)
{
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(message);
	}
	catch (Ogre::Exception &e)
	{
		ShowError("Error writing message to log\n" + e.getDescription());
	}
}

bool Skyscraper::ReportError(const std::string &message)
{
	try
	{
		if (Ogre::LogManager::getSingletonPtr())
			Ogre::LogManager::getSingleton().logMessage(message, Ogre::LML_CRITICAL);
	}
	catch (Ogre::Exception &e)
	{
		ShowError("Error writing message to log\n" + e.getDescription());
	}
	return false;
}

bool Skyscraper::ReportFatalError(const std::string &message)
{
	ReportError(message);
	ShowError(message);
	return false;
}

void Skyscraper::ShowError(const std::string &message)
{
	//show error dialog
	wxMessageDialog dialog(0, wxString::FromAscii(message.c_str()), _("Skyscraper"), wxOK | wxICON_ERROR);
	dialog.ShowModal();
}

void Skyscraper::Loop()
{
	//Main simulator loop

	ProfileManager::Reset();
	ProfileManager::Increment_Frame_Counter();

	//main menu routine
	if (StartupRunning == true)
	{
		DrawBackground();
		GetMenuInput();
		Render();
		return;
	}

	//run sim engine instances
	bool result = RunEngines();

	//delete an engine if requested
	HandleEngineShutdown();

	//exit if full shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		//if showmenu is true, unload simulator and return to main menu
		if (GetConfigBool("Skyscraper.Frontend.ShowMenu", true) == true)
			UnloadToMenu();
		//otherwise exit app
		else
			Quit();
	}

	if (result == false && (ConcurrentLoads == false || GetEngineCount() == 1))
		return;

	if (!active_engine)
		return;

	//make sure active engine is the one the camera is active in
	if (active_engine->IsCameraActive() == false)
		active_engine = FindActiveEngine();

	//exit if active engine is loading
	if (active_engine->IsLoading() == true)
		return;

	//update Caelum
	if (mCaelumSystem)
	{
		mCaelumSystem->notifyCameraChanged(mCamera);
		mCaelumSystem->setTimeScale(SkyMult);
		mCaelumSystem->updateSubcomponents(float(active_engine->GetSystem()->GetElapsedTime()) / 1000);
	}

	//render graphics
	Render();

	//handle a building reload
	HandleReload();

	//handle behavior when a user exits an engine area
	SwitchEngines();

	//ProfileManager::dumpAll();
}

void Skyscraper::DrawBackground()
{
	//draw menu background

	DrawImage("data/" + GetConfigString("Skyscraper.Frontend.Menu.Image", "menu.png"), 0, -1, -1, false);

	if (buttoncount == 0)
	{
		buttoncount = GetConfigInt("Skyscraper.Frontend.Menu.Buttons", 5);
		buttons = new buttondata[buttoncount];

		for (int i = 0; i < buttoncount; i++)
		{
			buttons[i].node = 0;
			buttons[i].drawn_selected = false;
			buttons[i].drawn_pressed = false;
			buttons[i].active_button = 0;
			buttons[i].rect = 0;
		}
	}

	for (int i = 0; i < buttoncount; i++)
	{
		std::string b1, b2, b3;
		float x = 0, y = 0;
		bool center = false;
		std::string number = ToString(i + 1);

		if (i == 0)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button1.Image", "button_triton.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button1.Selected", "button_triton_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button1.Pressed", "button_triton_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button1.X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button1.Y", -0.08f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button1.Center", true);
		}
		if (i == 1)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Image", "button_glasstower.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Selected", "button_glasstower_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Pressed", "button_glasstower_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button2.X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button2.Y", 0.125f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button2.Center", true);
		}
		if (i == 2)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Image", "button_searstower.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Selected", "button_searstower_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Pressed", "button_searstower_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button3.X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button3.Y", 0.333f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button3.Center", true);
		}
		if (i == 3)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Image", "button_simple.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Selected", "button_simple_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Pressed", "button_simple_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button4.X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button4.Y", 0.541f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button4.Center", true);
		}
		if (i == 4)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Image", "button_other.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Selected", "button_other_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Pressed", "button_other_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button5.X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button5.Y", 0.75f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button5.Center", true);
		}
		if (i > 4)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Image", "");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Selected", "");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Pressed", "");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button" + number + ".X", 0.0f);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button" + number + ".Y", 0.0f);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button" + number + ".Center", true);
		}

		DrawImage(b1, &buttons[i], x, y, center, b2, b3);
	}
}

void Skyscraper::DrawImage(const std::string &filename, buttondata *button, float x, float y, bool center, const std::string &filename_selected, const std::string &filename_pressed)
{
	//X and Y represent the image's top-left location.
	//values are -1 for the top left, 1 for the top right, -1 for the top, and 1 for the bottom
	//center is at 0, 0

	float w, h;
	int w_orig = 0, h_orig = 0, w2 = 0, h2 = 0;
	bool background = false;

	std::string material = "";
	std::string Filename = filename;

	if (filename == "")
		return;

	//exit if background has already been drawn
	if (background_image == Filename)
		return;

	Ogre::TextureManager::getSingleton().setVerbose(false);
	Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(Filename);
	if (tex.isNull() == false)
		material = Filename;

	//load image data from file, if not already preloaded
	if (material == "")
	{
		int count = 1;
		if (button)
			count = 3;

		for (int i = 0; i < count; i++)
		{
			if (i == 0)
				Filename = filename;
			if (i == 1)
				Filename = filename_selected;
			if (i == 2)
				Filename = filename_pressed;

			//create new material
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create(Filename, "General");

			//load image data from file
			Ogre::Image img;
			try
			{
				img.load(Filename, "General");
			}
			catch (Ogre::Exception &e)
			{
				ReportError("Error loading texture " + Filename + "\n" + e.getDescription());
				return;
			}

			w_orig = img.getWidth();
			h_orig = img.getHeight();

			//round up image size to power-of-2 value
			w2 = powf(2, ceilf(Log2((float)w_orig)));
			h2 = powf(2, ceilf(Log2((float)h_orig)));

			//create texture and blit image onto it - this solves texture quality issues on mainly D3D9
			tex = Ogre::TextureManager::getSingleton().createManual(Filename, "General", Ogre::TEX_TYPE_2D, w2, h2, 0, Ogre::PF_R8G8B8A8, Ogre::TU_STATIC);
			tex->getBuffer(0, 0)->blitFromMemory(img.getPixelBox(0, 0), Ogre::Box(0, 0, 0, w_orig, h_orig, img.getDepth()));

			//bind texture to material and set options
			Ogre::TextureUnitState *state = mat->getTechnique(0)->getPass(0)->createTextureUnitState(Filename);
			Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);
			pass->setDepthCheckEnabled(false);
			pass->setDepthWriteEnabled(false);
			pass->setLightingEnabled(false);
			pass->setTextureFiltering(Ogre::TFO_NONE);

			//rescale texture
			state->setTextureScale((float)w2 / (float)w_orig, (float)h2 / (float)h_orig);
			state->setTextureScroll(-(float(w2 - w_orig) / (float)w2) / 2.0f, -(float(h2 - h_orig) / (float)h2) / 2.0f);

			if (tex->hasAlpha() == true && button)
			{
				mat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				mat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
			}
		}
		material = Filename;
		if (button)
		{
			button->node = 0;
			button->drawn_selected = false;
			button->drawn_pressed = false;
			button->active_button = 0;
			button->rect = 0;
		}
		else
		{
			background_rect = 0;
			background_node = 0;
		}
	}

	//exit if requested button is already visible
	if (button)
	{
		if (button->drawn_selected == false && button->drawn_pressed == false)
		{
			if (button->active_button == 1)
				return;
			else
			{
				button->active_button = 1;
				material = filename;
			}
		}

		if (button->drawn_selected == true)
		{
			if (button->active_button == 2)
				return;
			else
			{
				button->active_button = 2;
				material = filename_selected;
			}
		}

		if (button->drawn_pressed == true)
		{
			if (button->active_button == 3)
				return;
			else
			{
				button->active_button = 3;
				material = filename_pressed;
			}
		}
	}

	//set values and draw button
	if (material != "")
	{
		w = w_orig / (mRenderWindow->getWidth() / 2.0);
		h = h_orig / (mRenderWindow->getHeight() / 2.0);
		if (button)
		{
			//delete previous object
			if (button->node)
				button->node->detachAllObjects();
			if (button->rect)
				delete button->rect;
			button->rect = 0;

			if (button->filename == "")
			{
				//store general button data
				button->filename = filename;
				button->filename_selected = filename_selected;
				button->filename_pressed = filename_pressed;

				button->offset_x = x;
				button->offset_y = y;
				if (center == true)
				{
					button->x = x - (w / 2);
					button->y = y - (h / 2);
				}
				else
				{
					button->x = x;
					button->y = y;
				}
				button->size_x = w;
				button->size_y = h;
			}

			x = button->x;
			y = button->y;
			w = button->size_x;
			h = button->size_y;
		}
		else
		{
			background_image = material;
			background = true;
			if (center == true)
			{
				x += -(w / 2);
				y += -(h / 2);
			}
		}

		//create rectangle
		Ogre::Rectangle2D* rect = new Ogre::Rectangle2D(true);
		rect->setCorners(x, -y, x + w, -(y + h));
		rect->setMaterial(material);
		if (background == true)
			rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

		//set infinite bounding box
		Ogre::AxisAlignedBox aabInf;
		aabInf.setInfinite();
		rect->setBoundingBox(aabInf);

		//attach scene node
		Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		node->attachObject(rect);
		if (button)
		{
			button->node = node;
			button->rect = rect;
		}
		else
		{
			background_node = node;
			background_rect = rect;
		}
	}
}

void Skyscraper::GetMenuInput()
{
	//input handler for main menu

	//exit if there aren't any buttons
	if (!buttons || buttoncount == 0)
		return;

	//get mouse coordinates
	int mouse_x = window->ScreenToClient(wxGetMousePosition()).x;
	int mouse_y = window->ScreenToClient(wxGetMousePosition()).y;

	for (int i = 0; i < buttoncount; i++)
	{
		buttondata *button = &buttons[i];

	    //only process buttons if main window is selected
        if (window->Active != false)
        {
			float mx = mouse_x;
			float my = mouse_y;
			float w = mx / window->GetClientSize().x;
			float h = my / window->GetClientSize().y;
			float mouse_x_rel = (w * 2) - 1;
			float mouse_y_rel = (h * 2) - 1;

        	//change button status based on mouse position and button press status
        	if (mouse_x_rel > button->x && mouse_x_rel < button->x + button->size_x && mouse_y_rel > button->y && mouse_y_rel < button->y + button->size_y)
        	{
        		if (button->drawn_selected == false && wxGetMouseState().LeftIsDown() == false)
        		{
        			if (button->drawn_pressed == true)
        			{
        				//user clicked on button
        				button->drawn_selected = true;
        				Click(i);
        				return;
        			}
        			button->drawn_selected = true;
        		}
        		if (button->drawn_pressed == false && wxGetMouseState().LeftIsDown() == true)
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

	std::string number = ToString(index + 1);
	std::string filename = "";

	if (index == 0)
		filename = GetConfigString("Skyscraper.Frontend.Menu.Button1.File", "Triton Center.bld");
	if (index == 1)
		filename = GetConfigString("Skyscraper.Frontend.Menu.Button2.File", "Glass Tower.bld");
	if (index == 2)
		filename = GetConfigString("Skyscraper.Frontend.Menu.Button3.File", "Sears Tower.bld");
	if (index == 3)
		filename = GetConfigString("Skyscraper.Frontend.Menu.Button4.File", "Simple.bld");
	if (index > 3)
		filename = GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".File", "");

	if (filename == "")
	{
		//show file selection dialog
		filename = SelectBuilding();
	}

	if (filename != "")
	{
		DeleteButtons();
		Load(filename);
	}
}

void Skyscraper::DeleteButtons()
{
	if (buttoncount > 0)
	{
		for (int i = 0; i < buttoncount; i++)
		{
			buttondata *button = &buttons[i];

			if (button->node)
			{
				button->node->detachAllObjects();
				button->node->getParent()->removeChild(button->node);
				button->node = 0;
			}
			if (button->rect)
				delete button->rect;
			button->rect = 0;
		}
		delete [] buttons;
		buttoncount = 0;
	}
	buttons = 0;

	if (background_node)
	{
		background_node->detachAllObjects();
		background_node->getParent()->removeChild(background_node);
		background_node = 0;
	}
	if (background_rect)
		delete background_rect;
	background_rect = 0;
	background_image = "";
}

void Skyscraper::StartSound()
{
	//load and start background music

	if (DisableSound == true)
		return;

	if (GetConfigBool("Skyscraper.Frontend.IntroMusic", true) == false)
		return;

	std::string filename = GetConfigString("Skyscraper.Frontend.IntroMusicFile", "intro.ogg");
	std::string filename_full = "data/" + filename;

	//load new sound
#if (FMOD_VERSION >> 16 == 4)
		FMOD_RESULT result = soundsys->createSound(filename_full.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_SOFTWARE | FMOD_LOOP_NORMAL), 0, &sound);
#else
		FMOD_RESULT result = soundsys->createSound(filename_full.c_str(), (FMOD_MODE)(FMOD_2D | FMOD_ACCURATETIME | FMOD_LOOP_NORMAL), 0, &sound);
#endif
	if (result != FMOD_OK)
	{
		ReportError("Can't load file '" + filename_full + "'");
		return;
	}

#if (FMOD_VERSION >> 16 == 4)
	result = soundsys->playSound(FMOD_CHANNEL_FREE, sound, true, &channel);
#else
	result = soundsys->playSound(sound, 0, true, &channel);
#endif

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
	if (channel)
		channel->stop();
	if (sound)
		sound->release();
	sound = 0;
}

std::string Skyscraper::SelectBuilding()
{
	//choose a building from a script file

	std::string filename = "";

	//get listing of building files
	wxArrayString filelist;
	wxDir::GetAllFiles(_("buildings/"), &filelist, _("*.bld"), wxDIR_FILES);

	//strip directory name and extension from entries
	for (int i = 0; i < (int)filelist.size(); i++)
	{
		filelist[i] = filelist[i].substr(10);
		filelist[i] = filelist[i].substr(0, filelist[i].length() - 4);
	}

	//sort list
	filelist.Sort();

	//show selection dialog window
	wxSingleChoiceDialog Selector (0, _("Select a Building"), _("Load Building"), filelist);

	if (Selector.ShowModal() == wxID_OK)
	{
		filename = Selector.GetStringSelection();
		filename += ".bld";
	}

	return filename;
}

bool Skyscraper::Load(const std::string &filename, EngineContext *parent, const Ogre::Vector3 &position, float rotation, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max)
{
	//load simulator and data file

	StartupRunning = false;

	//exit if no building specified
	if (filename == "")
		return false;

	//clear scene
	if (GetEngineCount() == 0)
		mSceneMgr->clearScene();

	//clear screen
	mRenderWindow->update();

	//set parent to master engine, if not set
	if (parent == 0 && GetEngineCount() >= 1)
		parent = GetFirstValidEngine();

	//Create simulator instance
	EngineContext* engine = CreateEngine(parent, position, rotation, area_min, area_max);

	if (!active_engine)
		active_engine = engine;

	//have instance load building
	bool result = engine->Load(filename);

	if (result == false)
	{
		if (GetEngineCount() == 1)
			UnloadToMenu();
		else
			DeleteEngine(engine);
		return false;
	}

	//override SBS startup render option, if specified
	if (RenderOnStartup == true)
		engine->GetSystem()->RenderOnStartup = true;

	return true;
}

bool Skyscraper::Start(EngineContext *engine)
{
	//start simulator

	if (!engine)
		return false;

	::SBS::SBS *Simcore = engine->GetSystem();

	if (engine == active_engine)
	{
		//the sky needs to be created before Prepare() is called
		bool sky_result = false;
		if (GetConfigBool("Skyscraper.Frontend.Caelum", true) == true)
			sky_result = InitSky(engine);

		//create old sky if Caelum is turned off, or failed to initialize
		if (sky_result == false)
			Simcore->CreateSky();

		//switch to fullscreen mode if specified
		if (GetConfigBool("Skyscraper.Frontend.FullScreen", false) == true)
			SetFullScreen(true);

		//resize main window
		if (FullScreen == false)
		{
			window->SetBackgroundColour(*wxBLACK);
			window->SetClientSize(GetConfigInt("Skyscraper.Frontend.ScreenWidth", 800), GetConfigInt("Skyscraper.Frontend.ScreenHeight", 600));
			window->Center();
		}
	}

	//start simulation
	if (!engine->Start(mCamera))
		return false;

	//close progress dialog if no engines are loading
	if (IsEngineLoading() == false)
		CloseProgressDialog();

	//load control panel
	if (engine == active_engine)
	{
		if (GetConfigBool("Skyscraper.Frontend.ShowControlPanel", true) == true)
		{
			if (!dpanel)
				dpanel = new DebugPanel(this, NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}
	}

	RefreshViewport();

	//run simulation
	Report("Running simulation...");
	StopSound();
	if (console)
		console->bSend->Enable(true);
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

void Skyscraper::UnloadToMenu()
{
	//unload to main menu

	Pause = false;
	UnloadSim();

	//cleanup sound
	StopSound();

	CloseProgressDialog();

	//return to main menu
	SetFullScreen(false);
	window->SetClientSize(GetConfigInt("Skyscraper.Frontend.Menu.Width", 640), GetConfigInt("Skyscraper.Frontend.Menu.Height", 480));
	window->Center();

	ConcurrentLoads = false;
	RenderOnStartup = false;

	StartSound();
	StartupRunning = true;
}

void Skyscraper::Quit()
{
	//exit app
	if(dpanel)
		dpanel->EnableTimer(false);

	wxGetApp().Exit();
}

Ogre::RenderWindow* Skyscraper::CreateRenderWindow(const Ogre::NameValuePairList* miscParams, const std::string& windowName)
{
	std::string name = windowName;

	int width, height;
	window->GetClientSize(&width, &height);

	Ogre::NameValuePairList params;
	if (miscParams)
		params = *miscParams;

	bool vsync = GetConfigBool("Skyscraper.Frontend.Vsync", true);
	if (vsync == true)
		params["vsync"] = "true";
	else
		params["vsync"] = "false";
	params["vsyncInterval"] = "1";
	params["externalWindowHandle"] = getOgreHandle();

#if defined(__WXMAC__)
	params["macAPI"] = "cocoa";
	params["macAPICocoaUseNSView"] = "true";
#endif

	//create the render window
	mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(name, width, height, false, &params);

	mRenderWindow->setActive(true);
	mRenderWindow->windowMovedOrResized();

	return mRenderWindow;
}

void Skyscraper::destroyRenderWindow()
{
	if (mRenderWindow)
	   Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);

	mRenderWindow->destroy();
	mRenderWindow = 0;
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
	GdkWindow* gdkWin = gtk_widget_get_window((GtkWidget*)window->GetHandle());
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
	return Ogre::StringConverter::toString((size_t)window->GetHandle());

#else
	#error Not supported on this platform!
#endif
}

int Skyscraper::GetConfigInt(const std::string &key, int default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, ToString(default_value));
	return ToInt(result);
}

std::string Skyscraper::GetConfigString(const std::string &key, const std::string &default_value)
{
	return configfile.getSetting(key, Ogre::StringUtil::BLANK, default_value);
}

bool Skyscraper::GetConfigBool(const std::string &key, bool default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, BoolToString(default_value));
	return ToBool(result);
}

float Skyscraper::GetConfigFloat(const std::string &key, float default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, ToString(default_value));
	return ToFloat(result);
}

bool Skyscraper::InitSky(EngineContext *engine)
{
	//initialize sky

	if (!engine)
		return false;

	//ensure graphics card and render system are capable of Caelum's shaders
	if (Renderer == "Direct3D9")
	{
		//on DirectX, Caelum requires a card capable of 3.0 shader levels, which would be
		//an ATI Radeon HD 2000, nVidia Geforce 6, Intel G965 or newer
		//Intel cards: http://www.intel.com/support/graphics/sb/cs-014257.htm
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//for general sky, require both DirectX pixel and vertex shaders 2.0
		if (profiles.find("ps_2_0") == profiles.end() ||
			profiles.find("vs_2_0") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 2.0 shaders not supported");

		//for clouds, require either DirectX pixel shaders 3.0 or nVidia fragment shaders 4.0
		if (profiles.find("ps_3_0") == profiles.end() &&
			profiles.find("fp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 fragment shaders not supported");

		//for clouds, require either DirectX vetex shaders 3.0 or nVidia vertex shaders 4.0
		if (profiles.find("vs_3_0") == profiles.end() &&
			profiles.find("vp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 vertex shaders not supported");
	}

	if (Renderer == "OpenGL")
	{
		//on OpenGL, Caelum requires hardware support for shaders (OpenGL 2.0 or newer)
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//require OpenGL ARB fragment programs
		if (profiles.find("arbfp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: fragment programs not supported");

		//require OpenGL ARB vertex programs
		if (profiles.find("arbvp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: vertex programs not supported");
	}

	//load Caelum resources
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/caelum", "FileSystem", "Caelum", false);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Caelum");

		if (!mCaelumSystem)
			mCaelumSystem = new Caelum::CaelumSystem(mRoot, mSceneMgr, Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);
		Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript(mCaelumSystem, SkyName);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error initializing Caelum:\nDetails: " + e.getDescription());
	}
	catch (...)
	{
		ReportFatalError("Error initializing Caelum");
	}

	if (!mCaelumSystem)
		return false;

	//attach caelum to running viewport
	try
	{
		mCaelumSystem->attachViewport(mViewport);
		mCaelumSystem->setAutoNotifyCameraChanged(false);
		mCaelumSystem->setSceneFogDensityMultiplier(GetConfigFloat("Skyscraper.Frontend.FogMultiplier", 0.1f) / 1000);
		if (GetConfigBool("Skyscraper.Frontend.EnableFog", true) == false)
			mCaelumSystem->setManageSceneFog(Ogre::FOG_NONE);
		mCaelumSystem->setManageAmbientLight(GetConfigBool("Skyscraper.Frontend.ModifyAmbient", false));

		//fix sky rotation
		Ogre::Quaternion rot(Ogre::Degree(180.0f), Ogre::Vector3::UNIT_Y);
		mCaelumSystem->getCaelumGroundNode()->setOrientation(rot);
		mCaelumSystem->getCaelumCameraNode()->setOrientation(rot);

		//have sky use SBS scaling factor
		float scale = 1 / engine->GetSystem()->UnitScale;
		mCaelumSystem->getCaelumGroundNode()->setScale(scale, scale, scale);
		mCaelumSystem->getCaelumCameraNode()->setScale(scale, scale, scale);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error setting Caelum parameters:\nDetails: " + e.getDescription());
	}
	catch (...)
	{
		ReportFatalError("Error setting Caelum parameters");
	}

	//set sky time multiplier if not already set
	if (SkyMult == 0)
		SkyMult = mCaelumSystem->getTimeScale();

	//set location if specified
	if (new_location == true)
	{
		mCaelumSystem->setObserverLatitude(Ogre::Degree(latitude));
		mCaelumSystem->setObserverLongitude(Ogre::Degree(longitude));
		new_location = false;
	}

	//set date/time if specified
	if (new_time == true)
	{
		mCaelumSystem->setJulianDay(datetime);
		new_time = false;
	}

	return true;
}

void Skyscraper::messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage)
{
	//callback function that receives OGRE log messages
	if (console)
	{
		console->Write(message);
		console->Update();
	}
}

void Skyscraper::ShowConsole(bool send_button)
{
	if (!console)
		console = new Console(this, NULL, -1);
	console->Show();
	console->Raise();
	console->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ConsoleX", 10), GetConfigInt("Skyscraper.Frontend.ConsoleY", 25)));
	console->bSend->Enable(send_button);
}

void Skyscraper::CreateProgressDialog(const std::string &message)
{
	//don't create progress dialog if concurrent loading is enabled, and one engine is already running
	if (GetEngineCount() > 1 && ConcurrentLoads == true)
	{
		if (GetFirstValidEngine()->IsRunning() == true)
			return;
	}

	if (!progdialog)
		progdialog = new wxProgressDialog(wxT("Loading..."), wxString::FromAscii(message.c_str()), 100, window);
	else
	{
		wxString msg = progdialog->GetMessage();
		msg += "\n";
		msg += message.c_str();
		progdialog->Update(progdialog->GetValue(), msg);
	}

	//stop control panel timer
	if (dpanel)
		dpanel->EnableTimer(false);
}

void Skyscraper::CloseProgressDialog()
{
	//close progress dialog
	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	//start control panel timer
	if (dpanel)
		dpanel->EnableTimer(true);
}

void Skyscraper::UpdateProgress()
{
	if (!progdialog)
		return;

	int total_percent = GetEngineCount() * 100;
	int current_percent = 0;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
			current_percent += engines[i]->GetProgress();
	}

	int final = ((float)current_percent / (float)total_percent) * 100;
	progdialog->Update(final);
}

void Skyscraper::SetFullScreen(bool enabled)
{
	//enable/disable fullscreen

	FullScreen = enabled;
	window->ShowFullScreen(FullScreen);
}

void Skyscraper::SetLocation(float latitude, float longitude)
{
	this->latitude = latitude;
	this->longitude = longitude;
	new_location = true;
}

void Skyscraper::SetDateTime(double julian_date_time)
{
	datetime = julian_date_time;
	new_time = true;
}

EngineContext* Skyscraper::CreateEngine(EngineContext *parent, const Ogre::Vector3 &position, float rotation, const Ogre::Vector3 &area_min, const Ogre::Vector3 &area_max)
{
	EngineContext* engine = new EngineContext(parent, this, mSceneMgr, soundsys, position, rotation, area_min, area_max);
	return engine;
}

bool Skyscraper::DeleteEngine(EngineContext *engine)
{
	//delete a specified sim engine instance

	if (!engine)
		return false;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i] == engine)
		{
			engines[i] = 0;
			delete engine;

			int count = GetEngineCount();

			if (active_engine == engine)
			{
				active_engine = 0;
				if (count > 0)
				{
					int number = GetFirstValidEngine()->GetNumber();
					SetActiveEngine(number);
				}
			}
			else if (active_engine)
				active_engine->RefreshCamera();

			//exit to main menu if all engines have been deleted
			if (count == 0)
				Shutdown = true;

			return true;
		}
	}
	return false;
}

void Skyscraper::DeleteEngines()
{
	//delete all sim emgine instances

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
			delete engines[i];
	}
	engines.clear();
	active_engine = 0;
}

EngineContext* Skyscraper::FindActiveEngine()
{
	//find engine instance with an active camera

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->IsCameraActive() == true)
				return engines[i];
		}
	}
	return active_engine;
}

void Skyscraper::SetActiveEngine(int number, bool switch_engines)
{
	//set an engine instance to be active

	EngineContext *engine = GetEngine(number);

	if (!engine)
		return;

	if (active_engine == engine)
		return;

	//don't switch if the camera's already active in the specified engine
	if (engine->IsCameraActive() == true)
		return;

	//don't switch to engine if it's loading
	if (engine->IsLoading() == true)
		return;

	CameraState state;
	bool state_set = false;

	if (active_engine)
	{
		//get previous engine's camera state
		if (switch_engines == true)
		{
			state = active_engine->GetCameraState();
			state_set = true;
		}

		//detach camera from current engine
		active_engine->DetachCamera(switch_engines);
	}

	Report("Setting engine " + ToString(number) + " as active");

	//switch context to new engine instance
	active_engine = engine;
	active_engine->AttachCamera(mCamera, !switch_engines);

	//apply camera state to new engine
	if (switch_engines == true && state_set == true)
		active_engine->SetCameraState(state, false);
}

bool Skyscraper::RunEngines()
{
	bool result = true;
	bool isloading = IsEngineLoading();

	if (ConcurrentLoads == true && isloading == true)
		RefreshViewport();

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (!engines[i])
			continue;

		//process engine run loops, and also prevent other instances from running if
		//one or more engines are loading
		if (ConcurrentLoads == true || isloading == false || engines[i]->IsLoading() == true)
		{
			bool run = true;
			if (i > 0 && ConcurrentLoads == false)
			{
				//if concurrent loads is off, skip running if previous engine is not finished loading
				if (engines[i - 1])
				{
					if (engines[i - 1]->IsLoading() == true && engines[i - 1]->IsLoadingFinished() == false)
						run = false;
				}
			}

			if (engines[i]->IsLoadingFinished() == false && run == true)
			{
				if (engines[i]->Run() == false)
					result = false;
			}
		}

		//start engine if loading is finished
		if (engines[i]->IsLoadingFinished() == true)
		{
			if (active_engine)
			{
				//exit if active engine is still loading
				if (active_engine->IsLoading() == true && active_engine->IsLoadingFinished() == false)
					continue;

				//exit if active engine is finished, but other buildings are still loading
				if (active_engine->IsLoadingFinished() == true && isloading == true)
					continue;
			}
			Start(engines[i]);
		}
	}
	return result;
}

bool Skyscraper::IsEngineLoading()
{
	//return true if an engine is loading

	bool result = false;
	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->IsLoading() == true && engines[i]->IsLoadingFinished() == false)
				result = true;
		}
	}
	return result;
}

void Skyscraper::HandleEngineShutdown()
{
	bool deleted = false;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->GetShutdownState() == true)
			{
				if (DeleteEngine(engines[i]) == true)
				{
					RefreshViewport();
					i--;
					deleted = true;
				}
			}
		}
	}

	//clean up empty engine slots at the end of the list
	if (deleted == true)
	{
		for (int i = (int)engines.size() - 1; i >= 0; i--)
		{
			if (!engines[i])
				engines.erase(engines.begin() + i);
			else
				break;
		}
	}
}

void Skyscraper::HandleReload()
{
	//reload building if requested

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->Reload == true)
			{
				Pause = false;
				engines[i]->DoReload();
			}
		}
	}
}

EngineContext* Skyscraper::GetEngine(int number)
{
	//get an engine by instance number

	if (number < 0 || number >= (int)engines.size())
		return 0;

	return engines[number];
}

int Skyscraper::GetEngineCount()
{
	//get number of valid engines

	int count = 0;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
			count++;
	}
	return count;
}

void Skyscraper::RaiseWindow()
{
	window->Raise();
}

void Skyscraper::RefreshConsole()
{
	if (console)
	{
		console->Refresh();
		console->Update();
	}
}

void Skyscraper::RefreshViewport()
{
	//refresh viewport to prevent rendering issues
	mViewport->_updateDimensions();
}

void Skyscraper::SwitchEngines()
{
	//if user is no longer inside the active engine, find a new engine to attach to

	if (!active_engine)
		return;

	if (active_engine->IsInside() == true)
		return;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i] != active_engine && engines[i])
		{
			if (engines[i]->IsInside() == true && engines[i]->IsCameraActive() == false)
			{
				SetActiveEngine(i, true);
				return;
			}
		}
	}

	//if user has stepped outside of all sim engines, revert the movement
	//to place them back into the active engine
	active_engine->RevertMovement();
}

bool Skyscraper::IsValidEngine(EngineContext *engine)
{
	if (!engine)
		return false;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i] == engine)
			return true;
	}
	return false;
}

bool Skyscraper::IsValidSystem(::SBS::SBS *sbs)
{
	if (!sbs)
		return false;

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
		{
			if (engines[i]->GetSystem() == sbs)
				return true;
		}
	}
	return false;
}

int Skyscraper::GetFreeInstanceNumber()
{
	//get an available engine instance number

	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (!engines[i])
			return i;
	}
	return (int)engines.size();
}

int Skyscraper::RegisterEngine(EngineContext *engine)
{
	//register an engine with the frontend, returning the assigned instance number

	int number = GetFreeInstanceNumber();

	if (number < (int)engines.size())
		engines[number] = engine;
	else
		engines.push_back(engine);

	return number;
}

EngineContext* Skyscraper::GetFirstValidEngine()
{
	for (int i = 0; i < (int)engines.size(); i++)
	{
		if (engines[i])
			return engines[i];
	}
	return 0;
}

}
