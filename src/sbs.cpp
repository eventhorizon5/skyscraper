/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.1 Alpha
	Copyright ©2005-2006 Ryan Thoryk
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

#include <crystalspace.h>
#include <sstream>
#include "sbs.h"
#include "camera.h"

CS_IMPLEMENT_APPLICATION

SBS *sbs; //self reference
Camera *c; //camera object

iObjectRegistry* object_reg;

SBS::SBS()
{
    sbs = this;

	//Set default horizontal scaling value
	HorizScale = 1;

	//Set feet scale value
	Feet = 3.125;

	//Set default starting elevator
	ElevatorNumber = 1;

	//Set default frame rate
	FrameRate = 30;
	FrameLimiter = true;

	//initialize other variables
	BuildingName = "";
	BuildingDesigner = "";
	BuildingLocation = "";
	BuildingDescription = "";
	BuildingVersion = "";
	Gravity = 0;
	IsRunning = false;
	ElevatorShafts = 0;
	TotalFloors = 0;
	Basements = 0;
	Elevators = 0;
	PipeShafts = 0;
	StairsNum = 0;
	RenderOnly = false;
	InputOnly = false;
	IsFalling = false;
	FallRate = 0;
	InStairwell = false;
	InElevator = false;
	FPSModifier = 0;
	FrameSync = false;
	EnableCollisions = false;
	BuildingFile = "";
}

SBS::~SBS()
{
	//engine deconstructor
	int i;
	delete c;
	c = 0;
	UserVariable.DeleteAll();

	for (i = -Basements; i <= TotalFloors; i++)
		delete &FloorArray[i];

	for (i = 1; i <= Elevators; i++)
		delete &ElevatorArray[i];

	FloorArray.DeleteAll();
	ElevatorArray.DeleteAll();
	sbs = 0;

	//delete objects
	if (Buildings)
		delete Buildings;

	if (External)
		delete External;

	if (Landscape)
		delete Landscape;

	if (ColumnFrame)
		delete ColumnFrame;
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
	BuildingFile.Insert(0, "/root/");
	LoadBuilding(BuildingFile.GetData());
	//if (LoadBuilding(BuildingFile.GetData()) != 0)

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

	//turn off floors
	for (int i=-Basements; i<=TotalFloors; i++)
		FloorArray[i]->Enabled(false);
	
	//turn on first/lobby floor
	FloorArray[0]->Enabled(true);

	//create skybox

	//start simulation
	csDefaultRunLoop (object_reg);

}

void SBS::Wait(long milliseconds)
{

}

