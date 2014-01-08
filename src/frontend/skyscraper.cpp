/* $Id$ */

/*
	Skyscraper 1.9 Alpha - Simulation Frontend
	Copyright (C)2003-2014 Ryan Thoryk
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

#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "unix.h"
#include "revmain.h"

SBS *Simcore;
Skyscraper *skyscraper;

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
	Skyscraper skyscraper;
	skyscraper.OnInit();
	if ((skyscraper.IsRunning == true && skyscraper.Pause == false) || skyscraper.StartupRunning == true)
	{
		while (true)
		{
			Ogre::WindowEventUtilities::messagePump();
			if(skyscraper.mRenderWindow->isClosed())
			{
			    return false;
			}
			skyscraper.Render();
		}
	}
	return 0;
}

bool Skyscraper::OnInit(void)
{
	version = "1.9";
	version_rev = SVN_REVSTR;
	version_state = "Alpha";
	version_frontend = version + ".0." + version_rev;
	skyscraper = this;
	MouseDown = false;
	IsRunning = false;
	StartupRunning = false;
	Starting = false;
	Pause = false;
	DisableSound = false;
	FullScreen = false;
	Shutdown = false;
	PositionOverride = false;
	Reload = false;
	mRoot = 0;
	mRenderWindow = 0;
	mViewport = 0;
	mSceneMgr = 0;
	mCamera = 0;
	sound = 0;
	channel = 0;
	SkyMult = 1;
	mCaelumSystem = 0;
	buttons = 0;
	buttoncount = 0;
	logger = 0;
	raised = false;

	//load config file
	try
	{
		configfile.load("skyscraper.ini");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading skyscraper.ini file\nDetails:" + e.getDescription());
	}

	//showconsole = GetConfigBool("Skyscraper.Frontend.ShowConsole", true);
	showconsole = false;

	//start and initialize OGRE
	if (!Initialize())
		return ReportError("Error initializing frontend");

	//load script processor
	processor = new ScriptProcessor();

	//set sky name
	SkyName = GetConfigString("Skyscraper.Frontend.SkyName", "DefaultSky");

	//autoload a building file if specified
	BuildingFile = GetConfigString("Skyscraper.Frontend.AutoLoad", "");
	if (BuildingFile != "")
		return Start();

	//show menu
	if (GetConfigBool("Skyscraper.Frontend.Menu.Show", true) == true)
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

	//cleanup
	Report("Cleaning up...");

	UnloadSim();

	//delete Caelum
	if (mCaelumSystem)
		delete mCaelumSystem;

	//unload script processor
	delete processor;
	processor = 0;

	//cleanup sound
	StopSound();
	if (soundsys)
		soundsys->release();

	/*if (mMouse)
		delete mMouse;
	if (mKeyboard)
		delete mKeyboard;
	if (mInputManager)
		delete mInputManager;*/

	skyscraper = 0;

	delete mRoot;
	delete logger;
	return 0;
}

void Skyscraper::UnloadSim()
{
	if (mCaelumSystem)
	{
		mCaelumSystem->clear();
		mCaelumSystem->detachAllViewports();
		delete mCaelumSystem;
		mCaelumSystem = 0;
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("Caelum");
	}

	//delete simulator object
	delete Simcore;
	Simcore = 0;
	Report("SBS unloaded\n");
}

