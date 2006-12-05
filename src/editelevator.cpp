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

#include "editelevator.h"

BEGIN_EVENT_TABLE(editelevator,wxDialog)
//(*EventTable(editelevator)
//*)
END_EVENT_TABLE()

editelevator::editelevator(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(editelevator)
	Create(parent,id,_("Elevator Parameters"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX,_T(""));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer6 = new wxBoxSizer(wxVERTICAL);
	StaticText1 = new wxStaticText(this,ID_STATICTEXT1,_("Elevator"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE,_("ID_STATICTEXT1"));
	Slider1 = new wxSlider(this,ID_SLIDER1,0,0,100,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_SLIDER1"));
	if ( 0 ) Slider1->SetTickFreq(0,0);
	if ( 0 ) Slider1->SetPageSize(0);
	if ( 0 ) Slider1->SetLineSize(0);
	if ( 0 ) Slider1->SetThumbLength(0);
	if ( 0 ) Slider1->SetTick(0);
	if ( 0 || 0 ) Slider1->SetSelection(0,0);
	StaticText2 = new wxStaticText(this,ID_STATICTEXT2,_("Floor"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE,_("ID_STATICTEXT2"));
	Slider2 = new wxSlider(this,ID_SLIDER2,0,0,100,wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_SLIDER2"));
	if ( 0 ) Slider2->SetTickFreq(0,0);
	if ( 0 ) Slider2->SetPageSize(0);
	if ( 0 ) Slider2->SetLineSize(0);
	if ( 0 ) Slider2->SetThumbLength(0);
	if ( 0 ) Slider2->SetTick(0);
	if ( 0 || 0 ) Slider2->SetSelection(0,0);
	BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	Button11 = new wxButton(this,ID_BUTTON11,_("Dump Floor List"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON11"));
	if (false) Button11->SetDefault();
	Button12 = new wxButton(this,ID_BUTTON12,_("Dump Queues"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON12"));
	if (false) Button12->SetDefault();
	BoxSizer7->Add(Button11,1,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,15);
	BoxSizer7->Add(Button12,1,wxLEFT|wxRIGHT|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,15);
	BoxSizer6->Add(StaticText1,0,wxALL|wxALIGN_CENTER,0);
	BoxSizer6->Add(Slider1,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	BoxSizer6->Add(StaticText2,0,wxTOP|wxALIGN_CENTER,10);
	BoxSizer6->Add(Slider2,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	BoxSizer6->Add(BoxSizer7,0,wxTOP|wxALIGN_CENTER_HORIZONTAL|wxALIGN_BOTTOM|wxEXPAND,15);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Control"));
	BoxSizer8 = new wxBoxSizer(wxVERTICAL);
	Button1 = new wxButton(this,ID_BUTTON1,_("Call"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON1"));
	if (false) Button1->SetDefault();
	Button2 = new wxButton(this,ID_BUTTON2,_("Go"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON2"));
	if (false) Button2->SetDefault();
	Button3 = new wxButton(this,ID_BUTTON3,_("Open"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON3"));
	if (false) Button3->SetDefault();
	Button4 = new wxButton(this,ID_BUTTON4,_("Open Manual"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON4"));
	if (false) Button4->SetDefault();
	Button5 = new wxButton(this,ID_BUTTON5,_("Emerg. Stop"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON5"));
	if (false) Button5->SetDefault();
	BoxSizer8->Add(Button1,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(Button2,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(-1,10,0);
	BoxSizer8->Add(Button3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(Button4,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer8->Add(-1,10,0);
	BoxSizer8->Add(Button5,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9 = new wxBoxSizer(wxVERTICAL);
	Button6 = new wxButton(this,ID_BUTTON6,_("Enqueue Up"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON6"));
	if (false) Button6->SetDefault();
	Button7 = new wxButton(this,ID_BUTTON7,_("Enqueue Down"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON7"));
	if (false) Button7->SetDefault();
	Button8 = new wxButton(this,ID_BUTTON8,_("Close"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON8"));
	if (false) Button8->SetDefault();
	Button9 = new wxButton(this,ID_BUTTON9,_("Close Manual"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON9"));
	if (false) Button9->SetDefault();
	Button10 = new wxButton(this,ID_BUTTON10,_("Alarm"),wxDefaultPosition,wxDefaultSize,0,wxDefaultValidator,_("ID_BUTTON10"));
	if (false) Button10->SetDefault();
	BoxSizer9->Add(Button6,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(Button7,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(-1,10,0);
	BoxSizer9->Add(Button8,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(Button9,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	BoxSizer9->Add(-1,10,0);
	BoxSizer9->Add(Button10,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP|wxEXPAND,0);
	StaticBoxSizer1->Add(BoxSizer8,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer1->Add(BoxSizer9,1,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer2->Add(BoxSizer6,2,wxTOP|wxALIGN_LEFT|wxALIGN_TOP,10);
	BoxSizer2->Add(StaticBoxSizer1,1,wxRIGHT|wxTOP|wxALIGN_RIGHT|wxALIGN_TOP,5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("General"));
	FlexGridSizer1 = new wxFlexGridSizer(0,3,0,0);
	StaticText3 = new wxStaticText(this,ID_STATICTEXT3,_("Number:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT3"));
	TextCtrl2 = new wxTextCtrl(this,ID_TEXTCTRL2,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL2"));
	if ( 0 ) TextCtrl2->SetMaxLength(0);
	StaticText5 = new wxStaticText(this,ID_STATICTEXT5,_("Name:"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT5"));
	TextCtrl1 = new wxTextCtrl(this,ID_TEXTCTRL1,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL1"));
	if ( 0 ) TextCtrl1->SetMaxLength(0);
	Button13 = new wxButton(this,ID_BUTTON13,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON13"));
	if (false) Button13->SetDefault();
	StaticText4 = new wxStaticText(this,ID_STATICTEXT4,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT4"));
	TextCtrl3 = new wxTextCtrl(this,ID_TEXTCTRL3,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL3"));
	if ( 0 ) TextCtrl3->SetMaxLength(0);
	StaticText6 = new wxStaticText(this,ID_STATICTEXT6,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT6"));
	TextCtrl4 = new wxTextCtrl(this,ID_TEXTCTRL4,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL4"));
	if ( 0 ) TextCtrl4->SetMaxLength(0);
	StaticText7 = new wxStaticText(this,ID_STATICTEXT7,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT7"));
	TextCtrl5 = new wxTextCtrl(this,ID_TEXTCTRL5,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL5"));
	if ( 0 ) TextCtrl5->SetMaxLength(0);
	StaticText8 = new wxStaticText(this,ID_STATICTEXT8,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT8"));
	TextCtrl6 = new wxTextCtrl(this,ID_TEXTCTRL6,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL6"));
	if ( 0 ) TextCtrl6->SetMaxLength(0);
	StaticText9 = new wxStaticText(this,ID_STATICTEXT9,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT9"));
	TextCtrl7 = new wxTextCtrl(this,ID_TEXTCTRL7,_("Text"),wxPoint(-1,75),wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL7"));
	if ( 0 ) TextCtrl7->SetMaxLength(0);
	StaticText10 = new wxStaticText(this,ID_STATICTEXT10,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT10"));
	TextCtrl8 = new wxTextCtrl(this,ID_TEXTCTRL8,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL8"));
	if ( 0 ) TextCtrl8->SetMaxLength(0);
	StaticText11 = new wxStaticText(this,ID_STATICTEXT11,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT11"));
	TextCtrl9 = new wxTextCtrl(this,ID_TEXTCTRL9,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL9"));
	if ( 0 ) TextCtrl9->SetMaxLength(0);
	StaticText12 = new wxStaticText(this,ID_STATICTEXT12,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT12"));
	TextCtrl10 = new wxTextCtrl(this,ID_TEXTCTRL10,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL10"));
	if ( 0 ) TextCtrl10->SetMaxLength(0);
	StaticText13 = new wxStaticText(this,ID_STATICTEXT13,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT13"));
	TextCtrl11 = new wxTextCtrl(this,ID_TEXTCTRL11,_("Text"),wxDefaultPosition,wxSize(75,-1),wxTE_READONLY,wxDefaultValidator,_("ID_TEXTCTRL11"));
	if ( 0 ) TextCtrl11->SetMaxLength(0);
	FlexGridSizer1->Add(StaticText3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl2,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText5,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl1,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(Button13,0,wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL,0);
	FlexGridSizer1->Add(StaticText4,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl3,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText6,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl4,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText7,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl5,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText8,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl6,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText9,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl7,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText10,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl8,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText11,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl9,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText12,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl10,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	FlexGridSizer1->Add(StaticText13,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer1->Add(TextCtrl11,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer1->Add(-1,-1,0);
	StaticBoxSizer2->Add(FlexGridSizer1,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10 = new wxBoxSizer(wxVERTICAL);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Position"));
	FlexGridSizer2 = new wxFlexGridSizer(0,2,0,0);
	StaticText14 = new wxStaticText(this,ID_STATICTEXT14,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT14"));
	TextCtrl12 = new wxTextCtrl(this,ID_TEXTCTRL12,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL12"));
	if ( 0 ) TextCtrl12->SetMaxLength(0);
	StaticText15 = new wxStaticText(this,ID_STATICTEXT15,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT15"));
	TextCtrl13 = new wxTextCtrl(this,ID_TEXTCTRL13,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL13"));
	if ( 0 ) TextCtrl13->SetMaxLength(0);
	StaticText16 = new wxStaticText(this,ID_STATICTEXT16,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT16"));
	TextCtrl14 = new wxTextCtrl(this,ID_TEXTCTRL14,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL14"));
	if ( 0 ) TextCtrl14->SetMaxLength(0);
	StaticText17 = new wxStaticText(this,ID_STATICTEXT17,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT17"));
	TextCtrl15 = new wxTextCtrl(this,ID_TEXTCTRL15,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL15"));
	if ( 0 ) TextCtrl15->SetMaxLength(0);
	StaticText18 = new wxStaticText(this,ID_STATICTEXT18,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT18"));
	TextCtrl16 = new wxTextCtrl(this,ID_TEXTCTRL16,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL16"));
	if ( 0 ) TextCtrl16->SetMaxLength(0);
	StaticText19 = new wxStaticText(this,ID_STATICTEXT19,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT19"));
	TextCtrl17 = new wxTextCtrl(this,ID_TEXTCTRL17,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL17"));
	if ( 0 ) TextCtrl17->SetMaxLength(0);
	StaticText20 = new wxStaticText(this,ID_STATICTEXT20,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT20"));
	TextCtrl18 = new wxTextCtrl(this,ID_TEXTCTRL18,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL18"));
	if ( 0 ) TextCtrl18->SetMaxLength(0);
	FlexGridSizer2->Add(StaticText14,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl12,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText15,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl13,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText16,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl14,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText17,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl15,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText18,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl16,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText19,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl17,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer2->Add(StaticText20,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer2->Add(TextCtrl18,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer5->Add(FlexGridSizer2,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Queues"));
	FlexGridSizer3 = new wxFlexGridSizer(0,2,0,0);
	StaticText38 = new wxStaticText(this,ID_STATICTEXT38,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT38"));
	TextCtrl36 = new wxTextCtrl(this,ID_TEXTCTRL36,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL36"));
	if ( 0 ) TextCtrl36->SetMaxLength(0);
	StaticText39 = new wxStaticText(this,ID_STATICTEXT39,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT39"));
	TextCtrl37 = new wxTextCtrl(this,ID_TEXTCTRL37,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL37"));
	if ( 0 ) TextCtrl37->SetMaxLength(0);
	StaticText40 = new wxStaticText(this,ID_STATICTEXT40,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT40"));
	TextCtrl38 = new wxTextCtrl(this,ID_TEXTCTRL38,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL38"));
	if ( 0 ) TextCtrl38->SetMaxLength(0);
	StaticText41 = new wxStaticText(this,ID_STATICTEXT41,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT41"));
	TextCtrl39 = new wxTextCtrl(this,ID_TEXTCTRL39,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL39"));
	if ( 0 ) TextCtrl39->SetMaxLength(0);
	FlexGridSizer3->Add(StaticText38,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(TextCtrl36,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText39,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(TextCtrl37,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText40,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(TextCtrl38,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer3->Add(StaticText41,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer3->Add(TextCtrl39,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer3->Add(FlexGridSizer3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10->Add(StaticBoxSizer5,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer10->Add(StaticBoxSizer3,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer7 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Motion"));
	FlexGridSizer6 = new wxFlexGridSizer(0,3,0,0);
	StaticText21 = new wxStaticText(this,ID_STATICTEXT21,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT21"));
	TextCtrl19 = new wxTextCtrl(this,ID_TEXTCTRL19,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL19"));
	if ( 0 ) TextCtrl19->SetMaxLength(0);
	Button14 = new wxButton(this,ID_BUTTON14,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON14"));
	if (false) Button14->SetDefault();
	StaticText22 = new wxStaticText(this,ID_STATICTEXT22,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT22"));
	TextCtrl20 = new wxTextCtrl(this,ID_TEXTCTRL20,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL20"));
	if ( 0 ) TextCtrl20->SetMaxLength(0);
	Button15 = new wxButton(this,ID_BUTTON15,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON15"));
	if (false) Button15->SetDefault();
	StaticText23 = new wxStaticText(this,ID_STATICTEXT23,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT23"));
	TextCtrl21 = new wxTextCtrl(this,ID_TEXTCTRL21,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL21"));
	if ( 0 ) TextCtrl21->SetMaxLength(0);
	Button16 = new wxButton(this,ID_BUTTON16,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON16"));
	if (false) Button16->SetDefault();
	StaticText24 = new wxStaticText(this,ID_STATICTEXT24,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT24"));
	TextCtrl22 = new wxTextCtrl(this,ID_TEXTCTRL22,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL22"));
	if ( 0 ) TextCtrl22->SetMaxLength(0);
	Button17 = new wxButton(this,ID_BUTTON17,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON17"));
	if (false) Button17->SetDefault();
	StaticText25 = new wxStaticText(this,ID_STATICTEXT25,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT25"));
	TextCtrl23 = new wxTextCtrl(this,ID_TEXTCTRL23,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL23"));
	if ( 0 ) TextCtrl23->SetMaxLength(0);
	Button18 = new wxButton(this,ID_BUTTON18,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON18"));
	if (false) Button18->SetDefault();
	StaticText26 = new wxStaticText(this,ID_STATICTEXT26,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT26"));
	TextCtrl24 = new wxTextCtrl(this,ID_TEXTCTRL24,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL24"));
	if ( 0 ) TextCtrl24->SetMaxLength(0);
	Button19 = new wxButton(this,ID_BUTTON19,_("Set"),wxDefaultPosition,wxDefaultSize,wxBU_EXACTFIT,wxDefaultValidator,_("ID_BUTTON19"));
	if (false) Button19->SetDefault();
	StaticText27 = new wxStaticText(this,ID_STATICTEXT27,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT27"));
	TextCtrl25 = new wxTextCtrl(this,ID_TEXTCTRL25,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL25"));
	if ( 0 ) TextCtrl25->SetMaxLength(0);
	StaticText28 = new wxStaticText(this,ID_STATICTEXT28,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT28"));
	TextCtrl26 = new wxTextCtrl(this,ID_TEXTCTRL26,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL26"));
	if ( 0 ) TextCtrl26->SetMaxLength(0);
	StaticText29 = new wxStaticText(this,ID_STATICTEXT29,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT29"));
	TextCtrl27 = new wxTextCtrl(this,ID_TEXTCTRL27,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL27"));
	if ( 0 ) TextCtrl27->SetMaxLength(0);
	FlexGridSizer6->Add(StaticText21,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl19,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button14,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText22,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl20,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button15,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText23,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl21,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button16,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText24,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl22,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button17,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText25,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl23,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button18,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText26,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl24,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(Button19,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(StaticText27,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl25,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	FlexGridSizer6->Add(StaticText28,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl26,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	FlexGridSizer6->Add(StaticText29,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer6->Add(TextCtrl27,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer6->Add(-1,-1,0);
	StaticBoxSizer7->Add(FlexGridSizer6,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	StaticBoxSizer8 = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Label"));
	FlexGridSizer7 = new wxFlexGridSizer(0,2,0,0);
	StaticText30 = new wxStaticText(this,ID_STATICTEXT30,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT30"));
	TextCtrl28 = new wxTextCtrl(this,ID_TEXTCTRL28,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL28"));
	if ( 0 ) TextCtrl28->SetMaxLength(0);
	StaticText31 = new wxStaticText(this,ID_STATICTEXT31,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT31"));
	TextCtrl29 = new wxTextCtrl(this,ID_TEXTCTRL29,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL29"));
	if ( 0 ) TextCtrl29->SetMaxLength(0);
	StaticText32 = new wxStaticText(this,ID_STATICTEXT32,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT32"));
	TextCtrl30 = new wxTextCtrl(this,ID_TEXTCTRL30,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL30"));
	if ( 0 ) TextCtrl30->SetMaxLength(0);
	StaticText33 = new wxStaticText(this,ID_STATICTEXT33,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT33"));
	TextCtrl31 = new wxTextCtrl(this,ID_TEXTCTRL31,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL31"));
	if ( 0 ) TextCtrl31->SetMaxLength(0);
	StaticText34 = new wxStaticText(this,ID_STATICTEXT34,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT34"));
	TextCtrl32 = new wxTextCtrl(this,ID_TEXTCTRL32,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL32"));
	if ( 0 ) TextCtrl32->SetMaxLength(0);
	StaticText35 = new wxStaticText(this,ID_STATICTEXT35,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT35"));
	TextCtrl33 = new wxTextCtrl(this,ID_TEXTCTRL33,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL33"));
	if ( 0 ) TextCtrl33->SetMaxLength(0);
	StaticText36 = new wxStaticText(this,ID_STATICTEXT36,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT36"));
	TextCtrl34 = new wxTextCtrl(this,ID_TEXTCTRL34,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL34"));
	if ( 0 ) TextCtrl34->SetMaxLength(0);
	StaticText37 = new wxStaticText(this,ID_STATICTEXT37,_("Label"),wxDefaultPosition,wxDefaultSize,0,_("ID_STATICTEXT37"));
	TextCtrl35 = new wxTextCtrl(this,ID_TEXTCTRL35,_("Text"),wxDefaultPosition,wxSize(75,-1),0,wxDefaultValidator,_("ID_TEXTCTRL35"));
	if ( 0 ) TextCtrl35->SetMaxLength(0);
	FlexGridSizer7->Add(StaticText30,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl28,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText31,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl29,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText32,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl30,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText33,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl31,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText34,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl32,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText35,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl33,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText36,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl34,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	FlexGridSizer7->Add(StaticText37,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	FlexGridSizer7->Add(TextCtrl35,0,wxLEFT|wxRIGHT|wxALIGN_LEFT|wxALIGN_TOP,5);
	StaticBoxSizer8->Add(FlexGridSizer7,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer2,0,wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_TOP,5);
	BoxSizer3->Add(BoxSizer10,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer7,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer3->Add(StaticBoxSizer8,0,wxRIGHT|wxTOP|wxBOTTOM|wxALIGN_LEFT|wxALIGN_TOP,5);
	BoxSizer1->Add(BoxSizer2,0,wxALL|wxALIGN_LEFT|wxALIGN_TOP,0);
	BoxSizer1->Add(BoxSizer3,0,wxALL|wxALIGN_CENTER|wxEXPAND,0);
	this->SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	//*)
}

editelevator::~editelevator()
{
}


void editelevator::OnInit(wxInitDialogEvent& event)
{

}
