/* $Id$ */

/*
	Scalable Building Simulator - Core
	The Skyscraper Project - Version 1.7 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
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

#ifdef _WIN32
	#define CS_IMPLEMENT_PLATFORM_APPLICATION
	#define CS_NO_MALLOC_OVERRIDE
#endif

#include <wx/wx.h>
#include <wx/variant.h>
#include <crystalspace.h>
#include "globals.h"
#include "sbs.h"
#include "unix.h"
#include "revsbs.h"

#ifdef _WIN32
	CS_IMPLEMENT_FOREIGN_DLL
#endif

SBS *sbs; //self reference

SBS::SBS()
{
	sbs = this;
	version = "0.7.0." + csString(SVN_REVSTR);
	version_state = "Alpha";

	//set up SBS object
	object = new Object();
	object->SetValues(this, 0, "SBS", "SBS", true);

	//Print SBS banner
	PrintBanner();

	//Pause for 2 seconds
	csSleep(2000);

	//initialize other variables
	BuildingName = "";
	BuildingDesigner = "";
	BuildingLocation = "";
	BuildingDescription = "";
	BuildingVersion = "";
	IsRunning = false;
	Floors = 0;
	Basements = 0;
	RenderOnly = false;
	InputOnly = false;
	IsFalling = false;
	InStairwell = false;
	InElevator = false;
	IsBuildingsEnabled = false;
	IsExternalEnabled = false;
	IsLandscapeEnabled = false;
	IsSkyboxEnabled = false;
	fps_frame_count = 0;
	fps_tottime = 0;
	FPS = 0;
	FrameRate = 30;
	FrameLimiter = false;
	AutoShafts = true;
	AutoStairs = true;
	ElevatorSync = false;
	ElevatorNumber = 1;
	mouse_x = 0;
	mouse_y = 0;
	wall_orientation = 1;
	floor_orientation = 2;
	DrawMainN = true;
	DrawMainP = true;
	DrawSideN = false;
	DrawSideP = false;
	DrawTop = false;
	DrawBottom = false;
	DrawMainNOld = true;
	DrawMainPOld = true;
	DrawSideNOld = false;
	DrawSidePOld = false;
	DrawTopOld = false;
	DrawBottomOld = false;
	delta = 0.01f;
	wall1a = false;
	wall1b = false;
	wall2a = false;
	wall2b = false;
	AutoX = true;
	AutoY = true;
	ReverseAxisValue = false;
	TextureOverride = false;
	ProcessElevators = true;
	FlipTexture = false;
	mainnegflip = 0;
	mainposflip = 0;
	sidenegflip = 0;
	sideposflip = 0;
	topflip = 0;
	bottomflip = 0;
	widthscale.SetSize(6);
	heightscale.SetSize(6);
	remaining_delta = 0;
	start_time = 0;
	running_time = 0;
	InShaft = false;
	DisableSound = false;
	MapIndex.SetSize(3);
	MapUV.SetSize(3);
	OldMapIndex.SetSize(3);
	OldMapUV.SetSize(3);
	MapVerts1.SetSize(3);
	MapVerts2.SetSize(3);
	MapVerts3.SetSize(3);
	OldMapVerts1.SetSize(3);
	OldMapVerts2.SetSize(3);
	OldMapVerts3.SetSize(3);
	MapMethod = 0;
	OldMapMethod = 0;
	RevX = false;
	RevY = false;
	RevZ = false;
	OldRevX = false;
	OldRevY = false;
	OldRevZ = false;
	PlanarFlat = false;
	OldPlanarFlat = false;
	for (int i = 0; i <= 2; i++)
	{
		MapIndex[i] = 0;
		OldMapIndex[i] = 0;
		OldMapUV[i] = 0;
		MapUV[i] = 0;
	}
	RecreateColliders = false;
	soundcount = 0;
	UnitScale = 1;
	Verbose = false;
	InterfloorOnTop = false;
	DefaultMapper = 0;
	ObjectCount = 0;
	FastDelete = false;
	Skybox_object = 0;
}

SBS::~SBS()
{
	//engine destructor

	Report("Deleting SBS objects...");

	FastDelete = true;

	//delete camera object
	if (camera)
		delete camera;
	camera = 0;

	//delete callbacks
	doorcallbacks.DeleteAll();
	buttoncallbacks.DeleteAll();

	//delete floors
	for (int i = 0; i < FloorArray.GetSize(); i++)
	{
		if (FloorArray[i].object)
			delete FloorArray[i].object;
		FloorArray[i].object = 0;
	}
	FloorArray.DeleteAll();

	//delete elevators
	for (int i = 0; i < ElevatorArray.GetSize(); i++)
	{
		if (ElevatorArray[i].object)
			delete ElevatorArray[i].object;
		ElevatorArray[i].object = 0;
	}
	ElevatorArray.DeleteAll();

	//delete shafts
	for (int i = 0; i < ShaftArray.GetSize(); i++)
	{
		if (ShaftArray[i].object)
			delete ShaftArray[i].object;
		ShaftArray[i].object = 0;
	}
	ShaftArray.DeleteAll();

	//delete stairs
	for (int i = 0; i < StairsArray.GetSize(); i++)
	{
		if (StairsArray[i].object)
			delete StairsArray[i].object;
		StairsArray[i].object = 0;
	}
	StairsArray.DeleteAll();

	//delete sounds
	for (int i = 0; i < sounds.GetSize(); i++)
	{
		if (sounds[i])
			delete sounds[i];
		sounds[i] = 0;
	}
	sounds.DeleteAll();

	//delete wall objects
	for (int i = 0; i < Buildings_walls.GetSize(); i++)
	{
		if (Buildings_walls[i])
			delete Buildings_walls[i];
		Buildings_walls[i] = 0;
	}
	for (int i = 0; i < External_walls.GetSize(); i++)
	{
		if (External_walls[i])
			delete External_walls[i];
		External_walls[i] = 0;
	}
	for (int i = 0; i < Landscape_walls.GetSize(); i++)
	{
		if (Landscape_walls[i])
			delete Landscape_walls[i];
		Landscape_walls[i] = 0;
	}

	SkyBox_state = 0;
	SkyBox = 0;
	Landscape_state = 0;
	Landscape = 0;
	External_state = 0;
	External = 0;
	Buildings_state = 0;
	Buildings = 0;

	//remove referenced sounds
	sndmanager->RemoveSounds();

	if (Skybox_object)
		delete Skybox_object;
	delete object;

	//remove all engine objects
	Report("Deleting CS engine objects...");
	engine->DeleteAll();

	ObjectArray.DeleteAll();

	//clear self reference
	sbs = 0;

	Report("Exiting");
}

bool SBS::Start()
{
	//Post-init startup code goes here, before the runloop

	//initialize mesh colliders
	csColliderHelper::InitializeCollisionWrappers(collision_sys, engine);

	//initialize camera/actor
	camera->CreateColliders();

	//move camera to start location
	camera->SetToStartPosition();
	camera->SetToStartDirection();
	camera->SetToStartRotation();

	//set sound listener object to initial position
	if (DisableSound == false)
	{
		if (sndrenderer->GetListener())
			SetListenerLocation(camera->GetPosition());
		else
		{
			ReportError("Sound listener object not available. Sound support disabled");
			DisableSound = true;
		}
	}

	//turn on main objects
	EnableBuildings(true);
	EnableLandscape(true);
	EnableExternal(true);
	EnableSkybox(true);

	//turn off floors
	for (int i = 0; i < TotalFloors(); i++)
		FloorArray[i].object->Enabled(false);

	//turn off shafts
	for (int i = 0; i < Shafts(); i++)
	{
		if (ShaftArray[i].object)
		{
			if (ShaftArray[i].object->ShowFullShaft == false)
			{
				ShaftArray[i].object->EnableWholeShaft(false, true);
				//enable extents
				ShaftArray[i].object->Enabled(ShaftArray[i].object->startfloor, true, true);
				ShaftArray[i].object->Enabled(ShaftArray[i].object->endfloor, true, true);
			}
			else
				ShaftArray[i].object->EnableWholeShaft(true, true, true);
		}
	}

	//turn off stairwells
	for (int i = 0; i < StairsNum(); i++)
	{
		if (StairsArray[i].object)
			StairsArray[i].object->EnableWholeStairwell(false);
	}

	//init elevators
	for (int i = 0; i < Elevators(); i++)
	{
		bool enable_elevators = sbs->confman->GetBool("Skyscraper.SBS.Elevator.IsEnabled", true);
		if (ElevatorArray[i].object)
		{
			//turn on shaft doors
			ElevatorArray[i].object->ShaftDoorsEnabled(0, camera->StartFloor, true);
			ElevatorArray[i].object->ShaftDoorsEnabled(0, GetShaft(ElevatorArray[i].object->AssignedShaft)->startfloor, true);
			ElevatorArray[i].object->ShaftDoorsEnabled(0, GetShaft(ElevatorArray[i].object->AssignedShaft)->endfloor, true);
			//disable objects
			ElevatorArray[i].object->EnableObjects(false);
			if (enable_elevators == false)
				ElevatorArray[i].object->Enabled(false);
		}
	}

	//turn on start floor
	if (GetFloor(camera->StartFloor))
	{
		GetFloor(camera->StartFloor)->Enabled(true);
		GetFloor(camera->StartFloor)->EnableGroup(true);
	}

	return true;
}

float SBS::AutoSize(float n1, float n2, bool iswidth, float offset, bool enable_force, bool force_mode)
{
	//Texture autosizing formulas

	if (offset == 0)
		offset = 1;

	if (iswidth == true)
	{
		if ((AutoX == true && enable_force == false) || (enable_force == true && force_mode == true))
			return fabs(n1 - n2) * offset;
		else
			return offset;
	}
	else
	{
		if ((AutoY == true && enable_force == false) || (enable_force == true && force_mode == true))
			return fabs(n1 - n2) * offset;
		else
			return offset;
	}
}

void SBS::PrintBanner()
{
	csPrintf("\n Scalable Building Simulator " + version + " " + version_state + "\n");
	csPrintf(" Copyright (C)2004-2010 Ryan Thoryk\n");
	csPrintf(" This software comes with ABSOLUTELY NO WARRANTY. This is free\n");
	csPrintf(" software, and you are welcome to redistribute it under certain\n");
	csPrintf(" conditions. For details, see the file gpl.txt\n\n");
}

void SBS::MainLoop()
{
	//Main simulator loop

	//This makes sure all timer steps are the same size, in order to prevent the physics from changing
	//depending on frame rate
	float elapsed = remaining_delta + (vc->GetElapsedTicks() / 1000.0);

	//calculate start and running time
	if (start_time == 0)
		start_time = vc->GetCurrentTicks() / 1000.0;
	running_time = (vc->GetCurrentTicks() / 1000.0) - start_time;

	//limit the elapsed value to prevent major slowdowns during debugging
	if (elapsed > 0.5)
		elapsed = 0.5;
	while (elapsed >= delta)
	{
		if (RenderOnly == false && InputOnly == false)
		{
			//Determine floor that the camera is on
			camera->UpdateCameraFloor();

			//run elevator handlers
			if (ProcessElevators == true)
			{
				for (int i = 1; i <= Elevators(); i++)
					GetElevator(i)->MonitorLoop();

				//check if the user is in an elevator
				camera->CheckElevator();
			}

			//check if the user is in a shaft
			if (AutoShafts == true)
				camera->CheckShaft();

			//check if the user is in a stairwell
			if (AutoStairs == true)
				camera->CheckStairwell();

			//open/close doors by using door callback
			ProcessDoors();

			//process call button callbacks
			ProcessCallButtons();

			//process misc operations on current floor
			GetFloor(camera->CurrentFloor)->Loop();

			//process auto areas
			CheckAutoAreas();

			//check if the user is outside (no code yet)

		}
		elapsed -= delta;
	}
	remaining_delta = elapsed;
}

void SBS::CalculateFrameRate()
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
}

bool SBS::Initialize(iSCF* scf, iObjectRegistry* objreg, iView* view, const char* rootdirectory, const char* directory_char)
{
	//initialize CS references
#ifdef _WIN32
	iSCF::SCF = scf;
#endif
	object_reg = objreg;
	engine = csQueryRegistry<iEngine> (object_reg);
	if (!engine)
		return ReportError("No engine plugin found");
	g3d = csQueryRegistry<iGraphics3D> (object_reg);
	if (!g3d)
		return ReportError("No 3D renderer plugin found");
	g2d = csQueryRegistry<iGraphics2D> (object_reg);
	if (!g2d)
		return ReportError("No 2D plugin found");
	loader = csQueryRegistry<iLoader> (object_reg);
	if (!loader)
		return ReportError("No loader plugin found");
	vc = csQueryRegistry<iVirtualClock> (object_reg);
	if (!vc)
		return ReportError("No virtual clock plugin found");
	vfs = csQueryRegistry<iVFS> (object_reg);
	if (!vfs)
		return ReportError("No VFS plugin found");
	collision_sys = csQueryRegistry<iCollideSystem> (object_reg);
	if (!collision_sys)
		return ReportError("No collision plugin found");
	rep = csQueryRegistry<iReporter> (object_reg);
	if (!rep)
		return ReportError("No reporter plugin found");
	sndrenderer = csQueryRegistry<iSndSysRenderer> (object_reg);
	if (!sndrenderer)
		ReportError("No sound renderer plugin found");
	sndloader = csQueryRegistry<iSndSysLoader> (object_reg);
	if (!sndloader)
		ReportError("No sound loader plugin found");
	sndmanager = csQueryRegistry<iSndSysManager> (object_reg);
	if (!sndmanager)
		return ReportError("No sound manager plugin found");
	confman = csQueryRegistry<iConfigManager> (object_reg);
	if (!confman)
		return ReportError("No configuration manager plugin found");
	this->view = view;
	if (!this->view)
		return ReportError("No iView object available");

	//create default sector
	area = engine->CreateSector("area");
	if (!area)
		return ReportError("No iSector object available");

	root_dir = rootdirectory;
	dir_char = directory_char;

	//load SBS configuration file
	//confman->AddDomain("/root/data/config/sbs.cfg", vfs, confman->ConfigPriorityApplication);

	//load default values from config file
	HorizScale = confman->GetFloat("Skyscraper.SBS.HorizScale", 1); //Set default horizontal scaling value
	SkyName = confman->GetStr("Skyscraper.SBS.SkyName", "noon");
	AutoShafts = confman->GetBool("Skyscraper.SBS.AutoShafts", true);
	AutoStairs = confman->GetBool("Skyscraper.SBS.AutoStairs", true);
	ShowFullShafts = confman->GetBool("Skyscraper.SBS.ShowFullShafts", false);
	ShowFullStairs = confman->GetBool("Skyscraper.SBS.ShowFullStairs", false);
	ShaftDisplayRange = confman->GetInt("Skyscraper.SBS.ShaftDisplayRange", 3);
	StairsDisplayRange = confman->GetInt("Skyscraper.SBS.StairsDisplayRange", 5);
	ShaftOutsideDisplayRange = confman->GetInt("Skyscraper.SBS.ShaftOutsideDisplayRange", 3);
	StairsOutsideDisplayRange = confman->GetInt("Skyscraper.SBS.StairsOutsideDisplayRange", 3);
	FloorDisplayRange = confman->GetInt("Skyscraper.SBS.FloorDisplayRange", 3);
	ProcessElevators = confman->GetBool("Skyscraper.SBS.ProcessElevators", true);
	DisableSound = confman->GetBool("Skyscraper.SBS.DisableSound", false);
	UnitScale = confman->GetFloat("Skyscraper.SBS.UnitScale", 5);
	Verbose = confman->GetBool("Skyscraper.SBS.Verbose", true);
	DefaultMapper = confman->GetInt("Skyscraper.SBS.TextureMapper", 0);
	ResetTextureMapping(true); //set default texture map values

	//disable sound if renderer or loader are not available
	if (!sndrenderer || !sndloader)
		DisableSound = true;

	//mount sign texture packs
	Mount("signs-sans.zip", "/root/signs/sans");
	Mount("signs-sans_bold.zip", "/root/signs/sans_bold");
	Mount("signs-sans_cond.zip", "/root/signs/sans_cond");
	Mount("signs-sans_cond_bold.zip", "/root/signs/sans_cond_bold");

	//load default textures
	csPrintf("Loading default textures...");
	LoadTexture("/root/data/brick1.jpg", "Default", 1, 1);
	LoadTexture("/root/data/gray2-sm.jpg", "ConnectionWall", 1, 1);
	LoadTexture("/root/data/metal1-sm.jpg", "Connection", 1, 1);
	csPrintf("Done\n");

	//create camera object
	camera = new Camera();
	
	return true;
}

bool SBS::LoadTexture(const char *filename, const char *name, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//first verify the filename
	csString filename2 = VerifyFile(filename);

	// Load a texture
	csRef<iTextureWrapper> wrapper = loader->LoadTexture(name, filename2, CS_TEXTURE_3D, 0, true, true, false);

	if (!wrapper)
		return ReportError("Error loading texture");

	//if texture has an alpha map, force binary alpha
	if (wrapper->GetTextureHandle()->GetAlphaMap() == true)
		wrapper->GetTextureHandle()->SetAlphaType(csAlphaMode::alphaBinary);

	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.Push(info);
	return true;
}

bool SBS::UnloadTexture(const char *name)
{
	//unloads a texture

	csRef<iTextureWrapper> wrapper = engine->GetTextureList()->FindByName(name);
	if (!wrapper)
		return false;
	if (!engine->GetTextureList()->Remove(wrapper))
		return false;
	return true;
}

bool SBS::LoadTextureCropped(const char *filename, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//loads only a portion of the specified texture

	iTextureManager *tm = g3d->GetTextureManager();

	//load image
	csRef<iImage> image = loader->LoadImage(filename, tm->GetTextureFormat());

	if (!image)
		return ReportError("LoadTextureCropped: Error loading image");

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = image->GetWidth();
	if (height < 1)
		height = image->GetHeight();

	if (x > image->GetWidth() || y > image->GetHeight())
		return ReportError("LoadTextureCropped: invalid coordinates");
	if (x + width > image->GetWidth() || y + height > image->GetHeight())
		return ReportError("LoadTextureCropped: invalid size");

	//crop image
	csRef<iImage> cropped_image = csImageManipulate::Crop(image, x, y, width, height);
	if (!cropped_image)
		return ReportError("LoadTextureCropped: Error cropping image");

	//register texture
	csRef<iTextureHandle> handle = tm->RegisterTexture(cropped_image, CS_TEXTURE_3D);
	if (!handle)
		return ReportError("LoadTextureCropped: Error registering texture");

	//if texture has an alpha map, force binary alpha
	if (handle->GetAlphaMap() == true)
		handle->SetAlphaType(csAlphaMode::alphaBinary);

	//create texture wrapper
	csRef<iTextureWrapper> wrapper = engine->GetTextureList()->NewTexture(handle);
	wrapper->QueryObject()->SetName(name);

	//create material
	csRef<iMaterial> material (engine->CreateBaseMaterial(wrapper));
	csRef<iMaterialWrapper> matwrapper = engine->GetMaterialList()->NewMaterial(material, name);

	wrapper->SetImageFile(cropped_image);

	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.Push(info);

	return true;
}

bool SBS::AddTextToTexture(const char *origname, const char *name, const char *font_filename, float font_size, const char *text, int x1, int y1, int x2, int y2, const char *h_align, const char *v_align, int ColorR, int ColorG, int ColorB, bool enable_force, bool force_mode)
{
	//adds text to the named texture, in the given box coordinates and alignment

	//h_align is either "left", "right" or "center" - default is center
	//v_align is either "top", "bottom", or "center" - default is center

	//if either x1 or y1 are -1, the value of 0 is used.
	//If either x2 or y2 are -1, the width or height of the texture is used.

	csString hAlign = h_align;
	csString vAlign = v_align;

	csString font_filename2 = VerifyFile(font_filename);

	//load font
	csRef<iFont> font = g2d->GetFontServer()->LoadFont(font_filename2, font_size);
	if (!font)
	{
		ReportError("AddTextToTexture: Invalid font");
		return false;
	}

	//get original texture
	csRef<iTextureWrapper> wrapper = engine->GetTextureList()->FindByName(origname);
	if (!wrapper)
	{
		ReportError("AddTextToTexture: Invalid original texture");
		return false;
	}

	//get texture tiling info
	float widthmult, heightmult;
	GetTextureTiling(origname, widthmult, heightmult);

	//get height and width of texture
	int width, height;
	wrapper->GetTextureHandle()->GetOriginalDimensions(width, height);

	//create new empty texture
	csRef<iTextureHandle> th = g3d->GetTextureManager()->CreateTexture(width, height, csimg2D, "argb8", CS_TEXTURE_3D);
	if (!th)
	{
		ReportError("AddTextToTexture: Error creating texture");
		th = 0;
		return false;
	}

	//force binary alpha on texture
	th->SetAlphaType(csAlphaMode::alphaBinary);

	//get new texture dimensions, if it was resized
	th->GetOriginalDimensions(width, height);

	//create a texture wrapper for the new texture
	csRef<iTextureWrapper> tex = engine->GetTextureList()->NewTexture(th);
	if (!tex)
	{
		ReportError("AddTextToTexture: Error creating texture wrapper");
		th = 0;
		return false;
	}
	
	//set texture name
	tex->QueryObject()->SetName(name);

	//create material
	csRef<iMaterial> material (engine->CreateBaseMaterial(tex));
	csRef<iMaterialWrapper> matwrapper = engine->GetMaterialList()->NewMaterial(material, name);
	
	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.Push(info);

	//set default values if specified
	if (x1 == -1)
		x1 = 0;
	if (y1 == -1)
		y1 = 0;
	if (x2 == -1)
		x2 = width;
	if (y2 == -1)
		y2 = height;

	iTextureHandle *handle = tex->GetTextureHandle();
	if (!handle)
	{
		ReportError("AddTextToTexture: No texture handle available");
		return false;
	}

	//set graphics rendering to the texture image
	g3d->SetRenderTarget(handle);
	if (!g3d->ValidateRenderTargets())
		return false;
	if (!g3d->BeginDraw (CSDRAW_2DGRAPHICS)) return false;

	//clear buffer with alpha mask and enable double buffering
	g2d->ClearAll(g2d->FindRGB(0, 0, 0, 0));
	g2d->DoubleBuffer(true);

	//draw original image onto backbuffer
	g3d->DrawPixmap(wrapper->GetTextureHandle(), 0, 0, width, height, 0, 0, width, height);

	//get texture size info
	int x, y, w, h;
	font->GetDimensions(text, w, h);

	//horizontal alignment
	if (hAlign == "left")
		x = x1;
	else if (hAlign == "right")
		x = x2 - w;
	else //center
		x = x1 + ((x2 - x1) >> 1) - (w >> 1);

	//vertical alignment
	if (vAlign == "top")
		y = y1;
	else if (vAlign == "bottom")
		y = y2 - h;
	else //center
		y = y1 + ((y2 - y1) >> 1) - (h >> 1);

	//write text
	g2d->Write(font, x, y, g2d->FindRGB(ColorR, ColorG, ColorB), -1, text);

	//finish with buffer
	g3d->FinishDraw();

	return true;
}

bool SBS::AddTextureOverlay(const char *orig_texture, const char *overlay_texture, const char *name, int x, int y, int width, int height, float widthmult, float heightmult, bool enable_force, bool force_mode)
{
	//draws the specified texture on top of another texture
	//orig_texture is the original texture to use; overlay_texture is the texture to draw on top of it

	//get original texture
	csRef<iImage> image1 = engine->GetTextureList()->FindByName(orig_texture)->GetImageFile();
	if (!image1)
	{
		ReportError("AddTextureOverlay: Invalid original texture");
		return false;
	}

	//get overlay texture
	csRef<iImage> image2 = engine->GetTextureList()->FindByName(overlay_texture)->GetImageFile();
	if (!image2)
	{
		ReportError("AddTextureOverlay: Invalid overlay texture");
		return false;
	}

	//set default values if specified
	if (x == -1)
		x = 0;
	if (y == -1)
		y = 0;
	if (width < 1)
		width = image2->GetWidth();
	if (height < 1)
		height = image2->GetHeight();

	if (x > image1->GetWidth() || y > image1->GetHeight())
		return ReportError("AddTextureOverlay: invalid coordinates");
	if (x + width > image1->GetWidth() || y + height > image1->GetHeight())
		return ReportError("AddTextureOverlay: invalid size");

	//copy overlay image onto source image
	csRef<csImageMemory> imagemem;
	imagemem.AttachNew(new csImageMemory(image1));

	imagemem->CopyScale(image2, x, y, width, height);

	//register new texture
	csRef<iTextureHandle> handle = g3d->GetTextureManager()->RegisterTexture(imagemem, CS_TEXTURE_3D);
	if (!handle)
		return ReportError("AddTextureOverlay: Error registering texture");

	//if texture has an alpha map, force binary alpha
	if (handle->GetAlphaMap() == true)
		handle->SetAlphaType(csAlphaMode::alphaBinary);

	//create texture wrapper
	csRef<iTextureWrapper> wrapper = engine->GetTextureList()->NewTexture(handle);
	wrapper->QueryObject()->SetName(name);

	//create material
	csRef<iMaterial> material (engine->CreateBaseMaterial(wrapper));
	csRef<iMaterialWrapper> matwrapper = engine->GetMaterialList()->NewMaterial(material, name);

	wrapper->SetImageFile(imagemem);

	//add texture multipliers for new texture
	TextureInfo info;
	info.name = name;
	info.widthmult = widthmult;
	info.heightmult = heightmult;
	info.enable_force = enable_force;
	info.force_mode = force_mode;
	textureinfo.Push(info);

	return true;
}

void SBS::AddLight(const char *name, float x, float y, float z, float radius, float r, float g, float b)
{
	csRef<iLightList> ll = area->GetLights();
	csRef<iLight> light = engine->CreateLight(name, csVector3(ToRemote(x), ToRemote(y), ToRemote(z)), radius, csColor(r, g, b));
	ll->Add(light);
}

int SBS::AddWallMain(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	WallObject *object = new WallObject(mesh, parent, true);
	int result = AddWallMain(object, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th);
	delete object;
	return result;
}

int SBS::AddWallMain2(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	WallObject2 *object = new WallObject2(mesh, parent, true);
	int result = AddWallMain(object, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, tw, th);
	delete object;
	return result;
}

int SBS::AddWallMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	//thingmesh version

	//determine axis of wall
	int axis = 0;
	if (fabs(x1 - x2) > fabs(z1 - z2))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	//convert to clockwise coordinates if on x-axis, or counterclockwise if on z-axis
	if ((x1 > x2 && axis == 1) || (z1 < z2 && axis == 2))
	{
		//reverse coordinates
		float temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
		temp = height_in1;
		height_in1 = height_in2;
		height_in2 = temp;
	}

	//Adds a wall with the specified dimensions
	csVector3 v1 (x1, altitude1 + height_in1, z1); //left top
	csVector3 v2 (x2, altitude2 + height_in2, z2); //right top
	csVector3 v3 (x2, altitude2, z2); //right base
	csVector3 v4 (x1, altitude1, z1); //left base

	csVector3 v5 = v1;
	csVector3 v6 = v2;
	csVector3 v7 = v3;
	csVector3 v8 = v4;

	//expand to specified thickness
	if (axis == 1)
	{
		//x axis
		if (wall_orientation == 0)
		{
			//left
			v5.z += thickness;
			v6.z += thickness;
			v7.z += thickness;
			v8.z += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			v1.z -= thickness / 2;
			v2.z -= thickness / 2;
			v3.z -= thickness / 2;
			v4.z -= thickness / 2;
			v5.z += thickness / 2;
			v6.z += thickness / 2;
			v7.z += thickness / 2;
			v8.z += thickness / 2;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.z -= thickness;
			v2.z -= thickness;
			v3.z -= thickness;
			v4.z -= thickness;
		}
	}
	else
	{
		//z axis
		if (wall_orientation == 0)
		{
			//left
			v5.x += thickness;
            v6.x += thickness;
            v7.x += thickness;
            v8.x += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			v1.x -= thickness / 2;
            v2.x -= thickness / 2;
            v3.x -= thickness / 2;
            v4.x -= thickness / 2;
            v5.x += thickness / 2;
            v6.x += thickness / 2;
            v7.x += thickness / 2;
            v8.x += thickness / 2;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.x -= thickness;
            v2.x -= thickness;
            v3.x -= thickness;
            v4.x -= thickness;
		}
	}

	//convert positions to remote (CS) values
	v1 = ToRemote(v1);
	v2 = ToRemote(v2);
	v3 = ToRemote(v3);
	v4 = ToRemote(v4);
	v5 = ToRemote(v5);
	v6 = ToRemote(v6);
	v7 = ToRemote(v7);
	v8 = ToRemote(v8);

	//create polygons and set names
	int index = -1;
	int tmpindex = -1;
	csString NewName;

	if (DrawMainN == true)
	{
		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.Append(":front");
		tmpindex = wallobject->AddQuad(NewName, v1, v2, v3, v4); //front wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawMainP == true)
	{
		NewName = name;
		NewName.Append(":back");
		tmpindex = wallobject->AddQuad(NewName, v6, v5, v8, v7); //back wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideN == true)
	{
		NewName = name;
		NewName.Append(":left");
		if (axis == 1)
			tmpindex = wallobject->AddQuad(NewName, v5, v1, v4, v8); //left wall
		else
			tmpindex = wallobject->AddQuad(NewName, v2, v6, v7, v3); //left wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideP == true)
	{
		NewName = name;
		NewName.Append(":right");
		if (axis == 1)
			tmpindex = wallobject->AddQuad(NewName, v2, v6, v7, v3); //right wall
		else
			tmpindex = wallobject->AddQuad(NewName, v5, v1, v4, v8); //right wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawTop == true)
	{
		NewName = name;
		NewName.Append(":top");
		tmpindex = wallobject->AddQuad(NewName, v5, v6, v2, v1); //top wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawBottom == true)
	{
		NewName = name;
		NewName.Append(":bottom");
		tmpindex = wallobject->AddQuad(NewName, v4, v3, v7, v8); //bottom wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	//set texture
	if (TextureOverride == false && FlipTexture == false)
		SetTexture(wallobject->state, index, texture, true, tw, th);
	else
	{
		ProcessTextureFlip(tw, th);
		int endindex = index + GetDrawWallsCount();
		if (TextureOverride == true)
		{
			for (int i = index; i < endindex; i++)
			{
				if (i - index == 0)
					SetTexture(wallobject->state, i, mainnegtex.GetData(), false, widthscale[0], heightscale[0]);
				if (i - index == 1)
					SetTexture(wallobject->state, i, mainpostex.GetData(), false, widthscale[1], heightscale[1]);
				if (i - index == 2)
					SetTexture(wallobject->state, i, sidenegtex.GetData(), false, widthscale[2], heightscale[2]);
				if (i - index == 3)
					SetTexture(wallobject->state, i, sidepostex.GetData(), false, widthscale[3], heightscale[3]);
				if (i - index == 4)
					SetTexture(wallobject->state, i, toptex.GetData(), false, widthscale[4], heightscale[4]);
				if (i - index == 5)
					SetTexture(wallobject->state, i, bottomtex.GetData(), false, widthscale[5], heightscale[5]);
			}
		}
		else
		{
			for (int i = index; i < endindex; i++)
				SetTexture(wallobject->state, i, texture, false, widthscale[i - index], heightscale[i - index]);
		}
	}

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wallobject->meshwrapper);
		CreateColliders(wallobject->meshwrapper);
	}

	return index;
}

int SBS::AddWallMain(WallObject2* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	//genmesh version

	//determine axis of wall
	int axis = 0;
	if (fabs(x1 - x2) > fabs(z1 - z2))
		//x axis
		axis = 1;
	else
		//z axis
		axis = 2;

	//convert to clockwise coordinates if on x-axis, or counterclockwise if on z-axis
	if ((x1 > x2 && axis == 1) || (z1 < z2 && axis == 2))
	{
		//reverse coordinates
		float temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
		temp = height_in1;
		height_in1 = height_in2;
		height_in2 = temp;
	}

	//Adds a wall with the specified dimensions
	csVector3 v1 (x1, altitude1 + height_in1, z1); //left top
	csVector3 v2 (x2, altitude2 + height_in2, z2); //right top
	csVector3 v3 (x2, altitude2, z2); //right base
	csVector3 v4 (x1, altitude1, z1); //left base

	csVector3 v5 = v1;
	csVector3 v6 = v2;
	csVector3 v7 = v3;
	csVector3 v8 = v4;

	//expand to specified thickness
	if (axis == 1)
	{
		//x axis
		if (wall_orientation == 0)
		{
			//left
			v5.z += thickness;
			v6.z += thickness;
			v7.z += thickness;
			v8.z += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			v1.z -= thickness / 2;
			v2.z -= thickness / 2;
			v3.z -= thickness / 2;
			v4.z -= thickness / 2;
			v5.z += thickness / 2;
			v6.z += thickness / 2;
			v7.z += thickness / 2;
			v8.z += thickness / 2;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.z -= thickness;
			v2.z -= thickness;
			v3.z -= thickness;
			v4.z -= thickness;
		}
	}
	else
	{
		//z axis
		if (wall_orientation == 0)
		{
			//left
			v5.x += thickness;
            v6.x += thickness;
            v7.x += thickness;
            v8.x += thickness;
		}
		if (wall_orientation == 1)
		{
			//center
			v1.x -= thickness / 2;
            v2.x -= thickness / 2;
            v3.x -= thickness / 2;
            v4.x -= thickness / 2;
            v5.x += thickness / 2;
            v6.x += thickness / 2;
            v7.x += thickness / 2;
            v8.x += thickness / 2;
		}
		if (wall_orientation == 2)
		{
			//right
			v1.x -= thickness;
            v2.x -= thickness;
            v3.x -= thickness;
            v4.x -= thickness;
		}
	}

	//convert positions to remote (CS) values
	v1 = ToRemote(v1);
	v2 = ToRemote(v2);
	v3 = ToRemote(v3);
	v4 = ToRemote(v4);
	v5 = ToRemote(v5);
	v6 = ToRemote(v6);
	v7 = ToRemote(v7);
	v8 = ToRemote(v8);

	//create polygons and set names
	int index = -1;
	int tmpindex = -1;
	csString NewName;

	if (DrawMainN == true)
	{
		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.Append(":front");
		tmpindex = wallobject->AddQuad(NewName, v1, v2, v3, v4); //front wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawMainP == true)
	{
		NewName = name;
		NewName.Append(":back");
		tmpindex = wallobject->AddQuad(NewName, v6, v5, v8, v7); //back wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideN == true)
	{
		NewName = name;
		NewName.Append(":left");
		if (axis == 1)
			tmpindex = wallobject->AddQuad(NewName, v5, v1, v4, v8); //left wall
		else
			tmpindex = wallobject->AddQuad(NewName, v2, v6, v7, v3); //left wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideP == true)
	{
		NewName = name;
		NewName.Append(":right");
		if (axis == 1)
			tmpindex = wallobject->AddQuad(NewName, v2, v6, v7, v3); //right wall
		else
			tmpindex = wallobject->AddQuad(NewName, v5, v1, v4, v8); //right wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawTop == true)
	{
		NewName = name;
		NewName.Append(":top");
		tmpindex = wallobject->AddQuad(NewName, v5, v6, v2, v1); //top wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawBottom == true)
	{
		NewName = name;
		NewName.Append(":bottom");
		tmpindex = wallobject->AddQuad(NewName, v4, v3, v7, v8); //bottom wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	//set texture
	/*if (TextureOverride == false && FlipTexture == false)
		SetTexture2(wallobject->state, index, texture, true, tw, th);
	else
	{
		ProcessTextureFlip(tw, th);
		int endindex = index + GetDrawWallsCount();
		if (TextureOverride == true)
		{
			for (int i = index; i < endindex; i++)
			{
				if (i - index == 0)
					SetTexture2(wallobject->state, i, mainnegtex.GetData(), false, widthscale[0], heightscale[0]);
				if (i - index == 1)
					SetTexture2(wallobject->state, i, mainpostex.GetData(), false, widthscale[1], heightscale[1]);
				if (i - index == 2)
					SetTexture2(wallobject->state, i, sidenegtex.GetData(), false, widthscale[2], heightscale[2]);
				if (i - index == 3)
					SetTexture2(wallobject->state, i, sidepostex.GetData(), false, widthscale[3], heightscale[3]);
				if (i - index == 4)
					SetTexture2(wallobject->state, i, toptex.GetData(), false, widthscale[4], heightscale[4]);
				if (i - index == 5)
					SetTexture2(wallobject->state, i, bottomtex.GetData(), false, widthscale[5], heightscale[5]);
			}
		}
		else
		{
			for (int i = index; i < endindex; i++)
				SetTexture2(wallobject->state, i, texture, false, widthscale[i - index], heightscale[i - index]);
		}
	}*/

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wallobject->meshwrapper);
		CreateColliders(wallobject->meshwrapper);
	}

	return index;
}

