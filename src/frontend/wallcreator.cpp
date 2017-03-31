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

#include "wallcreator.h"

//(*InternalHeaders(WallCreator)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(WallCreator)
const long WallCreator::ID_STATICTEXT1 = wxNewId();
const long WallCreator::ID_txtName = wxNewId();
const long WallCreator::ID_STATICTEXT2 = wxNewId();
const long WallCreator::ID_txtTexture = wxNewId();
const long WallCreator::ID_STATICTEXT3 = wxNewId();
const long WallCreator::ID_txtThickness = wxNewId();
const long WallCreator::ID_SpinThickness = wxNewId();
const long WallCreator::ID_STATICTEXT4 = wxNewId();
const long WallCreator::ID_TEXTCTRL1 = wxNewId();
const long WallCreator::ID_SpinX1 = wxNewId();
const long WallCreator::ID_STATICTEXT5 = wxNewId();
const long WallCreator::ID_txtZ1 = wxNewId();
const long WallCreator::ID_SpinZ1 = wxNewId();
const long WallCreator::ID_STATICTEXT6 = wxNewId();
const long WallCreator::ID_txtX2 = wxNewId();
const long WallCreator::ID_SpinX2 = wxNewId();
const long WallCreator::ID_STATICTEXT7 = wxNewId();
const long WallCreator::ID_txtZ2 = wxNewId();
const long WallCreator::ID_SpinZ2 = wxNewId();
const long WallCreator::ID_STATICTEXT8 = wxNewId();
const long WallCreator::ID_txtHeight1 = wxNewId();
const long WallCreator::ID_SpinHeight1 = wxNewId();
const long WallCreator::ID_STATICTEXT9 = wxNewId();
const long WallCreator::ID_txtHeight2 = wxNewId();
const long WallCreator::ID_SpinHeight2 = wxNewId();
const long WallCreator::ID_STATICTEXT10 = wxNewId();
const long WallCreator::ID_txtVoffset1 = wxNewId();
const long WallCreator::ID_SpinVoffset1 = wxNewId();
const long WallCreator::ID_STATICTEXT11 = wxNewId();
const long WallCreator::ID_txtVoffset2 = wxNewId();
const long WallCreator::ID_SpinVoffset2 = wxNewId();
const long WallCreator::ID_STATICTEXT12 = wxNewId();
const long WallCreator::ID_txtTW = wxNewId();
const long WallCreator::ID_SpinTW = wxNewId();
const long WallCreator::ID_STATICTEXT13 = wxNewId();
const long WallCreator::ID_txtTH = wxNewId();
const long WallCreator::ID_SpinTH = wxNewId();
const long WallCreator::ID_toggleAutosize = wxNewId();
const long WallCreator::ID_bOK = wxNewId();
//*)

BEGIN_EVENT_TABLE(WallCreator,wxDialog)
	//(*EventTable(WallCreator)
	//*)
END_EVENT_TABLE()

