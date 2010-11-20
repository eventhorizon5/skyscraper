/* $Id$ */

/*
	Scalable Building Simulator - Unix STL Globals
	The Skyscraper Project - Version 1.8 Alpha
	Copyright (C)2004-2010 Ryan Thoryk
	http://www.skyscrapersim.com
	http://sourceforge.net/projects/skyscraper
	Contact	- ryan@tliquest.net

	This program is	free software; you can redistribute it and/or
	modify it under	the terms of the GNU General Public License
	as published by	the Free Software Foundation; either version 2
	of the License,	or (at your option) any	later version.

	This program is	distributed in the hope	that it	will be	useful,
	but WITHOUT ANY	WARRANTY; without even the implied warranty of
	MERCHANTABILITY	or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have	received a copy	of the GNU General Public License
	along with this	program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _SBS_UNIX_H

#include <OgrePlatform.h>

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32

#define	_itoa itoa
#define	__strrev strrev
#define	_gcvt gcvt

const char* itoa(int value, char* str, int radix);
const char* strrev(char* szT);
const char* StrRev(char* szT);

#endif
#endif
