/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

//#include <wx/wx.h>
#include <wx/timer.h>
#include <crystalspace.h>
#include <sstream>
#include "sbs.h"
#include "camera.h"
#include "unix.h"

SBS *sbs; //self reference
Camera *c; //camera object
wxTimer *stimer; //timer object

iObjectRegistry* object_reg;

//Runloop Timer
class STimer : public wxTimer
{
public:
	STimer() { };
	virtual void Notify();
};

SBS::SBS()
{
	sbs = this;

	//Print SBS banner
	PrintBanner();

	//Pause for 2 seconds
	csSleep(2000);

	//Set default horizontal scaling value
	HorizScale = 1;

	//Set default starting elevator
	ElevatorNumber = 1;

	//Set default frame rate
	FrameRate = 40;
	FrameLimiter = true;

	//initialize other variables
	BuildingName = "";
	BuildingDesigner = "";
	BuildingLocation = "";
	BuildingDescription = "";
	BuildingVersion = "";
	Gravity = 0;
	IsRunning = false;
	Shafts = 0;
	TotalFloors = 0;
	Basements = 0;
	Elevators = 0;
	StairsNum = 0;
	RenderOnly = false;
	InputOnly = false;
	IsFalling = false;
	FallRate = 0;
	InStairwell = false;
	InElevator = false;
	FPSModifier = 1;
	FrameSync = true;
	EnableCollisions = false;
	BuildingFile = "";
	IsBuildingsEnabled = false;
	IsColumnFrameEnabled = false;
	IsExternalEnabled = false;
	IsLandscapeEnabled = false;
	IsSkyboxEnabled = false;
	fps_frame_count = 0;
	fps_tottime = 0;
	FPS = 0;
}

SBS::~SBS()
{
	//engine destructor

	//delete camera object
	delete c;
	c = 0;

	//delete floors
	for (int i = -Basements; i <= TotalFloors; i++)
	{
		delete FloorArray[i];
		FloorArray[i] = 0;
	}

	//delete elevators
	for (int i = 1; i <= Elevators; i++)
	{
		delete ElevatorArray[i];
		ElevatorArray[i] = 0;
	}

	//delete floor and elevator arrays
	FloorArray.DeleteAll();
	ElevatorArray.DeleteAll();

	//delete string arrays
	UserVariable.DeleteAll();
	BuildingData.DeleteAll();
}

void SBS::Start()
{
	//set up viewport
	view = csPtr<iView>(new csView (engine, g3d));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//load camera object
	c = new Camera();

	//set main simulation values
	InputOnly = false;
	RenderOnly = false;
	IsRunning = true;
	EnableCollisions = true;

	//clear user variables
	UserVariable.DeleteAll();
	UserVariable.SetSize(256);

	//load building data file
    Report("\nLoading building data from " + BuildingFile + "...\n");

	//Pause for 1 second
	csSleep(1000);

	BuildingFile.Insert(0, "/root/buildings/");
	LoadBuilding(BuildingFile.GetData());
	//if (LoadBuilding(BuildingFile.GetData()) != 0)

	//create skybox
	CreateSky();

	//Post-init startup code goes here, before the runloop
	engine->Prepare();

	//move camera to start location
	c->SetToStartPosition();
	c->SetToStartDirection();
	c->SetToStartRotation();

	//turn on main objects
	EnableBuildings(true);
	EnableLandscape(true);
	EnableExternal(true);
	EnableColumnFrame(true);
	EnableSkybox(true);

	//turn off floors
	for (int i = -Basements; i <= TotalFloors; i++)
		FloorArray[i]->Enabled(false);
	
	//turn off shafts
	for (int i = 0; i <= ShaftArray.GetSize(); i++)
	{
		for (int j = ShaftArray[i]->startfloor; j <= (ShaftArray[i]->startfloor - ShaftArray[i]->endfloor); j++)
			ShaftArray[i]->Enabled(j, false);
	}

	//turn on first/lobby floor
	FloorArray[0]->Enabled(true);

}

void SBS::Run()
{
	//start runloop
	
	Report("Running simulation...");

	//start simulation
	csDefaultRunLoop (object_reg);
    //stimer = new STimer();
    //stimer->Start(1000 / FrameRate);

}

void SBS::Wait(long milliseconds)
{

}