int SBS::AddFloorMain(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th)
{
	WallObject *object = new WallObject(mesh, parent, true);
	int result = AddFloorMain(object, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, tw, th);
	delete object;
	return result;
}

int SBS::AddFloorMain2(Object *parent, csRef<iMeshWrapper> mesh, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th)
{
	WallObject2 *object = new WallObject2(mesh, parent, true);
	int result = AddFloorMain(object, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, tw, th);
	delete object;
	return result;
}

int SBS::AddFloorMain(WallObject* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th)
{
	//Adds a floor with the specified dimensions and vertical offset
	//thingmesh version

	//convert to clockwise coordinates
	float temp;
	if (x1 > x2 && fabs(x1 - x2) > fabs(z1 - z2))
	{
		//reverse coordinates if the difference between x coordinates is greater
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
	}
	if (z1 > z2 && fabs(z1 - z2) > fabs(x1 - x2))
	{
		//reverse coordinates if the difference between z coordinates is greater
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
	}

	csVector3 v1, v2, v3, v4;

	if (ReverseAxisValue == false)
	{
		v1.Set(x1, altitude1, z1); //bottom left
		v2.Set(x2, altitude1, z1); //bottom right
		v3.Set(x2, altitude2, z2); //top right
		v4.Set(x1, altitude2, z2); //top left
	}
	else
	{
		v1.Set(x1, altitude1, z1); //bottom left
		v2.Set(x1, altitude1, z2); //top left
		v3.Set(x2, altitude2, z2); //top right
		v4.Set(x2, altitude2, z1); //bottom right
	}

	csVector3 v5 = v1;
	csVector3 v6 = v2;
	csVector3 v7 = v3;
	csVector3 v8 = v4;

	//expand to specified thickness
	if (floor_orientation == 0)
	{
		//bottom
		v5.y += thickness;
		v6.y += thickness;
		v7.y += thickness;
		v8.y += thickness;
	}
	if (floor_orientation == 1)
	{
		//center
		v1.y -= thickness / 2;
		v2.y -= thickness / 2;
		v3.y -= thickness / 2;
		v4.y -= thickness / 2;
		v5.y += thickness / 2;
		v6.y += thickness / 2;
		v7.y += thickness / 2;
		v8.y += thickness / 2;
	}
	if (floor_orientation == 2)
	{
		//top
		v1.y -= thickness;
		v2.y -= thickness;
		v3.y -= thickness;
		v4.y -= thickness;
	}

	//convert positions to remote (CS) values
	v1 = ToRemote(v1);
	v2 = ToRemote(v2);
	v3 = ToRemote(v3);
	v4 = ToRemote(v4);
	v5 = ToRemote(v5);
	v6 = ToRemote(v6);
	v7 = ToRemote(v7);
	v8 = ToRemote(v8);

	//create polygons and set names
	int index = -1;
	int tmpindex = -1;
	csString NewName;

	if (DrawMainN == true)
	{
		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.Append(":front");
		tmpindex = wallobject->AddQuad(NewName, v1, v2, v3, v4); //bottom wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawMainP == true)
	{
		NewName = name;
		NewName.Append(":back");
		tmpindex = wallobject->AddQuad(NewName, v8, v7, v6, v5); //top wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideN == true)
	{
		NewName = name;
		NewName.Append(":left");
		tmpindex = wallobject->AddQuad(NewName, v8, v5, v1, v4); //left wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideP == true)
	{
		NewName = name;
		NewName.Append(":right");
		tmpindex = wallobject->AddQuad(NewName, v6, v7, v3, v2); //right wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawTop == true)
	{
		NewName = name;
		NewName.Append(":top");
		tmpindex = wallobject->AddQuad(NewName, v5, v6, v2, v1); //front wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawBottom == true)
	{
		NewName = name;
		NewName.Append(":bottom");
		tmpindex = wallobject->AddQuad(NewName, v7, v8, v4, v3); //back wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	//set texture
	if (TextureOverride == false && FlipTexture == false)
		SetTexture(wallobject->state, index, texture, true, tw, th);
	else
	{
		ProcessTextureFlip(tw, th);
		int endindex = index + GetDrawWallsCount();
		if (TextureOverride == true)
		{
			for (int i = index; i < endindex; i++)
			{
				if (i - index == 0)
					SetTexture(wallobject->state, i, mainnegtex.GetData(), false, widthscale[0], heightscale[0]);
				if (i - index == 1)
					SetTexture(wallobject->state, i, mainpostex.GetData(), false, widthscale[1], heightscale[1]);
				if (i - index == 2)
					SetTexture(wallobject->state, i, sidenegtex.GetData(), false, widthscale[2], heightscale[2]);
				if (i - index == 3)
					SetTexture(wallobject->state, i, sidepostex.GetData(), false, widthscale[3], heightscale[3]);
				if (i - index == 4)
					SetTexture(wallobject->state, i, toptex.GetData(), false, widthscale[4], heightscale[4]);
				if (i - index == 5)
					SetTexture(wallobject->state, i, bottomtex.GetData(), false, widthscale[5], heightscale[5]);
			}
		}
		else
		{
			for (int i = index; i < endindex; i++)
				SetTexture(wallobject->state, i, texture, false, widthscale[i - index], heightscale[i - index]);
		}
	}

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wallobject->meshwrapper);
		CreateColliders(wallobject->meshwrapper);
	}

	return index;
}

int SBS::AddFloorMain(WallObject2* wallobject, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th)
{
	//Adds a floor with the specified dimensions and vertical offset
	//genmesh version

	//convert to clockwise coordinates
	float temp;
	if (x1 > x2 && fabs(x1 - x2) > fabs(z1 - z2))
	{
		//reverse coordinates if the difference between x coordinates is greater
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
	}
	if (z1 > z2 && fabs(z1 - z2) > fabs(x1 - x2))
	{
		//reverse coordinates if the difference between z coordinates is greater
		temp = x1;
		x1 = x2;
		x2 = temp;
		temp = z1;
		z1 = z2;
		z2 = temp;
		temp = altitude1;
		altitude1 = altitude2;
		altitude2 = temp;
	}

	csVector3 v1, v2, v3, v4;

	if (ReverseAxisValue == false)
	{
		v1.Set(x1, altitude1, z1); //bottom left
		v2.Set(x2, altitude1, z1); //bottom right
		v3.Set(x2, altitude2, z2); //top right
		v4.Set(x1, altitude2, z2); //top left
	}
	else
	{
		v1.Set(x1, altitude1, z1); //bottom left
		v2.Set(x1, altitude1, z2); //top left
		v3.Set(x2, altitude2, z2); //top right
		v4.Set(x2, altitude2, z1); //bottom right
	}

	csVector3 v5 = v1;
	csVector3 v6 = v2;
	csVector3 v7 = v3;
	csVector3 v8 = v4;

	//expand to specified thickness
	if (floor_orientation == 0)
	{
		//bottom
		v5.y += thickness;
		v6.y += thickness;
		v7.y += thickness;
		v8.y += thickness;
	}
	if (floor_orientation == 1)
	{
		//center
		v1.y -= thickness / 2;
		v2.y -= thickness / 2;
		v3.y -= thickness / 2;
		v4.y -= thickness / 2;
		v5.y += thickness / 2;
		v6.y += thickness / 2;
		v7.y += thickness / 2;
		v8.y += thickness / 2;
	}
	if (floor_orientation == 2)
	{
		//top
		v1.y -= thickness;
		v2.y -= thickness;
		v3.y -= thickness;
		v4.y -= thickness;
	}

	//convert positions to remote (CS) values
	v1 = ToRemote(v1);
	v2 = ToRemote(v2);
	v3 = ToRemote(v3);
	v4 = ToRemote(v4);
	v5 = ToRemote(v5);
	v6 = ToRemote(v6);
	v7 = ToRemote(v7);
	v8 = ToRemote(v8);

	//create polygons and set names
	int index = -1;
	int tmpindex = -1;
	csString NewName;

	if (DrawMainN == true)
	{
		NewName = name;
		if (GetDrawWallsCount() > 1)
			NewName.Append(":front");
		tmpindex = wallobject->AddQuad(NewName, v1, v2, v3, v4); //bottom wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawMainP == true)
	{
		NewName = name;
		NewName.Append(":back");
		tmpindex = wallobject->AddQuad(NewName, v8, v7, v6, v5); //top wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideN == true)
	{
		NewName = name;
		NewName.Append(":left");
		tmpindex = wallobject->AddQuad(NewName, v8, v5, v1, v4); //left wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawSideP == true)
	{
		NewName = name;
		NewName.Append(":right");
		tmpindex = wallobject->AddQuad(NewName, v6, v7, v3, v2); //right wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawTop == true)
	{
		NewName = name;
		NewName.Append(":top");
		tmpindex = wallobject->AddQuad(NewName, v5, v6, v2, v1); //front wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	if (DrawBottom == true)
	{
		NewName = name;
		NewName.Append(":bottom");
		tmpindex = wallobject->AddQuad(NewName, v7, v8, v4, v3); //back wall
	}
	if (tmpindex > index && index == -1)
		index = tmpindex;

	//set texture
	/*if (TextureOverride == false && FlipTexture == false)
		SetTexture2(wallobject->state, index, texture, true, tw, th);
	else
	{
		ProcessTextureFlip(tw, th);
		int endindex = index + GetDrawWallsCount();
		if (TextureOverride == true)
		{
			for (int i = index; i < endindex; i++)
			{
				if (i - index == 0)
					SetTexture2(wallobject->state, i, mainnegtex.GetData(), false, widthscale[0], heightscale[0]);
				if (i - index == 1)
					SetTexture2(wallobject->state, i, mainpostex.GetData(), false, widthscale[1], heightscale[1]);
				if (i - index == 2)
					SetTexture2(wallobject->state, i, sidenegtex.GetData(), false, widthscale[2], heightscale[2]);
				if (i - index == 3)
					SetTexture2(wallobject->state, i, sidepostex.GetData(), false, widthscale[3], heightscale[3]);
				if (i - index == 4)
					SetTexture2(wallobject->state, i, toptex.GetData(), false, widthscale[4], heightscale[4]);
				if (i - index == 5)
					SetTexture2(wallobject->state, i, bottomtex.GetData(), false, widthscale[5], heightscale[5]);
			}
		}
		else
		{
			for (int i = index; i < endindex; i++)
				SetTexture2(wallobject->state, i, texture, false, widthscale[i - index], heightscale[i - index]);
		}
	}*/

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wallobject->meshwrapper);
		CreateColliders(wallobject->meshwrapper);
	}

	return index;
}

void SBS::Report (const char* msg, ...)
{
	csString message = msg;
	message.ReplaceAll("%", "%%"); //allow percent signs

	if (rep)
		rep->ReportNotify("sbs", message);
	else
	{
		csPrintf(message);
		csPrintf("\n");
		fflush (stdout);
	}
	LastNotification = message;
}

bool SBS::ReportError (const char* msg, ...)
{
	csString message = msg;
	message.ReplaceAll("%", "%%"); //allow percent signs

	if (rep)
		rep->ReportError("sbs", message);
	else
	{
		csPrintf(message);
		csPrintf("\n");
		fflush (stdout);
	}
	LastError = message;
	return false;
}

int SBS::CreateWallBox(WallObject* wallobject, const char *name, const char *texture, float x1, float x2, float z1, float z2, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create 4 walls

	int firstidx = 0;
	int tmpidx = 0;
	int range = 0;
	int range2 = 0;

	if (inside == true)
	{
		//generate a box visible from the inside
		csBox3 box (csVector3(ToRemote(x1) * HorizScale, ToRemote(voffset), ToRemote(z1) * HorizScale), csVector3(ToRemote(x2) * HorizScale, ToRemote(voffset + height_in), ToRemote(z2) * HorizScale));
		firstidx = wallobject->AddQuad( //front
			name,
			box.GetCorner(CS_BOX_CORNER_xyz),
			box.GetCorner(CS_BOX_CORNER_Xyz),
			box.GetCorner(CS_BOX_CORNER_XYz),
			box.GetCorner(CS_BOX_CORNER_xYz));
		range++;
		wallobject->AddQuad( //right
			name,
			box.GetCorner(CS_BOX_CORNER_Xyz),
			box.GetCorner(CS_BOX_CORNER_XyZ),
			box.GetCorner(CS_BOX_CORNER_XYZ),
			box.GetCorner(CS_BOX_CORNER_XYz));
		range++;
		wallobject->AddQuad( //back
			name,
			box.GetCorner(CS_BOX_CORNER_XyZ),
			box.GetCorner(CS_BOX_CORNER_xyZ),
			box.GetCorner(CS_BOX_CORNER_xYZ),
			box.GetCorner(CS_BOX_CORNER_XYZ));
		range++;
		wallobject->AddQuad( //left
			name,
			box.GetCorner(CS_BOX_CORNER_xyZ),
			box.GetCorner(CS_BOX_CORNER_xyz),
			box.GetCorner(CS_BOX_CORNER_xYz),
			box.GetCorner(CS_BOX_CORNER_xYZ));
		range++;
		if (bottom == true)
		{
			wallobject->AddQuad( //bottom
				name,
				box.GetCorner(CS_BOX_CORNER_xyZ),
				box.GetCorner(CS_BOX_CORNER_XyZ),
				box.GetCorner(CS_BOX_CORNER_Xyz),
				box.GetCorner(CS_BOX_CORNER_xyz));
			range++;
		}
		if (top == true)
		{
			wallobject->AddQuad( //top
				name,
				box.GetCorner(CS_BOX_CORNER_xYz),
				box.GetCorner(CS_BOX_CORNER_XYz),
				box.GetCorner(CS_BOX_CORNER_XYZ),
				box.GetCorner(CS_BOX_CORNER_xYZ));
			range++;
		}
	}

	if (outside == true)
	{
		csBox3 box (csVector3(ToRemote(x1) * HorizScale, ToRemote(voffset), ToRemote(z1) * HorizScale), csVector3(ToRemote(x2) * HorizScale, ToRemote(voffset + height_in), ToRemote(z2) * HorizScale));
		tmpidx = wallobject->AddQuad( //front
			name,
			box.GetCorner(CS_BOX_CORNER_xYz),
			box.GetCorner(CS_BOX_CORNER_XYz),
			box.GetCorner(CS_BOX_CORNER_Xyz),
			box.GetCorner(CS_BOX_CORNER_xyz));
		range2++;
		if (inside == false)
			firstidx = tmpidx;
		wallobject->AddQuad( //right
			name,
			box.GetCorner(CS_BOX_CORNER_XYz),
			box.GetCorner(CS_BOX_CORNER_XYZ),
			box.GetCorner(CS_BOX_CORNER_XyZ),
			box.GetCorner(CS_BOX_CORNER_Xyz));
		range2++;
		wallobject->AddQuad( //back
			name,
			box.GetCorner(CS_BOX_CORNER_XYZ),
			box.GetCorner(CS_BOX_CORNER_xYZ),
			box.GetCorner(CS_BOX_CORNER_xyZ),
			box.GetCorner(CS_BOX_CORNER_XyZ));
		range2++;
		wallobject->AddQuad( //left
			name,
			box.GetCorner(CS_BOX_CORNER_xYZ),
			box.GetCorner(CS_BOX_CORNER_xYz),
			box.GetCorner(CS_BOX_CORNER_xyz),
			box.GetCorner(CS_BOX_CORNER_xyZ));
		range2++;
		if (bottom == true)
		{
			wallobject->AddQuad( //bottom
				name,
				box.GetCorner(CS_BOX_CORNER_xyz),
				box.GetCorner(CS_BOX_CORNER_Xyz),
				box.GetCorner(CS_BOX_CORNER_XyZ),
				box.GetCorner(CS_BOX_CORNER_xyZ));
			range2++;
		}
		if (top == true)
		{
			wallobject->AddQuad( //top
				name,
				box.GetCorner(CS_BOX_CORNER_xYZ),
				box.GetCorner(CS_BOX_CORNER_XYZ),
				box.GetCorner(CS_BOX_CORNER_XYz),
				box.GetCorner(CS_BOX_CORNER_xYz));
			range2++;
		}
	}

	//texture mapping
	bool result;
	iMaterialWrapper* tm = GetTextureMaterial(texture, result);
	wallobject->state->SetPolygonMaterial(csPolygonRange(firstidx, firstidx + range + range2), tm);
	wallobject->state->SetPolygonTextureMapping(csPolygonRange(firstidx, firstidx + range + range2), ToRemote(3));

	//polygon names
	csString NewName;
	if (inside == true)
	{
		NewName = name;
		NewName.Append(":inside");
		for (int i = firstidx; i <= firstidx + range; i++)
			wallobject->SetPolygonName(i, NewName);
	}
	if (outside == true)
	{
		NewName = name;
		NewName.Append(":outside");
		for (int i = tmpidx; i <= tmpidx + range2; i++)
			wallobject->SetPolygonName(i, NewName);
	}

	return firstidx;
}

int SBS::CreateWallBox2(WallObject* wallobject, const char *name, const char *texture, float CenterX, float CenterZ, float WidthX, float LengthZ, float height_in, float voffset, float tw, float th, bool inside, bool outside, bool top, bool bottom)
{
	//create 4 walls from a central point

	float x1;
	float x2;
	float z1;
	float z2;

	x1 = CenterX - (WidthX / 2);
	x2 = CenterX + (WidthX / 2);
	z1 = CenterZ - (LengthZ / 2);
	z2 = CenterZ + (LengthZ / 2);

	return CreateWallBox(wallobject, name, texture, x1, x2, z1, z2, height_in, voffset, tw, th, inside, outside, top, bottom);
}

void SBS::InitMeshes()
{
	//create object meshes
	Buildings = engine->CreateSectorWallsMesh (area, "Buildings");
	Buildings_state = scfQueryInterface<iThingFactoryState> (Buildings->GetMeshObject()->GetFactory());
	Buildings->SetZBufMode(CS_ZBUF_USE);
	Buildings->SetRenderPriority(sbs->engine->GetObjectRenderPriority());

	External = engine->CreateSectorWallsMesh (area, "External");
	External_state = scfQueryInterface<iThingFactoryState> (External->GetMeshObject()->GetFactory());
	External->SetZBufMode(CS_ZBUF_USE);
	External->SetRenderPriority(engine->GetObjectRenderPriority());

	Landscape = engine->CreateSectorWallsMesh (area, "Landscape");
	Landscape_state = scfQueryInterface<iThingFactoryState> (Landscape->GetMeshObject()->GetFactory());
	Landscape->SetZBufMode(CS_ZBUF_USE);
	Landscape->SetRenderPriority(sbs->engine->GetObjectRenderPriority());
}

int SBS::AddCustomWall(WallObject* wallobject, const char *name, const char *texture, csPoly3D &varray, float tw, float th)
{
	//Adds a wall from a specified array of 3D vectors

	int num;
	csPoly3D varray1;
	csPoly3D varray2;

	//get number of stored vertices
	num = varray.GetVertexCount();

	//Set horizontal scaling
	for (int i = 0; i < num; i++)
		varray1.AddVertex(ToRemote(varray[i].x) * HorizScale, ToRemote(varray[i].y), ToRemote(varray[i].z) * HorizScale);

	//create a second array with reversed vertices
	for (int i = num - 1; i >= 0; i--)
		varray2.AddVertex(varray1[i]);

	csVector2 x, y, z;

	//get extents for texture autosizing
	x = ToLocal(GetExtents(varray1, 1));
	y = ToLocal(GetExtents(varray1, 2));
	z = ToLocal(GetExtents(varray1, 3));

	//calculate autosizing
	csVector2 sizing = CalculateSizing(texture, x, y, z, tw, th);

	//create 2 polygons (front and back) from the vertex array
	int tmpindex, firstidx, numindices = 0;

	//get polygon native direction
	csVector3 direction = GetPolygonDirection(varray1);

	//if the polygon is facing right, down or to the back, reverse faces
	//to keep the vertices clockwise
	csPoly3D tmppoly;
	if (direction.x == 1 || direction.y == -1 || direction.z == 1)
	{
		tmppoly = varray1;
		varray1 = varray2;
		varray2 = tmppoly;
	}

	//add the polygons
	if (DrawMainN == true)
	{
		csString NewName;
		NewName = name;
		NewName.Append(":0");
		firstidx = wallobject->AddPolygon(NewName, varray1.GetVertices(), num);
		numindices++;
	}
	if (DrawMainP == true)
	{
		csString NewName;
		NewName = name;
		NewName.Append(":1");
		tmpindex = wallobject->AddPolygon(NewName, varray2.GetVertices(), num);
		if (DrawMainN == false)
			firstidx = tmpindex;
		numindices++;
	}

	csString polyname = wallobject->state->GetPolygonName(firstidx);
	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, polyname.GetData());
	if (!result)
		texname = "Default";
	wallobject->state->SetPolygonMaterial(csPolygonRange(firstidx, firstidx + (numindices - 1)), material);

	float tw2 = sizing.x, th2 = sizing.y;
	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = sizing.x * mw;
		th2 = sizing.y * mh;
	}

	//apply UV texture mapping
	ApplyTextureMapping(wallobject->state, firstidx, firstidx + (numindices - 1), tw2, th2);

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(wallobject->meshwrapper);
		CreateColliders(wallobject->meshwrapper);
	}

	return firstidx;
}

int SBS::AddTriangleWall(WallObject* wallobject, const char *name, const char *texture, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float tw, float th)
{
	//Adds a triangular wall with the specified dimensions
	csPoly3D varray;

	//set up temporary vertex array
	varray.AddVertex(x1, y1, z1);
	varray.AddVertex(x2, y2, z2);
	varray.AddVertex(x3, y3, z3);

	//pass data on to AddCustomWall function
	int firstidx = AddCustomWall(wallobject, name, texture, varray, tw, th);

	return firstidx;
}

void SBS::EnableBuildings(bool value)
{
	//turns buildings on/off
	EnableMesh(Buildings, value);
	IsBuildingsEnabled = value;
}

void SBS::EnableLandscape(bool value)
{
	//turns landscape on/off
	EnableMesh(Landscape, value);
	IsLandscapeEnabled = value;
}

void SBS::EnableExternal(bool value)
{
	//turns external on/off
	EnableMesh(External, value);
	IsExternalEnabled = value;
}

void SBS::EnableSkybox(bool value)
{
	//turns skybox on/off
	EnableMesh(SkyBox, value);
	IsSkyboxEnabled = value;
}

csVector2 SBS::GetExtents(const csVector3 *varray, int count, int coord)
{
	//returns the smallest and largest values from a specified coordinate type
	//(x, y, or z) from a vertex array (polygon).
	//first parameter must be a vertex array object
	//second must be either 1 (for x), 2 (for y) or 3 (for z)

	float esmall = 0;
	float ebig = 0;
	float tempnum = 0;
	int i = 0;

	//return 0,0 if coord value is out of range
	if (coord < 1 || coord > 3)
		return csVector2(0, 0);

	for (i = 0; i < count; i++)
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

csVector2 SBS::GetExtents(csPoly3D &varray, int coord)
{
	return GetExtents(varray.GetVertices(), varray.GetVertexCount(), coord);
}

csVector2 SBS::GetExtents(csRef<iThingFactoryState> state, int coord)
{
	return GetExtents(state->GetVertices(), state->GetVertexCount(), coord);
}

int SBS::CreateSky(const char *filenamebase)
{
	//set up SBS object
	Skybox_object = new Object();
	Skybox_object->SetValues(0, this->object, "Skybox", "Skybox", false);

	csString file = filenamebase;
	vfs->Mount("/root/sky", root_dir + "data" + dir_char + "sky-" + file + ".zip");

	//load textures
	LoadTexture("/root/sky/up.jpg", "SkyTop", 1, 1);
	LoadTexture("/root/sky/down.jpg", "SkyBottom", 1, 1);
	LoadTexture("/root/sky/left.jpg", "SkyLeft", 1, 1);
	LoadTexture("/root/sky/right.jpg", "SkyRight", 1, 1);
	LoadTexture("/root/sky/front.jpg", "SkyFront", 1, 1);
	LoadTexture("/root/sky/back.jpg", "SkyBack", 1, 1);

	SkyBox = (engine->CreateSectorWallsMesh (area, "SkyBox"));
	SkyBox_state = scfQueryInterface<iThingFactoryState> (SkyBox->GetMeshObject()->GetFactory());
	SkyBox->SetZBufMode(CS_ZBUF_NONE);
	SkyBox->SetRenderPriority(sbs->engine->GetSkyRenderPriority());

	//create a skybox that extends 30 miles (30 * 5280 ft) in each direction
	float skysize = 158400;
	int firstidx = SkyBox_state->AddInsideBox(ToRemote(csVector3(-skysize, -skysize, -skysize)), ToRemote(csVector3(skysize, skysize, skysize)));
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial("SkyBack", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx, firstidx), material);
	material = GetTextureMaterial("SkyRight", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 1, firstidx + 1), material);
	material = GetTextureMaterial("SkyFront", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 2, firstidx + 2), material);
	material = GetTextureMaterial("SkyLeft", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 3, firstidx + 3), material);
	material = GetTextureMaterial("SkyBottom", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 4, firstidx + 4), material);
	material = GetTextureMaterial("SkyTop", result);
	SkyBox_state->SetPolygonMaterial (csPolygonRange(firstidx + 5, firstidx + 5), material);

	SkyBox_state->SetPolygonTextureMapping (csPolygonRange(firstidx, firstidx + 5),
		csVector2 (0, 1),
		csVector2 (1, 1),
		csVector2 (1, 0));

	return firstidx;
}

