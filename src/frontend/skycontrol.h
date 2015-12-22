/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Sky Control Dialog
	Copyright (C)2003-2015 Ryan Thoryk
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

#ifndef SKYCONTROL_H
#define SKYCONTROL_H

//(*Headers(SkyControl)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class SkyControl: public wxDialog {
	public:

		SkyControl(DebugPanel* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~SkyControl();

		//(*Declarations(SkyControl)
		wxTextCtrl* tJulian;
		wxStaticLine* StaticLine2;
		wxToggleButton* bSet;
		wxButton* bSetLongitude;
		wxTextCtrl* tDay;
		wxTextCtrl* tHour;
		wxTextCtrl* tMultiplier;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* bSetMultiplier;
		wxStaticLine* StaticLine1;
		wxTextCtrl* tLatitude;
		wxStaticText* StaticText8;
		wxTextCtrl* tLongitude;
		wxStaticText* StaticText7;
		wxStaticText* StaticText4;
		wxTextCtrl* tMonth;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxTextCtrl* tYear;
		wxStaticText* StaticText6;
		wxTextCtrl* tSecond;
		wxTextCtrl* tMinute;
		wxStaticText* StaticText9;
		wxStaticText* StaticText11;
		wxButton* bSetLatitude;
		//*)
		void Loop();

	protected:

		//(*Identifiers(SkyControl)
		static const long ID_STATICTEXT1;
		static const long ID_tLatitude;
		static const long ID_bSetLatitude;
		static const long ID_STATICTEXT2;
		static const long ID_tLongitude;
		static const long ID_bSetLongitude;
		static const long ID_STATICTEXT11;
		static const long ID_tJulian;
		static const long ID_STATICLINE1;
		static const long ID_STATICLINE2;
		static const long ID_STATICTEXT3;
		static const long ID_tYear;
		static const long ID_STATICTEXT4;
		static const long ID_tMonth;
		static const long ID_STATICTEXT5;
		static const long ID_tDay;
		static const long ID_STATICTEXT6;
		static const long ID_tHour;
		static const long ID_STATICTEXT7;
		static const long ID_tMinute;
		static const long ID_STATICTEXT8;
		static const long ID_tSecond;
		static const long ID_bSet;
		static const long ID_STATICTEXT9;
		static const long ID_tMultiplier;
		static const long ID_bSetMultiplier;
		//*)
		void OnInit();

	private:

		//(*Handlers(SkyControl)
		void OnInit(wxInitDialogEvent& event);
		void On_bSetLatitude_Click(wxCommandEvent& event);
		void On_bSetLongitude_Click(wxCommandEvent& event);
		void On_bSet_Toggle(wxCommandEvent& event);
		void On_bSetMultiplier_Click(wxCommandEvent& event);
		//*)
		DebugPanel *panel;
		Caelum::CaelumSystem* system;

		DECLARE_EVENT_TABLE()
};

}

#endif
