/*
	Skyscraper 2.1 - Script Processor - Script Section
	Copyright (C)2003-2025 Ryan Thoryk
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

#ifndef SCRIPTSECTION_H
#define SCRIPTSECTION_H

namespace Skyscraper {

class ScriptProcessor::Section
{
friend class ScriptProcessor;

public:
	explicit Section(ScriptProcessor* parent);
	virtual ~Section() {}
	virtual void Reset() {}
	virtual int Run(std::string &LineData) { return 0; }
	int SplitData(const std::string &string, int start, bool calc = true);
	int SplitAfterEquals(const std::string &string, bool calc = true);
	std::string GetAfterEquals(const std::string &string, bool &found_equals);
	std::string GetBeforeEquals(const std::string &string, bool calc = true);
	bool GetRange(const std::string &string, int &start, int &end);
	int ScriptError(std::string message, bool warning = false);
	int ScriptError();
	int ScriptWarning(std::string message);
	bool IfProc(const std::string &expression);
	void StoreCommand(SBS::Object *object);
	int MathFunctions(std::string &LineData);
	::SBS::MeshObject* GetMeshObject(std::string name);
	bool GetElevatorCar(std::string &value, int floor, int &elevator, int &car);
	void GetDirectionStrings(int direction, std::string &face_direction, std::string &open_direction);

protected:
	ScriptProcessor *parent;
	SBS::SBS *Simcore;
	EngineContext *engine;
	std::vector<std::string> tempdata;
	bool warn_deprecated;
	ScriptProcessor::ConfigHandler *config;

	std::string Calc(const std::string &expression);
};

enum SectionType
{
	SECTION_NONE = 0,
	SECTION_GLOBAL = 1,
	SECTION_FLOOR = 2,
	SECTION_BUILDINGS = 3,
	SECTION_ELEVATOR = 4,
	SECTION_TEXTURE = 5,
	SECTION_ELEVATORCAR = 6,
	SECTION_VEHICLE = 7,
	SECTION_CONTROLLER = 8,
	SECTION_CALLSTATION = 9
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
	Vector3 MinExtent;
	Vector3 MaxExtent;
	int RangeL, RangeLOld;
	int RangeH, RangeHOld;
	long RangeStart, RangeStartOld;
	int Current, CurrentOld; //current range iteration
	SectionType SectionNum; //current section number
	std::string Context, ContextOld; //section context
	bool ReverseAxis;
	bool InWhile;
	bool setshaftdoors;
	bool CheckScript; //if true, quick-checks script execution, and exits
	std::string extrusion_texture; //side texture used for polygon extrusion
	Real extrusion_thickness; //polygon extrusion thickness
	Real extrusion_tw; //extrusion side texture tiling width
	Real extrusion_th; //extrusion side texture tiling height
	bool extrude;
};

class ScriptProcessor::GlobalsSection : public ScriptProcessor::Section
{
public:
	explicit GlobalsSection(ScriptProcessor* parent);
	~GlobalsSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::BuildingsSection : public ScriptProcessor::Section
{
public:
	explicit BuildingsSection(ScriptProcessor* parent);
	~BuildingsSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::TexturesSection : public ScriptProcessor::Section
{
public:
	explicit TexturesSection(ScriptProcessor* parent);
	~TexturesSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::CommandsSection : public ScriptProcessor::Section
{
public:
	explicit CommandsSection(ScriptProcessor* parent);
	~CommandsSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::FloorSection : public ScriptProcessor::Section
{
public:
	explicit FloorSection(ScriptProcessor* parent);
	~FloorSection() override {}
	void Reset() override;
	int Run(std::string &LineData) override;

private:
	int FloorCheck;
	int callbutton_controller;

	bool floorcache_firstrun;
	int cache_current;
	std::string cache_current_s;
	Real cache_height;
	std::string cache_height_s;
	Real cache_fullheight;
	std::string cache_fullheight_s;
	Real cache_interfloorheight;
	std::string cache_interfloorheight_s;
	Real cache_base;
	std::string cache_base_s;
};

class ScriptProcessor::ElevatorSection : public ScriptProcessor::Section
{
public:
	explicit ElevatorSection(ScriptProcessor* parent);
	~ElevatorSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::ElevatorCarSection : public ScriptProcessor::Section
{
public:
	explicit ElevatorCarSection(ScriptProcessor* parent);
	~ElevatorCarSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::VehicleSection : public ScriptProcessor::Section
{
public:
	explicit VehicleSection(ScriptProcessor* parent);
	~VehicleSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::ControllerSection : public ScriptProcessor::Section
{
public:
	explicit ControllerSection(ScriptProcessor* parent);
	~ControllerSection() override {}
	int Run(std::string &LineData) override;
};

class ScriptProcessor::CallStationSection : public ScriptProcessor::Section
{
public:
	explicit CallStationSection(ScriptProcessor* parent);
	~CallStationSection() override {}
	int Run(std::string &LineData) override;
};

}

#endif
