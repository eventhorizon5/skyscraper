/*
	Skyscraper 1.11 Alpha - Control Reference Form
	Copyright (C)2003-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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
#include <wx/string.h>
#include <wx/intl.h>
//*)

namespace Skyscraper {

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
	TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("Simulator Keyboard and Mouse Controls from the readme.txt file:\n\n--- Keyboard ---\n\nOn a Mac, the Control key is the Command key (to the right of the Option key), and Alt is the Option key\n\nW or Up Arrow - move forwards\nS or Down Arrow - move backwards\nA or Left Arrow - move left\nD or Right Arrow - move right\nPgUp or P - Look upwards\nPgDown or L - Look downwards\nSpace - Jump\nHome or O - Float upwards (or jump if collision detection is on)\nEnd or K - Float downwards (or crouch if collision detection is on)\nF1 - show control reference\nF2 - print render statistics on console\nF3 - Reset camera rotation and zoom to default\nF4 - Enable/disable wireframe mode\nF5 - Freelook (mouse look) mode\nF6 - Full camera reset (respawn)\nF7 - Show colliders\nF8 - Show mesh bounding boxes\nF9 - Toggle statistics display\nF10 - Fullscreen mode\nF11 - Take screenshot\nF12 - Show control panel\nShift - Run (hold down with other keys)\nControl - Walk slowly (hold down with other keys)\nAlt + arrow keys - Strafe\nAlt + PgUp/P - Spin right\nAlt + PgDown/L - Spin left\nPlus or ] - Zoom in (decrease FOV angle)\nMinus or [ - Zoom out (increase FOV angle)\nEsc - exit to main menu\nV - Toggle noclip mode (gravity and collisions off)\nB - Binoculars mode (hold down), and reset FOV to default\nC - Pick up object (physics-enabled models)\nX - Crouch\n; - (semicolon) - Load an additional building\n1 to 0 - (number keys) - Switch active camera to engine instance number (ex. 2 for building engine 2)\nE - Enter/Exit a vehicle\nCtrl-R - Reload current building\nCtrl-Alt-C - crash program (throw exception), used for testing handlers\n\n--- Mouse actions ---\n-the selection order of controls (buttons and switches can be reversed by right-clicking them\n-elevator shaft doors can be manually opened and closed by holding down the Shift key and clicking on them - note that this only works when the interlocks are released, which can be done in the elevator editor by clicking \"Set\" next to \"Interlocks\" on the related elevator\n-polygons (wall/floor sides) and other objects can be deleted by holding down both Ctrl and Alt, and clicking on the object\n-doors, controls/buttons, and call buttons can be locked/unlocked by holding down both the Ctrl and Shift keys while clicking on the side to lock, if you have the associated key\n"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	TextCtrl1->SetMinSize(wxSize(600,500));
	BoxSizer1->Add(TextCtrl1, 1, wxALL|wxEXPAND, 5);
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

}