WallCreator::WallCreator(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(WallCreator)
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, _("Wall Creator"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	FlexGridSizer1 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtName = new wxTextCtrl(this, ID_txtName, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtName"));
	FlexGridSizer1->Add(txtName, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Texture:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtTexture = new wxTextCtrl(this, ID_txtTexture, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtTexture"));
	FlexGridSizer1->Add(txtTexture, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Thickness:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtThickness = new wxTextCtrl(this, ID_txtThickness, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtThickness"));
	FlexGridSizer1->Add(txtThickness, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinThickness = new wxSpinButton(this, ID_SpinThickness, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinThickness"));
	SpinThickness->SetRange(-1000, 1000);
	FlexGridSizer1->Add(SpinThickness, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("X1:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtX1 = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(txtX1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinX1 = new wxSpinButton(this, ID_SpinX1, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinX1"));
	SpinX1->SetRange(-1000, 1000);
	FlexGridSizer1->Add(SpinX1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Z1:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtZ1 = new wxTextCtrl(this, ID_txtZ1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtZ1"));
	FlexGridSizer1->Add(txtZ1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinZ1 = new wxSpinButton(this, ID_SpinZ1, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinZ1"));
	SpinZ1->SetRange(0, 100);
	FlexGridSizer1->Add(SpinZ1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("X2:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtX2 = new wxTextCtrl(this, ID_txtX2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtX2"));
	FlexGridSizer1->Add(txtX2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinX2 = new wxSpinButton(this, ID_SpinX2, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinX2"));
	SpinX2->SetRange(0, 100);
	FlexGridSizer1->Add(SpinX2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Z2:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer1->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtZ2 = new wxTextCtrl(this, ID_txtZ2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtZ2"));
	FlexGridSizer1->Add(txtZ2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinZ2 = new wxSpinButton(this, ID_SpinZ2, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinZ2"));
	SpinZ2->SetRange(0, 100);
	FlexGridSizer1->Add(SpinZ2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Height 1:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer1->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtHeight1 = new wxTextCtrl(this, ID_txtHeight1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtHeight1"));
	FlexGridSizer1->Add(txtHeight1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinHeight1 = new wxSpinButton(this, ID_SpinHeight1, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinHeight1"));
	SpinHeight1->SetRange(0, 100);
	FlexGridSizer1->Add(SpinHeight1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Height 2:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer1->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtHeight2 = new wxTextCtrl(this, ID_txtHeight2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtHeight2"));
	FlexGridSizer1->Add(txtHeight2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinHeight2 = new wxSpinButton(this, ID_SpinHeight2, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinHeight2"));
	SpinHeight2->SetRange(0, 100);
	FlexGridSizer1->Add(SpinHeight2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Voffset1:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer1->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtVoffset1 = new wxTextCtrl(this, ID_txtVoffset1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtVoffset1"));
	FlexGridSizer1->Add(txtVoffset1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinVoffset1 = new wxSpinButton(this, ID_SpinVoffset1, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinVoffset1"));
	SpinVoffset1->SetRange(0, 100);
	FlexGridSizer1->Add(SpinVoffset1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Voffset2:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtVoffset2 = new wxTextCtrl(this, ID_txtVoffset2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtVoffset2"));
	FlexGridSizer1->Add(txtVoffset2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinVoffset2 = new wxSpinButton(this, ID_SpinVoffset2, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinVoffset2"));
	SpinVoffset2->SetRange(0, 100);
	FlexGridSizer1->Add(SpinVoffset2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("TW:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer1->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtTW = new wxTextCtrl(this, ID_txtTW, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtTW"));
	FlexGridSizer1->Add(txtTW, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinTW = new wxSpinButton(this, ID_SpinTW, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinTW"));
	SpinTW->SetRange(0, 100);
	FlexGridSizer1->Add(SpinTW, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText13 = new wxStaticText(this, ID_STATICTEXT13, _("TH:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	FlexGridSizer1->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	txtTH = new wxTextCtrl(this, ID_txtTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_txtTH"));
	FlexGridSizer1->Add(txtTH, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SpinTH = new wxSpinButton(this, ID_SpinTH, wxDefaultPosition, wxDefaultSize, wxSP_VERTICAL|wxSP_ARROW_KEYS, _T("ID_SpinTH"));
	SpinTH->SetRange(0, 100);
	FlexGridSizer1->Add(SpinTH, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	toggleAutosize = new wxToggleButton(this, ID_toggleAutosize, _("Autosize"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_toggleAutosize"));
	FlexGridSizer1->Add(toggleAutosize, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	FlexGridSizer1->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer1->Add(0,0,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(FlexGridSizer1);
	FlexGridSizer1->Fit(this);
	FlexGridSizer1->SetSizeHints(this);
	//*)
}

WallCreator::~WallCreator()
{
	//(*Destroy(WallCreator)
	//*)
}

                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                            