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
#include "sbs.h"

CS_IMPLEMENT_APPLICATION

SBS *sbs; //self reference
iObjectRegistry* object_reg;

SBS::SBS()
{
    sbs = this;
}

SBS::~SBS()
{
}

void SBS::Start()
{
	//Post-init startup code goes here, before the runloop
	engine->Prepare();

	//set up initial camera position
	view = csPtr<iView>(new csView (engine, g3d));
	view->GetCamera()->SetSector(area);
	view->GetCamera()->GetTransform().SetOrigin(csVector3(startposition.x, startposition.y, startposition.z));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//set main simulation values
	InputOnly = false;
	RenderOnly = false;

	//start simulation
	csDefaultRunLoop (object_reg);

}

void SBS::Wait(long milliseconds)
{

}

float AutoSize(float n1, float n2, bool iswidth)
{
//Texture autosizing formulas
//If any of the texture parameters are 0, then automatically size the
//texture using sizing algorithms
	if (((n1 < 0) && (n2 < 0)) || ((n1 >= 0) && (n2 >= 0)))
	{
		//if numbers have the same sign
	    if (abs(n1) >= abs(n2))
		{
			if (iswidth == true)
				return (abs(n1) - abs(n2)) * 0.086;
			if (iswidth == false)
				return (abs(n1) - abs(n2)) * 0.08;
		}
		else
		{
			if (iswidth == true)
				return (abs(n2) - abs(n1)) * 0.086;
			if (iswidth == false)
				return (abs(n2) - abs(n1)) * 0.08;
		}
	}
	else
	{
		//if numbers have different signs
	    if (n1 > n2)
		{
			if (iswidth == true)
				return (abs(n1) + abs(n2)) * 0.086;
			if (iswidth == false)
				return (abs(n1) + abs(n2)) * 0.08;
		}
		else
		{
			if (iswidth == true)
				return (abs(n2) + abs(n1)) * 0.086;
			if (iswidth == false)
				return (abs(n2) + abs(n1)) * 0.08;
		}
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
	float speed = (elapsed_time / 1000.0) * (0.06 * 20);

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
			rotY += speed;
		if (kbd->GetKeyState (CSKEY_LEFT))
			rotY -= speed;
		if (kbd->GetKeyState (CSKEY_PGUP))
			rotX += speed;
		if (kbd->GetKeyState (CSKEY_PGDN))
			rotX -= speed;
		if (kbd->GetKeyState (CSKEY_UP))
			c->Move (CS_VEC_FORWARD * 4 * speed);
		if (kbd->GetKeyState (CSKEY_DOWN))
			c->Move (CS_VEC_BACKWARD * 4 * speed);
	}

	// We now assign a new rotation transformation to the camera.  You
	// can think of the rotation this way: starting from the zero
	// position, you first rotate "rotY" radians on your Y axis to get
	// the first rotation.  From there you rotate "rotX" radians on the
	// your X axis to get the final rotation.  We multiply the
	// individual rotations on each axis together to get a single
	// rotation matrix.  The rotations are applied in right to left
	// order .
	csMatrix3 rot = csXRotMatrix3 (rotX) * csYRotMatrix3 (rotY);
	csOrthoTransform ot (rot, c->GetTransform().GetOrigin ());
	c->SetTransform (ot);

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
		sbs->elapsed_time = sbs->vc->GetElapsedTicks ();

		//get camera object
		sbs->c = sbs->view->GetCamera();

		sbs->SetupFrame ();
		return true;
	}
	else if (Event.Name == FinalProcess)
	{
		sbs->FinishFrame ();
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

	// these are used store the current orientation of the camera
	rotY = rotX = 0;

	//create 3D environment
	area = engine->CreateSector("area");

	return true;
}

bool SBS::LoadTexture(const char *name, const char *filename)
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

void SBS::AddLight(const char *name, float x, float y, float z, float radius, float r, float g, float b)
{
	ll = area->GetLights();
	light = engine->CreateLight(name, csVector3(x, y, z), radius, csColor(r, g, b));
	ll->Add(light);
}

void SBS::SetStartPosition(float x, float y, float z)
{
	startposition.x = x;
	startposition.y = y;
	startposition.z = z;
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

void SBS::AddWall(csRef<iThingFactoryState> dest, const char *texture, float x1, float z1, float x2, float z2, float wallheight, float altitude, float tw, float th)
{
	//Adds a wall with the specified dimensions
	dest->AddQuad(csVector3(x1, altitude, z1), csVector3(x2, altitude, z2), csVector3(x1, altitude + wallheight, z1), csVector3(x2, altitude + wallheight, z2));
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, material);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
}

void SBS::AddFloor(csRef<iThingFactoryState> dest, const char *texture, float x1, float z1, float x2, float z2, float altitude, float tw, float th)
{
	//Adds a floor with the specified dimensions and vertical offset
	dest->AddQuad(csVector3(x1, altitude, z1), csVector3(x2, altitude, z1), csVector3(x1, altitude, z2), csVector3(x2, altitude, z2));
	material = sbs->engine->GetMaterialList ()->FindByName (texture);
	dest->SetPolygonMaterial (CS_POLYRANGE_LAST, material);
	dest->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
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