int SBS::GetFloorNumber(float altitude, int lastfloor, bool checklastfloor)
{
	//Returns floor number located at a specified altitude

	if (TotalFloors() == 0)
		return 0;

	//check to see if altitude is below bottom floor
	if (altitude < GetFloor(-Basements)->Altitude)
		return -Basements;

	//if checklastfloor is specified, compare altitude with lastfloor
	if (checklastfloor == true)
	{
		float lastfloor_altitude = GetFloor(lastfloor)->Altitude;
		float upperfloor_altitude;
		if (lastfloor < Floors - 1)
			upperfloor_altitude = GetFloor(lastfloor + 1)->Altitude;
		else
			upperfloor_altitude = GetFloor(lastfloor)->Altitude + GetFloor(lastfloor)->FullHeight();

		if (upperfloor_altitude > altitude && lastfloor_altitude <= altitude)
			return lastfloor;
		else
		{
			//if altitude is below lastfloor, search downwards; otherwise search upwards
			if (altitude < lastfloor_altitude)
			{
				for (int i = lastfloor - 1; i >= -Basements; i--)
				{
					if (GetFloor(i + 1)->Altitude > altitude && GetFloor(i)->Altitude <= altitude)
						return i;
				}
			}
			else if (altitude >= upperfloor_altitude)
			{
				for (int i = lastfloor + 1; i < Floors; i++)
				{
					if (GetFloor(i - 1)->Altitude <= altitude && GetFloor(i)->Altitude > altitude)
						return i - 1;
					if (i == Floors - 1 && GetFloor(i)->Altitude <= altitude)
						return i; //return top floor if on top
				}
			}
		}
	}

	//otherwise do a slow linear search through floors
	for (int i = -Basements + 1; i < Floors; i++)
	{
		//check to see if altitude is within a floor (between the current floor's base and
		//the lower floor's base)
		if ((GetFloor(i)->Altitude > altitude) && (GetFloor(i - 1)->Altitude <= altitude))
			return i - 1;
		//check to see if altitude is above top floor's altitude
		if ((i == Floors - 1) && (altitude > GetFloor(i)->Altitude))
			return i;
	}
	return 0;
}

