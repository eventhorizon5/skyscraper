//Scalable Building Simulator - Simulator Core
//The Skyscraper Project - Version 1.1 Alpha
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "sbs.h"

SBS::SBS()
{
}

SBS::~SBS()
{
	//Uninitialize the variables
/*	pScene->DestroyAllMeshes ();

	pScene->Release ();
	pInput->Release ();
	pEngine->Release ();
	pAtmos->Release ();
	pCamera->Release ();
	pMatFactory->Release ();
	pLightEngine->Release ();
	pGraphicEffect->Release ();
	pTextureFactory->Release ();

	pScene = NULL;
	pInput = NULL;
	pEngine = NULL;
	pAtmos = NULL;
	pCamera = NULL;
	pMatFactory = NULL;
	pLightEngine = NULL;
	pGraphicEffect = NULL;
	pTextureFactory = NULL;
*/
}

void SBS::StartEngine()
{
    mainconsole = new Console(NULL);
    mainconsole->Show(TRUE);		
	PrintBanner();
/*	
	pEngine = CreateTVEngine();

	//Set the debug file before doing anything else
	pEngine->SetDebugFile ("c:\\debug.txt");

	//Initialize Windowed
	pEngine->Init3DWindowedMode ((long)WindowHandle, true);

	//Tell it to display the FPS
	pEngine->put_DisplayFPS(tvtrue);

	//AppPath is a small Macro function in tv3dcpp lib to return the path
	//of a file you pass to it, in this case we get the application path
	char path[256];
	char srchpath[256];
	
	HMODULE Module = (HMODULE)hInstance;
	GetModuleFileName(Module,path,255); 
	
	AppPath(path,srchpath);	
	
	//Set the search directory of the objects, textures, ...
	pEngine->SetSearchDirectory(srchpath);
	
	//We set the AngleSystem to Degrees
	pEngine->SetAngleSystem (TV_ANGLE_DEGREE);

	//Init input after main init, and check if it errors anywhere.
	pInput = CreateTVInputEngine();
	pScene = CreateTVScene();	
	pGlobals = CreateTVGlobals();
	pAtmos = CreateTVAtmosphere();
	pCamera = CreateTVCamera();
	pMatFactory = CreateTVMaterialFactory();
	pLightEngine = CreateTVLightEngine();
	pGraphicEffect = CreateTVGraphicEffect();
*/
}

void SBS::Wait(long milliseconds)
{

}

float SBS::AutoSize(float n1, float n2, bool iswidth)
{
//Texture autosizing formulas
//If any of the texture parameters are 0, then automatically size the
//texture using the Skyscraper 1.0 sizing algorithms

}

void SBS::PrintBanner()
{
	mainconsole->WxMemo1->AppendText("Scalable Building Simulator Starting\n");
}

void SBS::render()
{
/*	pEngine->Clear(tvfalse);						//Clear the screen
	pAtmos->Atmosphere_Render();
	pScene->RenderAllMeshes (tvfalse);
	pEngine->RenderToScreen ();					//Render the screen	
*/
}

void SBS::input()
{
/*	if(pInput->IsKeyPressed(TV_KEY_UP))
        pCamera->MoveRelative ((0.015 * pEngine->TimeElapsed()), 0, 0, tvfalse);

    if(pInput->IsKeyPressed(TV_KEY_DOWN))
        pCamera->MoveRelative ((-0.015 * pEngine->TimeElapsed()), 0, 0, tvfalse);

    if(pInput->IsKeyPressed(TV_KEY_LEFT))
        pCamera->RotateY (pEngine->TimeElapsed() * -6 / 100);

    if(pInput->IsKeyPressed(TV_KEY_RIGHT))
        pCamera->RotateY (pEngine->TimeElapsed() * 6 / 100);

	if(pInput->IsKeyPressed(TV_KEY_PAGEUP))
        pCamera->RotateX (-0.68);
    
	if(pInput->IsKeyPressed(TV_KEY_PAGEDOWN))
        pCamera->RotateX (0.68);

	if(pInput->IsKeyPressed(TV_KEY_HOME))
		pCamera->MoveRelative (0, 1, 0, tvfalse);

	if(pInput->IsKeyPressed(TV_KEY_END))
		pCamera->MoveRelative (0, -1, 0, tvfalse);

	if(pInput->IsKeyPressed(TV_KEY_F1))
		pEngine->ScreenShot ("../shot.bmp");

	if(pInput->IsKeyPressed(TV_KEY_A) && RideCar->GetPosition().z > 0)
		GoCar = true;

	if(pInput->IsKeyPressed(TV_KEY_SPACE))
		pCamera->SetRotation (0, 180, 0);
	
	lineend = pCamera->GetPosition();
	LineTest = lineend;

    if(lineend.x > linestart.x)
		LineTest.x = lineend.x + 2;
    if(lineend.x < linestart.x)
		LineTest.x = lineend.x - 2;
    if(lineend.z > linestart.z)
		LineTest.z = lineend.z + 2;
    if(lineend.z < linestart.z)
		LineTest.z = lineend.z - 2;

	//if (Room->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvtrue || HypnoRoom->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvtrue || HypnoDoor->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvtrue || HypnoRide->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvtrue || RideDoor->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvrue Or RideCar->Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING, 0) == tvtrue)
	//	pCamera->SetPosition (linestart.x, linestart.y, linestart.z);
	//else
	//	pCamera->SetPosition (lineend.x, lineend.y, lineend.z);
	//
	if(pInput->IsKeyPressed(TV_KEY_ESCAPE))		//Check if ESCAPE has been pressed.
	{		
		PostQuitMessage(0);
	}*/
}

void SBS::SlowToFPS(long FrameRate)
{

}

bool IsEven(int Number)
{

}
