/* $Id: profilergui.h 5263 2018-01-12 00:59:01Z ryan $ */

/*
	Skyscraper 1.11 Alpha - Profiler GUI
	Copyright (C)2003-2018 Ryan Thoryk
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

#ifndef PROFILERGUI_H
#define PROFILERGUI_H

//(*Headers(Profiler)
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class Profiler: public wxDialog
{
	public:

		Profiler(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~Profiler();
		void Loop();

		//(*Declarations(Profiler)
		wxTextCtrl* txtMain;
		wxCheckBox* chkAdvanced;
		wxCheckBox* chkCapture;
		//*)

	protected:

		//(*Identifiers(Profiler)
		static const long ID_chkAdvanced;
		static const long ID_chkCapture;
		static const long ID_txtMain;
		//*)

	private:

		//(*Handlers(Profiler)
		void OnResize(wxSizeEvent& event);
		//*)

		bool advanced;
		int count;
		DECLARE_EVENT_TABLE()
};

}

#endif