float SBS::GetDistance(float x1, float x2, float z1, float z2)
{
	//returns the distance between 2 2D vectors

	if (z1 == z2)
		return fabs(x1 - x2);
	if (x1 == x2)
		return fabs(z1 - z2);
	if ((x1 != x2) && (z2 != x2))
		return sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(z1 - z2), 2)); //calculate diagonals
	return 0;
}

void SBS::DumpVertices(WallObject* wallobject)
{
	//dumps a list of vertices from a mesh object to the console/logfile

	Report("--- Polygon Vertex Dump ---\n");
	for (int i = 0; i < wallobject->handles.GetSize(); i++)
	{
		int index = wallobject->handles[i];
		for (int j = 0; j < wallobject->state->GetPolygonVertexCount(index); i++)
		{
			csVector3 vertex = wallobject->state->GetPolygonVertex(index, j);
			Report(csString(_itoa(j, intbuffer, 10)) + ": " + csString(_gcvt(vertex.x, 6, buffer)) + ", " + csString(_gcvt(vertex.y, 6, buffer)) + ", " + csString(_gcvt(vertex.z, 6, buffer)));
		}
	}
}

void SBS::ListAltitudes()
{
	//dumps the floor altitude list

	Report("--- Floor Altitudes ---\n");
	for (int i = -Basements; i < Floors; i++)
		Report(csString(_itoa(i, intbuffer, 10)) + "(" + GetFloor(i)->ID + ")\t----\t" + csString(_gcvt(GetFloor(i)->FullHeight(), 6, buffer)) + "\t----\t" + csString(_gcvt(GetFloor(i)->Altitude, 6, buffer)));
}

Object* SBS::CreateShaft(int number, int type, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a shaft object

	for (size_t i = 0; i < ShaftArray.GetSize(); i++)
	{
		if (ShaftArray[i].number == number)
		{
			csString num;
			num = number;
			ReportError("Shaft " + num + " already exists");
			return 0;
		}
	}
	ShaftArray.SetSize(ShaftArray.GetSize() + 1);
	ShaftArray[ShaftArray.GetSize() - 1].number = number;
	ShaftArray[ShaftArray.GetSize() - 1].object = new Shaft(number, type, CenterX, CenterZ, _startfloor, _endfloor);
	return ShaftArray[ShaftArray.GetSize() - 1].object->object;
}

Object* SBS::CreateStairwell(int number, float CenterX, float CenterZ, int _startfloor, int _endfloor)
{
	//create a stairwell object

	for (size_t i = 0; i < StairsArray.GetSize(); i++)
	{
		if (StairsArray[i].number == number)
		{
			csString num;
			num = number;
			ReportError("Stairwell " + num + " already exists");
			return 0;
		}
	}
	StairsArray.SetSize(StairsArray.GetSize() + 1);
	StairsArray[StairsArray.GetSize() - 1].number = number;
	StairsArray[StairsArray.GetSize() - 1].object = new Stairs(number, CenterX, CenterZ, _startfloor, _endfloor);
	return StairsArray[StairsArray.GetSize() - 1].object->object;
}

iMaterialWrapper* SBS::ChangeTexture(iMeshWrapper *mesh, const char *texture, bool matcheck)
{
	//changes a texture - works with genmeshes only
	//if matcheck is true, exit if old and new textures are the same

	//exit if mesh pointer's invalid
	if (!mesh)
		return 0;

	//get new material
	csRef<iMaterialWrapper> newmat = engine->GetMaterialList()->FindByName(texture);

	//exit if old and new materials are the same
	if (matcheck == true)
	{
		if (mesh->GetMeshObject()->GetMaterialWrapper() == newmat)
			return 0;
	}

	//set material if valid
	if (newmat)
	{
		mesh->GetMeshObject()->SetMaterialWrapper(newmat);
		return newmat;
	}
	else //otherwise report error
		ReportError("ChangeTexture: Invalid texture '" + csString(texture) + "'");

	return 0;
}

void SBS::SetTexture(csRef<iThingFactoryState> mesh, int index, const char *texture, bool has_thickness, float tw, float th)
{
	//sets texture for a range of polygons
	//thingmesh version

	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->GetPolygonName(index));
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	float tw2 = tw, th2 = th;

	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	int endindex;
	if (has_thickness == true)
		endindex = index + GetDrawWallsCount() - 1;
	else
		endindex = index;

	if (TextureOverride == false)
	{
		for (int i = index; i <= endindex; i++)
		{
			mesh->SetPolygonMaterial(csPolygonRange(i, i), material);
			//set UV texture mapping
			csVector3 v1, v2, v3;
			GetTextureMapping(mesh, i, v1, v2, v3);
			mesh->SetPolygonTextureMapping (csPolygonRange(i, i),
				v1,
				csVector2 (MapUV[0].x * tw2, MapUV[0].y * th2),
				v2,
				csVector2 (MapUV[1].x * tw2, MapUV[1].y * th2),
				v3,
				csVector2 (MapUV[2].x * tw2, MapUV[2].y * th2));
		}
	}
	else
	{
			mesh->SetPolygonMaterial(csPolygonRange(index, index), material);
			//set UV texture mapping
			csVector3 v1, v2, v3;
			GetTextureMapping(mesh, index, v1, v2, v3);
			mesh->SetPolygonTextureMapping (csPolygonRange(index, index),
				v1,
				csVector2 (MapUV[0].x * tw2, MapUV[0].y * th2),
				v2,
				csVector2 (MapUV[1].x * tw2, MapUV[1].y * th2),
				v3,
				csVector2 (MapUV[2].x * tw2, MapUV[2].y * th2));
	}
}

void SBS::SetTexture2(csRef<iMeshWrapper> mesh, const char *texture, bool has_thickness, float tw, float th)
{
	//sets texture for a range of polygons
	//genmesh version

	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->QueryObject()->GetName());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	float tw2 = tw, th2 = th;

	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	/*int endindex;
	if (has_thickness == true)
		endindex = index + GetDrawWallsCount() - 1;
	else
		endindex = index;*/

	if (TextureOverride == false)
	{
		//for (int i = index; i <= endindex; i++)
		//{
			//create texture mapping table
			csVector2 table[] = {csVector2(tw2, th2), csVector2(0, th2), csVector2(tw2, 0), csVector2(0, 0)};

			//mesh->SetPolygonMaterial(csPolygonRange(i, i), material);
			//set UV texture mapping
			csVector3 v1, v2, v3;
			//GetTextureMapping(mesh, i, v1, v2, v3);
			/*mesh->SetPolygonTextureMapping (csPolygonRange(i, i),
				v1,
				csVector2 (MapUV[0].x * tw2, MapUV[0].y * th2),
				v2,
				csVector2 (MapUV[1].x * tw2, MapUV[1].y * th2),
				v3,
				csVector2 (MapUV[2].x * tw2, MapUV[2].y * th2));*/
		//}
	}
	else
	{
			//mesh->SetPolygonMaterial(csPolygonRange(index, index), material);
			//set UV texture mapping
			csVector3 v1, v2, v3;
			//GetTextureMapping(mesh, index, v1, v2, v3);
			/*mesh->SetPolygonTextureMapping (csPolygonRange(index, index),
				v1,
				csVector2 (MapUV[0].x * tw2, MapUV[0].y * th2),
				v2,
				csVector2 (MapUV[1].x * tw2, MapUV[1].y * th2),
				v3,
				csVector2 (MapUV[2].x * tw2, MapUV[2].y * th2));*/
	}
}

