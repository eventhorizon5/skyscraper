/* $Id$ */

/*
	Scalable Building Simulator - Global functions
	The Skyscraper Project - Version 1.4 Alpha
	Copyright (C)2005-2009 Ryan Thoryk
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

#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "globals.h"

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
	val = strtof(string, &endptr);

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
