/*
	Skyscraper 2.1 - Sky System
	Copyright (C)2004-2025 Ryan Thoryk
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

#include <OgreRoot.h>
#include "globals.h"
#include "sbs.h"
#include "profiler.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "sky.h"

using namespace SBS;

namespace Skyscraper {

//Sky System

SkySystem::SkySystem(VM *vm)
{
	this->vm = vm;
	SkyMult = 0;
	mCaelumSystem = 0;
	new_location = false;
	new_time = false;
	latitude = 0.0;
	longitude = 0.0;
	datetime = 0.0;
	sky_error = 0;
}

SkySystem::~SkySystem()
{
	//delete Caelum
	if (mCaelumSystem)
		delete mCaelumSystem;
}

void SkySystem::UnloadSky()
{
	//unload Caelum sky system

	new_time = false;

	if (mCaelumSystem)
	{
		Caelum::CaelumPlugin* ptr = Caelum::CaelumPlugin::getSingletonPtr();
		mCaelumSystem->clear();
		mCaelumSystem->detachAllViewports();
		delete mCaelumSystem;
		mCaelumSystem = 0;
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup("Caelum");
		Caelum::CaelumPlugin::getSingleton().shutdown();
		delete ptr;
	}
}

void SkySystem::CreateSky(EngineContext *engine)
{
	//create sky system

	bool use_caelum = vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.Caelum", true);
	bool sky_result = true;

	if (use_caelum == true)
	{
		//load Caelum plugin
		try
		{
			new Caelum::CaelumPlugin();
			Caelum::CaelumPlugin::getSingleton().initialise();
		}
		catch (Ogre::Exception &e)
		{
			if (e.getDescription() != "!msSingleton failed. There can be only one singleton")
				ReportFatalError("Error initializing Caelum plugin:\nDetails: " + e.getDescription());
			return;
		}
		sky_result = InitSky(engine);
	}

	//create old sky if Caelum is turned off, or failed to initialize
	if (sky_result == false || use_caelum == false)
		engine->GetSystem()->CreateSky();
}

bool SkySystem::InitSky(EngineContext *engine)
{
	//initialize sky

	if (!engine)
		return false;

	if (vm->GetHAL()->Renderer == "Direct3D11")
		return true;

	//ensure graphics card and render system are capable of Caelum's shaders
	if (vm->GetHAL()->Renderer == "Direct3D9")
	{
		//on DirectX, Caelum requires a card capable of 3.0 shader levels, which would be
		//an ATI Radeon HD 2000, nVidia Geforce 6, Intel G965 or newer
		//Intel cards: http://www.intel.com/support/graphics/sb/cs-014257.htm
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = vm->GetHAL()->mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//for general sky, require both DirectX pixel and vertex shaders 2.0
		if (profiles.find("ps_2_0") == profiles.end() ||
			profiles.find("vs_2_0") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 2.0 shaders not supported");

		//for clouds, require either DirectX pixel shaders 3.0 or nVidia fragment shaders 4.0
		if (profiles.find("ps_3_0") == profiles.end() &&
			profiles.find("fp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 fragment shaders not supported");

		//for clouds, require either DirectX vetex shaders 3.0 or nVidia vertex shaders 4.0
		if (profiles.find("vs_3_0") == profiles.end() &&
			profiles.find("vp40") == profiles.end())
				return ReportFatalError("Error initializing Caelum: 3.0 vertex shaders not supported");
	}

	if (vm->GetHAL()->Renderer == "OpenGL")
	{
		//on OpenGL, Caelum requires hardware support for shaders (OpenGL 2.0 or newer)
		Ogre::RenderSystemCapabilities::ShaderProfiles profiles = vm->GetHAL()->mRoot->getRenderSystem()->getCapabilities()->getSupportedShaderProfiles();

		//require OpenGL ARB fragment programs
		if (profiles.find("arbfp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: fragment programs not supported");

		//require OpenGL ARB vertex programs
		if (profiles.find("arbvp1") == profiles.end())
			return ReportFatalError("Error initializing Caelum: vertex programs not supported");
	}

	//load Caelum resources
	try
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data/caelum", "FileSystem", "Caelum", false);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Caelum");

		if (!mCaelumSystem)
			mCaelumSystem = new Caelum::CaelumSystem(vm->GetHAL()->mRoot, vm->GetHAL()->GetSceneManager(), Caelum::CaelumSystem::CAELUM_COMPONENTS_NONE);
		Caelum::CaelumPlugin::getSingleton().loadCaelumSystemFromScript(mCaelumSystem, SkyName);
	}
	catch (Ogre::Exception &e)
	{
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE //ignore Caelum errors on Mac, due to a Cg error (Cg is not available on ARM CPUs, and is not bundled with the Mac version)
		ReportFatalError("Error initializing Caelum:\nDetails: " + e.getDescription());
#endif
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error initializing Caelum");
		sky_error = true;
	}

	if (!mCaelumSystem)
	{
		sky_error = true;
		return false;
	}

	//attach caelum to running viewport
	try
	{
		for (size_t i = 0; i < vm->GetHAL()->mViewports.size(); i++)
		{
			mCaelumSystem->attachViewport(vm->GetHAL()->mViewports[i]);
		}
		mCaelumSystem->setAutoNotifyCameraChanged(false);
		mCaelumSystem->setSceneFogDensityMultiplier(vm->GetHAL()->GetConfigFloat(vm->GetHAL()->configfile, "Skyscraper.Frontend.Caelum.FogMultiplier", 0.1) / 1000);
		if (vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.Caelum.EnableFog", true) == false)
			mCaelumSystem->setManageSceneFog(Ogre::FOG_NONE);
		mCaelumSystem->setManageAmbientLight(vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.Caelum.ModifyAmbient", false));

		//fix sky rotation
		Quaternion rot(Degree(180.0), Vector3::UNIT_Y);
		mCaelumSystem->getCaelumGroundNode()->setOrientation(rot);
		mCaelumSystem->getCaelumCameraNode()->setOrientation(rot);

		//have sky use SBS scaling factor
		Real scale = 1 / engine->GetSystem()->GetUnitScale();
		mCaelumSystem->getCaelumGroundNode()->setScale(scale, scale, scale);
		mCaelumSystem->getCaelumCameraNode()->setScale(scale, scale, scale);
	}
	catch (Ogre::Exception &e)
	{
		ReportFatalError("Error setting Caelum parameters:\nDetails: " + e.getDescription());
		sky_error = true;
	}
	catch (...)
	{
		ReportFatalError("Error setting Caelum parameters");
		sky_error = true;
	}

	//set sky time multiplier if not already set
	if (SkyMult == 0)
		SkyMult = mCaelumSystem->getTimeScale();

	//set location if specified
	if (new_location == true)
	{
		mCaelumSystem->setObserverLatitude(Degree(latitude));
		mCaelumSystem->setObserverLongitude(Degree(longitude));
		new_location = false;
	}

	//use system time if specified
	if (vm->GetHAL()->GetConfigBool(vm->GetHAL()->configfile, "Skyscraper.Frontend.Caelum.UseSystemTime", false) == true && new_time == false)
		SetDateTimeNow();

	//set date/time if specified
	if (new_time == true)
	{
		mCaelumSystem->setJulianDay(datetime);
		new_time = false;
	}

	return true;
}

void SkySystem::UpdateSky()
{
	//update sky

	SBS_PROFILE_MAIN("Sky");

	if (mCaelumSystem)
	{
		for (size_t i = 0; i < vm->GetHAL()->mCameras.size(); i++)
		{
			mCaelumSystem->notifyCameraChanged(vm->GetHAL()->mCameras[i]);
		}
		mCaelumSystem->setTimeScale(SkyMult);
		mCaelumSystem->updateSubcomponents(Real(vm->GetActiveEngine()->GetSystem()->GetElapsedTime()) / 1000);
	}
}

void SkySystem::SetLocation(Real latitude, Real longitude)
{
	this->latitude = latitude;
	this->longitude = longitude;
	new_location = true;
}

void SkySystem::SetDateTimeNow()
{
	//set date and time to current time in UTC

	//get current time
	time_t now = time(0);

	//convert time to GMT
	tm* gmtm = gmtime(&now);
	if (gmtm == NULL)
		return;

	//convert time to Julian and set it
	double julian = Caelum::Astronomy::getJulianDayFromGregorianDateTime(gmtm->tm_year + 1900, gmtm->tm_mon + 1, gmtm->tm_mday, gmtm->tm_hour, gmtm->tm_min, gmtm->tm_sec);
	SetDateTime(julian);
}

void SkySystem::SetDateTime(double julian_date_time)
{
	datetime = julian_date_time;
	new_time = true;

	if (mCaelumSystem)
		mCaelumSystem->setJulianDay(datetime);
}

void SkySystem::GetTime(int &hour, int &minute, int &second)
{
	hour = -1;
	minute = -1;
	second = -1.0;

	if (!mCaelumSystem)
		return;

	Caelum::LongReal julian = mCaelumSystem->getJulianDay(), sec;
	int year, month, day;
	Caelum::Astronomy::getGregorianDateTimeFromJulianDay(julian, year, month, day, hour, minute, sec);
	second = (int)sec;
}

void SkySystem::GetDate(int &year, int &month, int &day)
{
	Caelum::LongReal julian = mCaelumSystem->getJulianDay(), second;
	int hour, minute;
	Caelum::Astronomy::getGregorianDateTimeFromJulianDay(julian, year, month, day, hour, minute, second);
	second = (int)second;
}

void SkySystem::EnableSky(bool value)
{
	//enable or disable sky system

	//enable/disable old skybox system in engine 0
	if (vm->GetEngine(0))
		vm->GetEngine(0)->GetSystem()->EnableSkybox(value);

	//enable/disable Caelum sky system
	if (mCaelumSystem)
	{
		mCaelumSystem->getCaelumGroundNode()->setVisible(value);
		mCaelumSystem->getCaelumCameraNode()->setVisible(value);
	}
}

void SkySystem::Report(const std::string &message)
{
	vm->GetHAL()->Report(message, "sky:");
}

bool SkySystem::ReportError(const std::string &message)
{
	return vm->GetHAL()->ReportError(message, "sky:");
}

bool SkySystem::ReportFatalError(const std::string &message)
{
	return vm->GetHAL()->ReportFatalError(message, "sky:");
}

}
