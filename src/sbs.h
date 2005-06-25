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

#include "globals.h"
#include "console.h"

bool IsEven(int Number);

class SBS
{
public:
    //Truevision globals
/*	ITVEngine pEngine; //truevision engine
    ITVInputEngine pInput; //input
    ITVGlobals pGlobals;
    ITVScene pScene; //3d environment
    ITVAtmosphere pAtmos; //sky
    ITVCamera pCamera; //main first-person view camera
    ITVMaterialFactory pMatFactory;
    ITVLightEngine pLightEngine; //lights
    ITVGraphicEffect pGraphicEffect; //effects
    ITVTextureFactory pTextureFactory; //textures
    ITVCollisionResult ColRes;
    ITVMesh Buildings; //external buildings
    ITVMesh External; //external walls for building
    ITVMesh Landscape; //outside landscape
    ITVMesh IntroMesh; //introduction mesh
*/
	//Internal data
    float Gravity; //gravity variable for physics algorithms
    bool IsRunning; //is sim engine running?
    int ElevatorShafts; //number of elevator shafts
	int TotalFloors; //number of above-ground floors including 0
	int Basements; //number of basement floors
	int Elevators; //number of elevators
	int PipeShafts; //number of pipe shafts
	int StairsNum; //number of stairwells
	bool RenderOnly; //skip sim processing and only render graphics
    bool InputOnly; //skip sim processing and only run input and rendering code
    bool IsFalling; //make user fall
    bool CameraFloor; //floor camera's on
    bool InStairwell; //true if user is in a stairwell
    int ElevatorNumber; //number of currently selected elevator
	//int SoundDivisor;
	//int SoundMaxDistance;
	bool FrameLimiter; //frame limiter toggle
    int FrameRate; //max frame rate
	bool EnableCollisions; //turns collisions on/off
    float CameraDefAltitude; //default vertical offset of camera from each floor

    //Camera initialization
    int CameraStartFloor; //starting floor
	float CameraStartPositionX;
	float CameraStartPositionZ;
//	D3DVECTOR CameraStartDirection;
//	D3DVECTOR CameraStartRotation;

	//Console
	Console *mainconsole;
	
	//public functions
	SBS();
	~SBS();
	void StartEngine();
	void Wait(long Milliseconds);
	float AutoSize(float n1, float n2, bool iswidth);
	void render();
	void input();
	void SlowToFPS(long FrameRate);

private:

	//private functions
	void PrintBanner();

};