bool SBS::NewElevator(int number)
{
	//create a new elevator object
	for (size_t i = 0; i < ElevatorArray.GetSize(); i++)
		if (ElevatorArray[i].number == number)
			return false;
	ElevatorArray.SetSize(ElevatorArray.GetSize() + 1);
	ElevatorArray[ElevatorArray.GetSize() - 1].number = number;
	ElevatorArray[ElevatorArray.GetSize() - 1].object = new Elevator(number);
	return true;
}

bool SBS::NewFloor(int number)
{
	//create a new floor object
	for (size_t i = 0; i < FloorArray.GetSize(); i++)
		if (FloorArray[i].number == number)
			return false;
	FloorArray.SetSize(FloorArray.GetSize() + 1);
	FloorArray[FloorArray.GetSize() - 1].number = number;
	FloorArray[FloorArray.GetSize() - 1].object = new Floor(number);

	if (number < 0)
		Basements++;
	else
		Floors++;
	return true;
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

Floor* SBS::GetFloor(int number)
{
	//return pointer to floor object

	if (FloorArray.GetSize() > 0)
	{
		//quick prediction
		if (Basements + number <= FloorArray.GetSize() - 1)
		{
			FloorMap floor = FloorArray[Basements + number];
			if (floor.number == number)
			{
				if (floor.object)
					return floor.object;
				else
					return 0;
			}
			else if (number < 0)
			{
				if (-(number + 1) <= FloorArray.GetSize() - 1)
				{
					floor = FloorArray[-(number + 1)];
					if (floor.number == number)
					{
						if (floor.object)
							return floor.object;
						else
							return 0;
					}
				}
			}
		}
	}

	for (size_t i = 0; i < FloorArray.GetSize(); i++)
		if (FloorArray[i].number == number)
			return FloorArray[i].object;
	return 0;
}

Elevator* SBS::GetElevator(int number)
{
	//return pointer to elevator object

	if (number < 1 || number > Elevators())
		return 0;

	if (ElevatorArray.GetSize() > number - 1)
	{
		//quick prediction
		if (ElevatorArray[number - 1].number == number)
		{
			if (ElevatorArray[number - 1].object)
				return ElevatorArray[number - 1].object;
			else
				return 0;
		}
	}

	for (size_t i = 0; i < ElevatorArray.GetSize(); i++)
		if (ElevatorArray[i].number == number)
			return ElevatorArray[i].object;
	return 0;
}

Shaft* SBS::GetShaft(int number)
{
	//return pointer to shaft object

	if (number < 1 || number > Shafts())
		return 0;

	if (ShaftArray.GetSize() > number - 1)
	{
		//quick prediction
		if (ShaftArray[number - 1].number == number)
		{
			if (ShaftArray[number - 1].object)
				return ShaftArray[number - 1].object;
			else
				return 0;
		}
	}

	for (size_t i = 0; i < ShaftArray.GetSize(); i++)
		if (ShaftArray[i].number == number)
			return ShaftArray[i].object;
	return 0;
}

Stairs* SBS::GetStairs(int number)
{
	//return pointer to stairs object

	if (number < 1 || number > StairsNum())
		return 0;

	if (StairsArray.GetSize() > number - 1)
	{
		//quick prediction
		if (StairsArray[number - 1].number == number)
		{
			if (StairsArray[number - 1].object)
				return StairsArray[number - 1].object;
			else
				return 0;
		}
	}

	for (size_t i = 0; i < StairsArray.GetSize(); i++)
		if (StairsArray[i].number == number)
			return StairsArray[i].object;
	return 0;
}

bool SBS::SetWallOrientation(const char *direction)
{
	//changes internal wall orientation parameter.
	//direction can either be "left" (negative), "center" (0), or "right" (positive).
	//default on startup is 1, or center.
	//the parameter is used to determine the location of the wall's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	csString temp = direction;
	temp.Downcase();

	if (temp == "left")
		wall_orientation = 0;
	else if (temp == "center")
		wall_orientation = 1;
	else if (temp == "right")
		wall_orientation = 2;
	else
	{
		ReportError("SetWallOrientation: Invalid wall orientation");
		return false;
	}
	return true;
}

int SBS::GetWallOrientation()
{
	return wall_orientation;
}

bool SBS::SetFloorOrientation(const char *direction)
{
	//changes internal floor orientation parameter.
	//direction can either be "bottom" (negative), "center" (0), or "top" (positive).
	//default on startup is 2, or top.
	//the parameter is used to determine the location of the floor's
	//x1/x2 or z1/z2 coordinates in relation to the thickness extents

	csString temp = direction;
	temp.Downcase();

	if (temp == "bottom")
		floor_orientation = 0;
	else if (temp == "center")
		floor_orientation = 1;
	else if (temp == "top")
		floor_orientation = 2;
	else
	{
		ReportError("SetFloorOrientation: Invalid floor orientation");
		return false;
	}
	return true;
}

int SBS::GetFloorOrientation()
{
	return floor_orientation;
}

void SBS::DrawWalls(bool MainN, bool MainP, bool SideN, bool SideP, bool Top, bool Bottom)
{
	//sets which walls should be drawn

	//first backup old parameters
	DrawMainNOld = DrawMainN;
	DrawMainPOld = DrawMainP;
	DrawSideNOld = DrawSideN;
	DrawSidePOld = DrawSideP;
	DrawTopOld = DrawTop;
	DrawBottomOld = DrawBottom;

	//now set new parameters
	DrawMainN = MainN;
	DrawMainP = MainP;
	DrawSideN = SideN;
	DrawSideP = SideP;
	DrawTop = Top;
	DrawBottom = Bottom;

}

void SBS::ResetWalls(bool ToDefaults)
{
	//if ToDefaults is true, this resets the DrawWalls data to the defaults.
	//if ToDefaults is false, this reverts the DrawWalls data to the previous settings.

	if (ToDefaults == true)
		DrawWalls(true, true, false, false, false, false);
	else
		DrawWalls(DrawMainNOld, DrawMainPOld, DrawSideNOld, DrawSidePOld, DrawTopOld, DrawBottomOld);
}

void SBS::SetPlanarMapping(bool flat, bool X, bool Y, bool Z)
{
	//sets planar texture mapping parameters
	//X, Y and Z reverse planar texture mapping per axis
	//Flat determines if depth should be ignored when mapping

	//first backup old parameters
	BackupMapping();

	//now set new parameters
	RevX = X;
	RevY = Y;
	RevZ = Z;
	MapUV[0] = csVector2(0, 0);
	MapUV[1] = csVector2(1, 0);
	MapUV[2] = csVector2(1, 1);
	PlanarFlat = flat;
	MapMethod = 0;
}

void SBS::SetTextureMapping(int vertindex1, csVector2 uv1, int vertindex2, csVector2 uv2, int vertindex3, csVector2 uv3)
{
	//Manually sets UV texture mapping.  Use ResetTextureMapping to return to default values

	BackupMapping();

	//set new values
	MapIndex[0] = vertindex1;
	MapIndex[1] = vertindex2;
	MapIndex[2] = vertindex3;
	MapUV[0] = uv1;
	MapUV[1] = uv2;
	MapUV[2] = uv3;
	MapMethod = 1;
}

void SBS::SetTextureMapping2(csString x1, csString y1, csString z1, csVector2 uv1, csString x2, csString y2, csString z2, csVector2 uv2, csString x3, csString y3, csString z3, csVector2 uv3)
{
	//Manually sets UV texture mapping (advanced version)
	//Use ResetTextureMapping to return to default values

	BackupMapping();

	MapVerts1[0] = x1;
	MapVerts1[1] = y1;
	MapVerts1[2] = z1;
	MapVerts2[0] = x2;
	MapVerts2[1] = y2;
	MapVerts2[2] = z2;
	MapVerts3[0] = x3;
	MapVerts3[1] = y3;
	MapVerts3[2] = z3;
	MapUV[0] = uv1;
	MapUV[1] = uv2;
	MapUV[2] = uv3;
	MapMethod = 2;
}

void SBS::BackupMapping()
{
	//backup texture mapping parameters
	if (MapMethod == 0)
	{
		OldRevX = RevX;
		OldRevY = RevY;
		OldRevZ = RevZ;
		OldPlanarFlat = PlanarFlat;
	}
	else
	{
		for (int i = 0; i <= 2; i++)
		{
			if (MapMethod == 1)
				OldMapIndex[i] = MapIndex[i];
			if (MapMethod == 2)
			{
				OldMapVerts1[i] = MapVerts1[i];
				OldMapVerts1[i] = MapVerts1[i];
				OldMapVerts1[i] = MapVerts1[i];
			}
		}
	}
	for (int i = 0; i <= 2; i++)
		OldMapUV[i] = MapUV[i];
	OldMapMethod = MapMethod;
}

void SBS::GetTextureMapping(iThingFactoryState *state, int index, csVector3 &v1, csVector3 &v2, csVector3 &v3)
{
	//returns texture mapping coordinates for the specified polygon index, in the v1, v2, and v3 vectors
	//this performs one of 3 methods - planar mapping, index mapping and manual vertex mapping

	if (MapMethod == 0)
	{
		//planar method

		csVector2 x, y, z;
		csPoly3D varray;
		bool rev_x = false, rev_z = false;

		//determine the largest projection dimension (the dimension that the polygon is generally on;
		//with a floor Y would be biggest)
		csPlane3 plane = state->GetPolygonObjectPlane(index);
		csVector3 normal = plane.GetNormal();
		int projDimension = 0; //x; faces left/right

		if (fabsf (normal.y) > fabsf (normal.x) && fabsf (normal.y) > fabsf (normal.z))
			projDimension = 1; //y biggest; faces up/down
		else if (fabsf (normal.z) > fabsf (normal.x))
			projDimension = 2; //z biggest; faces front/back
	    
		size_t selX = CS::Math::NextModulo3(projDimension);
		size_t selY = CS::Math::NextModulo3(selX);

		for (int i = 0; i < state->GetPolygonVertexCount(index); i++)
			varray.AddVertex(state->GetPolygonVertex(index, i)[selX], state->GetPolygonVertex(index, i)[selY], 0);

		if (RevX == true || (normal.x < 0.001 && normal.z < 0.001 && fabs(normal.x) > 0.999 && fabs(normal.z) > 0.999) || normal.z < -0.999)
			rev_x = true;

		if (RevZ == true || (normal.x > 0.001 && normal.z > 0.001 && fabs(normal.x) > 0.999 && fabs(normal.z) > 0.999) || normal.x > 0.999)
			rev_z = true;

		//get extents of both dimensions, since the polygon is projected in 2D as X and Y coordinates
		csVector2 a, b;
		a = GetExtents(varray, 1);
		b = GetExtents(varray, 2);

		//set the result 2D coordinates
		if (projDimension == 0)
		{
			if (rev_z == false)
			{
				v1.z = b.x; //left
				v2.z = b.y; //right
				v3.z = b.y; //right
			}
			else
			{
				v1.z = b.y; //right
				v2.z = b.x; //left
				v3.z = b.x; //left
			}
			if (RevY == false)
			{
				v1.y = a.y; //top
				v2.y = a.y; //top
				v3.y = a.x; //bottom
			}
			else
			{
				v1.y = a.x; //bottom
				v2.y = a.x; //bottom
				v3.y = a.y; //top
			}
		}
		if (projDimension == 1)
		{
			if (rev_x == false)
			{
				v1.x = b.x; //left
				v2.x = b.y; //right
				v3.x = b.y; //right
			}
			else
			{
				v1.x = b.y; //right
				v2.x = b.x; //left
				v3.x = b.x; //left
			}
			if (rev_z == false)
			{
				v1.z = a.y; //top
				v2.z = a.y; //top
				v3.z = a.x; //bottom
			}
			else
			{
				v1.z = a.x; //bottom
				v2.z = a.x; //bottom
				v3.z = a.y; //top
			}
		}
		if (projDimension == 2)
		{
			if (rev_x == false)
			{
				v1.x = a.x; //left
				v2.x = a.y; //right
				v3.x = a.y; //right
			}
			else
			{
				v1.x = a.y; //right
				v2.x = a.x; //left
				v3.x = a.x; //left
			}
			if (RevY == false)
			{
				v1.y = b.y; //top
				v2.y = b.y; //top
				v3.y = b.x; //bottom
			}
			else
			{
				v1.y = b.x; //bottom
				v2.y = b.x; //bottom
				v3.y = b.y; //top
			}
		}

		//use the plane equation to get the coordinate values of the dropped dimension
		if (projDimension == 0)
		{
			v1.x = -((plane.B() * v1.y) + (plane.C() * v1.z) + plane.D()) / plane.A(); //get X
			v2.x = -((plane.B() * v2.y) + (plane.C() * v2.z) + plane.D()) / plane.A(); //get X
			v3.x = -((plane.B() * v3.y) + (plane.C() * v3.z) + plane.D()) / plane.A(); //get X

			if (PlanarFlat == true)
				v3.x = v2.x;
		}
		if (projDimension == 1)
		{
			v1.y = -((plane.A() * v1.x) + (plane.C() * v1.z) + plane.D()) / plane.B(); //get Y
			v2.y = -((plane.A() * v2.x) + (plane.C() * v2.z) + plane.D()) / plane.B(); //get Y
			v3.y = -((plane.A() * v3.x) + (plane.C() * v3.z) + plane.D()) / plane.B(); //get Y

			if (PlanarFlat == true)
				v3.y = v2.y;
		}
		if (projDimension == 2)
		{
			v1.z = -((plane.A() * v1.x) + (plane.B() * v1.y) + plane.D()) / plane.C(); //get Z
			v2.z = -((plane.A() * v2.x) + (plane.B() * v2.y) + plane.D()) / plane.C(); //get Z
			v3.z = -((plane.A() * v3.x) + (plane.B() * v3.y) + plane.D()) / plane.C(); //get Z

			if (PlanarFlat == true)
				v3.z = v2.z;
		}
	}
	if (MapMethod == 1)
	{
		//index method
		v1 = state->GetPolygonVertex(index, MapIndex[0]);
		v2 = state->GetPolygonVertex(index, MapIndex[1]);
		v3 = state->GetPolygonVertex(index, MapIndex[2]);
	}
	if (MapMethod == 2)
	{
		//advanced manual vertex method

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				csString string;
				if (j == 0)
					string = MapVerts1[i];
				if (j == 1)
					string = MapVerts2[i];
				if (j == 2)
					string = MapVerts3[i];

				string.Downcase();

				//find X component
				int location = string.Find("x");
				if (location >= 0)
				{
					csString number = string.GetAt(location + 1);
					int num = state->GetPolygonVertexCount(index);
					if (atoi(number) < num)
						string.ReplaceAll("x" + number, _gcvt(state->GetPolygonVertex(index, atoi(number)).x, 12, buffer));
					else
						string.ReplaceAll("x" + number, "0"); //number value out of bounds
				}

				//find Y component
				location = string.Find("y");
				if (location >= 0)
				{
					csString number = string.GetAt(location + 1);
					int num = state->GetPolygonVertexCount(index);
					if (atoi(number) < num)
						string.ReplaceAll("y" + number, _gcvt(state->GetPolygonVertex(index, atoi(number)).y, 12, buffer));
					else
						string.ReplaceAll("y" + number, "0"); //number value out of bounds
				}

				//find Z component
				location = string.Find("z");
				if (location >= 0)
				{
					csString number = string.GetAt(location + 1);
					int num = state->GetPolygonVertexCount(index);
					if (atoi(number) < num)
						string.ReplaceAll("z" + number, _gcvt(state->GetPolygonVertex(index, atoi(number)).z, 12, buffer));
					else
						string.ReplaceAll("z" + number, "0"); //number value out of bounds
				}

				//store values
				if (i == 0)
				{
					if (j == 0)
						v1.x = atof(string);
					if (j == 1)
						v2.x = atof(string);
					if (j == 2)
						v3.x = atof(string);
				}
				if (i == 1)
				{
					if (j == 0)
						v1.y = atof(string);
					if (j == 1)
						v2.y = atof(string);
					if (j == 2)
						v3.y = atof(string);
				}
				if (i == 2)
				{
					if (j == 0)
						v1.z = atof(string);
					if (j == 1)
						v2.z = atof(string);
					if (j == 2)
						v3.z = atof(string);
				}
			}
		}
	}
}

void SBS::ResetTextureMapping(bool todefaults)
{
	//Resets UV texture mapping to defaults or previous values
	if (todefaults == true)
	{
		if (DefaultMapper == 0)
			SetPlanarMapping(false, false, false, false);
		if (DefaultMapper == 1)
			SetTextureMapping(0, csVector2(0, 0), 1, csVector2(1, 0), 2, csVector2(1, 1));
		if (DefaultMapper == 2)
			SetTextureMapping2("x0", "y0", "z0", csVector2(0, 0), "x1", "y1", "z1", csVector2(1, 0), "x2", "y2", "z2", csVector2(1, 1));
	}
	else
	{
		if (OldMapMethod == 0)
			SetPlanarMapping(OldPlanarFlat, OldRevX, OldRevY, OldRevZ);
		if (OldMapMethod == 1)
			SetTextureMapping(OldMapIndex[0], OldMapUV[0], OldMapIndex[1], OldMapUV[1], OldMapIndex[2], OldMapUV[2]);
		if (OldMapMethod == 2)
			SetTextureMapping2(OldMapVerts1[0], OldMapVerts1[1], OldMapVerts1[2], OldMapUV[0], OldMapVerts2[0], OldMapVerts2[1], OldMapVerts2[2], OldMapUV[1], OldMapVerts3[0], OldMapVerts3[1], OldMapVerts3[2], OldMapUV[2]);
	}
}

int SBS::GetDrawWallsCount()
{
	//gets the number of wall polygons enabled

	int sides = 0;

	if (DrawMainN == true)
		sides++;
	if (DrawMainP == true)
		sides++;
	if (DrawSideN == true)
		sides++;
	if (DrawSideP == true)
		sides++;
	if (DrawTop == true)
		sides++;
	if (DrawBottom == true)
		sides++;

	return sides;
}

