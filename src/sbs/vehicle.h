/*
	Scalable Building Simulator - Vehicle Object
	The Skyscraper Project - Version 1.12 Alpha
	Copyright (C)2004-2024 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

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

#ifndef _SBS_VEHICLE_H
#define _SBS_VEHICLE_H

namespace OgreBulletDynamics {
	class WheeledRigidBody;
	class VehicleTuning;
	class VehicleRayCaster;
	class RaycastVehicle;
}

namespace SBS {

class SBSIMPEXP Vehicle : public Object
{
public:
	int Number; //vehicle number
	std::string Name; //vehicle name
	bool Created; //has vehicle been created?

	Vector3 ChassisShift;
	std::string ChassisMesh;
	std::string WheelMesh;
	Real ChassisScale;
	Real WheelScale;
	Vector3 CameraPoint;

	Real MaxEngineForce;
	Real SteeringIncrement;
	Real SteeringClamp;
	Real WheelFriction;
	Real SuspensionStiffness;
	Real SuspensionDamping;
	Real SuspensionCompression;
	Real RollInfluence;
	Real SuspensionRestLength;
	Real MaxSuspensionTravelCm;
	Real FrictionSlip;

	Vehicle(Object *parent, const int number);
	~Vehicle();
	bool CreateChassis(Real restitution, Real friction, Real mass, Real linear_dampening, Real angular_dampening);
	bool AddWheel(bool engine, bool steerable, bool IsFrontWheel, Real radius, const Vector3 &ConnectionPoint, const Vector3 &Direction = Vector3(0, -1, 0), const Vector3 &Axle = Vector3(-1, 0, 0));
	void KeyPressed(bool left, bool right, bool down, bool up);
	void KeyReleased(bool left, bool right, bool down, bool up);
	void Loop();
	void Enabled(bool value);
	bool Create(const Vector3 &position);
	void OnMove(bool parent);
	void AttachCamera(bool value);
	Real GetWidth();
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

private:

	OgreBulletDynamics::WheeledRigidBody *mCarChassis;
	OgreBulletDynamics::VehicleTuning *mTuning;
	OgreBulletDynamics::VehicleRayCaster *mVehicleRayCaster;
	OgreBulletDynamics::RaycastVehicle *mVehicle;

	Object *vehicle;
	MeshObject *mChassis;
	std::vector<MeshObject*> mWheels;

	int WheelCount;
	std::vector<int> mWheelsEngine;
	std::vector<int> mWheelsSteerable;

	Real mEngineForce;
	Real mSteering;

	bool mSteeringLeft;
	bool mSteeringRight;

	bool camera_attached;
};

}

#endif