double AutoSize(double n1, double n2, bool iswidth, bool external, double offset)
{
//Texture autosizing formulas

	double size1;
	double size2;

	if (offset == 0)
		offset = 1;

	if (external == false)
	{
		size1 = 0.269 * offset;
		size2 = 0.25 * offset;
	}
	else
	{
		size1 = 0.072 * offset;
		size2 = 1 * offset;
	}

	if (iswidth == true)
		return abs(n1 - n2) * size1;
	else
	{
		if (external == false)
			return abs(n1 - n2) * size2;
		else
			return size2;
	}
	return 0;
}

void SBS::PrintBanner()
{
	csPrintf("\n Scalable Building Simulator 0.1 Alpha\n");
	csPrintf(" Copyright (C)2004-2006 Ryan Thoryk\n");
	csPrintf(" This software comes with ABSOLUTELY NO WARRANTY. This is free\n");
	csPrintf(" software, and you are welcome to redistribute it under certain\n");
	csPrintf(" conditions. For details, see the file gpl.txt\n");
}

void SBS::GetInput()
{
	// First get elapsed time from the virtual clock.
	elapsed_time = vc->GetElapsedTicks ();
	current_time = vc->GetCurrentTicks ();
	
	// Now rotate the camera according to keyboard state
	double speed = (elapsed_time / 1000.0) * (0.06 * 20);

	//if (kbd->GetKeyState('a'))
	
	if (kbd->GetKeyState (CSKEY_ESC))
		if (equeue) equeue->GetEventOutlet()->Broadcast (csevQuit(object_reg));

	if (kbd->GetKeyState (CSKEY_CTRL))
		speed *= 4;

	if (kbd->GetKeyState (CSKEY_SHIFT))
	{
		// If the user is holding down shift, the arrow keys will cause
		// the camera to strafe up, down, left or right from it's
		// current position.
		if (kbd->GetKeyState (CSKEY_RIGHT))
			c->Move (CS_VEC_RIGHT * 8 * speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			c->Move (CS_VEC_LEFT * 8 * speed);
		if (kbd->GetKeyState (CSKEY_UP))
			c->Move (CS_VEC_UP * 8 * speed);
		if (kbd->GetKeyState (CSKEY_DOWN))
			c->Move (CS_VEC_DOWN * 8 * speed);
	}
	else if (kbd->GetKeyState (CSKEY_ALT))
	{
		//rotate on the Z axis if the Alt key is pressed with the left/right arrows
		if (kbd->GetKeyState (CSKEY_RIGHT))
			c->Rotate(CS_VEC_FORWARD * speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			c->Rotate(CS_VEC_BACKWARD * speed);
	}
	else
	{
		// left and right cause the camera to rotate on the global Y
		// axis; page up and page down cause the camera to rotate on the
		// _camera's_ X axis (more on this in a second) and up and down
		// arrows cause the camera to go forwards and backwards.
		if (kbd->GetKeyState (CSKEY_RIGHT))
			c->Rotate(CS_VEC_UP * speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			c->Rotate(CS_VEC_DOWN * speed);
		if (kbd->GetKeyState (CSKEY_PGUP))
			c->Rotate(CS_VEC_RIGHT * speed);
		if (kbd->GetKeyState (CSKEY_PGDN))
			c->Rotate(CS_VEC_LEFT * speed);
		if (kbd->GetKeyState (CSKEY_UP))
		{
			double KeepAltitude;
			KeepAltitude = c->GetPosition().y;
			c->Move (CS_VEC_FORWARD * 8 * speed);
			if (c->GetPosition().y != KeepAltitude)
				c->SetPosition(csVector3(c->GetPosition().x, KeepAltitude, c->GetPosition().z));
		}
		if (kbd->GetKeyState (CSKEY_DOWN))
		{
			double KeepAltitude;
			KeepAltitude = c->GetPosition().y;
			c->Move (CS_VEC_BACKWARD * 8 * speed);
			if (c->GetPosition().y != KeepAltitude)
				c->SetPosition(csVector3(c->GetPosition().x, KeepAltitude, c->GetPosition().z));
		}

		if (kbd->GetKeyState (CSKEY_SPACE))
		{
			//reset view
			c->SetToStartDirection();
			c->SetToStartRotation();
		}

		//values from old version
		if (kbd->GetKeyState (CSKEY_HOME))
			c->Move (CS_VEC_UP * 8 * speed);
		if (kbd->GetKeyState (CSKEY_END))
			c->Move (CS_VEC_DOWN * 8 * speed);
	}
}

void SBS::Render()
{
	// Tell 3D driver we're going to display 3D things.
	//if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER ))
	if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN )) //clear screen also
		return;

	// Tell the camera to render into the frame buffer.
	view->Draw ();
}

void SBS::SetupFrame()
{
	//Main simulator loop
	
	//used to adjust speeds according to frame rate
	if (FrameSync == true)
		FPSModifier = FrameRate / FPS;
	else
		FPSModifier = 1;

	if (RenderOnly == false && InputOnly == false)
	{
		//Calls the Fall function, and if IsFalling is true then the user falls until they hit something
		if (EnableCollisions == true)
			Fall();

		//Determine floor that the camera is on
		c->UpdateCameraFloor();

		//run elevator handlers
		for (int i = 1; i <= Elevators; i++)
			ElevatorArray[i]->MonitorLoop();
	}

	if (RenderOnly == false)
		GetInput();

    Render();
}

void SBS::FinishFrame()
{
	g3d->FinishDraw();
	g3d->Print(0);
}


bool SBS::HandleEvent(iEvent& Event)
{
	//Event handler
	if (Event.Name == Process)
	{
		// First get elapsed time from the virtual clock.
		elapsed_time = vc->GetElapsedTicks ();

		//calculate frame rate
		fps_tottime += elapsed_time;
		fps_frame_count++;
		if (fps_tottime > 500)
		{
			FPS = (float (fps_frame_count) * 1000.0) / float (fps_tottime);
			fps_frame_count = 0;
			fps_tottime = 0;
		}

		SetupFrame ();
		return true;
	}
	else if (Event.Name == FinalProcess)
	{
		if (FrameLimiter == true)
			wxMilliSleep(1000 / FrameRate);

		FinishFrame ();
		return true;
	}
	return false;
}

static bool SBSEventHandler(iEvent& Event)
{
  if (sbs)
    return sbs->HandleEvent (Event);
  else
    return false;
}

void STimer::Notify()
{
	//timer loop code
	sbs->PushFrame();
}

void SBS::PushFrame()
{
	//timer loop event queue processor
	if (!equeue)
		return;
	if (vc)
		vc->Advance();
	equeue->Process();
}

bool SBS::Initialize(int argc, const char* const argv[], const char *windowtitle)
{
	object_reg = csInitializer::CreateEnvironment (argc, argv);
	if (!object_reg) return false;

	if (!csInitializer::RequestPlugins (object_reg,
  		CS_REQUEST_VFS,
		CS_REQUEST_OPENGL3D,
        CS_REQUEST_ENGINE,
		CS_REQUEST_FONTSERVER,
		CS_REQUEST_IMAGELOADER,
        CS_REQUEST_LEVELLOADER,
		CS_REQUEST_CONSOLEOUT,
		CS_REQUEST_REPORTER,
		CS_REQUEST_REPORTERLISTENER,
		CS_REQUEST_END))
		return ReportError ("Couldn't init app!");

	FocusGained = csevFocusGained (object_reg);
	FocusLost = csevFocusLost (object_reg);
	Process = csevProcess (object_reg);
	FinalProcess = csevFinalProcess (object_reg);
	KeyboardDown = csevKeyboardDown (object_reg);

	if (!csInitializer::SetupEventHandler (object_reg, SBSEventHandler))
		return ReportError ("Couldn't initialize event handler!");

	  // Check for commandline help.
	if (csCommandLineHelper::CheckHelp (object_reg))
	{
	    csCommandLineHelper::Help (object_reg);
	    return false;
	}

	csRef<iPluginManager> plugin_mgr (CS_QUERY_REGISTRY (object_reg, iPluginManager));

	vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
	kbd = CS_QUERY_REGISTRY (object_reg, iKeyboardDriver);
	if (!kbd) return ReportError ("No keyboard driver!");
	engine = CS_QUERY_REGISTRY (object_reg, iEngine);
	if (!engine) return ReportError ("No engine!");
	loader = CS_QUERY_REGISTRY (object_reg, iLoader);
	if (!loader) return ReportError ("No loader!");
	g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
	if (!g3d) return ReportError ("No 3D driver!");
	g2d = CS_QUERY_REGISTRY (object_reg, iGraphics2D);
	if (!g2d) return ReportError ("No 2D driver!");
	imageio = CS_QUERY_REGISTRY (object_reg, iImageIO);
	if (!imageio) return ReportError ("No image loader!");
	vfs = CS_QUERY_REGISTRY (object_reg, iVFS);
	if (!vfs) return ReportError ("No VFS!");
	console = CS_QUERY_REGISTRY (object_reg, iConsoleOutput);
	if (!console) return ReportError ("No ConsoleOutput!");
	plug = CS_LOAD_PLUGIN_ALWAYS (plugin_mgr, "crystalspace.utilities.bugplug");
	if (!plug) return ReportError ("No BugPlug!");
	if (plug) plug->IncRef ();

	stdrep = CS_QUERY_REGISTRY (object_reg, iStandardReporterListener);
	if (!stdrep) return ReportError ("No stdrep plugin!");
	stdrep->SetDebugFile ("/tmp/sbs_report.txt");
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_BUG, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_ERROR, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_WARNING, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_NOTIFY, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_DEBUG, true, false, false, false, true, false);

	//mount app's directory in VFS
	#ifndef CS_PLATFORM_WIN32
		vfs->Mount("/root/", csInstallationPathsHelper::GetAppDir(argv[0]) + "/");
	#else
		vfs->Mount("/root/", csInstallationPathsHelper::GetAppDir(argv[0]) + "\\");
	#endif
	
	iNativeWindow* nw = g2d->GetNativeWindow();
	if (nw) nw->SetTitle(windowtitle);

	font = g2d->GetFontServer()->LoadFont(CSFONT_LARGE);

	// Open the main system. This will open all the previously loaded plug-ins.
	if (!csInitializer::OpenApplication (object_reg))
		return ReportError ("Error opening system!");

	equeue = CS_QUERY_REGISTRY(object_reg, iEventQueue);

	// First disable the lighting cache. Our app is simple enough
	// not to need this.
		engine->SetLightingCacheMode (0);
		engine->SetAmbientLight(csColor(0.5, 0.5, 0.5));

	//create 3D environments
	area = engine->CreateSector("area");

	return true;
}

