/*
	Skyscraper 2.1 - Main Screen
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

#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#ifdef __WXWINDOWS__
#include <wx/frame.h>
#include <wx/panel.h>

class wxJoystick;

namespace Skyscraper {

class MainScreen : public wxFrame
{
public:
	MainScreen(Skyscraper *parent, int width, int height);
	virtual ~MainScreen();
	void OnIconize(wxIconizeEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnClose(wxCloseEvent& event);
	void ShowWindow();
	void OnIdle(wxIdleEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnActivate(wxActivateEvent & event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnMouseButton(wxMouseEvent& event);
	void EnableFreelook(bool value);
#ifndef __FreeBSD__
	void OnJoystickEvent(wxJoystickEvent& event);
#endif

	bool Active;
	bool InLoop;
	Skyscraper *frontend;
	wxPanel *panel;

private:
	void GetKeyStates(EngineContext *engine, wxKeyEvent& event, bool down);
	void ProcessMovement(EngineContext *engine, bool control = false, bool shift = false, bool angle_only = false);
	void HandleMouseMovement();

	//input system states
	bool boxes;
	bool colliders;
	int wireframe;
	bool strafe_left, strafe_right;
	bool float_up, float_down;
	bool spin_up, spin_down;
	bool turn_left, turn_right;
	bool look_up, look_down;
	bool step_forward, step_backward;

	char key_right, key_left, key_up, key_down, key_straferight, key_strafeleft, key_lookup, key_lookdown;
	char key_binoculars, key_crouch, key_floatup, key_floatdown, key_noclip;
	char key_pickup, key_load, key_enter;

	bool freelook;

#ifndef __FreeBSD__
	wxJoystick *joystick;
	int joy_buttons;
	int joy_click, joy_strafe, joy_fast, joy_turn, joy_forward;
#endif

	DECLARE_EVENT_TABLE()
};

}

#endif
#endif
