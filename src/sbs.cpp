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

#include <wx/timer.h>
#include <crystalspace.h>
#include "sbs.h"
#include "unix.h"

SBS *sbs; //self reference
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
	Gravity = 32.15; // 9.8 m/s/s
	IsRunning = false;
	Floors = 0;
	Basements = 0;
	RenderOnly = false;
	InputOnly = false;
	IsFalling = false;
	InStairwell = false;
	InElevator = false;
	FPSModifier = 1;
	FrameSync = true;
	EnableCollisions = true;
	BuildingFile = "";
	IsBuildingsEnabled = false;
	IsColumnFrameEnabled = false;
	IsExternalEnabled = false;
	IsLandscapeEnabled = false;
	IsSkyboxEnabled = false;
	fps_frame_count = 0;
	fps_tottime = 0;
	FPS = 0;
	AutoShafts = true;
	ElevatorSync = false;
	mouse_x = 0;
	mouse_y = 0;
	MouseDown = false;
}

SBS::~SBS()
{
	//engine destructor

	//delete camera object
	delete camera;
	camera = 0;

	//delete floors
	FloorArray.DeleteAll();	

	//delete elevators
	ElevatorArray.DeleteAll();

	//delete shafts
	ShaftArray.DeleteAll();

	//delete stairs
	StairsArray.DeleteAll();
}

void SBS::Start()
{
	//set up viewport
	view = csPtr<iView>(new csView (engine, g3d));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//create camera object
	camera = new Camera();

	//set running value
	IsRunning = true;

	//clear user variables
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

	//initialize mesh colliders
	csColliderHelper::InitializeCollisionWrappers (collision_sys, engine);

	//move camera to start location
	camera->SetToStartPosition();
	camera->SetToStartDirection();
	camera->SetToStartRotation();

	//turn on main objects
	EnableBuildings(true);
	EnableLandscape(true);
	EnableExternal(true);
	EnableColumnFrame(true);
	EnableSkybox(true);

	//turn off floors
	for (int i = 0; i < TotalFloors(); i++)
		FloorArray[i].object->Enabled(false);

	//turn off shafts
	for (int i = 0; i < Shafts(); i++)
	{
		if (ShaftArray[i].object)
			ShaftArray[i].object->EnableWholeShaft(false);
	}

	//turn on shaft elevator doors
	for (int i = 0; i < Elevators(); i++)
	{
		if (ElevatorArray[i].object)
			ElevatorArray[i].object->ShaftDoorsEnabled(camera->StartFloor, true);
	}

	//turn on first/lobby floor
	GetFloor(0)->Enabled(true);

}