void Skyscraper::Render()
{
	SBS_PROFILE_MAIN("Render");

	// Render to the frame buffer
	mRoot->renderOneFrame();
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
		return ReportFatalError("Error during initial OGRE check\nDetails:" + e.getDescription());
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
			return ReportFatalError("Error initializing OGRE\nDetails:" + e.getDescription());
		}
	}
	
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
		return ReportFatalError("Error configuring render system\nDetails:" + e.getDescription());
	}

	//initialize render window
	try
	{
		mRenderWindow = mRoot->initialise(true, "Skyscraper");
		mRenderWindow->setActive(true);
		mRenderWindow->windowMovedOrResized();
		//mRoot->getRenderSystem()->setWaitForVerticalBlank(false); //disable vsync
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error initializing render window\nDetails:" + e.getDescription());
	}
	mRoot->addFrameListener(this);

	//load resource configuration
	Ogre::ConfigFile cf;
	try
	{
		cf.load("resources.cfg");
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error loading resources.cfg\nDetails:" + e.getDescription());
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
		return ReportFatalError("Error initializing resources\nDetails:" + e.getDescription());
	}
	
	//create scene manager
	try
	{
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating scene manager\nDetails:" + e.getDescription());
	}

	//set ambient light
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	try
	{
		mCamera = mSceneMgr->createCamera("Main Camera");
		mViewport = mRenderWindow->addViewport(mCamera);
		//mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
		mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	}
	catch (Ogre::Exception &e)
	{
		return ReportFatalError("Error creating camera and viewport\nDetails:" + e.getDescription());
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

	//initialize OIS (input)
	size_t hWnd = 0;
	mRenderWindow->getCustomAttribute("WINDOW", &hWnd);
	if (!hWnd)
		return ReportFatalError("Error initializing OIS\n");

	mInputManager = OIS::InputManager::createInputSystem(hWnd);
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, false));
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, false));

	//OIS mouse state window dimensions
	unsigned int width, height, depth;
	int top, left;
	mRenderWindow->getMetrics(width, height, depth, left, top);
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

	//initialize FMOD (sound)
	Report("\nFMOD Sound System, copyright (C) Firelight Technologies Pty, Ltd., 1994-2010");

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
			Report("Sound initialized");
	}

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
			return ReportFatalError("Error initializing Caelum plugin:\nDetails:" + e.getDescription());
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

