/*
	Scalable Building Simulator - Global Functions
	The Skyscraper Project - Version 2.1
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

#include <algorithm>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include "globals.h"

namespace SBS {

bool IsEven(int Number)
{
	//Determine if the passed number is even.
	//If number divides evenly, return true

	if (((Real)Number / 2) == int((Real)Number / 2))
		return true;
	else
		return false;
}

bool IsNumeric(char character)
{
	//test to see if a character is numeric

	if (character > 47 && character < 58)
		return true;
	return false;
}

bool IsNumeric(const std::string &string)
{
	//test to see if a string is numeric

	Real num = 0;
	return IsNumeric(string, num);
}

bool IsNumeric(const std::string &string, int &number)
{
	//test to see if a string is numeric, and return number as integer

	Real num = 0;
	bool result = IsNumeric(string, num);
	number = (int)num;
	return result;
}

bool IsNumeric(const std::string &string, float &number)
{
	//test to see if a string is numeric, and return number as float

	char* end = 0;

#ifdef _WIN32
	number = (float)std::strtod(string.c_str(), &end);
#else
	number = std::strtof(string.c_str(), &end);
#endif

	return end != 0 && *end == 0;
}

bool IsNumeric(const std::string &string, double &number)
{
	//test to see if a string is numeric, and return number as float

	char* end = 0;

	number = std::strtod(string.c_str(), &end);

	return end != 0 && *end == 0;
}

std::string BoolToString(bool item)
{
	if (item == true)
		return "true";
	else
		return "false";
}

Real RadiansToDegrees(Real radians)
{
	//convert from radians to degrees
	return radians * (180 / pi);
}

Real DegreesToRadians(Real degrees)
{
	//convert from degrees to radians
	return degrees * (pi / 180);
}

Real Min(Real a, Real b, Real c)
{
	//return smallest value
	if (a <= b && a <= c)
		return a;
	if (b <= a && b <= c)
		return b;
	return c;
}

Real Max(Real a, Real b, Real c)
{
	//return largest value
	if (a >= b && a >= c)
		return a;
	if (b >= a && b >= c)
		return b;
	return c;
}

Real Min(Real a, Real b, Real c, Real d)
{
	//return smallest value
	if (a <= b && a <= c && a <= d)
		return a;
	if (b <= a && b <= c && b <= d)
		return b;
	if (c <= a && c <= b && c <= d)
		return c;
	return d;
}

Real Max(Real a, Real b, Real c, Real d)
{
	//return largest value
	if (a >= b && a >= c && a >= d)
		return a;
	if (b >= a && b >= c && b >= d)
		return b;
	if (c >= a && c >= b && c >= d)
		return c;
	return d;
}

std::string SetCaseCopy(std::string string, bool uppercase)
{
	//change case of a string
	SetCase(string, uppercase);
	return string;
}

void SetCase(std::string &string, bool uppercase)
{
	//change case of a string
	if (uppercase == true)
		std::transform(string.begin(), string.end(), string.begin(), ::toupper);
	else
		std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}

int FindWithCase(const std::string &string, bool uppercase, const std::string &key, int offset)
{
	//change case of a string, and return location of search key
	std::string newstring = SetCaseCopy(string, uppercase);
	return (int)newstring.find(key, offset);
}

void TrimString(std::string &string)
{
	//trim whitespace from string
	Ogre::StringUtil::trim(string, true, true);
}

std::string TrimStringCopy(std::string string)
{
	//trim whitespace from string
	Ogre::StringUtil::trim(string, true, true);
	return string;
}

void ReplaceAll(std::string &string, const std::string &original, const std::string &replacement)
{
	//replace all occurrences of "original" with "replacement"

	size_t position = 0;

	while(true)
	{
		position = string.find(original, position);
		if (position == string.npos)
			break;
		string.replace(position, strlen(original.c_str()), replacement);
	}
}

bool StartsWith(const std::string &string, const std::string &check_string, bool ignore_case)
{
	//check if a string starts with the contents of "check_string"

	if (ignore_case == true)
	{
		std::string lower = string.substr(0, check_string.size());
		SetCase(lower, false);

		if (lower == SetCaseCopy(check_string, false))
			return true;
	}
	else
	{
		if (string.substr(0, check_string.size()) == check_string)
			return true;
	}

	return false;
}

bool StartsWithNoCase(const std::string &string, const std::string &check_string)
{
	return StartsWith(string, check_string, true);
}

void SplitString(std::vector<std::string> &dest_array, const std::string &original_string, char separator)
{
	//split a string into an array of strings, divided by "separator"

	size_t startpos = 0;
	size_t endpos = 0;
	std::string newstring;

	dest_array.clear();
	std::string original = original_string;
	TrimString(original);

	endpos = original.find_first_of(separator, startpos);
	if (endpos == std::string::npos)
	{
		newstring = original.substr(startpos, endpos - startpos);
		TrimString(newstring);
		dest_array.emplace_back(newstring);
	}

	while (endpos != std::string::npos)
	{
		newstring = original.substr(startpos, endpos - startpos);
		TrimString(newstring);
		dest_array.emplace_back(newstring); //add to vector
		startpos = endpos + 1; //jump past separator
		endpos = original.find_first_of(separator, startpos); //find next
		if(endpos == std::string::npos)
		{
			//last one, so no 2nd param required to go to end of string
			newstring = original.substr(startpos);
			TrimString(newstring);
			dest_array.emplace_back(newstring);
		}
	}
}

std::string ToString(int number)
{
	char buffer[50];
#if defined(__VISUALC__)
	_snprintf_s(buffer, sizeof(buffer), 13, "%d", number);
#else
	snprintf(buffer, sizeof(buffer), "%d", number);
#endif
	return buffer;
}

std::string ToString(float number)
{
	char buffer[50];
#if defined(__VISUALC__)
	_snprintf_s(buffer, sizeof(buffer), 13, "%g", number);
#else
	snprintf(buffer, sizeof(buffer), "%g", (double)number);
#endif
	return buffer;
}

std::string ToString(double number)
{
	char buffer[50];
#if defined(__VISUALC__)
	_snprintf_s(buffer, sizeof(buffer), 13, "%g", number);
#else
	snprintf(buffer, sizeof(buffer), "%g", number);
#endif
	return buffer;
}

#if defined(__VISUALC__)
std::string ToString(size_t number)
{
	char buffer[50];
#if defined(__VISUALC__)
	_snprintf_s(buffer, sizeof(buffer), 13, "%lu", number);
#else
	snprintf(buffer, sizeof(buffer), "%lu", number);
#endif
	return buffer;
}
#endif

std::string ToString(unsigned long number)
{
	char buffer[50];
#if defined(__VISUALC__)
	_snprintf_s(buffer, sizeof(buffer), 13, "%lu", number);
#else
	snprintf(buffer, sizeof(buffer), "%lu", number);
#endif
	return buffer;
}

float Log2(float number)
{
	return logf(number) / logf(2.0f);
}

double Log2(double number)
{
	return log(number) / log(2.0);
}

float Round(float number, int decimal_places)
{
	//round float to specified decimal places

	if (decimal_places <= 0)
		return floorf(number + 0.5f);

	float multiplier = powf(10.0f, (float)decimal_places);
	float rounded = floorf((number * multiplier) + 0.5f) / multiplier;
	return rounded;
}

double Round(double number, int decimal_places)
{
	//round float to specified decimal places

	if (decimal_places <= 0)
		return floor(number + 0.5);

	double multiplier = pow(10.0, decimal_places);
	double rounded = floor((number * multiplier) + 0.5) / multiplier;
	return rounded;
}

Vector3 Round(const Vector3 &value, int decimal_places)
{
	//round a 3D vector to specified decimal places

	Vector3 result;
	result.x = Round(value.x, decimal_places);
	result.y = Round(value.y, decimal_places);
	result.z = Round(value.z, decimal_places);
	return result;
}

Vector2 Round(const Vector2 &value, int decimal_places)
{
	//round a 2D vector to specified decimal places

	Vector2 result;
	result.x = Round(value.x, decimal_places);
	result.y = Round(value.y, decimal_places);
	return result;
}

bool IsBoolean(std::string string)
{
	SetCase(string, false);
	return (string == "true" || string == "false");
}

Real ToFloat(const std::string &string)
{
	return atof(string.c_str());
}

int ToInt(const std::string &string)
{
	return atoi(string.c_str());
}

bool ToBool(std::string string)
{
	SetCase(string, false);

	if (string == "true")
		return true;
	return false;
}

std::string TruncateNumber(float value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)

	if ((int)value == value)
		decimals = 0; //value is an integer

	std::stringstream buffer;
	buffer.precision(decimals);
	buffer << std::fixed << value;

	return buffer.str();
}

std::string TruncateNumber(double value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)

	if ((int)value == value)
		decimals = 0; //value is an integer

	std::stringstream buffer;
	buffer.precision(decimals);
	buffer << std::fixed << value;

	return buffer.str();
}

std::string TruncateNumber(const std::string &value, int decimals)
{
	//truncates the numeric value to the specified number of decimal places (does not round)

	std::string number = value;

	if (decimals < 1)
		return number;

	int decimal = number.find(".");
	if (decimal < 0)
		return number;

	number.erase(decimal + decimals + 1);
	if (number.at(number.length() - 1) == '.')
		number = number.substr(0, number.length() - 1); //strip of extra decimal point if even
	return number;
}

}
