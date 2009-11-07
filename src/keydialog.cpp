/* $Id$ */

/*
	Skyscraper 1.6 Alpha - Control Reference Form
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

#include "keydialog.h"

//(*InternalHeaders(KeyDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(KeyDialog)
const long KeyDialog::ID_TEXTCTRL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(KeyDialog,wxDialog)
	//(*EventTable(KeyDialog)
	//*)
END_EVENT_TABLE()

KeyDialog::KeyDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(KeyDialog)
	wxBoxSizer* BoxSizer1;
	
	Create(parent, wxID_ANY, _("Control Reference"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxRESIZE_BORDER|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("Simulator Keyboard and Mouse Controls from the readme.txt file):\n\n--- Keyboard ---\nW or Up Arrow - move forwards\nS or Down Arrow - move backwards\nA or Left Arrow - move left\nD or Right Arrow - move right\nPgUp - Look upwards\nPgDown - Look downwards\nSpace - Jump\nHome - Float upwards (or jump if collision detection is on)\nEnd - Float downwards (or crouch if collision detection is on)\nF2 - print FPS on console\nF3 - Reset camera rotation and zoom to default\nF4 - Enable/disable wireframe mode\nF5 - Freelook (mouse look) mode\nF6 - Full camera reset (respawn)\nF10 - Fullscreen mode\nF11 - Take screenshot\nShift - Run (hold down with other keys)\nControl - Walk slowly (hold down with other keys)\nAlt + arrow keys - Strafe\nAlt + PgUp - Spin right\nAlt + PgDown - Spin left\nPlus - Zoom in (decrease FOV angle)\nMinus - Zoom out (increase FOV angle)\n\n--- Mouse actions ---\n-elevator shaft doors can be manually opened and closed by holding down the Shift key and clicking on them\n-hallway call buttons can be lit and unlit also by holding down Shift and clicking on them (this does not call an elevator, but just changes the light status)\n\n--- Debug options ---\nOther functions are available via a Crystal Space plugin called BugPlug.\nTo type a debug command: type ctrl-d and then press the bugplug key (so for wireframe mode, you\'d press ctrl-d, the screen will prompt you for a command, and then you\'d type \"e\".)\nTo select an object/mesh: type ctrl-s and then click on an object\n\nShow wireframe mode (the second command is needed due to a quirk with the wxGL plugin - also make sure you turn off the sky when doing this):\nctrl-d e\nctrl-d c\n\nDisable wireframe mode:\nctrl-d e\n\nchange gamma:\nctrl-d g\n\nshow camera info:\nctrl-d shift-c\n\ndump info on all CS engine objects:\nctrl-d d\n\ndump all 3D info to an xml file (saves to world0.xml):\nctrl-d f10\n\nchange FOV:\nctrl-d f\n\nchange FOV angle:\nctrl-d shift-f\n\nmove meshes:\nfirst select an object using ctrl-s\nthen,\nctrl-d shift-up = move up (+y)\nctrl-d shift-down = move down (-y)\nctrl-d shift-left = left (-x)\nctrl-d shift-right = right (+x)\nctrl-d shift-pgup = forward (+z)\nctrl-d shift-pgdn = backward (-z)\n\nscreenshots:\nctrl-d f11 = normal\nctrl-d shift-f11 = large\n\nshow/hide FPS display:\nctrl-d ctrl-alt-p\n\nhide selected mesh:\nctrl-d h\n\nunhide selected mesh:\nctrl-d shift-h\n\nselect mesh(es) by name:\nctrl-d ctrl-alt-m\n\nlist loaded CS plugins:\nctrl-d ctrl-alt-l\n"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl1->SetMinSize(wxSize(600,500));
	BoxSizer1->Add(TextCtrl1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	//*)
}

KeyDialog::~KeyDialog()
{
	//(*Destroy(KeyDialog)
	//*)
}