void Skyscraper::GetInput()
{
	SBS_PROFILE_MAIN("GetInput");

	//quit if main window isn't selected
	//if (window->Active == false)
		//return;

	static int wireframe;
	static bool wait, waitcheck, colliders;
	static unsigned int old_time;
	static int old_mouse_x, old_mouse_y;
	mKeyboard->capture();
	mMouse->capture();

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
	if (Simcore->camera->Freelook == false)
	{
		Simcore->mouse_x = mMouse->getMouseState().X.abs;
		Simcore->mouse_y = mMouse->getMouseState().Y.abs;
	}
	else
	{
		Simcore->mouse_x = mMouse->getMouseState().X.rel;
		Simcore->mouse_y = mMouse->getMouseState().Y.rel;
	}

	//if mouse coordinates changed, and we're in freelook mode, rotate camera
	if (Simcore->camera->Freelook == true && (old_mouse_x != Simcore->mouse_x || old_mouse_y != Simcore->mouse_y))
	{
		Ogre::Vector3 rotational;
		rotational.x = Ogre::Math::DegreesToRadians(Simcore->camera->Freelook_speed * ((float)Simcore->mouse_y / (mRenderWindow->getHeight() * 2)));
		rotational.y = Ogre::Math::DegreesToRadians(Simcore->camera->Freelook_speed * ((float)Simcore->mouse_x / (mRenderWindow->getWidth() * 2)));
		rotational.z = 0;
		rotational *= 60;
		Simcore->camera->desired_angle_velocity = rotational;
		Simcore->camera->angle_velocity = rotational;
	}

	//check if the user clicked on an object, and process it
	if (mMouse->getMouseState().buttonDown(OIS::MB_Left) == true && MouseDown == false)
	{
		MouseDown = true;
		Simcore->camera->MouseDown = MouseDown;
		Simcore->camera->ClickedObject(mKeyboard->isModifierDown(OIS::Keyboard::Shift), mKeyboard->isModifierDown(OIS::Keyboard::Ctrl), mKeyboard->isModifierDown(OIS::Keyboard::Alt));
	}

	//reset mouse state
	if (mMouse->getMouseState().buttonDown(OIS::MB_Left) == false)
	{
		MouseDown = false;
		Simcore->camera->MouseDown = MouseDown;
	}

	if (mKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		//int result = wxMessageBox(wxT("Exit and return to the main menu?"), wxT("Skyscraper"), wxYES_NO | wxCENTER);
		//if (result == wxYES)
			Shutdown = true;
	}

	if (mKeyboard->isKeyDown(OIS::KC_F2) && wait == false)
	{
		Report(Ogre::StringConverter::toString(Simcore->FPS));
		wait = true;
	}

	Simcore->camera->speed = 1;
	float speed_normal = Simcore->camera->cfg_speed;
	float speed_fast = Simcore->camera->cfg_speedfast;
	float speed_slow = Simcore->camera->cfg_speedslow;

	//crash test
	if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl) && mKeyboard->isModifierDown(OIS::Keyboard::Alt) && mKeyboard->isKeyDown(OIS::KC_C))
		throw;

	if (mKeyboard->isModifierDown(OIS::Keyboard::Ctrl))
	{
		if (mKeyboard->isKeyDown(OIS::KC_R))
		{
			Reload = true;
			return;
		}
		Simcore->camera->speed = speed_slow;
	}
	else if (mKeyboard->isModifierDown(OIS::Keyboard::Shift))
		Simcore->camera->speed = speed_fast;

	if (mKeyboard->isModifierDown(OIS::Keyboard::Alt))
	{
		//strafe movement
		if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
			Simcore->camera->Strafe(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
			Simcore->camera->Strafe(-speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
			Simcore->camera->Float(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
			Simcore->camera->Float(-speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_PGUP) || mKeyboard->isKeyDown(OIS::KC_P))
			Simcore->camera->Spin(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_PGDOWN) || mKeyboard->isKeyDown(OIS::KC_L))
			Simcore->camera->Spin(-speed_normal);
	}
	else
	{
		if (mKeyboard->isKeyDown(OIS::KC_V) && wait == false)
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
			if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
				Simcore->camera->Turn(speed_normal);
			if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
				Simcore->camera->Turn(-speed_normal);
		}
		else
		{
			if (mKeyboard->isKeyDown(OIS::KC_RIGHT) || mKeyboard->isKeyDown(OIS::KC_D))
				Simcore->camera->Strafe(speed_normal);
			if (mKeyboard->isKeyDown(OIS::KC_LEFT) || mKeyboard->isKeyDown(OIS::KC_A))
				Simcore->camera->Strafe(-speed_normal);
		}
		if (mKeyboard->isKeyDown(OIS::KC_PGUP) || mKeyboard->isKeyDown(OIS::KC_P))
			Simcore->camera->Look(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_PGDOWN) || mKeyboard->isKeyDown(OIS::KC_L))
			Simcore->camera->Look(-speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_UP) || mKeyboard->isKeyDown(OIS::KC_W))
			Simcore->camera->Step(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_DOWN) || mKeyboard->isKeyDown(OIS::KC_S))
			Simcore->camera->Step(-speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_SPACE) && wait == false)
		{
			if (Simcore->camera->IsOnGround() == true)
			{
				Simcore->camera->Jump();
				wait = true;
			}
		}
		if (mKeyboard->isKeyDown(OIS::KC_F3))
		{
			//reset view
			Simcore->camera->SetToStartDirection();
			Simcore->camera->SetToStartRotation();
			Simcore->camera->SetToDefaultFOV();
		}
		if (mKeyboard->isKeyDown(OIS::KC_F6))
		{
			//reset camera position and state
			Simcore->GetFloor(Simcore->camera->CurrentFloor)->Enabled(false);
			Simcore->GetFloor(Simcore->camera->CurrentFloor)->EnableGroup(false);
			Simcore->camera->EnableGravity(true);
			Simcore->camera->EnableCollisions(true);
			Simcore->camera->SetToStartPosition(true);
			Simcore->camera->SetToStartRotation();
			Simcore->camera->SetToDefaultFOV();
			Simcore->GetFloor(Simcore->camera->StartFloor)->Enabled(true);
			Simcore->GetFloor(Simcore->camera->StartFloor)->EnableGroup(true);
		}

		if (mKeyboard->isKeyDown(OIS::KC_F7) && wait == false)
		{
			//show colliders
			Simcore->ShowColliders(!colliders);
			colliders = !colliders;
			wait = true;
		}

		if (mKeyboard->isKeyDown(OIS::KC_F4) && wait == false)
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
		if (mKeyboard->isKeyDown(OIS::KC_F11) && wait == false)
		{
			mRenderWindow->writeContentsToTimestampedFile("screenshots/skyscraper-", ".jpg");
			wait = true;
		}
		/*if (mKeyboard->isKeyDown(OIS::KC_F12) && !dpanel)
		{
			//show control panel if closed
			dpanel = new DebugPanel(NULL, -1);
			dpanel->Show(true);
			dpanel->SetPosition(wxPoint(GetConfigInt("Skyscraper.Frontend.ControlPanelX", 10), GetConfigInt("Skyscraper.Frontend.ControlPanelY", 25)));
		}*/
		if (mKeyboard->isKeyDown(OIS::KC_F5) && wait == false)
		{
			//enable/disable freelook mode
			Simcore->camera->Freelook = !Simcore->camera->Freelook;
			/*if (Simcore->camera->Freelook == true)
				window->SetCursor(wxCURSOR_CROSS);
			else
				window->SetCursor(wxNullCursor);*/
			wait = true;
		}
		if (mKeyboard->isKeyDown(OIS::KC_F10) && wait == false)
		{
			//enable/disable fullscreen mode
			FullScreen = !FullScreen;
			const Ogre::ConfigOptionMap::iterator opti = mRoot->getRenderSystem()->getConfigOptions().find("Video Mode");
			Ogre::StringVector vmopts = Ogre::StringUtil::split(opti->second.currentValue, " x");
			unsigned int w = Ogre::StringConverter::parseUnsignedInt(vmopts[0]);
			unsigned int h = Ogre::StringConverter::parseUnsignedInt(vmopts[1]);
			mRenderWindow->setFullscreen(FullScreen, w, h);
			wait = true;
		}
		if (mKeyboard->isKeyDown(OIS::KC_SUBTRACT) || mKeyboard->isKeyDown(OIS::KC_LBRACKET))
		{
			//increase FOV angle
			float angle = Simcore->camera->GetFOVAngle() + Simcore->camera->cfg_zoomspeed;
			Simcore->camera->SetFOVAngle(angle);
		}
		if (mKeyboard->isKeyDown(OIS::KC_ADD) || mKeyboard->isKeyDown(OIS::KC_RBRACKET))
		{
			//decrease FOV angle
			float angle = Simcore->camera->GetFOVAngle() - Simcore->camera->cfg_zoomspeed;
			Simcore->camera->SetFOVAngle(angle);
		}

		//values from old version
		if (mKeyboard->isKeyDown(OIS::KC_HOME) || mKeyboard->isKeyDown(OIS::KC_O))
			Simcore->camera->Float(speed_normal);
		if (mKeyboard->isKeyDown(OIS::KC_END) || mKeyboard->isKeyDown(OIS::KC_K))
			Simcore->camera->Float(-speed_normal);
	}
}

