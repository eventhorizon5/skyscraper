/* $Id$ */

/*
	Scalable Building Simulator - Unix STL Code
	The Skyscraper Project - Version 1.1 Alpha
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
#include <algorithm>
#include "unix.h"

#ifndef CS_PLATFORM_WIN32

using namespace std;

const char* itoa(int value, char* str, int radix)
{
	int rem = 0;
	int pos = 0;
	char ch = '!';
	do
	{
		rem = value % radix;
		value /= radix;
		if ( 16 == radix )
		{
			if( rem >= 10 && rem <= 15 )
			{
				switch( rem )
				{
					case 10:
					ch = 'a';
					break;
					case 11:
					ch ='b';
					break;
					case 12:
					ch = 'c';
					break;
					case 13:
					ch ='d';
					break;
					case 14:
					ch = 'e';
					break;
					case 15:
					ch ='f';
					break;
				}
			}
		}
		if( '!' == ch )
		{
			str[pos++] = (char) ( rem + 0x30 );
		}
		else
		{
			str[pos++] = ch ;
		}
	}while( value != 0 );
	str[pos] = '\0';
	return StrRev(str);
}

//strrev the standard way

const char* strrev(char* szT)
{
	if ( !szT )	// handle null passed strings.
		return "";
	int i = strlen(szT);
	int t = !(i%2)? 1 : 0;	// check the length of the string .
	for(int j = i-1 , k = 0 ; j > (i/2 -t) ; j-- )
	{
		char ch = szT[j];
		szT[j] = szT[k];
		szT[k++] = ch;
	}
	return szT;
}

// strrev STL way .
const char* StrRev(char* szT)
{
	string s(szT);
	reverse(s.begin(), s.end());
	strncpy(szT, s.c_str(), s.size());
	szT[s.size()+1] = '\0';
	return szT;
}

#endif