bool SBS::LoadTexture(const char *filename, const char *name)
{
	// Load the texture from the standard library.  This is located in
	// CS/data/standard.zip and mounted as /lib/std using the Virtual
	// File System (VFS) plugin.
	if (!loader->LoadTexture (name, filename))
	{
	    ReportError("Error loading texture");
		return false;
	}
	return true;
}

void SBS::AddLight(const char *name, double x, double y, double z, double radius, double r, double g, double b)
{
	ll = area->GetLights();
	light = engine->CreateLight(name, csVector3(x, y, z), radius, csColor(r, g, b));
	ll->Add(light);
}

bool IsEven(int Number)
{
    //Determine if the passed number is even.
	//If number divides evenly, return true
	if ((Number / 2) == int(Number / 2))
		return true;
	else
		return false;
}

void Cleanup()
{
	//cleanup
	csPrintf ("Cleaning up...\n");
	sbs = 0;
	csInitializer::DestroyApplication (object_reg);
}

int SBS::AddWallMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double altitude1, double altitude2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides)
{
	//Adds a wall with the specified dimensions
	csVector3 v1 (x1, altitude1 + height_in1, z1); //left top
	csVector3 v2 (x2, altitude2 + height_in2, z2); //right top
	csVector3 v3 (x2, altitude2, z2); //right base
	csVector3 v4 (x1, altitude1, z1); //left base

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	if (DrawBothSides == true)
		dest->AddQuad(v4, v3, v2, v1);

	material = sbs->engine->GetMaterialList ()->FindByName (texture);

	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx), material);
	if (DrawBothSides == true)
		dest->SetPolygonMaterial (csPolygonRange(firstidx + 1, firstidx + 1), material);
	
	//reverse vector portions if specified
	if (revX == true)
	{
		v1.x = x2;
		v2.x = x1;
		v3.x = x1;
		v4.x = x2;
	}
	if (revY == true)
	{
		v1.y = altitude1;
		v2.y = altitude2;
		v3.y = altitude2 + height_in2;
		v4.y = altitude1 + height_in1;
	}
	if (revZ == true)
	{
		v1.z = z2;
		v2.z = z1;
		v3.z = z1;
		v4.z = z2;
	}
	
	//texture mapping is set from first 3 coordinates
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx),
		v1,
		csVector2 (0, 0),
		v2,
		csVector2 (tw, 0),
		v3,
		csVector2 (tw, th));
	if (DrawBothSides == true)
	{
		dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
			v1,
			csVector2 (tw, 0),
			v2,
			csVector2 (0, 0),
			v3,
			csVector2 (0, th));
	}
	return firstidx;
}

