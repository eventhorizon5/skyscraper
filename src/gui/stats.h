/*
	Skyscraper 2.0 Alpha - Statistics Dialog
	Copyright (C)2004-2024 Ryan Thoryk
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

#ifndef STATS_H
#define STATS_H

//(*Headers(Stats)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class Stats: public wxDialog
{
	public:

		Stats(DebugPanel* parent,wxWindowID id=wxID_ANY);
		virtual ~Stats();

		//(*Declarations(Stats)
		wxButton* bOK;
		wxCheckBox* chkGlobal;
		wxStaticText* StaticText10;
		wxStaticText* StaticText11;
		wxStaticText* StaticText12;
		wxStaticText* StaticText13;
		wxStaticText* StaticText14;
		wxStaticText* StaticText15;
		wxStaticText* StaticText16;
		wxStaticText* StaticText17;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxStaticText* StaticText20;
		wxStaticText* StaticText21;
		wxStaticText* StaticText22;
		wxStaticText* StaticText23;
		wxStaticText* StaticText24;
		wxStaticText* StaticText25;
		wxStaticText* StaticText26;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxStaticText* StaticText9;
		wxTextCtrl* tActions;
		wxTextCtrl* tBasements;
		wxTextCtrl* tDescription;
		wxTextCtrl* tDesigner;
		wxTextCtrl* tElevators;
		wxTextCtrl* tFloors;
		wxTextCtrl* tFrontendVersion;
		wxTextCtrl* tInElevator;
		wxTextCtrl* tInShaft;
		wxTextCtrl* tInStairwell;
		wxTextCtrl* tLocation;
		wxTextCtrl* tMeshes;
		wxTextCtrl* tName;
		wxTextCtrl* tObjects;
		wxTextCtrl* tPlatform;
		wxTextCtrl* tPolygons;
		wxTextCtrl* tRenderer;
		wxTextCtrl* tRunningTime;
		wxTextCtrl* tSBSVersion;
		wxTextCtrl* tShafts;
		wxTextCtrl* tSounds;
		wxTextCtrl* tStairs;
		wxTextCtrl* tTextures;
		wxTextCtrl* tVersion;
		wxTextCtrl* tWalls;
		//*)
		void Loop();

	protected:

		//(*Identifiers(Stats)
		static const wxWindowID ID_STATICTEXT18;
		static const wxWindowID ID_tName;
		static const wxWindowID ID_STATICTEXT17;
		static const wxWindowID ID_tDesigner;
		static const wxWindowID ID_STATICTEXT16;
		static const wxWindowID ID_tLocation;
		static const wxWindowID ID_STATICTEXT15;
		static const wxWindowID ID_tDescription;
		static const wxWindowID ID_STATICTEXT14;
		static const wxWindowID ID_tVersion;
		static const wxWindowID ID_STATICTEXT5;
		static const wxWindowID ID_tFloors;
		static const wxWindowID ID_STATICTEXT6;
		static const wxWindowID ID_tBasements;
		static const wxWindowID ID_STATICTEXT7;
		static const wxWindowID ID_tElevators;
		static const wxWindowID ID_STATICTEXT8;
		static const wxWindowID ID_tShafts;
		static const wxWindowID ID_STATICTEXT9;
		static const wxWindowID ID_tStairs;
		static const wxWindowID ID_STATICTEXT1;
		static const wxWindowID ID_tMeshes;
		static const wxWindowID ID_STATICTEXT3;
		static const wxWindowID ID_tTextures;
		static const wxWindowID ID_STATICTEXT4;
		static const wxWindowID ID_tActions;
		static const wxWindowID ID_STATICTEXT21;
		static const wxWindowID ID_tSounds;
		static const wxWindowID ID_STATICTEXT22;
		static const wxWindowID ID_tObjects;
		static const wxWindowID ID_STATICTEXT24;
		static const wxWindowID ID_tWalls;
		static const wxWindowID ID_STATICTEXT25;
		static const wxWindowID ID_tPolygons;
		static const wxWindowID ID_STATICTEXT10;
		static const wxWindowID ID_tInStairwell;
		static const wxWindowID ID_STATICTEXT11;
		static const wxWindowID ID_tInElevator;
		static const wxWindowID ID_STATICTEXT12;
		static const wxWindowID ID_tInShaft;
		static const wxWindowID ID_STATICTEXT13;
		static const wxWindowID ID_tRunningTime;
		static const wxWindowID ID_STATICTEXT20;
		static const wxWindowID ID_tFrontendVersion;
		static const wxWindowID ID_STATICTEXT23;
		static const wxWindowID ID_tSBSVersion;
		static const wxWindowID ID_STATICTEXT26;
		static const wxWindowID ID_tPlatform;
		static const wxWindowID ID_STATICTEXT2;
		static const wxWindowID ID_tRenderer;
		static const wxWindowID ID_bOK;
		static const wxWindowID ID_chkGlobal;
		//*)
		void OnInit();

	private:

		//(*Handlers(Stats)
		void On_bOK_Click(wxCommandEvent& event);
		//*)

		SBS::SBS *Simcore;
		DebugPanel *panel;

		DECLARE_EVENT_TABLE()
};

}

#endif
