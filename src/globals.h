/* $Id$ */

/*
	Scalable Building Simulator - SBS Engine Globals
	The Skyscraper Project - Version 1.1 Alpha
	Copyright (C)2005-2008 Ryan Thoryk
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

#include <wx/panel.h>
#include <wx/timer.h>
#include <crystalspace.h>
#include "ivideo/wxwin.h"

extern double AutoSize(double n1, double n2, bool iswidth);

#ifndef _SBS_GLOBALS_H
#define _SBS_GLOBALS_H

bool IsEven(int Number);
bool IsNumeric(const char *expression);
const char *BoolToString(bool item);
double RoundDouble(double value, int decimalplaces);
float RoundFloat(float value, int decimals);

#endif