int SBS::AddFloorMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th)
{
	//Adds a floor with the specified dimensions and vertical offset
	csVector3 v1 (x1, altitude1, z1); //bottom left
	csVector3 v4 (x2, altitude2, z1); //bottom right
	csVector3 v3 (x2, altitude2, z2); //top right
	csVector3 v2 (x1, altitude1, z2); //top left

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	dest->AddQuad(v4, v3, v2, v1);
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx + 1), material);
	//texture mapping is set from first 3 coordinates
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx + 1), 
		csVector2 (0, 0),
		csVector2 (tw, 0),
		csVector2 (tw, th));
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
		csVector2 (0, th),
		csVector2 (tw, th),
		csVector2 (tw, 0));
	return firstidx;
}

void SBS::Report (const char* msg, ...)
{
	va_list arg;
	va_start (arg, msg);
	csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
	if (rep)
		rep->ReportV (CS_REPORTER_SEVERITY_NOTIFY, "sbs", msg, arg);
	else
	{
		csPrintfV (msg, arg);
		csPrintf ("\n");
		fflush (stdout);
	}
	va_end (arg);
}

bool SBS::ReportError (const char* msg, ...)
{
	va_list arg;
	va_start (arg, msg);
	csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
	if (rep)
		rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "sbs", msg, arg);
	else
	{
		csPrintfV (msg, arg);
		csPrintf ("\n");
		fflush (stdout);
	}
	va_end (arg);
	return false;
}

