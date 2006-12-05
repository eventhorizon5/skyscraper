/* $Id$ */

/*
	Skyscraper 1.1 Alpha - Edit Elevator Form
	Copyright (C)2005-2006 Ryan Thoryk
	http://www.tliquest.net/skyscraper
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

#ifndef EDITELEVATOR_H
#define EDITELEVATOR_H

//(*Headers(editelevator)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class editelevator: public wxDialog
{
	public:

		editelevator(wxWindow* parent,wxWindowID id = -1);
		virtual ~editelevator();

		//(*Identifiers(editelevator)
		enum Identifiers
		{
		    ID_STATICTEXT1 = 0x1000,
		    ID_SLIDER1,
		    ID_STATICTEXT2,
		    ID_SLIDER2,
		    ID_BUTTON11,
		    ID_BUTTON12,
		    ID_BUTTON1,
		    ID_BUTTON2,
		    ID_BUTTON3,
		    ID_BUTTON4,
		    ID_BUTTON5,
		    ID_BUTTON6,
		    ID_BUTTON7,
		    ID_BUTTON8,
		    ID_BUTTON9,
		    ID_BUTTON10,
		    ID_STATICTEXT3,
		    ID_TEXTCTRL2,
		    ID_STATICTEXT5,
		    ID_TEXTCTRL1,
		    ID_BUTTON13,
		    ID_STATICTEXT4,
		    ID_TEXTCTRL3,
		    ID_STATICTEXT6,
		    ID_TEXTCTRL4,
		    ID_STATICTEXT7,
		    ID_TEXTCTRL5,
		    ID_STATICTEXT8,
		    ID_TEXTCTRL6,
		    ID_STATICTEXT9,
		    ID_TEXTCTRL7,
		    ID_STATICTEXT10,
		    ID_TEXTCTRL8,
		    ID_STATICTEXT11,
		    ID_TEXTCTRL9,
		    ID_STATICTEXT12,
		    ID_TEXTCTRL10,
		    ID_STATICTEXT13,
		    ID_TEXTCTRL11,
		    ID_STATICTEXT14,
		    ID_TEXTCTRL12,
		    ID_STATICTEXT15,
		    ID_TEXTCTRL13,
		    ID_STATICTEXT16,
		    ID_TEXTCTRL14,
		    ID_STATICTEXT17,
		    ID_TEXTCTRL15,
		    ID_STATICTEXT18,
		    ID_TEXTCTRL16,
		    ID_STATICTEXT19,
		    ID_TEXTCTRL17,
		    ID_STATICTEXT20,
		    ID_TEXTCTRL18,
		    ID_STATICTEXT38,
		    ID_TEXTCTRL36,
		    ID_STATICTEXT39,
		    ID_TEXTCTRL37,
		    ID_STATICTEXT40,
		    ID_TEXTCTRL38,
		    ID_STATICTEXT41,
		    ID_TEXTCTRL39,
		    ID_STATICTEXT21,
		    ID_TEXTCTRL19,
		    ID_BUTTON14,
		    ID_STATICTEXT22,
		    ID_TEXTCTRL20,
		    ID_BUTTON15,
		    ID_STATICTEXT23,
		    ID_TEXTCTRL21,
		    ID_BUTTON16,
		    ID_STATICTEXT24,
		    ID_TEXTCTRL22,
		    ID_BUTTON17,
		    ID_STATICTEXT25,
		    ID_TEXTCTRL23,
		    ID_BUTTON18,
		    ID_STATICTEXT26,
		    ID_TEXTCTRL24,
		    ID_BUTTON19,
		    ID_STATICTEXT27,
		    ID_TEXTCTRL25,
		    ID_STATICTEXT28,
		    ID_TEXTCTRL26,
		    ID_STATICTEXT29,
		    ID_TEXTCTRL27,
		    ID_STATICTEXT30,
		    ID_TEXTCTRL28,
		    ID_STATICTEXT31,
		    ID_TEXTCTRL29,
		    ID_STATICTEXT32,
		    ID_TEXTCTRL30,
		    ID_STATICTEXT33,
		    ID_TEXTCTRL31,
		    ID_STATICTEXT34,
		    ID_TEXTCTRL32,
		    ID_STATICTEXT35,
		    ID_TEXTCTRL33,
		    ID_STATICTEXT36,
		    ID_TEXTCTRL34,
		    ID_STATICTEXT37,
		    ID_TEXTCTRL35
		};
		//*)

	protected:

		//(*Handlers(editelevator)
		void OnInit(wxInitDialogEvent& event);
        //*)

		//(*Declarations(editelevator)
		wxBoxSizer* BoxSizer1;
		wxBoxSizer* BoxSizer2;
		wxBoxSizer* BoxSizer6;
		wxStaticText* StaticText1;
		wxSlider* Slider1;
		wxStaticText* StaticText2;
		wxSlider* Slider2;
		wxBoxSizer* BoxSizer7;
		wxButton* Button11;
		wxButton* Button12;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxBoxSizer* BoxSizer8;
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxButton* Button5;
		wxBoxSizer* BoxSizer9;
		wxButton* Button6;
		wxButton* Button7;
		wxButton* Button8;
		wxButton* Button9;
		wxButton* Button10;
		wxBoxSizer* BoxSizer3;
		wxStaticBoxSizer* StaticBoxSizer2;
		wxFlexGridSizer* FlexGridSizer1;
		wxStaticText* StaticText3;
		wxTextCtrl* TextCtrl2;
		wxStaticText* StaticText5;
		wxTextCtrl* TextCtrl1;
		wxButton* Button13;
		wxStaticText* StaticText4;
		wxTextCtrl* TextCtrl3;
		wxStaticText* StaticText6;
		wxTextCtrl* TextCtrl4;
		wxStaticText* StaticText7;
		wxTextCtrl* TextCtrl5;
		wxStaticText* StaticText8;
		wxTextCtrl* TextCtrl6;
		wxStaticText* StaticText9;
		wxTextCtrl* TextCtrl7;
		wxStaticText* StaticText10;
		wxTextCtrl* TextCtrl8;
		wxStaticText* StaticText11;
		wxTextCtrl* TextCtrl9;
		wxStaticText* StaticText12;
		wxTextCtrl* TextCtrl10;
		wxStaticText* StaticText13;
		wxTextCtrl* TextCtrl11;
		wxBoxSizer* BoxSizer10;
		wxStaticBoxSizer* StaticBoxSizer5;
		wxFlexGridSizer* FlexGridSizer2;
		wxStaticText* StaticText14;
		wxTextCtrl* TextCtrl12;
		wxStaticText* StaticText15;
		wxTextCtrl* TextCtrl13;
		wxStaticText* StaticText16;
		wxTextCtrl* TextCtrl14;
		wxStaticText* StaticText17;
		wxTextCtrl* TextCtrl15;
		wxStaticText* StaticText18;
		wxTextCtrl* TextCtrl16;
		wxStaticText* StaticText19;
		wxTextCtrl* TextCtrl17;
		wxStaticText* StaticText20;
		wxTextCtrl* TextCtrl18;
		wxStaticBoxSizer* StaticBoxSizer3;
		wxFlexGridSizer* FlexGridSizer3;
		wxStaticText* StaticText38;
		wxTextCtrl* TextCtrl36;
		wxStaticText* StaticText39;
		wxTextCtrl* TextCtrl37;
		wxStaticText* StaticText40;
		wxTextCtrl* TextCtrl38;
		wxStaticText* StaticText41;
		wxTextCtrl* TextCtrl39;
		wxStaticBoxSizer* StaticBoxSizer7;
		wxFlexGridSizer* FlexGridSizer6;
		wxStaticText* StaticText21;
		wxTextCtrl* TextCtrl19;
		wxButton* Button14;
		wxStaticText* StaticText22;
		wxTextCtrl* TextCtrl20;
		wxButton* Button15;
		wxStaticText* StaticText23;
		wxTextCtrl* TextCtrl21;
		wxButton* Button16;
		wxStaticText* StaticText24;
		wxTextCtrl* TextCtrl22;
		wxButton* Button17;
		wxStaticText* StaticText25;
		wxTextCtrl* TextCtrl23;
		wxButton* Button18;
		wxStaticText* StaticText26;
		wxTextCtrl* TextCtrl24;
		wxButton* Button19;
		wxStaticText* StaticText27;
		wxTextCtrl* TextCtrl25;
		wxStaticText* StaticText28;
		wxTextCtrl* TextCtrl26;
		wxStaticText* StaticText29;
		wxTextCtrl* TextCtrl27;
		wxStaticBoxSizer* StaticBoxSizer8;
		wxFlexGridSizer* FlexGridSizer7;
		wxStaticText* StaticText30;
		wxTextCtrl* TextCtrl28;
		wxStaticText* StaticText31;
		wxTextCtrl* TextCtrl29;
		wxStaticText* StaticText32;
		wxTextCtrl* TextCtrl30;
		wxStaticText* StaticText33;
		wxTextCtrl* TextCtrl31;
		wxStaticText* StaticText34;
		wxTextCtrl* TextCtrl32;
		wxStaticText* StaticText35;
		wxTextCtrl* TextCtrl33;
		wxStaticText* StaticText36;
		wxTextCtrl* TextCtrl34;
		wxStaticText* StaticText37;
		wxTextCtrl* TextCtrl35;
		//*)

	private:

		DECLARE_EVENT_TABLE()
};

#endif
