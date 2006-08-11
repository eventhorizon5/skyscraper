/*
	Skyscraper 1.1 Alpha - Loader Form
	Copyright ©2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
	http://sourceforge.net/projects/skyscraper
	Contact - ryan@tliquest.net

	$Id$
	
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

#include "loader.h"

BEGIN_EVENT_TABLE(Loader,wxDialog)
//(*EventTable(Loader)
//*)
END_EVENT_TABLE()

Loader::Loader(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(Loader)
    Create(parent,id,_("Building Loader"),wxDefaultPosition,wxDefaultSize,0);
    //*)
}

Loader::~Loader()
{
}


void Loader::OnInit(wxInitDialogEvent& event)
{

}