void Skyscraper::Report(std::string message)
{
	Ogre::LogManager::getSingleton().logMessage(message);
}

bool Skyscraper::ReportError(std::string message)
{
	Ogre::LogManager::getSingleton().logMessage(message, Ogre::LML_CRITICAL);
	return false;
}

bool Skyscraper::ReportFatalError(std::string message)
{
	ReportError(message);

	//show error dialog
	//wxMessageDialog *dialog = new wxMessageDialog(0, wxString::FromAscii(message.c_str()), wxString::FromAscii("Skyscraper"), wxOK | wxICON_ERROR);
	//dialog->ShowModal();
	//delete dialog;

	return false;
}

void Skyscraper::Loop()
{
	//Main simulator loop

	SBSProfileManager::Reset();
	SBSProfileManager::Increment_Frame_Counter();

	//main menu routine
	if (IsRunning == false)
	{
		//DrawBackground();
		GetMenuInput();
		//Render();
		return;
	}

	if (!Simcore)
		return;

	//force window raise on startup
	if (Simcore->GetCurrentTime() > 0 && raised == false)
	{
		//window->Raise();
		raised = true;
	}

	//Simcore->AdvanceClock();
	//if (IsRunning == true)
		//Simcore->CalculateFrameRate();

	//run SBS main loop
	Simcore->MainLoop();

	//get input
	GetInput();

	//process camera loop
	//Simcore->camera->Loop();

	//update Caelum
	if (mCaelumSystem)
	{
		mCaelumSystem->notifyCameraChanged(mCamera);
		mCaelumSystem->setTimeScale(SkyMult);
		mCaelumSystem->updateSubcomponents(float(Simcore->GetElapsedTime()) / 1000);
	}

	//render graphics
	//Render();

	//exit if shutdown request received
	if (Shutdown == true)
	{
		Shutdown = false;
		//if showmenu is true, unload simulator and return to main menu
		if (GetConfigBool("Skyscraper.Frontend.ShowMenu", true) == true)
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
		//mouse->Reset();
		Start();
	}

	//SBSProfileManager::dumpAll();
}

