/* $Id$ */

/*
	Scalable Building Simulator - SBS Engine Globals
	The Skyscraper Project - Version 1.10 Alpha
	Copyright (C)2004-2015 Ryan Thoryk
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

#ifndef _SBS_GLOBALS_H
#define _SBS_GLOBALS_H

#include <string>
#include <vector>

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

SBSIMPEXP bool IsEven(int Number);
SBSIMPEXP bool IsNumeric(char character);
SBSIMPEXP bool IsNumeric(const char *string);
SBSIMPEXP bool IsNumeric(const char *string, int &number);
SBSIMPEXP bool IsNumeric(const char *string, float &number);
SBSIMPEXP const char *BoolToString(bool item);
SBSIMPEXP float RadiansToDegrees(float radians);
SBSIMPEXP float DegreesToRadians(float degrees);
SBSIMPEXP float Min3(float a, float b, float c);
SBSIMPEXP float Max3(float a, float b, float c);
SBSIMPEXP const char* SetCaseCopy(const char *string, bool uppercase);
SBSIMPEXP std::string SetCaseCopy(std::string string, bool uppercase);
SBSIMPEXP void SetCase(std::string &string, bool uppercase);
SBSIMPEXP int FindWithCase(const char *string, bool uppercase, const char *key, int offset);
SBSIMPEXP void TrimString(std::string &string);
SBSIMPEXP void ReplaceAll(std::string &string, const char *original, const char *replacement);
SBSIMPEXP bool StartsWith(std::string &string, const char *check_string, bool ignore_case = false);
SBSIMPEXP void SplitString(std::vector<std::string> &dest_array, const char *original_string, char separator);
SBSIMPEXP const char* ToString(int number);
SBSIMPEXP std::string ToString2(int number);
SBSIMPEXP const char* ToString(float number);
SBSIMPEXP std::string ToString2(float number);
SBSIMPEXP float Log2(float number);
SBSIMPEXP float Round(float number);
SBSIMPEXP bool IsBoolean(std::string string);

#endif