double AutoSize(double n1, double n2, bool iswidth, bool external, double offset)
{
//Texture autosizing formulas
//If any of the texture parameters are 0, then automatically size the
//texture using sizing algorithms

	double size1;
	double size2;

	if (offset == 0)
		offset = 1;

	if (external == false)
	{
		size1 = 0.086 * offset;
		size2 = 0.08 * offset;
	}
	else
	{
		size1 = 0.023 * offset;
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
	Report("Scalable Building Simulator 0.1");
	Report("Copyright 2004-2006 Ryan Thoryk");
	Report("This software comes with ABSOLUTELY NO WARRANTY. This is free");
	Report("software, and you are welcome to redistribute it under certain");
	Report("conditions. For details, see the file gpl.txt");
}

void SBS::SlowToFPS(long FrameRate)
{

}

void SBS::SetupFrame()
{
	// First get elapsed time from the virtual clock.
	csTicks elapsed_time = vc->GetElapsedTicks ();
	// Now rotate the camera according to keyboard state
	double speed = (elapsed_time / 1000.0) * (0.06 * 20);

	if (kbd->GetKeyState (CSKEY_CTRL))
		speed *= 2;

	if (kbd->GetKeyState (CSKEY_SHIFT))
	{
		// If the user is holding down shift, the arrow keys will cause
		// the camera to strafe up, down, left or right from it's
		// current position.
		if (kbd->GetKeyState (CSKEY_RIGHT))
			c->Move (CS_VEC_RIGHT * 4 * speed);
		if (kbd->GetKeyState (CSKEY_LEFT))
			c->Move (CS_VEC_LEFT * 4 * speed);
		if (kbd->GetKeyState (CSKEY_UP))
			c->Move (CS_VEC_UP * 4 * speed);
		if (kbd->GetKeyState (CSKEY_DOWN))
			c->Move (CS_VEC_DOWN * 4 * speed);
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
			c->Move (CS_VEC_FORWARD * 4 * speed);
		if (kbd->GetKeyState (CSKEY_DOWN))
			c->Move (CS_VEC_BACKWARD * 4 * speed);

		if (kbd->GetKeyState (CSKEY_SPACE))
		{
			c->SetToStartDirection();
			c->SetToStartRotation();
		}
	}

	// Tell 3D driver we're going to display 3D things.
	//if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER ))
	if (!g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN )) //clear screen also
		return;

	// Tell the camera to render into the frame buffer.
	view->Draw ();
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

		SetupFrame ();
		return true;
	}
	else if (Event.Name == FinalProcess)
	{
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

	stdrep = CS_QUERY_REGISTRY (object_reg, iStandardReporterListener);
	if (!stdrep) return ReportError ("No stdrep plugin!");
	stdrep->SetDebugFile ("/tmp/sbs_report.txt");
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_BUG, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_ERROR, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_WARNING, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_NOTIFY, true, false, false, false, true, false);
	stdrep->SetMessageDestination (CS_REPORTER_SEVERITY_DEBUG, true, false, false, false, true, false);

	//mount app's directory in VFS
	vfs->Mount("/root/", csInstallationPathsHelper::GetAppDir(argv[0]) + "\\");
	
	iNativeWindow* nw = g2d->GetNativeWindow();
	if (nw) nw->SetTitle(windowtitle);

	font = g2d->GetFontServer()->LoadFont(CSFONT_LARGE);

	PrintBanner();

	csSleep(3000);

	// Open the main system. This will open all the previously loaded plug-ins.
	if (!csInitializer::OpenApplication (object_reg))
		return ReportError ("Error opening system!");

	// First disable the lighting cache. Our app is simple enough
	// not to need this.
		engine->SetLightingCacheMode (0);
		engine->SetAmbientLight(csColor(0.5, 0.5, 0.5));

	//create 3D environment
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
	csInitializer::DestroyApplication (object_reg);
}

void SBS::AddWallMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double height_in1, double height_in2, double altitude1, double altitude2, double tw, double th)
{
	//Adds a wall with the specified dimensions
	csVector3 v1 (Feet * x1, Feet * (altitude1 + height_in1), Feet * z1); //left top
	csVector3 v2 (Feet * x2, Feet * (altitude2 + height_in2), Feet * z2); //right top
	csVector3 v3 (Feet * x2, Feet * altitude2, Feet * z2); //right base
	csVector3 v4 (Feet * x1, Feet * altitude1, Feet * z1); //left base

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	dest->AddQuad(v4, v3, v2, v1);
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx + 1), material);
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx),
		csVector2 (0, 0),
		csVector2 (tw, 0),
		csVector2 (tw, th));
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
		csVector2 (0, th),
		csVector2 (tw, th),
		csVector2 (tw, 0));
}

void SBS::AddFloorMain(csRef<iThingFactoryState> dest, const char *texture, double x1, double z1, double x2, double z2, double altitude1, double altitude2, double tw, double th)
{
	//Adds a floor with the specified dimensions and vertical offset
	csVector3 v1 (Feet * x1, Feet * altitude1, Feet * z1); //bottom left
	csVector3 v4 (Feet * x2, Feet * altitude2, Feet * z1); //bottom right
	csVector3 v3 (Feet * x2, Feet * altitude2, Feet * z2); //top right
	csVector3 v2 (Feet * x1, Feet * altitude1, Feet * z2); //top left

	int firstidx = dest->AddQuad(v1, v2, v3, v4);
	dest->AddQuad(v4, v3, v2, v1);
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (csPolygonRange(firstidx, firstidx + 1), material);
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx + 1), 
		csVector2 (0, 0),
		csVector2 (tw, 0),
		csVector2 (tw, th));
	dest->SetPolygonTextureMapping (csPolygonRange(firstidx + 1, firstidx + 1),
		csVector2 (0, th),
		csVector2 (tw, th),
		csVector2 (tw, 0));
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

