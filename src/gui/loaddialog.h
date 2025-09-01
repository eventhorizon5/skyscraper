/*
	Skyscraper 2.1 - Load Building Dialog
	Copyright (C)2003-2025 Ryan Thoryk
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
		void SetEngineParent(EngineContext *parent);

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
		wxStaticText* StaticText11;
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
		wxTextCtrl* txtParent;
		//*)

	protected:

		//(*Identifiers(LoadDialog)
		static const wxWindowID ID_tFilename;
		static const wxWindowID ID_bSelect;
		static const wxWindowID ID_STATICTEXT11;
		static const wxWindowID ID_txtParent;
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_PosX;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_tPosY;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_tPosZ;
		static const wxWindowID ID_STATICLINE3;
		static const wxWindowID ID_STATICLINE4;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_tRotation;
		static const wxWindowID ID_chkCutLandscape;
		static const wxWindowID ID_chkCutBuildings;
		static const wxWindowID ID_chkCutExternal;
		static const wxWindowID ID_chkCutFloors;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_tMinX;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_tMinY;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_tMinZ;
		static const wxWindowID ID_STATICLINE1;
		static const wxWindowID ID_STATICLINE2;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_tMaxX;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_tMaxY;
		static const wxWindowID ID_STATICTEXT10;
		static const wxWindowID ID_tMaxZ;
		static const wxWindowID ID_bLoad;
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

