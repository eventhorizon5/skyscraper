/*
	Skyscraper 1.12 Alpha - Script Processor - Script Section
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
#include "floor.h"
#include "elevator.h"
#include "elevatorcar.h"
#include "shaft.h"
#include "stairs.h"
#include "model.h"
#include "random.h"
#include "scriptproc.h"
#include "section.h"

using namespace SBS;

namespace Skyscraper {

ScriptProcessor::Section::Section(ScriptProcessor *parent)
{
	this->parent = parent;
	engine = parent->GetEngine();
	Simcore = engine->GetSystem();
	warn_deprecated = engine->GetFrontend()->GetConfigBool("Skyscraper.Frontend.WarnDeprecated", false);
	config = parent->GetConfigHandler();
}

int ScriptProcessor::Section::SplitData(const std::string &string, int start, bool calc)
{
	//split data into separate strings starting at the "start" character
	//delimeter is a comma ","
	//returns the number of parameters found
	//if calc is on, calc_skip can be used to specify an index that does not cooperate properly with calculations, such as filenames

	std::string data = string;
	std::string stringbuffer;

	//verify length of input string
	if ((int)data.size() < start)
		return 0;

	SplitString(tempdata, data.substr(start), ',');
	if (calc == true)
	{
		for (size_t i = 0; i < tempdata.size(); i++)
		{
			stringbuffer = parent->Calc(tempdata[i]);
			tempdata[i] = stringbuffer;
		}
	}
	return (int)tempdata.size();
}

int ScriptProcessor::Section::SplitAfterEquals(const std::string &string, bool calc)
{
	//get and split data after equal sign
	//returns -1 if equal sign not found
	//returns 0 if no parameters found

	std::string data = string;
	int loc = data.find("=", 0);
	if (loc < 0)
		return -1;

	std::string temp = data.substr(loc + 1);
	TrimString(temp);

	if (temp == "")
		return 0;

	SplitString(tempdata, temp, ',');
	if (calc == true)
	{
		for (size_t i = 0; i < tempdata.size(); i++)
		{
			std::string buffer = parent->Calc(tempdata[i]);
			tempdata[i] = buffer;
		}
	}
	return (int)tempdata.size();
}

std::string ScriptProcessor::Section::GetAfterEquals(const std::string &string, bool &found_equals)
{
	//return data after equal sign

	found_equals = false;

	if (string.size() <= 1)
		return "";

	//find equals sign
	int loc = string.find("=", 1);

	if (loc < 0)
		return "";

	found_equals = true;

	std::string temp = string.substr(loc + 1);
	TrimString(temp);
	return temp;
}

std::string ScriptProcessor::Section::GetBeforeEquals(const std::string &string, bool calc)
{
	//return data right before equal sign

	if (string.size() <= 1)
		return "";

	//find equal sign
	int equals = string.find("=", 1);

	if (equals == -1)
		return "";

	std::string str = string.substr(0, equals);
	TrimString(str);

	//find space before equal sign
	int loc = str.find_first_of(" ", 0);
	int loc2 = -1;

	//if a space is missing, find start of numeric characters
	if (loc == -1)
	{
		for (int i = 0; i < str.size(); i++)
		{
			if (IsNumeric(str[i]))
			{
				loc2 = i;
				break;
			}
		}
	}

	std::string str2;

	if (loc2 == -1)
		str2 = str.substr(loc);
	else
		str2 = str.substr(loc2);
	TrimString(str2);

	if (calc == true)
		return Calc(str2);

	return str2;
}

bool ScriptProcessor::Section::GetRange(const std::string &string, int &start, int &end)
{
	//calculate range value, such as "1 - 3", and fill in the 'start' and 'end' values

	if (string.find("-", 1) > 0)
	{
		//found a range marker
		std::string str1 = string.substr(0, string.find("-", 1));
		std::string str2 = string.substr(string.find("-", 1) + 1);
		TrimString(str1);
		TrimString(str2);
		if (!IsNumeric(str1, start) || !IsNumeric(str2, end))
			return ScriptError("Invalid value");
		if (end < start)
		{
			int temp = start;
			start = end;
			end = temp;
		}

		return true;
	}

	return false;
}

int ScriptProcessor::Section::ScriptError(std::string message, bool warning)
{
	return parent->ScriptError(message, warning);
}

int ScriptProcessor::Section::ScriptError()
{
	return parent->ScriptError();
}

int ScriptProcessor::Section::ScriptWarning(std::string message)
{
	return parent->ScriptWarning(message);
}

bool ScriptProcessor::Section::IfProc(const std::string &expression)
{
	//IF statement processor

	int temp1;
	std::string tmpcalc = expression;
	std::string one;
	std::string two;
	int start, end;
	bool check;

	//first remove all whitespace from the string
	ReplaceAll(tmpcalc, " ", "");

	//first check for bad and/or character sets
	if (int(tmpcalc.find("&&")) >= 0 || int(tmpcalc.find("||")) >= 0 || int(tmpcalc.find("==")) >= 0 || int(tmpcalc.find("!=")) >= 0)
	{
		ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
		return false;
	}

	//find parenthesis
	do
	{
		start = tmpcalc.find("(", 0);
		if (start >= 0)
		{
			//find matching parenthesis
			int match = 1;
			int end = -1;
			for (int i = start + 1; i < (int)tmpcalc.length(); i++)
			{
				char &tmpchar = tmpcalc.at(i);
				if (tmpchar == '(')
					match++;
				if (tmpchar == ')')
					match--;
				if (match == 0)
				{
					end = i;
					break;
				}
			}
			if (end != -1)
			{
				//call function recursively
				std::string newdata;
				if (IfProc(tmpcalc.substr(start + 1, end - start - 1)) == true)
					newdata = "true";
				else
					newdata = "false";
				//construct new string
				one = tmpcalc.substr(0, start);
				if (end < (int)tmpcalc.length() - 1)
					two = tmpcalc.substr(end + 1);
				else
					two = "";
				tmpcalc = one + newdata + two;
			}
			else
			{
				ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
				return false;
			}
		}
		else
			break;
	} while (1 == 1);
	//find number of operators and recurse if multiple found
	int operators;
	int operators2;
	do
	{
		operators = 0;
		operators2 = 0;
		start = 0;
		end = 0;
		check = false;
		for (int i = 1; i < (int)tmpcalc.length(); i++)
		{
			char &tmpchar = tmpcalc.at(i);
			if (tmpchar == '=' || tmpchar == '!' || tmpchar == '<' || tmpchar == '>')
				operators++;

			if (tmpchar == '&' || tmpchar == '|')
			{
				check = true;
				operators2++;
				if (operators == 1 && operators2 == 2)
				{
					//handle 2 and/if operators
					end = i;
					start = 0;
					operators = 2;
					break;
				}
				if (operators == 1)
				{
					operators = 2;
					end = i;
				}
				else if (operators == 0)
				{
					operators = 1;
					start = i + 1;
					end = (int)tmpcalc.length();
				}
			}
		}
		//return error if multiple standard operators are found, but no and/or operator (ex. if[5 = 5 = 5])
		if (operators > 1 && check == false)
		{
			ScriptError("Syntax error in IF operation: '" + tmpcalc + "' (might be nested)");
			return false;
		}
		if (operators > 1)
		{
			std::string newdata;
			if (IfProc(tmpcalc.substr(start, end - start)) == true)
				newdata = "true";
			else
				newdata = "false";
			//construct new string
			one = tmpcalc.substr(0, start);
			two = tmpcalc.substr(end);
			tmpcalc = one + newdata + two;
		}
		else
			break;
	} while (1 == 1);
	//return value if none found
	if (operators == 0)
		return ToBool(tmpcalc);

	//otherwise perform comparisons
	temp1 = tmpcalc.find("=", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("!", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one != two)
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("<", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			if(ToFloat(one) < ToFloat(two))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.find(">", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (IsNumeric(one) == true && IsNumeric(two) == true)
		{
			if(ToFloat(one) > ToFloat(two))
				return true;
			else
				return false;
		}
		else
			return false;
	}
	temp1 = tmpcalc.find("&", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == "true" && two == "true")
			return true;
		else
			return false;
	}
	temp1 = tmpcalc.find("|", 1);
	if (temp1 > 0)
	{
		one = tmpcalc.substr(0, temp1);
		two = tmpcalc.substr(temp1 + 1);
		if (one == "true" || two == "true")
			return true;
		else
			return false;
	}
	return ToBool(tmpcalc);
}

void ScriptProcessor::Section::StoreCommand(::SBS::Object *object)
{
	parent->StoreCommand(object);
}

int ScriptProcessor::Section::MathFunctions(std::string &LineData)
{
	//functions for advanced math

	int start, first, last;
	Real value, result;
	std::string tempdata;

	int check = LineData.find("(", 0);

	if (check < 0)
		return sContinue;

	//calculate cosine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("cos(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = cosf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate sine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("sin(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = sinf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate tangent
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("tan(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = tanf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc cosine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("acos(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value < -1 || value > 1)
			return ScriptError("Invalid value: " + tempdata);

		result = acosf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc sine
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("asin(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value < -1 || value > 1)
			return ScriptError("Invalid value: " + tempdata);

		result = asinf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc tangent
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("atan(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = atanf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate arc tangent with 2 parameters
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("atan2(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1));
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1));

		Real value1, value2;
		if (!IsNumeric(tempdata1, value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2, value2))
			return ScriptError("Invalid value: " + tempdata2);

		result = atan2f(value1, value2);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate square root
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("sqrt(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = sqrtf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate absolute value
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("abs(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = std::abs(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate exponential function
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("exp(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		result = expf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate natural logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = logf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate common logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log10(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = log10f(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate binary logarithm
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("log2(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = Log2(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate remainder
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("mod(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1));
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1));

		Real value1, value2;
		if (!IsNumeric(tempdata1, value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2, value2))
			return ScriptError("Invalid value: " + tempdata2);

		if (value2 == 0)
			return ScriptError("Invalid value: " + tempdata2);

		result = fmodf(value1, value2);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate hypotenuse
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("hypot(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		int mid = LineData.find(",", first);
		last = LineData.find(")", start);
		if (last < 0 || mid < 0)
			return ScriptError("Syntax error");

		std::string tempdata1 = Calc(LineData.substr(first + 1, mid - first - 1));
		std::string tempdata2 = Calc(LineData.substr(mid + 1, last - mid - 1));

		Real value1, value2;
		if (!IsNumeric(tempdata1, value1))
			return ScriptError("Invalid value: " + tempdata1);
		if (!IsNumeric(tempdata2, value2))
			return ScriptError("Invalid value: " + tempdata2);

		if (value2 == 0)
			return ScriptError("Invalid value: " + tempdata2);

		result = sqrtf(powf(value1, 2) + powf(value2, 2));
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate ceiling
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("ceil(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = ceilf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate floor
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("flr(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		result = floorf(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//calculate random number
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("rnd(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		tempdata = Calc(LineData.substr(first + 1, last - first - 1));
		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		RandomGen rnd(time(0));
		result = rnd.Get(value);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	//round a number
	while(true)
	{
		start = SetCaseCopy(LineData, false).find("round(", 0);
		if (start > 0)
		{
			//break if preceding letter is found
			char check = LineData[start - 1];
			if (check >= 65 && check <= 122)
				break;
		}
		else if (start < 0)
			break;

		first = LineData.find("(", start);
		last = LineData.find(")", start);
		if (last < 0)
			return ScriptError("Syntax error");

		int option = LineData.find(",", first);
		std::string decimals;
		int decimal = 0;
		if (option > 0 && option < last)
		{
			tempdata = Calc(LineData.substr(first + 1, option - first - 1));
			decimals = Calc(LineData.substr(option + 1, last - option - 1));
		}
		else
		{
			tempdata = Calc(LineData.substr(first + 1, last - first - 1));
			decimals = "0";
		}

		if (!IsNumeric(tempdata, value))
			return ScriptError("Invalid value: " + tempdata);

		if (!IsNumeric(decimals, decimal))
			return ScriptError("Invalid value: " + tempdata);

		if (value <= 0)
			return ScriptError("Invalid value: " + tempdata);

		if (decimal < 0)
			return ScriptError("Invalid value: " + decimals);

		result = Round(value, decimal);
		LineData = LineData.substr(0, start) + ToString(result) + LineData.substr(last + 1);
	}

	return sContinue;
}

std::string ScriptProcessor::Section::Calc(const std::string &expression)
{
	return parent->Calc(expression);
}

MeshObject* ScriptProcessor::Section::GetMeshObject(std::string name)
{
	//get a system mesh object, or search for a custom one by name

	SetCase(name, false);

	//get a system mesh object
	if (name == "floor")
	{
		if (config->SectionNum == 2)
			return Simcore->GetFloor(config->Current)->Level;
		return 0;
	}
	else if (name == "interfloor")
	{
		if (config->SectionNum == 2)
			return Simcore->GetFloor(config->Current)->Interfloor;
		return 0;
	}
	else if (name == "columnframe")
	{
		if (config->SectionNum == 2)
			return Simcore->GetFloor(config->Current)->ColumnFrame;
		return 0;
	}
	else if (name == "elevatorcar")
	{
		if (config->SectionNum == 6)
			return Simcore->GetElevator(config->CurrentOld)->GetCar(config->Current)->Mesh;
		return 0;
	}
	else if (name == "elevator")
	{
		if (config->SectionNum == 4)
			return Simcore->GetElevator(config->Current)->GetCar(1)->Mesh;
		return 0;
	}
	else if (name == "external")
		return Simcore->External;
	else if (name == "landscape")
		return Simcore->Landscape;
	else if (name == "buildings")
		return Simcore->Buildings;
	else if (name.substr(0, 5) == "shaft")
	{
		if (config->SectionNum == 2)
		{
			//get a shaft mesh object, or a model in a shaft

			std::string num, modelname;
			int marker = (int)name.find(":");
			if (marker > 0)
				modelname = name.substr(marker + 1);

			if (marker > 0)
				num = name.substr(5, (int)name.length() - marker - 1);
			else
				num = name.substr(5);

			TrimString(num);
			int number;
			if (!IsNumeric(num, number))
				return 0;

			Shaft *shaft = Simcore->GetShaft(number);
			if (!shaft)
				return 0;

			if (marker > 0)
			{
				Model *model = 0;

				if (shaft->GetLevel(config->Current))
					model = shaft->GetLevel(config->Current)->GetModel(modelname);

				if (model)
				{
					if (model->IsCustom() == true)
						return model->GetMeshObject();
				}
				return 0;
			}
			else
			{
				if (shaft->GetLevel(config->Current))
					return shaft->GetLevel(config->Current)->GetMeshObject();
				else
					return 0;
			}
		}
		return 0;
	}
	else if (name.substr(0, 9) == "stairwell")
	{
		if (config->SectionNum == 2)
		{
			//get a stairwell mesh object, or a model in a stairwell

			std::string num, modelname;
			int marker = (int)name.find(":");
			if (marker > 0)
				modelname = name.substr(marker + 1);

			if (marker > 0)
				num = name.substr(9, (int)name.length() - marker - 1);
			else
				num = name.substr(9);

			TrimString(num);
			int number;
			if (!IsNumeric(num, number))
				return 0;

			Stairwell *stairs = Simcore->GetStairwell(number);
			if (!stairs)
				return 0;

			if (marker > 0)
			{
				Model *model = 0;

				if (stairs->GetLevel(config->Current))
					model = stairs->GetLevel(config->Current)->GetModel(modelname);

				if (model)
				{
					if (model->IsCustom() == true)
						return model->GetMeshObject();
				}
				return 0;
			}
			else
			{
				if (stairs->GetLevel(config->Current))
					return stairs->GetLevel(config->Current)->GetMeshObject();
				else
					return 0;
			}
		}
		return 0;
	}

	//get a custom model mesh

	Model* model = 0;

	if (config->SectionNum == 2)
		model = Simcore->GetFloor(config->Current)->GetModel(name);
	else if (config->SectionNum == 4)
		model = Simcore->GetElevator(config->Current)->GetCar(1)->GetModel(name);
	else if (config->SectionNum == 6)
		model = Simcore->GetElevator(config->CurrentOld)->GetCar(config->Current)->GetModel(name);
	else
		model = Simcore->GetModel(name);

	if (model)
	{
		if (model->IsCustom() == true)
			return model->GetMeshObject();
	}
	return 0;
}

bool ScriptProcessor::Section::GetElevatorCar(std::string &value, int floor, int &elevator, int &car)
{
	//returns an elevator and car number for the specified string
	//for example, use "1" for Elevator 1
	//or "1:1" for Elevator 1 Car 1

	TrimString(value);

	if (IsNumeric(value, elevator))
	{
		if (!Simcore->GetElevator(elevator))
		{
			ScriptError("Invalid elevator " + ToString(elevator));
			return false;
		}

		car = 1;

		if (Simcore->GetFloor(floor))
		{
			ElevatorCar *obj = Simcore->GetElevator(elevator)->GetCarForFloor(floor);
			if (obj)
				car = obj->Number;
		}

		return true;
	}

	int pos = value.find(":");
	if (pos == -1)
	{
		elevator = 0;
		car = 0;
		ScriptError("Invalid elevator value");
		return false;
	}

	elevator = ToInt(value.substr(0, pos));
	car = ToInt(value.substr(pos + 1));

	//verify elevator and car objects
	Elevator *elev = Simcore->GetElevator(elevator);
	if (!elev)
	{
		ScriptError("Invalid elevator " + ToString(elevator));
		return false;
	}
	if (!elev->GetCar(car))
	{
		ScriptError("Invalid elevator car " + ToString(car));
		return false;
	}

	return true;
}

void ScriptProcessor::Section::GetDirectionStrings(int direction, std::string &face_direction, std::string &open_direction)
{
	//direction table:
	//1 = faces left, opens left
	//2 = faces left, opens right
	//3 = faces right, opens right
	//4 = faces right, opens left
	//5 = faces front, opens front
	//6 = faces front, opens back
	//7 = faces back, opens back
	//8 = faces back, opens front

	face_direction = "";
	open_direction = "";

	if (direction == 1 || direction == 2)
		face_direction = "left";
	if (direction == 3 || direction == 4)
		face_direction = "right";
	if (direction == 5 || direction == 6)
		face_direction = "front";
	if (direction == 7 || direction == 8)
		face_direction = "back";

	if (direction == 1 || direction == 4)
		open_direction = "left";
	else if (direction == 2 || direction == 3)
		open_direction = "right";
	else if (direction == 5 || direction == 8)
		open_direction = "front";
	else if (direction == 6 || direction == 7)
		open_direction = "back";
}

ScriptProcessor::ConfigHandler::ConfigHandler()
{
	Reset();
}

void ScriptProcessor::ConfigHandler::Reset()
{
	//shared variables
	setkey = false;
	keyvalue = 0;
	lockvalue = 0;
	MinExtent = Vector3::ZERO;
	MaxExtent = Vector3::ZERO;
	RangeL = 0;
	RangeLOld = 0;
	RangeH = 0;
	RangeHOld = 0;
	RangeStart = 0;
	RangeStartOld = 0;
	Current = 0;
	CurrentOld = 0;
	SectionNum = 0;
	Context = "None";
	ContextOld = "";
	ReverseAxis = false;
	InWhile = false;
	setshaftdoors = false;
	CheckScript = false;
}

}