void SBS::Run()
{
	//start runloop

	Report("Running simulation...");

	//start simulation
	csDefaultRunLoop (object_reg);
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
		return fabs(n1 - n2) * size1;
	else
	{
		if (external == false)
			return fabs(n1 - n2) * size2;
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

	//get mouse pointer coordinates
	mouse_x = mouse->GetLastX();
	mouse_y = mouse->GetLastY();

	//check if the user clicked on an object, and process it
	if (mouse->GetLastButton(0) == true && MouseDown == false)
	{
		MouseDown = true;
		camera->ClickedObject();
	}

	//reset mouse state
	if (mouse->GetLastButton(0) == false)
		MouseDown = false;

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
			camera->Move (CS_VEC_RIGHT, speed * 8);
		if (kbd->GetKeyState (CSKEY_LEFT))
			camera->Move (CS_VEC_LEFT, speed * 8);
		if (kbd->GetKeyState (CSKEY_UP))
			camera->Move (CS_VEC_UP, speed * 8);
		if (kbd->GetKeyState (CSKEY_DOWN))
			camera->Move (CS_VEC_DOWN, speed * 8);
	}
	else if (kbd->GetKeyState (CSKEY_ALT))
	{
		//rotate on the Z axis if the Alt key is pressed with the left/right arrows
		if (kbd->GetKeyState (CSKEY_RIGHT))
			camera->Rotate(CS_VEC_FORWARD, speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			camera->Rotate(CS_VEC_BACKWARD, speed);
	}
	else
	{
		// left and right cause the camera to rotate on the global Y
		// axis; page up and page down cause the camera to rotate on the
		// _camera's_ X axis (more on this in a second) and up and down
		// arrows cause the camera to go forwards and backwards.
		if (kbd->GetKeyState (CSKEY_RIGHT))
			camera->Rotate(CS_VEC_UP, speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			camera->Rotate(CS_VEC_DOWN, speed);
		if (kbd->GetKeyState (CSKEY_PGUP))
			camera->Rotate(CS_VEC_RIGHT, speed);
		if (kbd->GetKeyState (CSKEY_PGDN))
			camera->Rotate(CS_VEC_LEFT, speed);
		if (kbd->GetKeyState (CSKEY_UP))
		{
			double KeepAltitude;
			KeepAltitude = camera->GetPosition().y;
			camera->Move (CS_VEC_FORWARD, speed * 8);
			if (camera->GetPosition().y != KeepAltitude)
				camera->SetPosition(csVector3(camera->GetPosition().x, KeepAltitude, camera->GetPosition().z));
		}
		if (kbd->GetKeyState (CSKEY_DOWN))
		{
			double KeepAltitude;
			KeepAltitude = camera->GetPosition().y;
			camera->Move (CS_VEC_BACKWARD, speed * 8);
			if (camera->GetPosition().y != KeepAltitude)
				camera->SetPosition(csVector3(camera->GetPosition().x, KeepAltitude, camera->GetPosition().z));
		}

		if (kbd->GetKeyState (CSKEY_SPACE))
		{
			//reset view
			camera->SetToStartDirection();
			camera->SetToStartRotation();
		}

		//values from old version
		if (kbd->GetKeyState (CSKEY_HOME))
			camera->Move (CS_VEC_UP, speed * 8);
		if (kbd->GetKeyState (CSKEY_END))
			camera->Move (CS_VEC_DOWN, speed * 8);
	}
}

void SBS::Render()
{
	// Tell 3D driver we're going to display 3D things.
	if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN ))
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
		//Process gravity
		if (EnableCollisions == true)
			camera->Gravity();

		//Determine floor that the camera is on
		camera->UpdateCameraFloor();

		//run elevator handlers
		for (int i = 1; i <= Elevators(); i++)
			GetElevator(i)->MonitorLoop();

		//check if the user is in an elevator
		camera->CheckElevator();

		//Check if the user is in a shaft
		if (AutoShafts == true)
			camera->CheckShaft();

		//check if the user is outside

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
		CS_REQUEST_PLUGIN("crystalspace.collisiondetection.opcode", iCollideSystem),
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
	mouse = CS_QUERY_REGISTRY(object_reg, iMouseDriver);
	if (!mouse) return ReportError("No Mouse Driver!");
	collision_sys = csQueryRegistry<iCollideSystem> (object_reg);
	if (!collision_sys) return ReportError("No collision detection driver!");
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

void Cleanup()
{
	//cleanup
	csPrintf ("Cleaning up...\n");
	sbs = 0;
	csInitializer::DestroyApplication (object_reg);
}

int SBS::AddWallMain(csRef<iThingFactoryState> dest, const char *name, const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double altitude1, double altitude2, double tw, double th, bool revX, bool revY, bool revZ, bool DrawBothSides)
{
	//Adds a wall with the specified dimensions
	csVector3 v1 (x1, altitude1 + height_in1, z1); //left top
	csVector3 v2 (x2, altitude2 + height_in2, z2); //right top
	csVector3 v3 (x2, altitude2, z2); //right base
	csVector3 v4 (x1, altitude1, z1); //left base

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	if (DrawBothSides == true)
		dest->AddQuad(v4, v3, v2, v1);

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

	//set texture
	SetTexture(dest, firstidx, texture, DrawBothSides, tw, th);

	//set polygon names
	csString NewName;
	NewName = name;
	if (DrawBothSides == true)
		NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	if (DrawBothSides == true)
	{
		NewName = name;
		NewName.Append(":1");
		dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);
	}

	return firstidx;
}

