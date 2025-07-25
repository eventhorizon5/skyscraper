/*
	Scalable Building Simulator - Object Container
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2025 Ryan Thoryk
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

#ifndef _SBS_CONTAINER_H
#define _SBS_CONTAINER_H

#include <any>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include "globals.h"
#include "sbs.h"

namespace SBS {

class Container {
private:
    std::map<std::type_index, std::any> containers;

public:
	Container() {}
	~Container() {}

    //add a value of type T to its corresponding vector
    template<typename T>
    void add(const T& value)
	{
        std::type_index key = typeid(T);
        if (containers.find(key) == containers.end())
            containers[key] = std::vector<T>{};  // initialize new vector<T>
        std::any_cast<std::vector<T>&>(containers[key]).emplace_back(value);
    }

	template<typename T>
	void remove(const T& value)
	{
		std::type_index key = typeid(T);
		size_t index = containers.find(key);
		if (index == containers.end())
			return;
		std::any_cast<std::vector<T>&>(containers[key]).erase(std::any_cast<std::vector<T>&>(containers[key]).begin() + index);
	}

    //get reference to vector<T>
    template<typename T>
    std::vector<T>& getVector()
	{
        std::type_index key = typeid(T);
        if (containers.find(key) == containers.end())
            throw std::runtime_error("No vector for this type.");
        return std::any_cast<std::vector<T>&>(containers[key]);
    }
};

}

#endif
