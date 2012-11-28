/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Parameter Viewer Dialog
	Copyright (C)2003-2012 Ryan Thoryk
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

#ifndef PARAMETERVIEWER_H
#define PARAMETERVIEWER_H

//(*Headers(ParameterViewer)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class ParameterViewer: public wxDialog
{
	public:

		ParameterViewer(wxWindow* parent, wxString object_type, wxString object_parent, bool createobject, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ParameterViewer();

		//(*Declarations(ParameterViewer)
		wxStaticText* l7;
		wxTextCtrl* t7;
		wxTextCtrl* t20;
		wxButton* bTextures;
		wxTextCtrl* t9;
		wxStaticText* l15;
		wxStaticText* l20;
		wxStaticText* l4;
		wxTextCtrl* t14;
		wxTextCtrl* t13;
		wxTextCtrl* t19;
		wxTextCtrl* t15;
		wxStaticText* l2;
		wxButton* bCancel;
		wxTextCtrl* t12;
		wxTextCtrl* t1;
		wxStaticText* l17;
		wxStaticText* StaticText1;
		wxStaticText* l10;
		wxStaticText* l11;
		wxStaticText* l3;
		wxStaticText* l1;
		wxStaticText* l9;
		wxTextCtrl* t11;
		wxStaticText* l13;
		wxStaticText* l12;
		wxTextCtrl* t16;
		wxStaticText* l6;
		wxTextCtrl* t18;
		wxStaticText* l16;
		wxTextCtrl* t5;
		wxTextCtrl* t17;
		wxTextCtrl* t6;
		wxTextCtrl* t10;
		wxTextCtrl* tCommand;
		wxStaticText* l8;
		wxTextCtrl* t8;
		wxTextCtrl* t2;
		wxStaticText* l14;
		wxTextCtrl* t3;
		wxStaticText* tDesc;
		wxButton* bOK;
		wxStaticText* tVariables;
		wxTextCtrl* t4;
		wxStaticText* l5;
		wxStaticText* l18;
		wxStaticText* l19;
		//*)
		wxString objecttype;
		wxString objectparent;
		wxString command;
		bool create;  //true to create an object, false to modify


	protected:

		//(*Identifiers(ParameterViewer)
		static const long ID_tDesc;
		static const long ID_tVariables;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL6;
		static const long ID_STATICTEXT7;
		static const long ID_TEXTCTRL7;
		static const long ID_STATICTEXT8;
		static const long ID_TEXTCTRL8;
		static const long ID_STATICTEXT9;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT10;
		static const long ID_TEXTCTRL10;
		static const long ID_l11;
		static const long ID_t11;
		static const long ID_l12;
		static const long ID_t12;
		static const long ID_l13;
		static const long ID_t13;
		static const long ID_l14;
		static const long ID_t14;
		static const long ID_l15;
		static const long ID_t15;
		static const long ID_l16;
		static const long ID_t16;
		static const long ID_l17;
		static const long ID_t17;
		static const long ID_l18;
		static const long ID_t18;
		static const long ID_l19;
		static const long ID_t19;
		static const long ID_l20;
		static const long ID_t20;
		static const long ID_STATICTEXT11;
		static const long ID_tCommand;
		static const long ID_bCancel;
		static const long ID_bTextures;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(ParameterViewer)
		void On_bCancel_Click(wxCommandEvent& event);
		void On_bOK_Click(wxCommandEvent& event);
		void ChangeParam(wxCommandEvent& event);
		void On_bTextures_Click(wxCommandEvent& event);
		//*)

		bool Setup();

		DECLARE_EVENT_TABLE()
};

#endif