int SBS::AddFloorMain(csRef<iThingFactoryState> dest, const char *name, const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th, bool DrawBothSides)
{
	//Adds a floor with the specified dimensions and vertical offset
	csVector3 v1 (x1, altitude1, z1); //bottom left
	csVector3 v4 (x2, altitude2, z1); //bottom right
	csVector3 v3 (x2, altitude2, z2); //top right
	csVector3 v2 (x1, altitude1, z2); //top left

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	if (DrawBothSides == true)
		dest->AddQuad(v4, v3, v2, v1);
	
	//set texture
	SetTexture(dest, firstidx, texture, DrawBothSides, tw, th);

	//set polygon names
	csString NewName;
	NewName = name;
	if (DrawBothSides == true)
		NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	if (DrawBothSides == true)
	{
		NewName = name;
		NewName.Append(":1");
		dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);
	}

	return firstidx;
}

void SBS::DeleteWall(csRef<iThingFactoryState> dest, int index)
{
	//delete wall polygons (front and back) from specified mesh
	dest->RemovePolygon(index);
	dest->RemovePolygon(index + 1);
}


void SBS::DeleteFloor(csRef<iThingFactoryState> dest, int index)
{
	//delete floor polygons (front and back) from specified mesh
	dest->RemovePolygon(index);
	dest->RemovePolygon(index + 1);
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

int SBS::CreateWallBox(csRef<iThingFactoryState> dest, const char *name, const char *texture, double x1, double x2, double z1, double z2, double height_in, double voffset, double tw, double th)
{
	//create 4 walls

	iMaterialWrapper* tm;

	int firstidx = dest->AddInsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	//set polygon names
	csString NewName;
	NewName = name;
	NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	NewName = name;
	NewName.Append(":1");
	dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);

	return firstidx;
}

int SBS::CreateWallBox2(csRef<iThingFactoryState> dest, const char *name, const char *texture, double CenterX, double CenterZ, double WidthX, double LengthZ, double height_in, double voffset, double tw, double th)
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
	tm = engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(x1 * HorizScale, voffset, z1 * HorizScale), csVector3(x2 * HorizScale, voffset + height_in, z2 * HorizScale));
	tm = engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	//set polygon names
	csString NewName;
	NewName = name;
	NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	NewName = name;
	NewName.Append(":1");
	dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);

	return firstidx;
}

void SBS::InitMeshes()
{
	//create object meshes
	Buildings = engine->CreateSectorWallsMesh (area, "Buildings");
	Buildings_state = scfQueryInterface<iThingFactoryState> (Buildings->GetMeshObject()->GetFactory());
	Buildings->SetZBufMode(CS_ZBUF_USE);

	External = engine->CreateSectorWallsMesh (area, "External");
	External_state = scfQueryInterface<iThingFactoryState> (External->GetMeshObject()->GetFactory());
	External->SetZBufMode(CS_ZBUF_USE);

	Landscape = engine->CreateSectorWallsMesh (area, "Landscape");
	Landscape_state = scfQueryInterface<iThingFactoryState> (Landscape->GetMeshObject()->GetFactory());
	Landscape->SetZBufMode(CS_ZBUF_USE);

	ColumnFrame = engine->CreateSectorWallsMesh (area, "ColumnFrame");
	ColumnFrame_state = scfQueryInterface<iThingFactoryState> (ColumnFrame->GetMeshObject()->GetFactory());
	ColumnFrame->SetZBufMode(CS_ZBUF_USE);
}

int SBS::AddCustomWall(csRef<iThingFactoryState> dest, const char *name, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
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
		tempw1 = fabs(x.y - x.x);
		tempw2 = fabs(z.y - z.x);
		tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, IsExternal, tw);
	}
	th2 = AutoSize(0, fabs(y.y - y.x), false, IsExternal, th);

	//create 2 polygons (front and back) from the vertex array
	int firstidx = dest->AddPolygon(varray1.GetVertices(), num);
	dest->AddPolygon(varray2.GetVertices(), num);

	material = engine->GetMaterialList ()->FindByName (texture);
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

	//set polygon names
	csString NewName;
	NewName = name;
	NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	NewName = name;
	NewName.Append(":1");
	dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);

	return firstidx;
}

