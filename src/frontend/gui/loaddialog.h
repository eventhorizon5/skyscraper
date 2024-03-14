/*
	Skyscraper 1.12 Alpha - Load Building Dialog
	Copyright (C)2003-2024 Ryan Thoryk
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

#ifndef LOADDIALOG_H
#define LOADDIALOG_H

//(*Headers(LoadDialog)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class LoadDialog: public wxDialog
{
	public:

		LoadDialog(DebugPanel *root, wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LoadDialog();

		//(*Declarations(LoadDialog)
		wxButton* bLoad;
		wxButton* bSelect;
		wxCheckBox* chkCutBuildings;
		wxCheckBox* chkCutExternal;
		wxCheckBox* chkCutFloors;
		wxCheckBox* chkCutLandscape;
		wxStaticLine* StaticLine1;
		wxStaticLine* StaticLine2;
		wxStaticLine* StaticLine3;
		wxStaticLine* StaticLine4;
		wxStaticText* StaticText10;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* tFilename;
		wxTextCtrl* tMaxX;
		wxTextCtrl* tMaxY;
		wxTextCtrl* tMaxZ;
		wxTextCtrl* tMinX;
		wxTextCtrl* tMinY;
		wxTextCtrl* tMinZ;
		wxTextCtrl* tPosX;
		wxTextCtrl* tPosY;
		wxTextCtrl* tPosZ;
		wxTextCtrl* tRotation;
		//*)

	protected:

		//(*Identifiers(LoadDialog)
		static const long ID_tFilename;
		static const long ID_bSelect;
		static const long ID_STATICTEXT1;
		static const long ID_PosX;
		static const long ID_STATICTEXT4;
		static const long ID_tPosY;
		static const long ID_STATICTEXT5;
		static const long ID_tPosZ;
		static const long ID_STATICLINE3;
		static const long ID_STATICLINE4;
		static const long ID_STATICTEXT2;
		static const long ID_tRotation;
		static const long ID_chkCutLandscape;
		static const long ID_chkCutBuildings;
		static const long ID_chkCutExternal;
		static const long ID_chkCutFloors;
		static const long ID_STATICTEXT3;
		static const long ID_tMinX;
		static const long ID_STATICTEXT6;
		static const long ID_tMinY;
		static const long ID_STATICTEXT7;
		static const long ID_tMinZ;
		static const long ID_STATICLINE1;
		static const long ID_STATICLINE2;
		static const long ID_STATICTEXT8;
		static const long ID_tMaxX;
		static const long ID_STATICTEXT9;
		static const long ID_tMaxY;
		static const long ID_STATICTEXT10;
		static const long ID_tMaxZ;
		static const long ID_bLoad;
		//*)

	private:

		//(*Handlers(LoadDialog)
		void On_bSelect_Click(wxCommandEvent& event);
		void On_bLoad_Click(wxCommandEvent& event);
		//*)

		DebugPanel *panel;

		DECLARE_EVENT_TABLE()
};

}

#endif

