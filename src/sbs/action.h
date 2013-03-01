/* $Id$ */

/*
        Scalable Building Simulator - Action Interface Class
        The Skyscraper Project - Version 1.9 Alpha
        Copyright (C)2004-2013 Ryan Thoryk
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

#ifndef _SBS_ACTION_H
#define _SBS_ACTION_H

class SBSIMPEXP Action
{
public:
	//functions
	Action(const std::string name, std::vector<Object*> &action_parents, const std::string &command, const std::vector<std::string> &parameters);
	Action(const std::string name, std::vector<Object*> &action_parents, const std::string &command);
	~Action();
	bool DoAction();
	const char *GetName();
	const char *GetCommandName();
	const Object *GetParent(int number);
	const char *GetParentName(int number);
	const char *GetParentType(int number);
	int GetParentCount();
	int GetParameterCount();
	const char *GetParameter(int index);

private:

	std::string command_name;
	std::vector<std::string> command_parameters;
	int state;
	std::string name;
	std::vector<Object*> parent_objects;
};

#endif
