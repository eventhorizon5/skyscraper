/* $Id$ */

/*
	Scalable Building Simulator - Global functions
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2007 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#include <crystalspace.h>
#include <string>
#include <ctype.h>
#include "globals.h"

bool IsEven(int Number)
{
	//Determine if the passed number is even.
	//If number divides evenly, return true
	if ((Number / 2) == int(Number / 2))
		return true;
	else
		return false;
}

bool IsNumeric(const char *expression)
{
	//test to see if a string is numeric
	
	for (int i = 0; i < strlen(expression) - 1; i++)
	{
		if (isdigit(expression[i]) == false && expression[i] != '-' && expression[i] != '.')
			return false;
	}
	return true;
}

const char *BoolToString(bool item)
{
	if (item == true)
		return "true";
	else
		return "false";
}

double RoundDouble(double value, int decimalplaces)
{
	//found at http://www.codeguru.com/forum/archive/index.php/t-180415.html
	double decimalpts, resultmult;

	if (value < 0.0)
	{
		value = fabs(value);
		resultmult = -1.0;
	}
	else
		resultmult = 1.0;

	// decimalpts effectively moves the decimal points over so the floor()
	// function returns the rounded version [thanks to the 0.5]
	double base = 10;
	decimalpts = pow(base, decimalplaces);
	return (floor((value * decimalpts) + 0.5) / decimalpts) * resultmult;
}
