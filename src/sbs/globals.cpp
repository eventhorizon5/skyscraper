/* $Id$ */

/*
	Scalable Building Simulator - Global functions
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2011 Ryan Thoryk
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

#include <algorithm>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <OgreString.h>
#include "globals.h"

const double pi = 3.14159265;

bool IsEven(int Number)
{
	//Determine if the passed number is even.
	//If number divides evenly, return true

	float temp = Number;
	if ((temp / 2) == int(temp / 2))
		return true;
	else
		return false;
}

bool IsNumeric(const char *string)
{
	//test to see if a string is numeric

	float a;
	return IsNumeric(string, a);
}

bool IsNumeric(const char *string, int &number)
{
	//test to see if a string is numeric, and return number as integer

	int base = 10;
	char *endptr;
	long val;

	errno = 0;
	val = strtol(string, &endptr, base);

	//check for errors
	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0))
	{
		//perror("strtol");
		return false;
	}

	//no digits found
	if (endptr == string)
		return false;

	//if extra characters were found after a number
	if (*endptr != '\0')
		return false;

	//returned a number
	number = (int)val;
	return true;
}

bool IsNumeric(const char *string, float &number)
{
	//test to see if a string is numeric, and return number as float

	char *endptr;
	float val;

	errno = 0;
#ifdef _WIN32
	val = (float)strtod(string, &endptr);
#else
	val = strtof(string, &endptr);
#endif

	//check for errors
	//if ((errno == ERANGE && (val == FLOAT_MAX || val == FLOAT_MIN)) || (errno != 0 && val == 0))
	if (errno == ERANGE || (errno != 0 && val == 0))
	{
		//perror("strtol");
		return false;
	}

	//no digits found
	if (endptr == string)
		return false;

	//if extra characters were found after a number
	if (*endptr != '\0')
		return false;

	//returned a number
	number = val;
	return true;
}

const char *BoolToString(bool item)
{
	if (item == true)
		return "true";
	else
		return "false";
}

float RadiansToDegrees(float radians)
{
	//convert from radians to degrees
	return radians * (180 / pi);
}

float DegreesToRadians(float degrees)
{
	//convert from degrees to radians
	return degrees * (pi / 180);
}

float Min3(float a, float b, float c)
{
	//return smallest value
	if (a <= b && a <= c)
		return a;
	if (b <= a && b <= c)
		return b;
	return c;
}

float Max3(float a, float b, float c)
{
	//return largest value
	if (a >= b && a >= c)
		return a;
	if (b >= a && b >= c)
		return b;
	return c;
}

const char* SetCaseCopy(const char *string, bool uppercase)
{
	//change case of a string
	std::string newstring = string;
	SetCase(newstring, uppercase);
	return newstring.c_str();
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

int FindWithCase(const char *string, bool uppercase, const char *key, int offset)
{
	//change case of a string, and return location of search key
	std::string newstring = SetCaseCopy(string, uppercase);
	newstring.find(newstring, offset);
	int loc = newstring.find(newstring, offset);
	if (loc == std::string::npos)
		return -1;
	else
		return loc;
}

void TrimString(std::string &string)
{
	//trim whitespace from string
	Ogre::StringUtil::trim(string, true, true);
}

void ReplaceAll(std::string &string, const char *original, const char *replacement)
{
	//replace all occurances of "original" with "replacement"

	std::string newstring = Ogre::StringUtil::replaceAll(string, original, replacement);
	string = newstring;
}
bool StartsWith(std::string &string, const char *check_string, bool ignore_case)
{
	//check if a string starts with the contents of "check_string"

	if (ignore_case == true)
	{
		int result = FindWithCase(string.c_str(), false, check_string, 0);
		if (result == 0)
			return true;
		return false;
	}
	else
	{
		int result = string.find(check_string, 0);
		if (result >= 0 && result != std::string::npos)
			return true;
		return false;
	}
}

void SplitString(std::vector<std::string> &dest_array, const char *original_string, char separator)
{
	//split a string into an array of strings, divided by "separator"

    int startpos = 0;
    int endpos = 0;
    std::string newstring;

	dest_array.clear();
	std::string original = original_string;
	TrimString(original);

    endpos = original.find_first_of(separator, startpos);
	if (endpos == -1)
	{
		newstring = original.substr(startpos, endpos - startpos);
		TrimString(newstring);
		dest_array.push_back(newstring);
	}

    while (endpos != -1)
    {
    	newstring = original.substr(startpos, endpos - startpos);
    	TrimString(newstring);
        dest_array.push_back(newstring); //add to vector
        startpos = endpos + 1; //jump past separator
        endpos = original.find_first_of(separator, startpos); //find next
        if(endpos == -1)
        {
            //last one, so no 2nd param required to go to end of string
        	newstring = original.substr(startpos);
        	TrimString(newstring);
            dest_array.push_back(newstring);
        }
    }
}