int SBS::CreateWallBox(csRef<iThingFactoryState> dest, const char *texture, double x1, double x2, double z1, double z2, double height_in, double voffset, double tw, double th)
{
	//create 4 walls
	
	iMaterialWrapper* tm;
	
	int firstidx = dest->AddInsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	return firstidx;
}

int SBS::CreateWallBox2(csRef<iThingFactoryState> dest, const char *texture, double CenterX, double CenterZ, double WidthX, double LengthZ, double height_in, double voffset, double tw, double th)
{
	//create 4 walls from a central point
	
	iMaterialWrapper* tm;
	double x1;
	double x2;
	double z1;
	double z2;

	x1 = CenterX - (WidthX / 2);
	x2 = CenterX + (WidthX / 2);
	z1 = CenterZ - (LengthZ / 2);
	z2 = CenterZ + (LengthZ / 2);

	int firstidx = dest->AddInsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below
	return firstidx;
}

void SBS::InitMeshes()
{
	//initialize floor and elevator object container arrays
	int i;
	FloorArray.DeleteAll();
	FloorArray.SetSize(Basements + TotalFloors + 1);

	for (i = -Basements; i <= TotalFloors; i++)
		FloorArray[i] = new Floor(i);

	ElevatorArray.DeleteAll();
	ElevatorArray.SetSize(Elevators + 1);

	for (i = 1; i <= Elevators; i++)
		ElevatorArray[i] = new Elevator(i);

	ShaftArray.SetSize(Shafts);

	//create object meshes
	Buildings = sbs->engine->CreateSectorWallsMesh (sbs->area, "Buildings");
	Buildings_object = Buildings->GetMeshObject ();
	Buildings_factory = Buildings_object->GetFactory();
	Buildings_state = scfQueryInterface<iThingFactoryState> (Buildings_factory);
	Buildings->SetZBufMode(CS_ZBUF_USE);

	External = sbs->engine->CreateSectorWallsMesh (sbs->area, "External");
	External_object = External->GetMeshObject ();
	External_factory = External_object->GetFactory();
	External_state = scfQueryInterface<iThingFactoryState> (External_factory);
	External->SetZBufMode(CS_ZBUF_USE);

	Landscape = sbs->engine->CreateSectorWallsMesh (sbs->area, "Landscape");
	Landscape_object = Landscape->GetMeshObject ();
	Landscape_factory = Landscape_object->GetFactory();
	Landscape_state = scfQueryInterface<iThingFactoryState> (Landscape_factory);
	Landscape->SetZBufMode(CS_ZBUF_USE);

	ColumnFrame = sbs->engine->CreateSectorWallsMesh (sbs->area, "ColumnFrame");
	ColumnFrame_object = ColumnFrame->GetMeshObject ();
	ColumnFrame_factory = ColumnFrame_object->GetFactory();
	ColumnFrame_state = scfQueryInterface<iThingFactoryState> (ColumnFrame_factory);
	ColumnFrame->SetZBufMode(CS_ZBUF_USE);
}