void Skyscraper::DrawBackground()
{
	//draw menu background

	int w = mRenderWindow->getWidth();
	int h = mRenderWindow->getHeight();

	DrawImage(std::string("data/" + GetConfigString("Skyscraper.Frontend.Menu.Image", "menu.png")).c_str(), 0, -1, -1, false);

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
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button1.X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button1.Y", -0.08);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button1.Center", true);
		}
		if (i == 1)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Image", "button_glasstower.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Selected", "button_glasstower_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button2.Pressed", "button_glasstower_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button2.X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button2.Y", 0.125);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button2.Center", true);
		}
		if (i == 2)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Image", "button_searstower.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Selected", "button_searstower_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button3.Pressed", "button_searstower_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button3.X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button3.Y", 0.333);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button3.Center", true);
		}
		if (i == 3)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Image", "button_simple.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Selected", "button_simple_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button4.Pressed", "button_simple_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button4.X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button4.Y", 0.541);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button4.Center", true);
		}
		if (i == 4)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Image", "button_other.png");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Selected", "button_other_selected.png");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button5.Pressed", "button_other_pressed.png");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button5.X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button5.Y", 0.75);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button5.Center", true);
		}
		if (i > 4)
		{
			b1 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Image", "");
			b2 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Selected", "");
			b3 = "data/" + GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".Pressed", "");
			x = GetConfigFloat("Skyscraper.Frontend.Menu.Button" + number + ".X", 0.0);
			y = GetConfigFloat("Skyscraper.Frontend.Menu.Button" + number + ".Y", 0.0);
			center = GetConfigBool("Skyscraper.Frontend.Menu.Button" + number + ".Center", true);
		}

		DrawImage(b1.c_str(), &buttons[i], x, y, center, b2.c_str(), b3.c_str());
	}
}

