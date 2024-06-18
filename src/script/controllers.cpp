/*
	Skyscraper 1.12 Alpha - Script Processor - Controller Section
	Copyright (C)2003-2024 Ryan Thoryk
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

#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "enginecontext.h"
#include "controller.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::ControllerSection::ControllerSection(ScriptProcessor *parent) : Section(parent)
{

}

int ScriptProcessor::ControllerSection::Run(std::string &LineData)
{
	//Process controllers

	//create vehicle if not created already
	Simcore->NewController(config->Current);

	//replace variables with actual values
	ReplaceAll(LineData, "%controller%", ToString(config->Current));

	//IF/While statement stub (continue to global commands for processing)
	if (StartsWithNoCase(LineData, "if") || StartsWithNoCase(LineData, "while"))
		return sContinue;

	//process math functions
	if (MathFunctions(LineData) == sError)
		return sError;

	//process functions
	if (parent->FunctionProc() == true)
		return sNextLine;

	//get text after equal sign
	bool equals;
	std::string value = GetAfterEquals(LineData, equals);

	//get controller object
	DispatchController *c = Simcore->GetController(config->Current);

	//parameters

	//Name parameter
	if (StartsWithNoCase(LineData, "name"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		c->Name = value;
		return sNextLine;
	}
	//DestinationDispatch parameter
	if (StartsWithNoCase(LineData, "destinationdispatch"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		c->DestinationDispatch = ToBool(value);
		return sNextLine;
	}
	//Hybrid parameter
	if (StartsWithNoCase(LineData, "hybrid"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		c->Hybrid = ToBool(value);
		return sNextLine;
	}
	//Range parameter
	if (StartsWithNoCase(LineData, "range"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, c->Range))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//MaxPassengers parameter
	if (StartsWithNoCase(LineData, "maxpassengers"))
	{
		if (equals == false)
			return ScriptError("Syntax error");
		std::string str = Calc(value);
		if (!IsNumeric(str, c->MaxPassengers))
			return ScriptError("Invalid value");
		return sNextLine;
	}
	//Elevators parameter
	if (StartsWithNoCase(LineData, "elevators"))
	{
		//copy string listing of elevators into array
		int params = SplitAfterEquals(LineData, false);
		if (params < 1)
			return ScriptError("Syntax Error");

		for (int line = 0; line < params; line++)
		{
			int start, end;
			if (GetRange(tempdata[line], start, end))
			{
				for (int k = start; k <= end; k++)
				{
					if (!c->AddElevator(k))
						return ScriptError();
				}
			}
			else
			{
				std::string str = Calc(tempdata[line]);
				int data;
				if (!IsNumeric(str, data))
					return ScriptError("Invalid value");
				if (!c->AddElevator(data))
					return ScriptError();
			}
		}
		return sNextLine;
	}
	//Reprocess parameter
	if (StartsWithNoCase(LineData, "reprocess"))
	{
		if (equals == false)
			return ScriptError("Syntax error");

		c->Reprocess = ToBool(value);
		return sNextLine;
	}

	//handle end of controller section
	if (StartsWithNoCase(LineData, "<endcontroller>") && config->RangeL == config->RangeH)
	{
		config->SectionNum = 0;
		config->Context = "None";
		engine->Report("Finished controller");
		return sNextLine;
	}

	//handle controller range
	if (config->RangeL != config->RangeH && StartsWithNoCase(LineData, "<endcontroller"))
	{
		if (config->Current < config->RangeH)
		{
			config->Current++;
			parent->line = config->RangeStart;  //loop back
			return sNextLine;
		}
		else
		{
			config->SectionNum = 0; //break out of loop
			config->Context = "None";
			config->RangeL = 0;
			config->RangeH = 0;
			engine->Report("Finished controllers");
			return sNextLine;
		}
	}

	return sContinue;
}

}