csVector3 SBS::GetPoint(csRef<iThingFactoryState> mesh, const char *polyname, const csVector3 &start, const csVector3 &end)
{
	//do a line intersection with a specified mesh, and return
	//the intersection point
	int polyindex = -1;

	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		csString tmpname = mesh->GetPolygonName(i);
		if (tmpname.Find("(") == 0)
		{
			//strip object number
			int loc = tmpname.Find(")");
			tmpname.DeleteAt(0, loc + 1);
		}
		if (polyname == tmpname)
		{
			polyindex = i;
			break;
		}
	}

	if (polyindex >= 0)
	{
		//do a plane intersection with a line
		csVector3 isect;
		float dist;
		csPlane3 plane = mesh->GetPolygonObjectPlane(polyindex);
		csIntersect3::SegmentPlane(ToRemote(start), ToRemote(end), plane, isect, dist);
		return ToLocal(isect);
	}
	return 0;
}

void SBS::Cut(csRef<iMeshWrapper> mesh, csArray<WallObject*> &wallarray, csVector3 start, csVector3 end, bool cutwalls, bool cutfloors, csVector3 mesh_origin, csVector3 object_origin, int checkwallnumber, const char *checkstring)
{
	//cuts a rectangular hole in the polygons within the specified range
	//mesh_origin is a modifier for meshes with relative polygon coordinates (used only for calculating door positions) - in this you specify the mesh's global position
	//object_origin is for the object's (such as a shaft) central position, used for calculating wall offsets

	if (cutwalls == false && cutfloors == false)
		return;

	//get thing factory state
	csRef<iThingFactoryState> state = scfQueryInterface<iThingFactoryState> (mesh->GetMeshObject()->GetFactory());

	//convert values to remote
	start = ToRemote(start);
	end = ToRemote(end);
	mesh_origin = ToRemote(mesh_origin);
	object_origin = ToRemote(object_origin);

	csPoly3D temppoly, temppoly2, temppoly3, temppoly4, temppoly5, worker;
	int addpolys;
	int tmpindex;
	int tmpindex_tmp;
	int polycount;
	bool polycheck;
	if (checkwallnumber == 1)
	{
		wall1a = false;
		wall1b = false;
	}
	if (checkwallnumber == 2)
	{
		wall2a = false;
		wall2b = false;
	}

	//step through each polygon
	polycount = state->GetPolygonCount();
	for (int i = 0; i < polycount; i++)
	{
		temppoly.MakeEmpty();
		temppoly2.MakeEmpty();
		temppoly3.MakeEmpty();
		temppoly4.MakeEmpty();
		temppoly5.MakeEmpty();
		worker.MakeEmpty();
		addpolys = 0;
		tmpindex = -1;
		tmpindex_tmp = -1;
		csVector2 extentsx, extentsy, extentsz;
		polycheck = false;

		//copy source polygon vertices
		csString name = state->GetPolygonName(i);
		for (int j = 0; j < state->GetPolygonVertexCount(i); j++)
			temppoly.AddVertex(state->GetPolygonVertex(i, j));

		//make sure the polygon is not outside the cut area
		if (temppoly.ClassifyX(start.x) != CS_POL_FRONT &&
			temppoly.ClassifyX(end.x) != CS_POL_BACK &&
			temppoly.ClassifyY(start.y) != CS_POL_FRONT &&
			temppoly.ClassifyY(end.y) != CS_POL_BACK &&
			temppoly.ClassifyZ(start.z) != CS_POL_FRONT &&
			temppoly.ClassifyZ(end.z) != CS_POL_BACK)
		{
			//Report("Cutting polygon " + name);

			extentsx = GetExtents(temppoly, 1);
			extentsy = GetExtents(temppoly, 2);
			extentsz = GetExtents(temppoly, 3);

			//is polygon a wall?
			if (extentsy.x != extentsy.y)
			{
				if (cutwalls == true)
				{
					//wall
					if (fabs(extentsx.x - extentsx.y) > fabs(extentsz.x - extentsz.y))
					{
						//wall is facing forward/backward

						//get left side
						worker = temppoly;
						worker.SplitWithPlaneX(temppoly, temppoly2, start.x);
						worker.MakeEmpty();

						//get right side
						if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else
							worker = temppoly;
						worker.SplitWithPlaneX(temppoly3, temppoly2, end.x);
						worker.MakeEmpty();

						//get lower
						if (temppoly3.GetVertexCount() > 0)
							worker = temppoly3;
						else if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else if (temppoly.GetVertexCount() > 0)
							worker = temppoly;
						worker.SplitWithPlaneY(temppoly3, temppoly4, start.y);
						worker.MakeEmpty();

						//get upper
						if (temppoly4.GetVertexCount() > 0)
							worker = temppoly4;
						else if (temppoly3.GetVertexCount() > 0)
							worker = temppoly3;
						else if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else if (temppoly.GetVertexCount() > 0)
							worker = temppoly;
						worker.SplitWithPlaneY(temppoly5, temppoly4, end.y);
						worker.MakeEmpty();
					}
					else
					{
						//wall is facing left/right

						//get left side
						worker = temppoly;
						worker.SplitWithPlaneZ(temppoly, temppoly2, start.z);
						worker.MakeEmpty();

						//get right side
						if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else
							worker = temppoly;
						worker.SplitWithPlaneZ(temppoly3, temppoly2, end.z);
						worker.MakeEmpty();

						//get lower
						if (temppoly3.GetVertexCount() > 0)
							worker = temppoly3;
						else if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else if (temppoly.GetVertexCount() > 0)
							worker = temppoly;
						worker.SplitWithPlaneY(temppoly3, temppoly4, start.y);
						worker.MakeEmpty();

						//get upper
						if (temppoly4.GetVertexCount() > 0)
							worker = temppoly4;
						else if (temppoly3.GetVertexCount() > 0)
							worker = temppoly3;
						else if (temppoly2.GetVertexCount() > 0)
							worker = temppoly2;
						else if (temppoly.GetVertexCount() > 0)
							worker = temppoly;
						worker.SplitWithPlaneY(temppoly5, temppoly4, end.y);
						worker.MakeEmpty();
					}
					polycheck = true;
					//store extents of temppoly5 for door sides if needed
					if (checkwallnumber > 0 && checkwallnumber < 3)
					{
						if (name.Find(checkstring) >= 0)
						{
							float extent;
							if (checkwallnumber == 2 && (wall2a == false || wall2b == false))
							{
								//level walls
								if (wall2a == true)
									wall2b = true;
								wall2a = true;
								extent = GetExtents(temppoly5, 1).x + mesh_origin.x;
								if (wall2b == false || (wall2b == true && fabs(extent - object_origin.x) > fabs(wall_extents_x.x - object_origin.x)))
									wall_extents_x.x = extent;
								extent = GetExtents(temppoly5, 3).x + mesh_origin.z;
								if (wall2b == false || (wall2b == true && fabs(extent - object_origin.z) > fabs(wall_extents_z.x - object_origin.z)))
									wall_extents_z.x = extent;
								wall_extents_y = GetExtents(temppoly5, 2) + mesh_origin.y;
							}
							else if (wall1a == false || wall1b == false)
							{
								//shaft walls
								if (wall1a == true)
									wall1b = true;
								wall1a = true;
								extent = GetExtents(temppoly5, 1).y + mesh_origin.x;
								if (wall1b == false || (wall1b == true && fabs(extent - object_origin.x) < fabs(wall_extents_x.y - object_origin.x)))
									wall_extents_x.y = extent;
								extent = GetExtents(temppoly5, 3).y + mesh_origin.z;
								if (wall1b == false || (wall1b == true && fabs(extent - object_origin.z) < fabs(wall_extents_z.y - object_origin.z)))
									wall_extents_z.y = extent;
							}
						}
					}
				}
			}
			else if (cutfloors == true)
			{
				//floor

				//get left side
				worker = temppoly;
				worker.SplitWithPlaneX(temppoly, temppoly2, start.x);
				worker.MakeEmpty();

				//get right side
				if (temppoly2.GetVertexCount() > 0)
					worker = temppoly2;
				else
					worker = temppoly;
				worker.SplitWithPlaneX(temppoly3, temppoly2, end.x);
				worker.MakeEmpty();

				//get lower
				if (temppoly3.GetVertexCount() > 0)
					worker = temppoly3;
				else if (temppoly2.GetVertexCount() > 0)
					worker = temppoly2;
				else if (temppoly.GetVertexCount() > 0)
					worker = temppoly;
				worker.SplitWithPlaneZ(temppoly3, temppoly4, start.z);
				worker.MakeEmpty();

				//get upper
				if (temppoly4.GetVertexCount() > 0)
					worker = temppoly4;
				else if (temppoly3.GetVertexCount() > 0)
					worker = temppoly3;
				else if (temppoly2.GetVertexCount() > 0)
					worker = temppoly2;
				else if (temppoly.GetVertexCount() > 0)
					worker = temppoly;
				worker.SplitWithPlaneZ(temppoly5, temppoly4, end.z);
				worker.MakeEmpty();

				polycheck = true;
			}

			if (polycheck == true)
			{
				//get texture data from original polygon
				iMaterialWrapper *oldmat = state->GetPolygonMaterial(i);
				csVector3 oldvector;
				csMatrix3 mapping;
				state->GetPolygonTextureMapping(i, mapping, oldvector);

				//get wall object
				WallObject *wallobject = GetWallObject(wallarray, i);

				//delete original polygon
				wallobject->DeletePolygon(i, false);
				i--;
				polycount--;

				//create splitted polygons
				if (temppoly.GetVertexCount() > 2)
				{
					addpolys++;
					tmpindex_tmp = wallobject->AddPolygon(name, temppoly.GetVertices(), temppoly.GetVertexCount());
					if (tmpindex == -1)
						tmpindex = tmpindex_tmp;
				}
				temppoly.MakeEmpty();
				if (temppoly2.GetVertexCount() > 2)
				{
					addpolys++;
					tmpindex_tmp = wallobject->AddPolygon(name, temppoly2.GetVertices(), temppoly2.GetVertexCount());
					if (tmpindex == -1)
						tmpindex = tmpindex_tmp;
				}
				temppoly2.MakeEmpty();
				if (temppoly3.GetVertexCount() > 2)
				{
					addpolys++;
					tmpindex_tmp = wallobject->AddPolygon(name, temppoly3.GetVertices(), temppoly3.GetVertexCount());
					if (tmpindex == -1)
						tmpindex = tmpindex_tmp;
				}
				temppoly3.MakeEmpty();
				if (temppoly4.GetVertexCount() > 2)
				{
					addpolys++;
					tmpindex_tmp = wallobject->AddPolygon(name, temppoly4.GetVertices(), temppoly4.GetVertexCount());
					if (tmpindex == -1)
						tmpindex = tmpindex_tmp;
				}
				temppoly4.MakeEmpty();

				//apply material to new polygon set
				if (addpolys > 0)
				{
					state->SetPolygonMaterial(csPolygonRange(tmpindex, tmpindex + addpolys - 1), oldmat);
					state->SetPolygonTextureMapping(csPolygonRange(tmpindex, tmpindex + addpolys - 1), mapping, oldvector);
				}
			}
		}
	}

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}
}

float SBS::MetersToFeet(float meters)
{
	//converts meters to feet
	return meters * 3.2808399f;
}

float SBS::FeetToMeters(float feet)
{
	//converts feet to meters
	return feet / 3.2808399f;
}

int SBS::AddDoorwayWalls(WallObject *wallobject, const char *texture, float tw, float th)
{
	//add joining doorway polygons if needed

	int index = 0;
	if (wall1a == true && wall2a == true)
	{
		//convert values to local
		wall_extents_x = ToLocal(wall_extents_x);
		wall_extents_y = ToLocal(wall_extents_y);
		wall_extents_z = ToLocal(wall_extents_z);

		wall1a = false;
		wall1b = false;
		wall2a = false;
		wall2b = false;
		DrawWalls(true, true, false, false, false, false);
		if (fabs(wall_extents_x.x - wall_extents_x.y) > fabs(wall_extents_z.x - wall_extents_z.y))
		{
			//doorway is facing forward/backward
			index = AddWallMain(wallobject, "DoorwayLeft", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.x, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th);
			AddWallMain(wallobject, "DoorwayRight", texture, 0, wall_extents_x.y, wall_extents_z.x, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th);
			AddFloorMain(wallobject, "DoorwayTop", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y, wall_extents_y.y, tw, th);
		}
		else
		{
			//doorway is facing left/right
			AddWallMain(wallobject, "DoorwayLeft", texture, 0, wall_extents_x.x, wall_extents_z.y, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th);
			AddWallMain(wallobject, "DoorwayRight", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.y, wall_extents_z.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.y - wall_extents_y.x, wall_extents_y.x, wall_extents_y.x, tw, th);
			AddFloorMain(wallobject, "DoorwayTop", texture, 0, wall_extents_x.x, wall_extents_z.x, wall_extents_x.y, wall_extents_z.y, wall_extents_y.y, wall_extents_y.y, tw, th);
		}
		ResetWalls();
	}
	return index;
}

void SBS::SetAutoSize(bool x, bool y)
{
	//enable or disable texture autosizing
	AutoX = x;
	AutoY = y;
}

void SBS::GetAutoSize(bool &x, bool &y)
{
	//return autosizing values
	x = AutoX;
	y = AutoY;
}

void SBS::ReverseAxis(bool value)
{
	//reverse wall/floor altitude axis
	ReverseAxisValue = value;
}

bool SBS::GetReverseAxis()
{
	return ReverseAxisValue;
}

void SBS::SetListenerLocation(const csVector3 &location)
{
	//set position of sound listener object
	if (DisableSound == false)
		sndrenderer->GetListener()->SetPosition(location);
}

void SBS::SetListenerDirection(const csVector3 &front, const csVector3 &top)
{
	//set direction of sound listener object
	if (DisableSound == false)
		sndrenderer->GetListener()->SetDirection(front, top);
}

void SBS::SetListenerDistanceFactor(float factor)
{
	if (DisableSound == false)
		sndrenderer->GetListener()->SetDistanceFactor(factor);
}

float SBS::GetListenerDistanceFactor()
{
	if (DisableSound == false)
		return sndrenderer->GetListener()->GetDistanceFactor();
	else
		return 0;
}

void SBS::SetListenerRollOffFactor(float factor)
{
	if (DisableSound == false)
		sndrenderer->GetListener()->SetRollOffFactor(factor);
}

float SBS::GetListenerRollOffFactor()
{
	if (DisableSound == false)
		return sndrenderer->GetListener()->GetRollOffFactor();
	else
		return 0;
}

void SBS::SetTextureOverride(const char *mainneg, const char *mainpos, const char *sideneg, const char *sidepos, const char *top, const char *bottom)
{
	//set override textures and enable override
	mainnegtex = mainneg;
	mainnegtex.Trim();
	mainpostex = mainpos;
	mainpostex.Trim();
	sidenegtex = sideneg;
	sidenegtex.Trim();
	sidepostex = sidepos;
	sidepostex.Trim();
	toptex = top;
	toptex.Trim();
	bottomtex = bottom;
	bottomtex.Trim();
	TextureOverride = true;
}

void SBS::SetTextureFlip(int mainneg, int mainpos, int sideneg, int sidepos, int top, int bottom)
{
	//flip a texture on a specified side either horizontally or vertically (or both)
	//parameters are:
	//0 = no flipping
	//1 = flip horizontally
	//2 = flip vertically
	//3 = flip both

	mainnegflip = mainneg;
	mainposflip = mainpos;
	sidenegflip = sideneg;
	sideposflip = sidepos;
	topflip = top;
	bottomflip = bottom;
	FlipTexture = true;
}

WallObject* SBS::AddWall(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float height_in1, float height_in2, float altitude1, float altitude2, float tw, float th)
{
	//meshname can either be:
	//external, landscape, or buildings

	//Adds a wall with the specified dimensions
	csString mesh = meshname;
	mesh.Trim();

	//Set horizontal scaling
	x1 = x1 * HorizScale;
	x2 = x2 * HorizScale;
	z1 = z1 * HorizScale;
	z2 = z2 * HorizScale;

	//calculate autosizing
	float tmpheight;
	if (height_in1 > height_in2)
		tmpheight = height_in1;
	else
		tmpheight = height_in2;
	csVector2 sizing = sbs->CalculateSizing(texture, csVector2(x1, x2), csVector2(0, tmpheight), csVector2(z1, z2), tw, th);

	WallObject *wall;
	if (mesh.CompareNoCase("external") == true)
		wall = CreateWallObject(External_walls, External, this->object, name);
	if (mesh.CompareNoCase("buildings") == true)
		wall = CreateWallObject(Buildings_walls, Buildings, this->object, name);
	if (mesh.CompareNoCase("landscape") == true)
		wall = CreateWallObject(Landscape_walls, Landscape, this->object, name);

	AddWallMain(wall, name, texture, thickness, x1, z1, x2, z2, height_in1, height_in2, altitude1, altitude2, sizing.x, sizing.y);
	return wall;
}

WallObject* SBS::AddFloor(const char *meshname, const char *name, const char *texture, float thickness, float x1, float z1, float x2, float z2, float altitude1, float altitude2, float tw, float th)
{
	//meshname can either be:
	//external, landscape, or buildings

	//Adds a floor with the specified dimensions and vertical offset
	float tw2;
	float th2;
	csString mesh = meshname;
	mesh.Trim();

	//Set horizontal scaling
	x1 = x1 * HorizScale;
	x2 = x2 * HorizScale;
	z1 = z1 * HorizScale;
	z2 = z2 * HorizScale;

	bool force_enable, force_mode;
	GetTextureForce(texture, force_enable, force_mode);

	//Call texture autosizing formulas
	tw2 = AutoSize(x1, x2, true, tw, force_enable, force_mode);
	th2 = AutoSize(z1, z2, false, th, force_enable, force_mode);

	WallObject *wall;
	if (mesh.CompareNoCase("external") == true)
		wall = CreateWallObject(External_walls, External, this->object, name);
	if (mesh.CompareNoCase("buildings") == true)
		wall = CreateWallObject(Buildings_walls, Buildings, this->object, name);
	if (mesh.CompareNoCase("landscape") == true)
		wall = CreateWallObject(Landscape_walls, Landscape, this->object, name);

	AddFloorMain(wall, name, texture, thickness, x1, z1, x2, z2, altitude1, altitude2, tw2, th2);
	return wall;
}