int SBS::AddCustomFloor(csRef<iThingFactoryState> dest, const char *name, const char *texture, csPoly3D &varray, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
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
		tempw1 = fabs(x.y - x.x);
		tempw2 = fabs(z.y - z.x);
		tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, IsExternal, tw);
	}
	th2 = AutoSize(0, fabs(y.y - y.x), false, IsExternal, th);

	//create 2 polygons (front and back) from the vertex array
	int firstidx = dest->AddPolygon(varray.GetVertices(), num);
	dest->AddPolygon(varray1.GetVertices(), num);

	material = engine->GetMaterialList ()->FindByName (texture);
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

	//set polygon names
	csString NewName;
	NewName = name;
	NewName.Append(":0");
	dest->SetPolygonName(csPolygonRange(firstidx, firstidx), NewName);
	NewName = name;
	NewName.Append(":1");
	dest->SetPolygonName(csPolygonRange(firstidx + 1, firstidx + 1), NewName);

	return firstidx;
}

int SBS::AddTriangleWall(csRef<iThingFactoryState> dest, const char *name, const char *texture, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double tw, double th, bool revX, bool revY, bool revZ, bool IsExternal)
{
	//Adds a triangular wall with the specified dimensions
	csPoly3D varray;

	//set up temporary vertex array
	varray.AddVertex(x1, y1, z1);
	varray.AddVertex(x2, y2, z2);
	varray.AddVertex(x3, y3, z3);

	//pass data on to AddCustomWall function
	int firstidx = AddCustomWall(dest, name, texture, varray, tw, th, revX, revY, revZ, IsExternal);

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
	SkyBox_state = scfQueryInterface<iThingFactoryState> (SkyBox->GetMeshObject()->GetFactory());
	SkyBox->SetZBufMode(CS_ZBUF_USE);

	int firstidx = SkyBox_state->AddInsideBox(csVector3(-100000, -100000, -100000), csVector3(100000, 100000, 100000));
	material = engine->GetMaterialList ()->FindByName ("SkyBack");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx, firstidx), material);
	material = engine->GetMaterialList ()->FindByName ("SkyRight");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 1, firstidx + 1), material);
	material = engine->GetMaterialList ()->FindByName ("SkyFront");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 2, firstidx + 2), material);
	material = engine->GetMaterialList ()->FindByName ("SkyLeft");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 3, firstidx + 3), material);
	material = engine->GetMaterialList ()->FindByName ("SkyBottom");
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 4, firstidx + 4), material);
	material = engine->GetMaterialList ()->FindByName ("SkyTop");
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

	//check to see if altitude is below bottom floor
	if (altitude < GetFloor(-Basements)->Altitude)
		return -Basements;

	for (int i = -Basements + 1; i <= Floors; i++)
	{
		//check to see if altitude is within a floor (between the current floor's base and
		//the lower floor's base)
		if ((GetFloor(i)->Altitude > altitude) && (GetFloor(i - 1)->Altitude <= altitude))
			return i - 1;
		//check to see if altitude is above top floor's altitude
		if ((i == Floors) && (altitude > GetFloor(i)->Altitude))
			return i;
	}
}

double SBS::GetDistance(double x1, double x2, double z1, double z2)
{
	//returns the distance between 2 2D vectors

	if (z1 == z2)
		return fabs(x1 - x2);
	if (x1 == x2)
		return fabs(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(z1 - z2), 2)); //calculate diagonals
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
	for (int i = -Basements; i <= Floors; i++)
		Report(csString(_itoa(i, intbuffer, 10)) + "(" + GetFloor(i)->ID + ")\t----\t" + csString(_gcvt(GetFloor(i)->FullHeight(), 6, buffer)) + "\t----\t" + csString(_gcvt(GetFloor(i)->Altitude, 6, buffer)));
}

void SBS::CreateShaft(int number, int type, double CenterX, double CenterZ, int _startfloor, int _endfloor)
{
	//create a shaft object

	for (int i = 0; i < ShaftArray.GetSize(); i++)
		if (ShaftArray[i].number == number)
			return;
	ShaftArray.SetSize(ShaftArray.GetSize() + 1);
	ShaftArray[ShaftArray.GetSize() - 1].number = number;
	ShaftArray[ShaftArray.GetSize() - 1].object = new Shaft(number, type, CenterX, CenterZ, _startfloor, _endfloor);
}

void SBS::CreateStairwell(int number, double CenterX, double CenterZ, int _startfloor, int _endfloor)
{
	//create a stairwell object

	for (int i = 0; i < StairsArray.GetSize(); i++)
		if (StairsArray[i].number == number)
			return;
	StairsArray.SetSize(StairsArray.GetSize() + 1);
	StairsArray[StairsArray.GetSize() - 1].number = number;
	StairsArray[StairsArray.GetSize() - 1].object = new Stairs(number, CenterX, CenterZ, _startfloor, _endfloor);
}

