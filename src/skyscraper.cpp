/* $Id$ */

/*
	Skyscraper 1.3 Alpha - Simulation Frontend
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
#include "ivideo/wxwin.h"
#include "globals.h"
#include "skyscraper.h"
#include "sbs.h"
#include "debugpanel.h"
#include "loader.h"

CS_IMPLEMENT_APPLICATION
IMPLEMENT_APP(Skyscraper)

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

#ifdef CS_PLATFORM_WIN32

#ifndef SW_SHOWNORMAL
	#define SW_SHOWNORMAL 1
#endif

int main (int argc, char* argv[])
{
	return WinMain(GetModuleHandle(0), 0, GetCommandLineA(), SW_SHOWNORMAL);
}

#endif

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

	//Create main window
	window = new MainScreen();
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
	{
		ReportError("Error initializing Crystal Space");
		return false;
	}

	//draw background
	//DrawBackground();
	//StartupRunning = true;
	//return true;

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

	//Create new simulator object
	Simcore = new SBS();

	#if defined(wxUSE_UNICODE) && wxUSE_UNICODE
	BuildingFile = Selector->GetFilename().mb_str().data();
	#else
	BuildingFile = Selector->GetFilename();
	#endif

	//initialize SBS
	Simcore->Initialize(iSCF::SCF, engine, loader, vc, view, vfs, collision_sys, rep, sndrenderer, sndloader, material, area, root_dir.GetData(), dir_char.GetData());

	//load building data file
	Simcore->Report("\nLoading building data from " + BuildingFile + "...\n");

	//Pause for 1 second
	csSleep(1000);

	BuildingFile.Insert(0, "/root/buildings/");
	LoadBuilding(BuildingFile.GetData());
	//if (LoadBuilding(BuildingFile.GetData()) != 0)

	//delete dialog
	delete Selector;
	Selector = 0;

	//the sky needs to be created before Prepare() is called
	Simcore->CreateSky(Simcore->SkyName);

	engine->Prepare();

	//start simulation
	Simcore->Start();

	//load dialogs
	dpanel = new DebugPanel(NULL, -1);
	dpanel->Show(true);
	dpanel->SetPosition(wxPoint(10, 10));
	window->Raise();

	//show main window
	window->ShowWindow();

	//run simulation
	Simcore->Report("Running simulation...");
	Simcore->IsRunning = true;
	IsRunning = true;
	StartupRunning = false;

	return true;
}

int Skyscraper::OnExit()
{
	//clean up

	//delete frame printer object
	printer.Invalidate();

	//delete wx canvas
	delete canvas;
	canvas = 0;

	if(dpanel)
	{
		if(dpanel->timer)
			dpanel->timer->Stop();
		dpanel->Destroy();
	}
	delete Simcore;
	Simcore = 0;
	delete window;
	window = 0;
	skyscraper = 0;

	//cleanup
	csPrintf ("Cleaning up...\n");

	//clear references
	wxwin = 0;
	area = 0;
	engine = 0;
	loader = 0;
	g3d = 0;
	g2d = 0;
	kbd = 0;
	vc = 0;
	view = 0;
	console = 0;
	font = 0;
	vfs = 0;
	imageio = 0;
	cmdline = 0;
	strings = 0;
	stdrep = 0;
	equeue = 0;
	plug = 0;
	collision_sys = 0;
	mouse = 0;
	rep = 0;
	sndrenderer = 0;
	sndloader = 0;
	material = 0;
	image = 0;

	csInitializer::DestroyApplication (object_reg);

	return 0;
}

MainScreen::MainScreen() : wxFrame(0, -1, wxT("Skyscraper 1.3 Alpha"), wxDefaultPosition, wxSize(640, 480), wxDEFAULT_FRAME_STYLE)
{
	this->Center();
	new wxPanel(this, -1, wxPoint(0, 0), wxSize(1, 1));
	panel = new wxPanel(this, -1, wxPoint(0, 0), this->GetClientSize());
	//this->SetTitle(wxString::FromAscii(windowtitle));
}

MainScreen::~MainScreen()
{

}

void MainScreen::OnIconize(wxIconizeEvent& event)
{
	//csPrintf("got iconize %d\n", (int)event.Iconized());
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
	if (skyscraper->IsRunning == true || skyscraper->StartupRunning == true)
		skyscraper->PushFrame();
	event.RequestMore();
}

void Skyscraper::Render()
{
	// Tell 3D driver we're going to display 3D things.
	if (Simcore->IsSkyboxEnabled == false)
	{
		if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER | CSDRAW_CLEARSCREEN))
			return;
	}
	else
	{
		if (!g3d->BeginDraw(engine->GetBeginDrawFlags() | CSDRAW_3DGRAPHICS | CSDRAW_CLEARZBUFFER))
			return;
	}

	// Tell the camera to render into the frame buffer.
	view->Draw();
}

void Skyscraper::SetupFrame()
{
	//Main simulator loop
	if (IsRunning == false)
	{
		GetMenuInput();
		RenderMenu();
		return;
	}

	//resize canvas if needed
	if (canvas->GetSize().GetWidth() != canvas_width || canvas->GetSize().GetHeight() != canvas_height)
	{
		//update canvas size values
		canvas_width = canvas->GetSize().GetWidth();
		canvas_height = canvas->GetSize().GetHeight();

		//resize viewport
		view->SetAutoResize(false);
		float oldfov = view->GetCamera()->GetFOVAngle();
		wxwin->GetWindow()->SetSize(canvas->GetSize());
		view->GetCamera()->SetFOVAngle(oldfov, canvas_width);
		view->GetCamera()->SetPerspectiveCenter(canvas_width / 2, canvas_height / 2);
		view->SetRectangle(0, 0, canvas_width, canvas_height);
		g3d->SetDimensions(canvas_width, canvas_height);
		view->ClearView();
	}

	Simcore->RenderOnly = RenderOnly;
	Simcore->InputOnly = InputOnly;

	//run SBS main loop
	Simcore->MainLoop();

	if (RenderOnly == false)
		GetInput();

	//process camera loop
	Simcore->camera->Loop();

	Render();
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

	if (!csInitializer::RequestPlugins(object_reg,
		CS_REQUEST_VFS,
		CS_REQUEST_PLUGIN("crystalspace.graphics2d.wxgl", iGraphics2D),
		CS_REQUEST_OPENGL3D,
		CS_REQUEST_ENGINE,
		CS_REQUEST_FONTSERVER,
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
	vfs = csQueryRegistry<iVFS> (object_reg);
	if (!vfs) return ReportError ("Failed to locate VFS");
	console = csQueryRegistry<iConsoleOutput> (object_reg);
	if (!console) return ReportError ("Failed to locate console output driver");
	mouse = csQueryRegistry<iMouseDriver> (object_reg);
	if (!mouse) return ReportError("Failed to locate mouse driver");
	collision_sys = csQueryRegistry<iCollideSystem> (object_reg);
	if (!collision_sys) return ReportError("Failed to locate collision detection driver");
	sndrenderer = csQueryRegistry<iSndSysRenderer> (object_reg);
	if (!sndrenderer) return ReportError("Failed to locate sound renderer");
	sndloader = csQueryRegistry<iSndSysLoader> (object_reg);
	if (!sndloader) return ReportError("Failed to locate sound loader");
	plug = csLoadPluginAlways (plugin_mgr, "crystalspace.utilities.bugplug");
	if (!plug) return ReportError ("Failed to locate BugPlug!");
	if (plug) plug->IncRef ();
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

	//mount app's directory in VFS
	#ifndef CS_PLATFORM_WIN32
		dir_char = "/";
	#else
		dir_char = "\\";
	#endif
	root_dir = csInstallationPathsHelper::GetAppDir(argv[0]) + dir_char;
	vfs->Mount("/root/", root_dir);

	g2d = g3d->GetDriver2D();
	g2d->AllowResize(true); //allow canvas resizing
	wxwin = scfQueryInterface<iWxWindow> (g2d);
	if(!wxwin) return ReportError("Canvas is no iWxWindow plugin!");
	wxwin->SetParent(RenderObject);
	canvas = RenderObject;
	canvas_width = canvas->GetSize().GetWidth();
	canvas_height = canvas->GetSize().GetHeight();

	//font = g2d->GetFontServer()->LoadFont(CSFONT_LARGE);

	// Open the main system. This will open all the previously loaded plug-ins.
	if (!csInitializer::OpenApplication (object_reg))
		return ReportError ("Error opening system!");

	//initialize frame printer
	printer.AttachNew(new FramePrinter(object_reg));

	//initialize event queue
	equeue = csQueryRegistry<iEventQueue> (object_reg);

	// First disable the lighting cache. Our app is simple enough
	// not to need this.
	engine->SetLightingCacheMode (0);
	engine->SetAmbientLight(csColor(0.5, 0.5, 0.5));

	//create 3D environments
	area = engine->CreateSector("area");

	//set up viewport
	view = csPtr<iView>(new csView (engine, g3d));
	view->SetRectangle(0, 0, g2d->GetWidth(), g2d->GetHeight());

	//clear user variables
	UserVariable.SetSize(256);

	return true;
}

void Skyscraper::GetInput()
{
	// First get elapsed time from the virtual clock.
	elapsed_time = vc->GetElapsedTicks ();
	current_time = vc->GetCurrentTicks ();

	// Now rotate the camera according to keyboard state
	//float speed = elapsed_time / 1000.0f;

	//fix for the camera velocities due to the non-event driven key system
	Simcore->camera->desired_velocity.Set(0, 0, 0);
	Simcore->camera->desired_angle_velocity.Set(0, 0, 0);

	//get mouse pointer coordinates
	Simcore->mouse_x = mouse->GetLastX();
	Simcore->mouse_y = mouse->GetLastY();

	//check if the user clicked on an object, and process it
	if (mouse->GetLastButton(0) == true && MouseDown == false)
	{
		MouseDown = true;
		Simcore->camera->ClickedObject();
	}

	//reset mouse state
	if (mouse->GetLastButton(0) == false)
		MouseDown = false;

	//if (wxGetKeyState(WXK_ESCAPE))

	if (wxGetKeyState(WXK_F2))
		Report(wxVariant(Simcore->FPS).GetString().ToAscii());

	Simcore->camera->speed = 1;

	if (wxGetKeyState(WXK_CONTROL))
		Simcore->camera->speed = 0.5;
	else if (wxGetKeyState(WXK_SHIFT))
		Simcore->camera->speed = 2;

	if (wxGetKeyState(WXK_ALT))
	{
		//strafe movement
		if (wxGetKeyState(WXK_RIGHT))
			Simcore->camera->Strafe(1);
		if (wxGetKeyState(WXK_LEFT))
			Simcore->camera->Strafe(-1);
		if (wxGetKeyState(WXK_UP))
			Simcore->camera->Float(1);
		if (wxGetKeyState(WXK_DOWN))
			Simcore->camera->Float(-1);
	}
	else
	{
		if (wxGetKeyState(WXK_RIGHT))
			Simcore->camera->Turn(1);
		if (wxGetKeyState(WXK_LEFT))
			Simcore->camera->Turn(-1);
		if (wxGetKeyState(WXK_PRIOR)) //page up
			Simcore->camera->Look(1);
		if (wxGetKeyState(WXK_NEXT)) //page down
			Simcore->camera->Look(-1);
		if (wxGetKeyState(WXK_UP))
			Simcore->camera->Step(1);
		if (wxGetKeyState(WXK_DOWN))
			Simcore->camera->Step(-1);
		if (wxGetKeyState(WXK_SPACE))
			Simcore->camera->Jump();
		if (wxGetKeyState(WXK_F3))
		{
			//reset view
			Simcore->camera->SetToStartDirection();
			Simcore->camera->SetToStartRotation();
		}

		//values from old version
		if (wxGetKeyState(WXK_HOME))
			Simcore->camera->Float(1);
		if (wxGetKeyState(WXK_END))
			Simcore->camera->Float(-1);
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
	csRef<iFile> imagefile = vfs->Open("/root/data/menu.png", VFS_FILE_READ);
	if (imagefile.IsValid())
	{
		csRef<iDataBuffer> filedata = imagefile->GetAllData();
		image = imageio->Load(filedata, CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);
	}
	int w = g2d->GetWidth();
	int h = g2d->GetHeight();
	int w2 = image->GetWidth();
	int h2 = image->GetHeight();
	const int bx = (w - w2) / 2;
	const int by = h / 2;
	if (!g3d->BeginDraw(CSDRAW_2DGRAPHICS))
		return;
	g2d->Clear(0);
	g2d->SetClipRect(0, 0, w, h);
	if (image.IsValid())
		g2d->Blit(0, 0, w2, h2, (unsigned char*)image->GetImageData());
}

void Skyscraper::GetMenuInput()
{
	//input handler for main menu

}

void Skyscraper::RenderMenu()
{
	//render function for main menu
	//g2d->Clear(black);
}
