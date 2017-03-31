/* $Id$ */

/*
	Skyscraper 1.11 Alpha - Wall Creator Dialog
	Copyright (C)2003-2017 Ryan Thoryk
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

#ifndef WALLCREATOR_H
#define WALLCREATOR_H

//(*Headers(WallCreator)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/spinbutt.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class WallCreator: public wxDialog
{
	public:

		WallCreator(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~WallCreator();

		//(*Declarations(WallCreator)
		wxSpinButton* SpinZ1;
		wxSpinButton* SpinX1;
		wxTextCtrl* txtVoffset2;
		wxStaticText* StaticText13;
		wxSpinButton* SpinThickness;
		wxTextCtrl* txtZ2;
		wxTextCtrl* txtName;
		wxTextCtrl* txtVoffset1;
		wxTextCtrl* txtTW;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
		wxStaticText* StaticText3;
		wxSpinButton* SpinX2;
		wxSpinButton* SpinHeight1;
		wxTextCtrl* txtHeight1;
		wxSpinButton* SpinVoffset1;
		wxTextCtrl* txtThickness;
		wxTextCtrl* txtX1;
		wxTextCtrl* txtZ1;
		wxStaticText* StaticText8;
		wxStaticText* StaticText12;
		wxSpinButton* SpinZ2;
		wxTextCtrl* txtTexture;
		wxStaticText* StaticText7;
		wxSpinButton* SpinHeight2;
		wxStaticText* StaticText4;
		wxSpinButton* SpinTH;
		wxTextCtrl* txtHeight2;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxButton* bOK;
		wxSpinButton* SpinVoffset2;
		wxToggleButton* toggleAutosize;
		wxTextCtrl* txtX2;
		wxStaticText* StaticText9;
		wxSpinButton* SpinTW;
		wxTextCtrl* txtTH;
		wxStaticText* StaticText11;
		//*)

	protected:

		//(*Identifiers(WallCreator)
		static const long ID_STATICTEXT1;
		static const long ID_txtName;
		static const long ID_STATICTEXT2;
		static const long ID_txtTexture;
		static const long ID_STATICTEXT3;
		static const long ID_txtThickness;
		static const long ID_SpinThickness;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL1;
		static const long ID_SpinX1;
		static const long ID_STATICTEXT5;
		static const long ID_txtZ1;
		static const long ID_SpinZ1;
		static const long ID_STATICTEXT6;
		static const long ID_txtX2;
		static const long ID_SpinX2;
		static const long ID_STATICTEXT7;
		static const long ID_txtZ2;
		static const long ID_SpinZ2;
		static const long ID_STATICTEXT8;
		static const long ID_txtHeight1;
		static const long ID_SpinHeight1;
		static const long ID_STATICTEXT9;
		static const long ID_txtHeight2;
		static const long ID_SpinHeight2;
		static const long ID_STATICTEXT10;
		static const long ID_txtVoffset1;
		static const long ID_SpinVoffset1;
		static const long ID_STATICTEXT11;
		static const long ID_txtVoffset2;
		static const long ID_SpinVoffset2;
		static const long ID_STATICTEXT12;
		static const long ID_txtTW;
		static const long ID_SpinTW;
		static const long ID_STATICTEXT13;
		static const long ID_txtTH;
		static const long ID_SpinTH;
		static const long ID_toggleAutosize;
		static const long ID_bOK;
		//*)

	private:

		//(*Handlers(WallCreator)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
                                                                                                        
                                                                                                                                                                                                                                                                                                                        