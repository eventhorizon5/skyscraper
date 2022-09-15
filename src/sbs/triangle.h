/* $Id: triangle.h 5263 2018-01-12 00:59:01Z ryan $ */

/*
	Scalable Building Simulator - Triangle Type
	The Skyscraper Project - Version 1.11 Alpha
	Copyright (C)2004-2018 Ryan Thoryk
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

//triangles have a, b and c components (each a vertex index)

#ifndef _SBS_TRIANGLE_H
#define _SBS_TRIANGLE_H

namespace SBS {

struct Triangle
{
	unsigned int a, b, c;

	inline Triangle(unsigned int a, unsigned int b, unsigned int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}

	inline Triangle()
	{
		this->a = 0;
		this->b = 0;
		this->c = 0;
	}

	inline bool operator == (const Triangle &other) const
	{
		return (a == other.a && b == other.b && c == other.c);
	}

	inline bool operator != (const Triangle &other) const
	{
		return !(*this == other);
	}

	inline Triangle& operator += (const Triangle &other)
	{
		a += other.a;
		b += other.b;
		c += other.b;
		return *this;
	}

	inline Triangle& operator += (const unsigned int value)
	{
		a += value;
		b += value;
		c += value;
		return *this;
	}

	inline Triangle& operator -= (const Triangle &other)
	{
		a -= other.a;
		b -= other.b;
		c -= other.b;
		return *this;
	}

	inline Triangle& operator -= (const unsigned int value)
	{
		a -= value;
		b -= value;
		c -= value;
		return *this;
	}

	inline Triangle& operator = (const Triangle &other)
	{
		a = other.a;
		b = other.b;
		c = other.c;
		return *this;
	}

	inline Triangle operator + (const Triangle &other) const
	{
		return Triangle(a + other.a, b + other.b, c + other.c);
	}

	inline Triangle operator + (const unsigned int value) const
	{
		return Triangle(a + value, b + value, c + value);
	}

	inline Triangle operator - (const Triangle &other) const
	{
		return Triangle(a - other.a, b - other.b, c - other.c);
	}

	inline Triangle operator - (const unsigned int value) const
	{
		return Triangle(a - value, b - value, c - value);
	}
};

}

#endif
