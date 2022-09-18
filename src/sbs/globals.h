/* $Id$ */

/*
	Scalable Building Simulator - Global Functions
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2022 Ryan Thoryk
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

#ifndef _SBS_GLOBALS_H
#define _SBS_GLOBALS_H

#include <string>
#include <vector>
//#include <OgreVector2.h>
//#include <OgreVector3.h>
#include <Ogre.h>

//DLL Exporting
#ifdef _WIN32
	#if defined(_MSC_VER)
		#define __VISUALC__ _MSC_VER
	#endif
	#if defined(__VISUALC__) || defined(__GNUC__)
		#ifdef SBS_DLL
			#define SBSIMPEXP __declspec(dllexport)
			#define SBSIMPEXP_DATA(type) __declspec(dllexport) type
		#else
			#define SBSIMPEXP __declspec(dllimport)
			#define SBSIMPEXP_DATA(type) __declspec(dllimport) type
		#endif
	#else
		#define SBSIMPEXP
		#define SBSIMPEXP_DATA(type) type
	#endif
#endif

#ifndef SBSIMPEXP
	#define SBSIMPEXP
	#define SBSIMPEXP_DATA(type)
#endif

typedef Ogre::Real Real;

namespace SBS {

const Real pi = 3.14159265;

SBSIMPEXP bool IsEven(int Number);
SBSIMPEXP bool IsNumeric(char character);
SBSIMPEXP bool IsNumeric(const std::string &string);
SBSIMPEXP bool IsNumeric(const std::string &string, int &number);
SBSIMPEXP bool IsNumeric(const std::string &string, float &number);
SBSIMPEXP bool IsNumeric(const std::string &string, double &number);
SBSIMPEXP std::string BoolToString(bool item);
SBSIMPEXP Real RadiansToDegrees(Real radians);
SBSIMPEXP Real DegreesToRadians(Real degrees);
SBSIMPEXP Real Min(Real a, Real b, Real c);
SBSIMPEXP Real Max(Real a, Real b, Real c);
SBSIMPEXP Real Min(Real a, Real b, Real c, Real d);
SBSIMPEXP Real Max(Real a, Real b, Real c, Real d);
SBSIMPEXP std::string SetCaseCopy(std::string string, bool uppercase);
SBSIMPEXP void SetCase(std::string &string, bool uppercase);
SBSIMPEXP int FindWithCase(const std::string &string, bool uppercase, const std::string &key, int offset);
SBSIMPEXP void TrimString(std::string &string);
SBSIMPEXP std::string TrimStringCopy(std::string string);
SBSIMPEXP void ReplaceAll(std::string &string, const std::string &original, const std::string &replacement);
SBSIMPEXP bool StartsWith(const std::string &string, const std::string &check_string, bool ignore_case = false);
SBSIMPEXP void SplitString(std::vector<std::string> &dest_array, const std::string &original_string, char separator);
SBSIMPEXP std::string ToString(int number);
SBSIMPEXP std::string ToString(float number);
SBSIMPEXP std::string ToString(double number);
SBSIMPEXP float Log2(float number);
SBSIMPEXP double Log2(double number);
SBSIMPEXP float Round(float number, int decimal_places = 0);
SBSIMPEXP double Round(double number, int decimal_places = 0);
SBSIMPEXP Ogre::Vector3 Round(const Ogre::Vector3 &value, int decimal_places = 0);
SBSIMPEXP Ogre::Vector2 Round(const Ogre::Vector2 &value, int decimal_places = 0);
SBSIMPEXP bool IsBoolean(std::string string);
SBSIMPEXP Real ToFloat(const std::string &string);
SBSIMPEXP int ToInt(const std::string &string);
SBSIMPEXP bool ToBool(std::string string);
SBSIMPEXP std::string TruncateNumber(float value, int decimals);
SBSIMPEXP std::string TruncateNumber(double value, int decimals);
SBSIMPEXP std::string TruncateNumber(const std::string &number, int decimals);

}

#endif