WallObject* SBS::AddGround(const char *name, const char *texture, float x1, float z1, float x2, float z2, float altitude, int tile_x, int tile_z)
{
	//Adds ground based on a tiled-floor layout, with the specified dimensions and vertical offset
	//this does not support thickness

	csVector3 v1, v2, v3, v4;

	float minx, minz, maxx, maxz;

	//get min and max values
	if (x1 < x2)
	{
		minx = x1;
		maxx = x2;
	}
	else
	{
		minx = x2;
		maxx = x1;
	}
	if (z1 < z2)
	{
		minz = z1;
		maxz = z2;
	}
	else
	{
		minz = z2;
		maxz = z1;
	}

	WallObject *wall = CreateWallObject(Landscape_walls, Landscape, this->object, name);

	//create polygon tiles
	for (float i = minx; i < maxx; i += tile_x)
	{
		int sizex, sizez;

		if (i + tile_x > maxx)
			sizex = maxx - i;
		else
			sizex = tile_x;

		for (float j = minz; j < maxz; j += tile_z)
		{
			if (j + tile_z > maxz)
				sizez = maxz - i;
			else
				sizez = tile_z;

			AddFloorMain(wall, name, texture, 0, i, j, i + sizex, j + sizez, altitude, altitude, 1, 1);
		}
	}
	return wall;
}

void SBS::EnableFloorRange(int floor, int range, bool value, bool enablegroups, int shaftnumber)
{
	//turn on/off a range of floors
	//if range is 3, show shaft on current floor (floor), and 1 floor below and above (3 total floors)
	//if range is 1, show only the current floor (floor)

	//range must be greater than 0
	if (range < 1)
		range = 1;

	//range must be an odd number; if it's even, then add 1
	if (IsEven(range) == true)
		range++;

	int additionalfloors;
	if (range > 1)
		additionalfloors = (range - 1) / 2;
	else
		additionalfloors = 0;

	//disable floors 1 floor outside of range
	if (value == true)
	{
		if (IsValidFloor(floor - additionalfloors - 1))
			GetFloor(floor - additionalfloors - 1)->Enabled(false);
		if (IsValidFloor(floor + additionalfloors + 1))
			GetFloor(floor + additionalfloors + 1)->Enabled(false);
	}

	//enable floors within range
	for (int i = floor - additionalfloors; i <= floor + additionalfloors; i++)
	{
		if (IsValidFloor(i))
		{
			if (shaftnumber > 0)
			{
				//if a shaft is specified, only show the floor if it is in the related shaft's ShowFloorsList array
				if (GetShaft(shaftnumber)->ShowFloors == true)
				{
					if (GetShaft(shaftnumber)->ShowFloorsList.Find(i) != -1 && value == true)
					{
						GetFloor(i)->Enabled(true);
						if (enablegroups == true)
							GetFloor(i)->EnableGroup(true);
					}
					else
					{
						GetFloor(i)->Enabled(false);
						if (enablegroups == true)
							GetFloor(i)->EnableGroup(false);
					}
				}
			}
			else
			{
				GetFloor(i)->Enabled(value);
				if (enablegroups == true)
					GetFloor(i)->EnableGroup(value);
			}
		}
	}
}

bool SBS::RegisterDoorCallback(Door *door)
{
	//register a door object for callbacks (used for door movement)

	int index = doorcallbacks.Find(door);

	if (index == csArrayItemNotFound)
	{
		//if door isn't already in the array, add it
		doorcallbacks.Push(door);
	}
	else
	{
		//otherwise change door's direction
		if (doorcallbacks[index])
			doorcallbacks[index]->OpenDoor = !doorcallbacks[index]->OpenDoor;
	}
	return true;
}

bool SBS::UnregisterDoorCallback(Door *door)
{
	int index = doorcallbacks.Find(door);

	if (index != csArrayItemNotFound && doorcallbacks[index])
	{
		//unregister existing door callback
		if (doorcallbacks[index]->IsMoving == false)
		{
			doorcallbacks.Delete(door);
			return true;
		}
		else
		{
			Report("Door in use; cannot unregister callback");
			return false;
		}
	}
	else
		return false;
}

bool SBS::RegisterCallButtonCallback(CallButton *button)
{
	//register a door object for callbacks (used for door movement)

	int index = buttoncallbacks.Find(button);

	if (index == csArrayItemNotFound)
	{
		//if call button isn't already in the array, add it
		buttoncallbacks.Push(button);
	}
	else
		return false;
	return true;
}

bool SBS::UnregisterCallButtonCallback(CallButton *button)
{
	int index = buttoncallbacks.Find(button);

	if (index != csArrayItemNotFound && buttoncallbacks[index])
	{
		//unregister existing call button callback
		buttoncallbacks.Delete(button);
	}
	else
		return false;
	return true;
}

void SBS::ProcessTextureFlip(float tw, float th)
{
	//process texture flip info

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	for (int i = 0; i <= 5; i++)
	{
		widthscale[i] = tw;
		heightscale[i] = th;
	}

	//texture flipping
	if (FlipTexture == true)
	{
		int info;
		for (int i = 0; i <= 5; i++)
		{
			info = 0;
			if (i == 0)
				info = mainnegflip;
			if (i == 1)
				info = mainposflip;
			if (i == 2)
				info = sidenegflip;
			if (i == 3)
				info = sideposflip;
			if (i == 4)
				info = topflip;
			if (i == 5)
				info = bottomflip;

			if (info == 1 || info == 3)
				widthscale[i] = -tw;
			if (info == 2 || info == 3)
				heightscale[i] = -th;
		}
	}
}

iMaterialWrapper* SBS::GetTextureMaterial(const char *texture, bool &result, const char *polygon_name)
{
	//perform a lookup on a texture, and return as a material
	//returns false in &result if texture load failed, and if default material was used instead
	iMaterialWrapper *material = engine->GetMaterialList()->FindByName(texture);

	if (!material)
	{
		//if material's not found, display a warning and use a default material
		csString message;
		if (polygon_name)
			message = "Texture '" + csString(texture) + "' not found for polygon '" + csString(polygon_name) + "'; using default material";
		else
			message = "Texture '" + csString(texture) + "' not found; using default material";
		ReportError(message);
		//set to default material
		material = engine->GetMaterialList()->FindByName("Default");
		result = false;
	}
	else
		result = true;
	return material;
}

bool SBS::GetTextureTiling(const char *texture, float &tw, float &th)
{
	//get per-texture tiling values from the textureinfo array
	for (int i = 0; i < textureinfo.GetSize(); i++)
	{
		if (textureinfo[i].name == texture)
		{
			tw = textureinfo[i].widthmult;
			th = textureinfo[i].heightmult;
			return true;
		}
	}
	return false;
}

bool SBS::GetTextureForce(const char *texture, bool &enable_force, bool &force_mode)
{
	//get per-texture tiling values from the textureinfo array
	for (int i = 0; i < textureinfo.GetSize(); i++)
	{
		if (textureinfo[i].name == texture)
		{
			enable_force = textureinfo[i].enable_force;
			force_mode = textureinfo[i].force_mode;
			return true;
		}
	}
	return false;
}

void SBS::EnableMesh(csRef<iMeshWrapper> mesh, bool value)
{
	//enables or disables a mesh
	if (mesh)
	{
		if (value == true)
		{
			mesh->GetFlags().Reset(CS_ENTITY_INVISIBLEMESH);
			mesh->GetFlags().Reset(CS_ENTITY_NOSHADOWS);
			mesh->GetFlags().Reset(CS_ENTITY_NOHITBEAM);
		}
		else
		{
			mesh->GetFlags().Set(CS_ENTITY_INVISIBLEMESH);
			mesh->GetFlags().Set(CS_ENTITY_NOSHADOWS);
			mesh->GetFlags().Set(CS_ENTITY_NOHITBEAM);
		}
	}
}

iMeshWrapper* SBS::AddGenWall(csRef<iMeshWrapper> mesh, const char *texture, float x1, float z1, float x2, float z2, float height, float altitude, float tw, float th)
{
	//add a simple wall in a general mesh (currently only used for objects that change textures)

	//get texture
	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->QueryObject()->GetName());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	float tw2 = tw, th2 = th;

	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	//create texture mapping table
	csVector2 table[] = {csVector2(tw2, th2), csVector2(0, th2), csVector2(tw2, 0), csVector2(0, 0)};

	//create a quad, map the texture, and append to the mesh
	CS::Geometry::TesselatedQuad wall (csVector3(ToRemote(x2), ToRemote(altitude), ToRemote(z2)), csVector3(ToRemote(x1), ToRemote(altitude), ToRemote(z1)), csVector3(ToRemote(x2), ToRemote(altitude + height), ToRemote(z2)));
	CS::Geometry::TableTextureMapper mapper(table);
	wall.SetMapper(&mapper);
	wall.Append(mesh->GetFactory());

	//set lighting factor
	mesh->GetMeshObject()->SetColor(csColor(1, 1, 1));

	//set texture
	mesh->GetMeshObject()->SetMaterialWrapper(material);

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}

	return mesh;
}

void SBS::ProcessCallButtons()
{
	//process all registered call buttons

	//the up and down sections need to be processed separately due to the removal of callbacks
	//during the run of each

	for (int i = 0; i < buttoncallbacks.GetSize(); i++)
	{
		//process up calls
		if (buttoncallbacks[i])
			buttoncallbacks[i]->Loop(true);
	}

	for (int i = 0; i < buttoncallbacks.GetSize(); i++)
	{
		//process down calls
		if (buttoncallbacks[i])
			buttoncallbacks[i]->Loop(false);
	}
}

void SBS::ProcessDoors()
{
	//process all registered doors
	for (int i = 0; i < doorcallbacks.GetSize(); i++)
	{
		if (doorcallbacks[i])
		{
			if (doorcallbacks[i]->IsMoving == true)
				doorcallbacks[i]->MoveDoor();
			else
				UnregisterDoorCallback(doorcallbacks[i]);
		}
	}
}

int SBS::GetDoorCallbackCount()
{
	//return the number of registered door callbacks
	return doorcallbacks.GetSize();
}

int SBS::GetCallButtonCallbackCount()
{
	//return the number of registered call button callbacks
	return buttoncallbacks.GetSize();
}

bool SBS::Mount(const char *filename, const char *path)
{
	//mounts a zip file into the virtual filesystem

	csString file = filename;
	csString Path = path;

	Report("Mounting " + file + " as path " + Path);
	bool status = vfs->Mount(path, root_dir + "data" + dir_char + file);
	if (status == false)
		ReportError("Error mounting file " + file);
	return status;
}

void SBS::FreeTextureImages()
{
	//unload images in all texture wrappers

	for (int i = 0; i < engine->GetTextureList()->GetCount(); i++)
		engine->GetTextureList()->Get(i)->SetImageFile(0);
}

void SBS::AddFloorAutoArea(csVector3 start, csVector3 end)
{
	//adds an auto area that enables/disables floors

	AutoArea newarea;
	newarea.box = csBox3(start, end);
	newarea.inside = false;
	newarea.camerafloor = 0;
	FloorAutoArea.Push(newarea);
}

void SBS::CheckAutoAreas()
{
	//check all automatic areas
	
	csVector3 position = camera->GetPosition();
	int floor = camera->CurrentFloor;

	for (int i = 0; i < FloorAutoArea.GetSize(); i++)
	{
		//reset inside value if floor changed
		if (FloorAutoArea[i].camerafloor != floor)
			FloorAutoArea[i].inside = false;

		if (FloorAutoArea[i].box.In(position) == true && FloorAutoArea[i].inside == false)
		{
			//user moved into box; enable floors
			FloorAutoArea[i].inside = true;
			FloorAutoArea[i].camerafloor = floor;
			if (floor > -Basements)
			{
				GetFloor(floor - 1)->Enabled(true);
				GetFloor(floor - 1)->EnableGroup(true);
			}
			GetFloor(floor)->Enabled(true);
			GetFloor(floor)->EnableGroup(true);
			if (floor < Floors - 1)
			{
				GetFloor(floor + 1)->Enabled(true);
				GetFloor(floor + 1)->EnableGroup(true);
			}
		}
		if (FloorAutoArea[i].box.In(position) == false && FloorAutoArea[i].inside == true)
		{
			//user moved out of box; disable floors except current
			FloorAutoArea[i].inside = false;
			FloorAutoArea[i].camerafloor = 0;
			if (floor > -Basements)
			{
				GetFloor(floor - 1)->Enabled(false);
				GetFloor(floor - 1)->EnableGroup(false);
			}
			if (floor < Floors - 1)
			{
				GetFloor(floor + 1)->Enabled(false);
				GetFloor(floor + 1)->EnableGroup(false);
			}
			GetFloor(floor)->Enabled(true);
			GetFloor(floor)->EnableGroup(true);
		}
	}
}

int SBS::GetMeshCount()
{
	//return total number of mesh objects
	return engine->GetMeshes()->GetCount();
}

int SBS::GetTextureCount()
{
	//return total number of textures
	return engine->GetTextureList()->GetCount();
}

int SBS::GetMaterialCount()
{
	//return total number of materials
	return engine->GetMaterialList()->GetCount();
}

int SBS::GetMeshFactoryCount()
{
	//return total number of mesh factories
	return engine->GetMeshFactories()->GetCount();
}

void SBS::CreateColliders(csRef<iMeshWrapper> mesh)
{
	//create colliders for the given mesh
	csColliderHelper::InitializeCollisionWrapper(collision_sys, mesh);
}

void SBS::DeleteColliders(csRef<iMeshWrapper> mesh)
{
	//delete colliders for the given mesh
	csColliderWrapper *collider = csColliderWrapper::GetColliderWrapper(mesh->QueryObject());
	if (collider)
		engine->RemoveObject(collider);
}

Object* SBS::AddSound(const char *name, const char *filename, csVector3 position, int volume, int speed, float min_distance, float max_distance, float dir_radiation, csVector3 direction)
{
	//create a looping sound object
	sounds.SetSize(sounds.GetSize() + 1);
	Sound *sound = sounds[sounds.GetSize() - 1];
	sound = new Sound(this->object, name, false);

	//set parameters and play sound
	sound->SetPosition(position);
	sound->SetDirection(direction);
	sound->SetVolume(volume);
	sound->SetSpeed(speed);
	sound->SetMinimumDistance(min_distance);
	sound->SetMaximumDistance(max_distance);
	sound->SetDirection(direction);
	sound->SetDirectionalRadiation(dir_radiation);
	sound->Load(filename);
	sound->Loop(true);
	sound->Play();

	return sound->object;
}

int SBS::GetSoundCount()
{
	//return total number of allocated sounds
	return soundcount;
}

void SBS::IncrementSoundCount()
{
	soundcount++;
}

void SBS::DecrementSoundCount()
{
	soundcount--;
}

float SBS::ToLocal(float remote_value)
{
	//convert remote (Crystal Space) vertex positions to local (SBS) positions
	return remote_value * UnitScale;
}

csVector2 SBS::ToLocal(csVector2 remote_value)
{
	//convert remote (Crystal Space) vertex positions to local (SBS) positions
	return remote_value * UnitScale;
}

csVector3 SBS::ToLocal(csVector3 remote_value)
{
	//convert remote (Crystal Space) vertex positions to local (SBS) positions
	return remote_value * UnitScale;
}

float SBS::ToRemote(float local_value)
{
	//convert local (SBS) vertex positions to remote (Crystal Space) positions
	return local_value / UnitScale;
}

csVector2 SBS::ToRemote(csVector2 local_value)
{
	//convert local (SBS) vertex positions to remote (Crystal Space) positions
	return local_value / UnitScale;
}

csVector3 SBS::ToRemote(csVector3 local_value)
{
	//convert local (SBS) vertex positions to remote (Crystal Space) positions
	return local_value / UnitScale;
}

int SBS::GetObjectCount()
{
	//return number of registered SBS objects
	return ObjectCount;
}

Object* SBS::GetObject(int number)
{
	//return object pointer from global array
	if (number >= 0 && number < ObjectArray.GetSize())
		return ObjectArray[number];
	else
		return 0;
}

int SBS::RegisterObject(Object *object)
{
	//add object to global array
	ObjectCount++;
	return ObjectArray.Push(object);
}

bool SBS::UnregisterObject(int number)
{
	//remove object
	//note - this doesn't delete the objects
	ObjectCount--;

	if (number < ObjectArray.GetSize())
	{
		if (ObjectArray[number])
		{
			if (ObjectArray[number]->GetNumber() == number)
			{
				ObjectArray[number] = 0;
				return true;
			}
		}
	}
	return false;
}

csVector3 SBS::GetWallExtents(csRef<iThingFactoryState> state, const char *name, float altitude, bool get_max)
{
	//return the X and Z extents of a standard wall (by name) at a specific altitude, by doing a double plane cut

	csString newname;
	csString name2 = name;
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
			newname = name2;
		if (i == 1)
			newname = name2 + ":0";
		if (i == 2)
			newname = name2 + ":1";
		if (i == 3)
			newname = name2 + ":front";
		if (i == 4)
			newname = name2 + ":back";
		if (i == 5)
			newname = name2 + ":left";
		if (i == 6)
			newname = name2 + ":right";

		int index = -1;
		for (int j = 0; j < state->GetPolygonCount(); j++)
		{
			csString tmpname = state->GetPolygonName(j);
			if (tmpname.Find("(") == 0)
			{
				//strip object number
				int loc = tmpname.Find(")");
				tmpname.DeleteAt(0, loc + 1);
			}
			if (newname == tmpname)
			{
				index = j;
				break;
			}
		}

		if (index >= 0)
		{
			csPoly3D original, tmp1, tmp2;
			for (int i = 0; i < state->GetPolygonVertexCount(index); i++)
				original.AddVertex(state->GetPolygonVertex(index, i));

			//if given altitude is outside of polygon's range, return 0
			csVector2 yextents = GetExtents(original, 2);
			float tmpaltitude = ToRemote(altitude);
			if (tmpaltitude < yextents.x || tmpaltitude > yextents.y)
				return 0;

			//get upper
			original.SplitWithPlaneY(tmp1, tmp2, tmpaltitude - 0.001);

			//get lower part of upper
			tmp2.SplitWithPlaneY(original, tmp1, tmpaltitude + 0.001);

			csVector3 result;
			if (get_max == false)
			{
				//get minimum extents
				result.x = ToLocal(GetExtents(original, 1).x);
				result.z = ToLocal(GetExtents(original, 3).x);
			}
			else
			{
				//get maximum extents
				result.x = ToLocal(GetExtents(original, 1).y);
				result.z = ToLocal(GetExtents(original, 3).y);
			}
			result.y = altitude;
			return result;
		}
	}
	return 0;
}