void Skyscraper::DrawImage(const char *filename, buttondata *button, float x, float y, bool center, const char *filename_selected, const char *filename_pressed)
{
	//X and Y represent the image's top-left location.
	//values are -1 for the top left, 1 for the top right, -1 for the top, and 1 for the bottom
	//center is at 0, 0

	float w, h;
	bool background = false;

	std::string material = "";
	std::string Filename = filename;

	if (!filename)
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

			Ogre::MaterialPtr mat;
			try
			{
				mat = Ogre::MaterialManager::getSingleton().create(Filename, "General");
				tex = Ogre::TextureManager::getSingleton().load(Filename, "General", Ogre::TEX_TYPE_2D, 0);
			}
			catch (Ogre::Exception &e)
			{
				ReportError("Error loading texture " + Filename + "\n" + e.getDescription());
				return;
			}

			mat->getTechnique(0)->getPass(0)->createTextureUnitState(Filename);
			mat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
			mat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
			mat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
			mat->getTechnique(0)->getPass(0)->setTextureFiltering(Ogre::TFO_NONE);

			if (tex->hasAlpha() == true && button)
			{
				mat->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
				mat->getTechnique(0)->getPass(0)->setAlphaRejectSettings(Ogre::CMPF_GREATER_EQUAL, 128);
			}
		}
		material = filename;
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
		w = tex->getWidth() / (mRenderWindow->getWidth() / 2.0);
		h = tex->getHeight() / (mRenderWindow->getHeight() / 2.0);
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

	//exit if simulator is starting
	if (Starting == true)
		return;

	//exit if there aren't any buttons
	if (!buttons || buttoncount == 0)
		return;

	//get mouse coordinates
	mMouse->capture();
	int mouse_x = mMouse->getMouseState().X.abs;
	int mouse_y = mMouse->getMouseState().Y.abs;

	for (int i = 0; i < buttoncount; i++)
	{
		buttondata *button = &buttons[i];

	    //only process buttons if main window is selected
        //if (window->Active != false)
        //{
			float mx = mouse_x;
			float my = mouse_y;
			float w = mx / mRenderWindow->getWidth();
			float h = my / mRenderWindow->getHeight();
			float mouse_x_rel = (w * 2) - 1;
			float mouse_y_rel = (h * 2) - 1;

        	//change button status based on mouse position and button press status
        	if (mouse_x_rel > button->x && mouse_x_rel < button->x + button->size_x && mouse_y_rel > button->y && mouse_y_rel < button->y + button->size_y)
        	{
        		if (button->drawn_selected == false && mMouse->getMouseState().buttonDown(OIS::MB_Left) == false)
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
        		if (button->drawn_pressed == false && mMouse->getMouseState().buttonDown(OIS::MB_Left) == true)
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
        //}
	}
}

