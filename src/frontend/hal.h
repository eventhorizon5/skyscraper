/*
	Skyscraper 2.1 - Hardware Abstraction Layer
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

#ifndef HAL_H
#define HAL_H

#include <OgrePrerequisites.h>
#include <OgreCommon.h>
#include <Ogre.h>
#include <OgreLog.h>
#include <OgreTrays.h>

namespace Skyscraper {

class VM;

class HAL
{
public:
    HAL(VM *vm);
    ~HAL();
	bool Initialize(const std::string &data_path);
	bool Render();
	void ClickedObject(bool left, bool shift, bool ctrl, bool alt, bool right, Real scale, bool center_only);
	void UnclickedObject();
	Ogre::ConfigFile* LoadConfiguration(const std::string &filename, bool delete_after_use = false);
	int GetConfigInt(Ogre::ConfigFile *file, const std::string &key, int default_value);
	std::string GetConfigString(Ogre::ConfigFile *file, const std::string &key, const std::string &default_value);
	bool GetConfigBool(Ogre::ConfigFile *file, const std::string &key, bool default_value);
	Real GetConfigFloat(Ogre::ConfigFile *file, const std::string &key, Real default_value);
	bool PlaySound(const std::string &filename);
	void StopSound();
	void ClearScene();
	void ToggleStats();
	void EnableStats(bool value);
	FMOD::System* GetSoundSystem();
	Ogre::RenderWindow* GetRenderWindow();
	void Clear();
	Ogre::SceneManager* GetSceneManager();
	Ogre::RenderWindow* CreateRenderWindow(const std::string &name, int width, int height, const Ogre::NameValuePairList &params);
	void DestroyRenderWindow();
	void RefreshViewport();
    void UpdateOpenXR();
	void ReInit();
	void Report(const std::string &message, const std::string &prompt);
	bool ReportError(const std::string &message, const std::string &prompt);
	bool ReportFatalError(const std::string &message, const std::string &prompt);

	bool RTSS;
	std::string Renderer;
	bool DisableSound;
	std::vector<Ogre::Camera*> mCameras;
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	std::vector<Ogre::Viewport*> mViewports;

private:
 
	//OGRE engine data
	Ogre::SceneManager* mSceneMgr;
	Ogre::OverlaySystem* mOverlaySystem;

    //OGRE log manager
	Ogre::LogManager* logger;

	//sound data
	FMOD::System *soundsys;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

    //reporting functions
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);

    //stats
	OgreBites::TrayManager* mTrayMgr;
	int show_stats;

    VM *vm;
};

}

#endif