int SBS::AddCustomWall(csRef<iThingFactoryState> dest, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
{
	//Adds a wall from a specified array of 3D vectors
	double tw2 = tw;
	double th2;
	double tempw1;
	double tempw2;
	int num;
	int i;
	csPoly3D varray1;
	csPoly3D varray2;

	//get number of stored vertices
	num = varray.GetVertexCount();

	//Set horizontal scaling
	for (i = 0; i < num; i++)
		varray1.AddVertex(varray[i].x * HorizScale, varray[i].y, varray[i].z * HorizScale);

	//create a second array with reversed vertices
	for (i = num - 1; i >= 0; i--)
		varray2.AddVertex(varray1[i]);

	csVector2 x, y, z;

	//get extents for texture autosizing
	x = GetExtents(varray1, 1);
	y = GetExtents(varray1, 2);
	z = GetExtents(varray1, 3);

	//Call texture autosizing formulas
	if (z.x == z.y)
		tw2 = AutoSize(x.x, x.y, true, IsExternal, tw);
	if (x.x == x.y)
		tw2 = AutoSize(z.x, z.y, true, IsExternal, tw);
	if ((z.x != z.y) && (x.x != x.y))
	{
		//calculate diagonals
		tempw1 = abs(x.y - x.x);
		tempw2 = abs(z.y - z.x);
	    tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, IsExternal, tw);
	}
	th2 = AutoSize(0, abs(y.y - y.x), false, IsExternal, th);

	//create 2 polygons (front and back) from the vertex array
	int firstidx = dest->AddPolygon(varray1.GetVertices(), num);
	dest->AddPolygon(varray2.GetVertices(), num);

	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx + 1), material);
	
	//reverse extents if specified
	float tmpv;
	if (revX == true)
	{
		tmpv = x.x;
		x.x = x.y;
		x.y = tmpv;
	}
	if (revY == true)
	{
		tmpv = y.x;
		y.x = y.y;
		y.y = tmpv;
	}
	if (revZ == true)
	{
		tmpv = z.x;
		z.x = z.y;
		z.y = tmpv;
	}
	
	//texture mapping is set from 3 manual vectors (origin, width extent,
	//height extent) in a square layout
	csVector3 v1 (x.x, y.y, z.x); //top left
	csVector3 v2 (x.y, y.y, z.y); //top right
	csVector3 v3 (x.y, y.x, z.y); //bottom right

	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx),
		v1,
		csVector2 (0, 0),
		v2,
		csVector2 (tw2, 0),
		v3,
		csVector2 (tw2, th2));
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
		v1,
		csVector2 (tw2, 0),
		v2,
		csVector2 (0, 0),
		v3,
		csVector2 (0, th2));

	return firstidx;
}

int SBS::AddCustomFloor(csRef<iThingFactoryState> dest, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
{
	//Adds a wall from a specified array of 3D vectors
	double tw2 = tw;
	double th2;
	double tempw1;
	double tempw2;
	int num;
	int i;
	csPoly3D varray1;

	//get number of stored vertices
	num = varray.GetVertexCount();

	//Set horizontal scaling
	for (i = 0; i < num; i++)
		varray1.AddVertex(varray[i].x * HorizScale, varray[i].y, varray[i].z * HorizScale);

	//create a second array with reversed vertices
	for (i = num - 1; i >= 0; i--)
		varray1.AddVertex(varray1[i]);

	csVector2 x, y, z;

	//get extents for texture autosizing
	x = GetExtents(varray, 1);
	y = GetExtents(varray, 2);
	z = GetExtents(varray, 3);

	//Call texture autosizing formulas
	if (z.x == z.y)
		tw2 = AutoSize(x.x, x.y, true, IsExternal, tw);
	if (x.x == x.y)
		tw2 = AutoSize(z.x, z.y, true, IsExternal, tw);
	if ((z.x != z.y) && (x.x != x.y))
	{
		//calculate diagonals
		tempw1 = abs(x.y - x.x);
		tempw2 = abs(z.y - z.x);
	    tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, IsExternal, tw);
	}
	th2 = AutoSize(0, abs(y.y - y.x), false, IsExternal, th);

	//create 2 polygons (front and back) from the vertex array
	int firstidx = dest->AddPolygon(varray.GetVertices(), num);
	dest->AddPolygon(varray1.GetVertices(), num);

	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx + 1), material);
	
	//reverse extents if specified
	float tmpv;
	if (revX == true)
	{
		tmpv = x.x;
		x.x = x.y;
		x.y = tmpv;
	}
	if (revY == true)
	{
		tmpv = y.x;
		y.x = y.y;
		y.y = tmpv;
	}
	if (revZ == true)
	{
		tmpv = z.x;
		z.x = z.y;
		z.y = tmpv;
	}
	
	//texture mapping is set from 3 manual vectors (origin, width extent,
	//height extent) in a square layout
	csVector3 v1 (x.x, y.y, z.x); //top left
	csVector3 v2 (x.y, y.y, z.y); //top right
	csVector3 v3 (x.y, y.x, z.y); //bottom right

	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx),
		v1,
		csVector2 (0, 0),
		v2,
		csVector2 (tw2, 0),
		v3,
		csVector2 (tw2, th2));
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
		v1,
		csVector2 (tw2, 0),
		v2,
		csVector2 (0, 0),
		v3,
		csVector2 (0, th2));

	return firstidx;
}