void Skyscraper::Click(int index)
{
	//user clicked a button

	std::string number = ToString(index + 1);

	if (index == 0)
		BuildingFile = GetConfigString("Skyscraper.Frontend.Menu.Button1.File", "Triton Center.bld");
	if (index == 1)
		BuildingFile = GetConfigString("Skyscraper.Frontend.Menu.Button2.File", "Glass Tower.bld");
	if (index == 2)
		BuildingFile = GetConfigString("Skyscraper.Frontend.Menu.Button3.File", "Sears Tower.bld");
	if (index == 3)
		BuildingFile = GetConfigString("Skyscraper.Frontend.Menu.Button4.File", "Simple.bld");
	if (index > 3)
		BuildingFile = GetConfigString("Skyscraper.Frontend.Menu.Button" + number + ".File", "");

	if (BuildingFile == "")
	{
		//show file selection dialog
		SelectBuilding();
	}

	if (BuildingFile != "")
	{
		DeleteButtons();
		Start();
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
	if (channel)
		channel->stop();
	if (sound)
		sound->release();
	sound = 0;
}

bool Skyscraper::SelectBuilding()
{
	//choose a building from a script file
	/*BuildingFile = "";
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
	Selector = 0;*/

	return true;
}

bool Skyscraper::Start()
{
	//start simulator

	//clear scene
	mSceneMgr->clearScene();

	//clear screen
	mRenderWindow->update();

	Starting = true;

	//Create and initialize simulator
	Simcore = new SBS(mRenderWindow, mSceneMgr, mCamera, root_dir.c_str(), dir_char.c_str(), soundsys);

	//load building data file
	Report("\nLoading building data from " + BuildingFile + "...\n");
	Simcore->BuildingFilename = BuildingFile;

	//Pause for 1 second
	//wxSleep(1);

	if (Reload == false)
		BuildingFile.insert(0, "buildings/");

	//load script processor object and load building
	bool loaderror = false;

	processor->Reset();

	if (!processor->LoadDataFile(BuildingFile.c_str()))
	{
		loaderror = true;
		ReportFatalError("Error loading building file\n");
	}
	if (loaderror == false)
	{
		if (!processor->Run())
		{
			loaderror = true;
			ReportError("Error processing building\n");
		}
	}

	//report on missing files, if any
	if (loaderror == false)
		processor->ReportMissingFiles();

	if (loaderror == true)
	{
		Unload();
		return false;
	}

	//the sky needs to be created before Prepare() is called
	bool sky_result = false;
	if (GetConfigBool("Skyscraper.Frontend.Caelum", true) == true)
		sky_result = InitSky();

	//create old sky if Caelum is turned off, or failed to initialize
	if (sky_result == false)
		Simcore->CreateSky(Simcore->SkyName.c_str());

	//have SBS prepare objects for use (upload geometry data to graphics card, etc)
	Simcore->Prepare();

	//clear screen
	mRoot->renderOneFrame();

	//start simulation
	if (!Simcore->Start())
		return ReportError("Error starting simulator\n");

	//set to saved position if reloading building
	if (PositionOverride == true)
	{
		PositionOverride = false;
		Simcore->camera->SetPosition(override_position);
		Simcore->camera->SetRotation(override_rotation);
	}

	//run simulation
	Report("Running simulation...");
	StopSound();
	Simcore->IsRunning = true;
	IsRunning = true;
	Starting = false;
	StartupRunning = false;
	Reload = false;
	return true;
}

void Skyscraper::Unload()
{
	//unload sim
	BuildingFile = "";
	IsRunning = false;
	Starting = false;
	Pause = false;
	raised = false;
	UnloadSim();

	//cleanup sound
	StopSound();

	DrawBackground();
	StartSound();
	StartupRunning = true;
	//mouse->Reset();
}

void Skyscraper::Quit()
{
	//exit app
}

int Skyscraper::GetConfigInt(std::string key, int default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseInt(result);
}

std::string Skyscraper::GetConfigString(std::string key, std::string default_value)
{
	return configfile.getSetting(key, Ogre::StringUtil::BLANK, default_value);
}

bool Skyscraper::GetConfigBool(std::string key, bool default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseBool(result);
}

float Skyscraper::GetConfigFloat(std::string key, float default_value)
{
	std::string result = configfile.getSetting(key, Ogre::StringUtil::BLANK, Ogre::StringConverter::toString(default_value));
	return Ogre::StringConverter::parseReal(result);
}

bool Skyscraper::InitSky()
{
	//initialize sky

	//ensure graphics card and render system are capable of Caelum's shaders
	if (mRoot->getRenderSystem()->getCapabilities()->getRenderSystemName() == "Direct3D9 Rendering Subsystem")
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

	if (mRoot->getRenderSystem()->getCapabilities()->getRenderSystemName() == "OpenGL Rendering Subsystem")
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
		ReportFatalError("Error initializing Caelum:\nDetails:" + e.getDescription());
	}
	catch (...)
	{
		ReportFatalError("Error initializing Caelum");
	}

	//attach caelum to running viewport
	if (mCaelumSystem)
	{
		try
		{
			mCaelumSystem->attachViewport(mCamera->getViewport());
			mCaelumSystem->setAutoNotifyCameraChanged(false);
			mCaelumSystem->setSceneFogDensityMultiplier(GetConfigFloat("Skyscraper.Frontend.FogMultiplier", 0.1f) / 1000);
			if (GetConfigBool("Skyscraper.Frontend.EnableFog", true) == false)
				mCaelumSystem->setManageSceneFog(Ogre::FOG_NONE);
			mCaelumSystem->setManageAmbientLight(GetConfigBool("Skyscraper.Frontend.ModifyAmbient", false));
		}
		catch (Ogre::Exception &e)
		{
			ReportFatalError("Error setting Caelum parameters:\nDetails:" + e.getDescription());
		}
		catch (...)
		{
			ReportFatalError("Error setting Caelum parameters");
		}
	}
	else
		return false;

	SkyMult = GetConfigInt("Skyscraper.Frontend.SkyMult", 50);

	return true;
}

ScriptProcessor* Skyscraper::GetScriptProcessor()
{
	return processor;
}

void Skyscraper::messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage)
{
	//callback function that receives OGRE log messages
}

bool Skyscraper::frameStarted(const Ogre::FrameEvent& evt)
{
	if (Simcore)
		Simcore->elapsed_time = evt.timeSinceLastFrame * 1000;

	Loop();
	return true;
}

