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

#ifndef SKY_H
#define SKY_H

#include <Caelum.h>
#include "vm.h"

namespace Caelum {
	class CaelumSystem;
}

namespace Skyscraper {

class EngineContext;

class VMIMPEXP SkySystem
{
public:
    explicit SkySystem(VM *vm);
    ~SkySystem();
	void CreateSky(EngineContext *engine);
	void UnloadSky();
	bool InitSky(EngineContext *engine);
	void EnableSky(bool value);
	void UpdateSky();
	inline Caelum::CaelumSystem* GetCaelumSystem() { return mCaelumSystem; };
	void SetLocation(Real latitude, Real longitude);
	void SetDateTimeNow();
	void SetDateTime(double julian_date_time);
	void GetTime(int &hour, int &minute, int &second);
	void GetDate(int &year, int &month, int &day);

    int SkyMult; //sky time multiplier
	std::string SkyName;

private:

	Caelum::CaelumSystem *mCaelumSystem;
 
    //reporting functions
	void Report(const std::string &message);
	bool ReportError(const std::string &message);
	bool ReportFatalError(const std::string &message);

	bool new_location, new_time;
	Real latitude, longitude;
	double datetime;
	bool sky_error;

    VM* vm;
};

}

#endif