int SBS::AddTriangleWall(csRef<iThingFactoryState> dest, const char *texture, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
{
	//Adds a triangular wall with the specified dimensions
	csPoly3D varray;

	//set up temporary vertex array
	varray.AddVertex(x1, y1, z1);
	varray.AddVertex(x2, y2, z2);
	varray.AddVertex(x3, y3, z3);

	//pass data on to AddCustomWall function
	int firstidx = AddCustomWall(dest, texture, varray, tw, th, revX, revY, revZ, IsExternal);

	return firstidx;
}

csString SBS::Calc(const char *expression)
{
	//performs a calculation operation on a string
	//for example, the string "1 + 1" would output to "2"

	int temp1;
	csString tmpcalc = expression;
	char buffer[20];
	tmpcalc.Trim();

	//general math
	temp1 = tmpcalc.Find("+", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) + atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc.GetData();
	}
	temp1 = tmpcalc.Find("-", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) - atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc.GetData();
	}
	temp1 = tmpcalc.Find("/", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) / atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc.GetData();
	}
	temp1 = tmpcalc.Find("*", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) * atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc.GetData();
	}
	
	//boolean operators
	temp1 = tmpcalc.Find("=", 0);
	if (temp1 > 0)
	{
		if (atof(tmpcalc.Slice(0, temp1)) == atof(tmpcalc.Slice(temp1 + 1)))
			return "true";
		else
			return "false";
	}
	temp1 = tmpcalc.Find("!", 0);
	if (temp1 > 0)
	{
		if (atof(tmpcalc.Slice(0, temp1)) != atof(tmpcalc.Slice(temp1 + 1)))
			return "true";
		else
			return "false";
	}
	temp1 = tmpcalc.Find("<", 0);
	if (temp1 > 0)
	{
		if (atof(tmpcalc.Slice(0, temp1)) < atof(tmpcalc.Slice(temp1 + 1)))
			return "true";
		else
			return "false";
	}
	temp1 = tmpcalc.Find(">", 0);
	if (temp1 > 0)
	{
		if (atof(tmpcalc.Slice(0, temp1)) > atof(tmpcalc.Slice(temp1 + 1)))
			return "true";
		else
			return "false";
	}
	
	return tmpcalc.GetData();
}

bool IsNumeric(const char *expression)
{
	//returns true if the string is numeric; otherwise returns false	
	csString s;
	s = expression;
	char test;
	for (int i = 0; i < s.Length(); i++)
	{
		test = s.GetAt(i);
		if((test <= '0' || test >= '9') && test != '.')
			return false;
	}
	return true;
}

