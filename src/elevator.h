//Scalable Building Simulator - Elevator Subsystem Class
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

class Elevator
{
public:

	int Number; //elevator number
	int QueuePositionDirection; //queue processing direction
	bool PauseQueueSearch; //pause queue processor
	float ElevatorSpeed; //maximum elevator speed
	bool ElevatorSync; //true if user should move with elevator
	//Public ElevatorMusic As TVSoundWave3D
	//Public ElevatorSounds As TVSoundWave3D
	bool MoveElevator; //Tells elevator to start going to specified floor
    int MoveElevatorFloor; //floor to move elevator to
	int GotoFloor; //floor to go to
	int OpenDoor; //1=open doors, -1=close doors
	float Acceleration; //percentage of speed increase
	float OpenSpeed; //elevator opening/closing speed

	//functions
	Elevator(int number);
	~Elevator();
	void CreateElevator(float x, float y, int floor, int direction);
	void AddRoute(int floor, int direction);
	void DeleteRoute(int floor, int direction);
	void Alarm();
	void CallElevator(int floor, int direction);
	void StopElevator();
	void OpenHatch();
	void OpenDoorsEmergency();
	void OpenShaftDoors(int floor);
	void ProcessCallQueue();
	int GetElevatorFloor();
	void MonitorLoop();
	void CloseDoorsEmergency();
	csVector3 GetPosition();
	void AddWall(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddFloor(const char *texture, float x1, float z1, float x2, float z2, float voffset, float tw, float th);
	void AddFloorIndicator(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset);
	void AddButtonPanel(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddPanels(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddDoors(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddPlaque(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);
	void AddCallButtons(const char *texture, float x1, float z1, float x2, float z2, float height, float voffset, float tw, float th);

private:
	csRef<iMeshWrapper> ElevatorMesh; //elevator mesh object
		csRef<iThingState> ElevatorTS;
		csRef<iThingFactoryState> ElevatorState;
	csRef<iMeshWrapper> FloorIndicator; //floor indicator object
		csRef<iThingState> FloorIndicatorTS;
		csRef<iThingFactoryState> FloorIndicatorState;
	csRef<iMeshWrapper> ElevatorDoorL; //left inside door
		csRef<iThingState> ElevatorDoorL_TS;
		csRef<iThingFactoryState> ElevatorDoorL_State;
	csRef<iMeshWrapper> ElevatorDoorR; //right inside door
		csRef<iThingState> ElevatorDoorR_TS;
		csRef<iThingFactoryState> ElevatorDoorR_State;
	csRef<iMeshWrapper> Plaque; //plaque object
		csRef<iThingState> PlaqueTS;
		csRef<iThingFactoryState> PlaqueState;
	csRef<iMeshWrapper> CallButtonsUp; //up call button
		csRef<iThingState> CallButtonsUp_TS;
		csRef<iThingFactoryState> CallButtonsUp_State;
	csRef<iMeshWrapper> CallButtonsDown; //down call button
		csRef<iThingState> CallButtonsDown_TS;
		csRef<iThingFactoryState> CallButtonsDown_State;
	csRefArray<iMeshWrapper> Buttons; //elevator button array

	//Internal elevator simulation data
	csString UpQueue; //up call queue ***Change these to sorted arrays
	csString DownQueue; //down call queue
	float ElevatorStart; //elevator vertical starting location
	int ElevatorFloor; //current elevator floor
	bool DoorsOpen; //elevator door state
	int ElevatorDirection; //-1=down, 1=up, 0=stopped
	float DistanceToTravel; //distance in Y to travel
	float Destination; //elevator destination Y value
	float ElevatorRate;
	float StoppingDistance;
	bool CalculateStoppingDistance;
	bool Brakes;
	float ElevatorDoorSpeed;
	float ElevatorDoorPos; //original elevator door position
	bool ElevWait;

	//functions
	void OpenDoors();
	void CloseDoors();
	void MoveElevatorToFloor();

};
