/*
	Skyscraper 2.1 - Network Core
	Copyright (C)2025 Ryan Thoryk
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

//DLL Exporting
#ifdef _WIN32
	#if defined(_MSC_VER)
		#define __VISUALC__ _MSC_VER
	#endif
	#if defined(__VISUALC__) || defined(__GNUC__)
		#if defined(VMNET_EXPORTS)
			#define VMNETIMPEXP __declspec(dllexport)
			#define VMNETIMPEXP_DATA(type) __declspec(dllexport) type
		#else
			#define VMNETIMPEXP __declspec(dllimport)
			#define VMNETIMPEXP_DATA(type) __declspec(dllimport) type
		#endif
	#else
		#define VMNETIMPEXP
		#define VMNETIMPEXP_DATA(type) type
	#endif
#endif

#ifndef VMNETIMPEXP
	#define VMNETIMPEXP
	#define VMNETIMPEXP_DATA(type)
#endif
