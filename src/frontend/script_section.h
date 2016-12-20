/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Script Processor - Script Section
	Copyright (C)2003-2016 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@skyscrapersim.com

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

namespace Skyscraper {

class ScriptProcessor::Section
{
friend class ScriptProcessor;

public:
	Section(ScriptProcessor* parent);
	virtual ~Section() {}
	virtual void Reset() {}
	virtual int Run(std::string &LineData) { return 0; }
	int SplitData(const std::string &string, int start, bool calc = true);
	int SplitAfterEquals(const std::string &string, bool calc = true);
	std::string GetAfterEquals(const std::string &string);
	int ScriptError(std::string message, bool warning = false);
	int ScriptError();
	int ScriptWarning(std::string message);
	bool IfProc(const std::string &expression);
	void StoreCommand(SBS::Object *object);
	int MathFunctions(std::string &LineData);
	::SBS::MeshObject* GetMeshObject(std::string name);
	bool GetElevatorCar(std::string &value, int floor, int &elevator, int &car);

protected:
	ScriptProcessor *parent;
	SBS::SBS *Simcore;
	EngineContext *engine;
	std::vector<std::string> tempdata;
	bool warn_deprecated;
	ScriptProcessor::ConfigHandler *config;

	std::string Calc(const std::string &expression);
};

class ScriptProcessor::ConfigHandler
{
public:
	ConfigHandler();
	~ConfigHandler() {}
	void Reset();

	//shared values for sections
	bool setkey;
	int keyvalue;
	int lockvalue;
	Ogre::Vector3 MinExtent;
	Ogre::Vector3 MaxExtent;
	int RangeL, RangeLOld;
	int RangeH, RangeHOld;
	long RangeStart, RangeStartOld;
	int Current, CurrentOld; //current range iteration
	int SectionNum; //current section number
	std::string Context, ContextOld; //section context
	bool ReverseAxis;
	bool InWhile;
	bool setshaftdoors;
	bool CheckScript; //if true, quick-checks script execution, and exits
};

class ScriptProcessor::GlobalsSection : public ScriptProcessor::Section
{
public:
	GlobalsSection(ScriptProcessor* parent);
	~GlobalsSection() {}
	int Run(std::string &LineData);
};

class ScriptProcessor::BuildingsSection : public ScriptProcessor::Section
{
public:
	BuildingsSection(ScriptProcessor* parent);
	~BuildingsSection() {}
	int Run(std::string &LineData);
};

class ScriptProcessor::TexturesSection : public ScriptProcessor::Section
{
public:
	TexturesSection(ScriptProcessor* parent);
	~TexturesSection() {}
	int Run(std::string &LineData);
};

class ScriptProcessor::CommandsSection : public ScriptProcessor::Section
{
public:
	CommandsSection(ScriptProcessor* parent);
	~CommandsSection() {}
	int Run(std::string &LineData);
};

class ScriptProcessor::FloorSection : public ScriptProcessor::Section
{
public:
	FloorSection(ScriptProcessor* parent);
	~FloorSection() {}
	void Reset();
	int Run(std::string &LineData);

private:
	int FloorCheck;
	std::vector<int> callbutton_elevators;

	bool floorcache_firstrun;
	int cache_current;
	std::string cache_current_s;
	float cache_height;
	std::string cache_height_s;
	float cache_fullheight;
	std::string cache_fullheight_s;
	float cache_interfloorheight;
	std::string cache_interfloorheight_s;
	float cache_base;
	std::string cache_base_s;
};

class ScriptProcessor::ElevatorSection : public ScriptProcessor::Section
{
public:
	ElevatorSection(ScriptProcessor* parent);
	~ElevatorSection() {}
	int Run(std::string &LineData);
};

class ScriptProcessor::ElevatorCarSection : public ScriptProcessor::Section
{
public:
	ElevatorCarSection(ScriptProcessor* parent);
	~ElevatorCarSection() {}
	int Run(std::string &LineData);
};

}