void SBS::CreateWallBox(csRef<iThingFactoryState> dest, const char *texture, double x1, double x2, double z1, double z2, double height_in, double voffset, double tw, double th)
{
	//create 4 walls
	
	iMaterialWrapper* tm;
	
	dest->AddInsideBox(csVector3(Feet * x1, Feet * voffset, Feet * z1), csVector3(Feet * x2, Feet * (voffset + height_in), Feet * z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(Feet * x1, Feet * voffset, Feet * z1), csVector3(Feet * x2, Feet * (voffset + height_in), Feet * z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below
}

void SBS::CreateWallBox2(csRef<iThingFactoryState> dest, const char *texture, double CenterX, double CenterZ, double WidthX, double LengthZ, double height_in, double voffset, double tw, double th)
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

	dest->AddInsideBox(csVector3(Feet * x1, Feet * voffset, Feet * z1), csVector3(Feet * x2, Feet * (voffset + height_in), Feet * z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below

	dest->AddOutsideBox(csVector3(Feet * x1, Feet * voffset, Feet * z1), csVector3(Feet * x2, Feet * (voffset + height_in), Feet * z2));
	tm = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3); //see todo below
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

	//create object meshes
	Buildings = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Buildings"));
	Buildings_object = Buildings->GetMeshObject ();
	Buildings_factory = Buildings_object->GetFactory();
	Buildings_state = scfQueryInterface<iThingFactoryState> (Buildings_factory);
	Buildings->SetZBufMode(CS_ZBUF_USE);

	External = (sbs->engine->CreateSectorWallsMesh (sbs->area, "External"));
	External_object = External->GetMeshObject ();
	External_factory = External_object->GetFactory();
	External_state = scfQueryInterface<iThingFactoryState> (External_factory);
	External->SetZBufMode(CS_ZBUF_USE);

	Landscape = (sbs->engine->CreateSectorWallsMesh (sbs->area, "Landscape"));
	Landscape_object = Landscape->GetMeshObject ();
	Landscape_factory = Landscape_object->GetFactory();
	Landscape_state = scfQueryInterface<iThingFactoryState> (Landscape_factory);
	Landscape->SetZBufMode(CS_ZBUF_USE);

	ColumnFrame = (sbs->engine->CreateSectorWallsMesh (sbs->area, "ColumnFrame"));
	ColumnFrame_object = ColumnFrame->GetMeshObject ();
	ColumnFrame_factory = ColumnFrame_object->GetFactory();
	ColumnFrame_state = scfQueryInterface<iThingFactoryState> (ColumnFrame_factory);
	ColumnFrame->SetZBufMode(CS_ZBUF_USE);

}

/*void SBS::AddPolygonWall(csRef<iThingFactoryState> dest, const char *texture, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double FloorHeight, double tw, double th, bool IsExternal)
{
	//Adds a wall from specified vertices

	dest->AddQuad(csVector3(Feet * x1, Feet * altitude1, Feet * z1), csVector3(Feet * x1, Feet * (altitude1 + height_in1), Feet * z1), csVector3(Feet * x2, Feet * (altitude2 + height_in2), Feet * z2), csVector3(Feet * x2, Feet * altitude2, Feet * z2));
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, material);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
}
*/

void SBS::AddTriangleWall(csRef<iThingFactoryState> dest, const char *texture, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double FloorHeight, double tw, double th, bool IsExternal)
{
	//Adds a triangular wall with the specified dimensions
	//***note - possibly replace this with the AddPolygonWall function

	//Set horizontal scaling
	x1 = x1 * HorizScale;
	x2 = x2 * HorizScale;
	x3 = x3 * HorizScale;
	z1 = z1 * HorizScale;
	z2 = z2 * HorizScale;
	z3 = z3 * HorizScale;

	dest->AddTriangle(csVector3(Feet * x1, Feet * y1, Feet * z1), csVector3(Feet * x2, Feet * y2, Feet * z2), csVector3(Feet * x3, Feet * y3, Feet * z3));
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, material);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
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
		return tmpcalc;
	}
	temp1 = tmpcalc.Find("-", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) - atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc;
	}
	temp1 = tmpcalc.Find("/", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) / atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc;
	}
	temp1 = tmpcalc.Find("*", 0);
	if (temp1 > 0)
	{
		tmpcalc = _gcvt(atof(tmpcalc.Slice(0, temp1).GetData()) * atof(tmpcalc.Slice(temp1 + 1).GetData()), 12, buffer);
		if (tmpcalc.GetAt(tmpcalc.Length() - 1) == '.')
			tmpcalc = tmpcalc.Slice(0, tmpcalc.Length() - 1); //strip of extra decimal point if even
		return tmpcalc;
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
}