iMaterialWrapper *SBS::ChangeTexture(iMeshObject *mesh, csRef<iMaterialWrapper> oldmat, const char *texture)
{
	//changes a texture

	csRef<iThingState> thingstate = scfQueryInterface<iThingState> (mesh);

	//get new material
	csRef<iMaterialWrapper> newmat = engine->GetMaterialList()->FindByName(texture);

	//switch material
	thingstate->ClearReplacedMaterials();
	thingstate->ReplaceMaterial(oldmat, newmat);

	return newmat;
}

void SBS::SetTexture(csRef<iThingFactoryState> mesh, int index, const char *texture, bool BothSides, double tw, double th)
{
	//sets a polygon's texture

	material = engine->GetMaterialList()->FindByName(texture);

	mesh->SetPolygonMaterial(csPolygonRange(index, index), material);
	if (BothSides == true)
		mesh->SetPolygonMaterial(csPolygonRange(index + 1, index + 1), material);

	//texture mapping is set from first 3 coordinates
	mesh->SetPolygonTextureMapping (csPolygonRange(index, index),
		mesh->GetPolygonVertex(index, 0),
		csVector2 (0, 0),
		mesh->GetPolygonVertex(index, 1),
		csVector2 (tw, 0),
		mesh->GetPolygonVertex(index, 2),
		csVector2 (tw, th));
	if (BothSides == true)
	{
		mesh->SetPolygonTextureMapping (csPolygonRange(index + 1, index + 1),
			mesh->GetPolygonVertex(index + 1, 3),
			csVector2 (tw, 0),
			mesh->GetPolygonVertex(index + 1, 2),
			csVector2 (0, 0),
			mesh->GetPolygonVertex(index + 1, 1),
			csVector2 (0, th));
	}
}

void SBS::NewElevator(int number)
{
	//create a new elevator object
	for (int i = 0; i < ElevatorArray.GetSize(); i++)
		if (ElevatorArray[i].number == number)
			return;
	ElevatorArray.SetSize(ElevatorArray.GetSize() + 1);
	ElevatorArray[ElevatorArray.GetSize() - 1].number = number;
	ElevatorArray[ElevatorArray.GetSize() - 1].object = new Elevator(number);
}

void SBS::NewFloor(int number)
{
	//create a new floor object
	for (int i = 0; i < FloorArray.GetSize(); i++)
		if (FloorArray[i].number == number)
			return;
	FloorArray.SetSize(FloorArray.GetSize() + 1);
	FloorArray[FloorArray.GetSize() - 1].number = number;
	FloorArray[FloorArray.GetSize() - 1].object = new Floor(number);

	if (number < 0)
		Basements++;
	else
		Floors++;
}

int SBS::Elevators()
{
	//return the number of elevators
	return ElevatorArray.GetSize();
}

int SBS::TotalFloors()
{
	//return the number of floors
	return FloorArray.GetSize();
}

int SBS::Shafts()
{
	//return the number of shafts
	return ShaftArray.GetSize();
}

int SBS::StairsNum()
{
	//return the number of stairs
	return StairsArray.GetSize();
}

Floor *SBS::GetFloor(int number)
{
	//return pointer to floor object

	for (int i = 0; i < FloorArray.GetSize(); i++)
		if (FloorArray[i].number == number)
			return FloorArray[i].object;
	return 0;
}

Elevator *SBS::GetElevator(int number)
{
	//return pointer to elevator object

	for (int i = 0; i < ElevatorArray.GetSize(); i++)
		if (ElevatorArray[i].number == number)
			return ElevatorArray[i].object;
	return 0;
}

Shaft *SBS::GetShaft(int number)
{
	//return pointer to shaft object

	for (int i = 0; i < ShaftArray.GetSize(); i++)
		if (ShaftArray[i].number == number)
			return ShaftArray[i].object;
	return 0;
}

Stairs *SBS::GetStairs(int number)
{
	//return pointer to stairs object

	for (int i = 0; i < StairsArray.GetSize(); i++)
		if (StairsArray[i].number == number)
			return StairsArray[i].object;
	return 0;
}
