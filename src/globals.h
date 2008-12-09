/* $Id$ */

/*
	Scalable Building Simulator - SBS Engine Globals
	The Skyscraper Project - Version 1.2 Alpha
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

#ifndef _SBS_GLOBALS_H
#define _SBS_GLOBALS_H

//DLL Exporting
#ifdef _WIN32
	#if defined(__VISUALC__) || defined(__BORLANDC__) || defined(__GNUC__) || defined(__WATCOMC__)
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
SBSIMPEXP bool IsNumeric(const char *expression);
SBSIMPEXP const char *BoolToString(bool item);

#endif