csVector3 SBS::GetPolygonDirection(csPoly3D &polygon)
{
	//returns the direction the polygon faces, in a 3D vector
	//for example, <-1, 0, 0> means that the wall faces left.

	//get largest normal

	csVector3 normal = polygon.ComputeNormal();

	int largest_normal = 0; //x

	if (fabsf(normal.y) > fabsf(normal.x) && fabsf(normal.y) > fabsf(normal.z))
		largest_normal = 1; //y biggest
	else if (fabsf(normal.z) > fabsf(normal.x))
		largest_normal = 2; //z biggest

	if (largest_normal == 0)
	{
		if (normal.x < 0)
			return csVector3(1, 0, 0);
		else
			return csVector3(-1, 0, 0);
	}

	if (largest_normal == 1)
	{
		if (normal.y < 0)
			return csVector3(0, 1, 0);
		else
			return csVector3(0, -1, 0);
	}

	if (largest_normal == 2)
	{
		if (normal.z < 0)
			return csVector3(0, 0, 1);
		else
			return csVector3(0, 0, -1);
	}
}

csVector2 SBS::CalculateSizing(const char *texture, csVector2 x, csVector2 y, csVector2 z, float tw, float th)
{
	//calculate autosizing based on polygon extents

	//Call texture autosizing formulas
	float tw2 = tw, th2 = th;

	bool force_enable, force_mode;
	GetTextureForce(texture, force_enable, force_mode);

	if (z.x == z.y)
		tw2 = AutoSize(x.x, x.y, true, tw, force_enable, force_mode);
	if (x.x == x.y)
		tw2 = AutoSize(z.x, z.y, true, tw, force_enable, force_mode);
	if ((z.x != z.y) && (x.x != x.y))
	{
		//calculate diagonals
		float tempw1, tempw2;
		if (x.x > x.y)
			tempw1 = x.x - x.y;
		else
			tempw1 = x.y - x.x;
		if (z.x > z.y)
			tempw2 = z.x - z.y;
		else
			tempw2 = z.y - z.x;
		tw2 = AutoSize(0, sqrt(pow(tempw1, 2) + pow(tempw2, 2)), true, tw, force_enable, force_mode);
	}
	th2 = AutoSize(y.x, y.y, false, th, force_enable, force_mode);

	//return results
	return csVector2(tw2, th2);
}

void SBS::ApplyTextureMapping(iThingFactoryState *state, int start_index, int end_index, float tw, float th)
{
	//apply UV texture mapping
	for (int i = start_index; i <= end_index; i++)
	{
		csVector3 v1, v2, v3;
		GetTextureMapping(state, i, v1, v2, v3);
		state->SetPolygonTextureMapping(csPolygonRange(i, i),
			v1,
			csVector2(MapUV[0].x * tw, MapUV[0].y * th),
			v2,
			csVector2(MapUV[1].x * tw, MapUV[1].y * th),
			v3,
			csVector2(MapUV[2].x * tw, MapUV[2].y * th));
	}
}

WallObject* SBS::CreateWallObject(csArray<WallObject*> &array, csRef<iMeshWrapper> mesh, Object *parent, const char *name)
{
	//create a new polygon object in the given array

	array.SetSize(array.GetSize() + 1);
	array[array.GetSize() - 1] = new WallObject(mesh);
	array[array.GetSize() - 1]->name = name;
	array[array.GetSize() - 1]->parent_array = &array;
	array[array.GetSize() - 1]->SetValues(array[array.GetSize() - 1], parent, "Wall", name, false);
	return array[array.GetSize() - 1];
}

WallObject* SBS::GetWallObject(csArray<WallObject*> &wallarray, int polygon_index)
{
	//returns the wall object that contains the specified polygon index
	for (int i = 0; i < wallarray.GetSize(); i++)
	{
		for (int j = 0; j < wallarray[i]->handles.GetSize(); j++)
		{
			if (wallarray[i]->handles[j] == polygon_index)
				return wallarray[i];
		}
	}
	return 0;
}

csString SBS::TruncateNumber(double value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	csString number, dec;
	dec = decimals;
	dec.Trim();
	number = value;

	//format number if it's a float
	if ((int)value != value)
		number.Format("%." + dec + "f", value);

	return number;
}

csString SBS::TruncateNumber(float value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	csString number, dec;
	dec = decimals;
	dec.Trim();
	number = value;

	//format number if it's a float
	if ((int)value != value)
		number.Format("%." + dec + "f", value);

	return number;
}

csString SBS::TruncateNumber(const char *value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)
	csString number = value;
	
	if (decimals < 1)
		return number;
	number.Truncate(number.Find(".") + decimals + 1);
	if (number.GetAt(number.Length() - 1) == '.')
		number = number.Slice(0, number.Length() - 1); //strip of extra decimal point if even
	return number;
}

bool SBS::IsValidFloor(int floor)
{
	//determine if a floor is valid

	if (GetFloor(floor))
		return true;
	return false;
}

csString SBS::DumpState()
{
	//dump basic simulator state to a character array

	csString output = "SBS version: " + version + "\n";
	output.Append("Building Name: " + BuildingName + "\n");
	output.Append("Building Filename: " + BuildingFilename + "\n");
	output.Append("Building Version: " + BuildingVersion + "\n");
	output.Append("InStairwell: ");
	output.Append(wxString::FromAscii(BoolToString(InStairwell)).ToAscii());
	output.Append("\n");
	output.Append("InElevator: ");
	output.Append(wxString::FromAscii(BoolToString(InElevator)).ToAscii());
	output.Append("\n");
	output.Append("InShaft: ");
	output.Append(wxString::FromAscii(BoolToString(InShaft)).ToAscii());
	output.Append("\n");
	output.Append("ElevatorNumber: ");
	output.Append(wxVariant((int)ElevatorNumber).GetString().ToAscii());
	output.Append("\n");
	output.Append("ElevatorSync: ");
	output.Append(wxString::FromAscii(BoolToString(ElevatorSync)).ToAscii());
	output.Append("\n");
	output.Append("Running Time: ");
	output.Append(TruncateNumber(running_time, 2));
	output.Append("\n");
	output.Append("BuildingsEnabled: ");
	output.Append(wxString::FromAscii(BoolToString(IsBuildingsEnabled)).ToAscii());
	output.Append("\n");
	output.Append("ExternalEnabled: ");
	output.Append(wxString::FromAscii(BoolToString(IsExternalEnabled)).ToAscii());
	output.Append("\n");
	output.Append("LandscapeEnabled: ");
	output.Append(wxString::FromAscii(BoolToString(IsLandscapeEnabled)).ToAscii());
	output.Append("\n");
	output.Append("SkyboxEnabled: ");
	output.Append(wxString::FromAscii(BoolToString(IsSkyboxEnabled)).ToAscii());
	output.Append("\n");
	output.Append("Verbose: ");
	output.Append(wxString::FromAscii(BoolToString(Verbose)).ToAscii());
	output.Append("\n");
	output.Append("InterfloorOnTop: ");
	output.Append(wxString::FromAscii(BoolToString(InterfloorOnTop)).ToAscii());
	output.Append("\n");
	output.Append("Object Count: ");
	output.Append(wxVariant((int)ObjectCount).GetString().ToAscii());
	output.Append("\n");
	if (camera)
	{
		output.Append("Camera Floor: ");
		output.Append(wxVariant((int)camera->CurrentFloor).GetString().ToAscii());
		output.Append("\n");
		output.Append("Camera Position: " + TruncateNumber(camera->GetPosition().x, 2) + ", " + TruncateNumber(camera->GetPosition().y, 2) + ", " + TruncateNumber(camera->GetPosition().z, 2) + "\n");
	}

	return output;
}

bool SBS::DeleteObject(Object *object)
{
	//object deletion routine
	//this should be called to delete a simulator object during runtime

	csString number;
	number = object->GetNumber();
	bool deleted = false;

	if (!object)
	{
		sbs->Report("Invalid object " + number);
		return false;
	}

	if (!object->GetRawObject())
	{
		sbs->Report("Invalid raw object " + number);
		return false;
	}

	//don't delete permanent objects
	if (object->IsPermanent() == true)
	{
		sbs->Report("Cannot delete permanent object " + number);
		return false;
	}

	csString type = object->GetType();

	//perform standard delete based on object type
	if (type == "Floor")
	{
		//FloorArray.Delete((Floor*)object->GetRawObject());
		delete (Floor*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Elevator")
	{
		//ElevatorArray.Delete((Elevator*)object->GetRawObject());
		delete (Elevator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "ButtonPanel")
	{
		delete (ButtonPanel*)object->GetRawObject();
		deleted = true;
	}
	if (type == "CallButton")
	{
		delete (CallButton*)object->GetRawObject();
		deleted = true;
	}
	if (type == "DirectionalIndicator")
	{
		delete (DirectionalIndicator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Door")
	{
		delete (Door*)object->GetRawObject();
		deleted = true;
	}
	if (type == "ElevatorDoor")
	{
		delete (ElevatorDoor*)object->GetRawObject();
		deleted = true;
	}
	if (type == "FloorIndicator")
	{
		delete (FloorIndicator*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Shaft")
	{
		//ShaftArray.Delete((Shaft*)object->GetRawObject());
		delete (Shaft*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Sound")
	{
		delete (Sound*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Stairs")
	{
		//StairsArray.Delete((Stairs*)object->GetRawObject());
		delete (Stairs*)object->GetRawObject();
		deleted = true;
	}
	if (type == "Wall")
	{
		WallObject *obj = (WallObject*)object->GetRawObject();
		obj->DeletePolygons();
		delete obj;
		deleted = true;
	}

	if (deleted == true)
	{
		engine->Prepare();
		sbs->Report("Deleted object " + number);
		return true;
	}
	return false;
}

bool SBS::DeleteObject(int object)
{
	//delete object by numeric ID
	return DeleteObject(GetObject(object));
}

void SBS::RemoveFloor(Floor *floor)
{
	//remove a floor (does not delete the object)
	for (int i = 0; i < FloorArray.GetSize(); i++)
	{
		if (FloorArray[i].object == floor)
		{
			FloorArray.DeleteIndex(i);
			return;
		}
	}
}

void SBS::RemoveElevator(Elevator *elevator)
{
	//remove an elevator (does not delete the object)
	for (int i = 0; i < ElevatorArray.GetSize(); i++)
	{
		if (ElevatorArray[i].object == elevator)
		{
			ElevatorArray.DeleteIndex(i);
			return;
		}
	}
}

void SBS::RemoveShaft(Shaft *shaft)
{
	//remove a shaft (does not delete the object)
	for (int i = 0; i < ShaftArray.GetSize(); i++)
	{
		if (ShaftArray[i].object == shaft)
		{
			ShaftArray.DeleteIndex(i);
			return;
		}
	}
}

void SBS::RemoveStairs(Stairs *stairs)
{
	//remove a stairs object (does not delete the object)
	for (int i = 0; i < StairsArray.GetSize(); i++)
	{
		if (StairsArray[i].object == stairs)
		{
			StairsArray.DeleteIndex(i);
			return;
		}
	}
}

void SBS::RemoveSound(Sound *sound)
{
	//remove a sound from the array
	//this does not delete the object
	sounds.Delete(sound);
}

const char* SBS::VerifyFile(const char *filename)
{
	//verify a filename
	//if it does, return the same filename
	//otherwise search the related folder and find a matching filename with a different
	//case (fixes case-sensitivity issues mainly on Linux)
	//returns the original string if not found

	csString file = filename;
	if (vfs->Exists(filename))
		return filename;

	csString directory;
	int loc1 = file.FindLast("/");
	int loc2 = file.FindLast("\\");
	int loc = loc1;
	if (loc2 > 0)
		loc = loc2;

	directory = file.Slice(0, loc + 1);

	csRef<iStringArray> array = vfs->FindFiles(directory);
	for (int i = 0; i < array->GetSize(); i++)
	{
		csString check = array->Get(i);
		if (check.CompareNoCase(filename) == true)
			return check;
	}
	return filename;
}

bool SBS::FileExists(const char *filename, bool relative)
{
	//check to see if the specified file exists
	//the name must begin with the "/root/" suffix if relative is false

	csString file = filename;

	if (relative == true)
		file.Insert(0, "/root/");

	if (vfs->Exists(filename))
		return true;
	csString verify = VerifyFile(filename);
	if (verify != file)
		return true;
	return false;
}

csRef<iMeshWrapper> SBS::CreateMesh(const char *name)
{
	//create a new mesh wrapper and factory
	csString factname = name;
	factname.Append(" factory");

	//create mesh wrapper and factory
	csRef<iMeshWrapper> mesh = CS::Geometry::GeneralMeshBuilder::CreateFactoryAndMesh(engine, area, name, factname);

	//set zbuf mode to "USE" by default
	mesh->SetZBufMode(CS_ZBUF_USE);

	//set render priority to "object" by default
	mesh->SetRenderPriority(engine->GetObjectRenderPriority());

	//create a default material (otherwise the system complains if a mesh is used without a material)
	mesh->GetMeshObject()->SetMaterialWrapper(sbs->engine->GetMaterialList()->FindByName("Default"));

	return mesh;
}

csRef<iGeneralMeshSubMesh> SBS::PolyMesh(csRef<iMeshWrapper> mesh, const char *name, const char *texture, csArray<csVector3> vertices, float tw, float th)
{
	//create custom genmesh geometry, apply a texture map and material, and return the created submesh

	//get genmesh factory state
	csRef<iGeneralFactoryState> state = scfQueryInterface<iGeneralFactoryState>(mesh->GetFactory()->GetMeshObjectFactory());

	//get texture material
	csString texname = texture;
	bool result;
	csRef<iMaterialWrapper> material = GetTextureMaterial(texture, result, mesh->QueryObject()->GetName());
	if (!result)
		texname = "Default";

	if (tw == 0)
		tw = 1;
	if (th == 0)
		th = 1;

	//get texture tiling information
	float tw2 = tw, th2 = th;
	float mw, mh;
	if (GetTextureTiling(texname.GetData(), mw, mh))
	{
		//multiply the tiling parameters (tw and th) by
		//the stored multipliers for that texture
		tw2 = tw * mw;
		th2 = th * mh;
	}

	//create texture mapping table
	//csVector2 table[] = {csVector2(tw2, th2), csVector2(0, th2), csVector2(tw2, 0), csVector2(0, 0)};
	csVector2 table[] = {csVector2(0, 0), csVector2(tw2, 0), csVector2(tw2, th2), csVector2(0, th2)};
	CS::Geometry::TableTextureMapper mapper(table);

	//set up untriangulated mesh object
	CS::Geometry::csContour3 origmesh;
	for (int i = 0; i < vertices.GetSize(); i++)
		origmesh.Push(ToRemote(vertices[i]));

	//triangulate mesh
	csTriangleMesh trimesh;
	CS::Geometry::Triangulate3D::Process(origmesh, trimesh);

	//set up geometry arrays
	csDirtyAccessArray<csVector3> mesh_vertices;
	csDirtyAccessArray<csVector2> mesh_texels;
	csDirtyAccessArray<csVector3> mesh_normals;
	csDirtyAccessArray<csTriangle> mesh_triangles;

	//initialize geometry arrays
	int size = trimesh.GetVertexCount();
	mesh_vertices.SetSize(size);
	mesh_texels.SetSize(size);
	mesh_normals.SetSize(size);
	mesh_triangles.SetSize(trimesh.GetTriangleCount());

	//populate vertices, normals, and texels for mesh data
	for (int i = 0; i < size; i++)
	{
		mesh_normals[i] = mesh_vertices[i] = trimesh.GetVertices()[i];
		mesh_normals[i].Normalize();
		mesh_texels[i] = mapper.Map(mesh_vertices[i], mesh_normals[i], i);

		int a, c;
		a = i - 1;
		if (a == -1)
			a = size - 1;
		c = i + 1;
		if (c == size)
			c = 0;
	}

	//get number of existing vertices before adding any
	int count = state->GetVertexCount();

	//add vertices to mesh, from the vertex, texel, and normal arrays
	for (int i = 0; i < mesh_vertices.GetSize(); i++)
		state->AddVertex(mesh_vertices[i], mesh_texels[i], mesh_normals[i], csColor4(0, 0, 0));

	//set up triangle buffer
	csRef<iRenderBuffer> buffer = csRenderBuffer::CreateIndexRenderBuffer(trimesh.GetTriangleCount() * 3, CS_BUF_STATIC, CS_BUFCOMP_UNSIGNED_INT, 0, trimesh.GetVertexCount());
	csTriangle *triangleData = (csTriangle*)buffer->Lock(CS_BUF_LOCK_NORMAL);

	//add triangles to mesh
	for (int i = 0; i < trimesh.GetTriangleCount(); i++)
	{
		csTriangle tri = trimesh.GetTriangle(i);
		tri.a += count;
		tri.b += count;
		tri.c += count;
		state->AddTriangle(tri); //add triangle to genmesh
		triangleData[i] = tri; //add triangle to submesh buffer
	}

	//finish with submesh buffer
	buffer->Release();

	//reprocess mesh
	state->Invalidate();

	//create submesh and set material
	csRef<iGeneralMeshSubMesh> submesh = state->AddSubMesh(buffer, material, name);

	//set ambient lighting factor
	CS::Lighting::SimpleStaticLighter::ConstantColor(mesh, csColor(0.5, 0.5, 0.5));

	//recreate colliders if specified
	if (RecreateColliders == true)
	{
		DeleteColliders(mesh);
		CreateColliders(mesh);
	}

	return submesh;
}