void SBS::EnableBuildings(bool value)
{
	//turns buildings on/off
	if (value == true)
	{
		Buildings->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Buildings->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Buildings->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		Buildings->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Buildings->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Buildings->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	IsBuildingsEnabled = value;
}

void SBS::EnableLandscape(bool value)
{
	//turns landscape on/off
	if (value == true)
	{
		Landscape->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		Landscape->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		Landscape->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		Landscape->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		Landscape->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		Landscape->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	IsLandscapeEnabled = value;
}

void SBS::EnableExternal(bool value)
{
	//turns external on/off
	if (value == true)
	{
		External->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		External->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		External->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		External->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		External->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		External->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	IsExternalEnabled = value;
}

void SBS::EnableColumnFrame(bool value)
{
	//turns column frame on/off
	if (value == true)
	{
		ColumnFrame->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		ColumnFrame->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		ColumnFrame->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		ColumnFrame->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		ColumnFrame->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		ColumnFrame->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	IsColumnFrameEnabled = value;
}

void SBS::EnableSkybox(bool value)
{
	//turns skybox on/off
	if (value == true)
	{
		SkyBox->GetFlags().Reset (CS_ENTITY_INVISIBLEMESH);
		SkyBox->GetFlags().Reset (CS_ENTITY_NOSHADOWS);
		SkyBox->GetFlags().Reset (CS_ENTITY_NOHITBEAM);
	}
	else
	{
		SkyBox->GetFlags().Set (CS_ENTITY_INVISIBLEMESH);
		SkyBox->GetFlags().Set (CS_ENTITY_NOSHADOWS);
		SkyBox->GetFlags().Set (CS_ENTITY_NOHITBEAM);
	}
	IsSkyboxEnabled = value;
}

csVector2 SBS::GetExtents(csPoly3D &varray, int coord)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vector array.
	//first parameter must be a vector array object
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	double esmall;
	double ebig;
	double tempnum;
	int i;
	int num = varray.GetVertexCount();

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return csVector2(0, 0);

	for (i = 0; i < num; i++)
	{
		if (coord == 1)
			tempnum = varray[i].x;
		if (coord == 2)
			tempnum = varray[i].y;
		if (coord == 3)
			tempnum = varray[i].z;
		
		if (i == 0)
		{
			esmall = tempnum;
			ebig = tempnum;
		}
		else
		{
			if (tempnum < esmall)
				esmall = tempnum;
			if (tempnum > ebig)
				ebig = tempnum;
		}
	}
	
	return csVector2(esmall, ebig);
}

int SBS::CreateSky()
{
	SkyBox = (engine->CreateSectorWallsMesh (area, "SkyBox"));
	SkyBox_object = SkyBox->GetMeshObject ();
	SkyBox_factory = SkyBox_object->GetFactory();
	SkyBox_state = scfQueryInterface<iThingFactoryState> (SkyBox_factory);
	SkyBox->SetZBufMode(CS_ZBUF_USE);

	int firstidx = SkyBox_state->AddInsideBox(csVector3(-100000, -100000, -100000), csVector3(100000, 100000, 100000));
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyBack");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx, firstidx), material);
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyRight");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 1, firstidx + 1), material);
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyFront");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 2, firstidx + 2), material);
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyLeft");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 3, firstidx + 3), material);
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyBottom");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 4, firstidx + 4), material);
	material = sbs->engine->GetMaterialList ()->FindByName ("SkyTop");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 5, firstidx + 5), material);

	SkyBox_state->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx + 5),
		csVector2 (0, 1),
		csVector2 (1, 1),
		csVector2 (1, 0));

	return firstidx;
}

int SBS::GetFloorNumber(double altitude)
{
	//Returns floor number located at a specified altitude
	
	for (int i = -Basements + 1; i <= TotalFloors; i++)
	{
		if ((FloorArray[i]->Altitude > altitude) && (FloorArray[i - 1]->Altitude <= altitude))
			return i - 1;
		if ((i == TotalFloors) && (altitude > FloorArray[i]->Altitude))
			return i;
	}
}

double SBS::GetDistance(double x1, double x2, double z1, double z2)
{
	//returns the distance between 2 2D vectors

	if (z1 == z2)
		return abs(x1 - x2);
	if (x1 == x2)
		return abs(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrt(pow(abs(x1 - x2), 2) + pow(abs(z1 - z2), 2)); //calculate diagonals
}

void SBS::Fall()
{

}

void SBS::DumpVertices(csRef<iThingFactoryState> mesh)
{
	//dumps a list of vertices from a mesh object to the console/logfile

	Report("--- Vertex Dump ---\n");
	for (int i = 0; i < mesh->GetVertexCount(); i++)
		Report(csString(_itoa(i, intbuffer, 10)) + ": " + csString(_gcvt(mesh->GetVertices()[i].x, 6, buffer)) + ", " + csString(_gcvt(mesh->GetVertices()[i].y, 6, buffer)) + ", " + csString(_gcvt(mesh->GetVertices()[i].z, 6, buffer)));
}

void SBS::ListAltitudes()
{
	//dumps the floor altitude list

	Report("--- Floor Altitudes ---\n");
	for (int i = -Basements; i <= TotalFloors; i++)
		Report(csString(_itoa(i, intbuffer, 10)) + "(" + FloorArray[i]->ID + ")\t----\t" + csString(_gcvt(FloorArray[i]->FullHeight(), 6, buffer)) + "\t----\t" + csString(_gcvt(FloorArray[i]->Altitude, 6, buffer)));
}
