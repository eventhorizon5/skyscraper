/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Engine Context
	Copyright (C)2004-2015 Ryan Thoryk
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

#ifndef ENGINECONTEXT_H
#define ENGINECONTEXT_H

namespace Skyscraper {

class Skyscraper;

class EngineContext
{
public:

	bool Reload;

	EngineContext(Skyscraper *frontend, Ogre::SceneManager* mSceneManager, FMOD::System *fmodsystem, int instance_number, const Ogre::Vector3 &position = Ogre::Vector3::ZERO);
	~EngineContext();
	ScriptProcessor* GetScriptProcessor();
	SBS::SBS *GetSystem() { return Simcore; }
	bool IsCameraActive();
	bool Run();
	void Shutdown();
	bool GetShutdownState() { return shutdown; }
	bool Load(std::string filename);
	bool IsLoading() { return loading; }
	bool IsRunning() { return running; }
	void DoReload();
	std::string GetFilename();
	bool Start(Ogre::Camera *camera);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);
	bool IsLoadingFinished();
	void UpdateProgress(int percent);
	int GetProgress() { return progress; }

private:

	void StartSim();
	void UnloadSim();

	Skyscraper* frontend; //frontend
	ScriptProcessor* processor; //script processor
	SBS::SBS *Simcore; //sim engine instance
	int instance; //instance number
	unsigned long finish_time;
	bool shutdown;
	bool loading;
	bool running;
	bool raised;
	int progress;

	//override information
	bool PositionOverride;
	Ogre::Vector3 override_position;
	Ogre::Vector3 override_rotation;
	int override_floor;
	bool override_collisions;
	bool override_gravity;
	bool override_freelook;

	Ogre::SceneManager* mSceneManager;
	FMOD::System *fmodsystem;
	Ogre::Vector3 position;
};

}

#endif
