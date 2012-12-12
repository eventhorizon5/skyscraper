/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Statistics Dialog
	Copyright (C)2004-2012 Ryan Thoryk
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

#ifndef STATS_H
#define STATS_H

//(*Headers(Stats)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

class Stats: public wxDialog
{
	public:

		Stats(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~Stats();

		//(*Declarations(Stats)
		wxTextCtrl* tInStairwell;
		wxStaticText* StaticText24;
		wxTextCtrl* tTextures;
		wxStaticText* StaticText22;
		wxStaticText* StaticText21;
		wxStaticText* StaticText13;
		wxTextCtrl* tSounds;
		wxStaticText* StaticText14;
		wxStaticText* StaticText15;
		wxTextCtrl* tSBSVersion;
		wxTextCtrl* tStartTime;
		wxStaticText* StaticText17;
		wxTextCtrl* tElevators;
		wxTextCtrl* tVersion;
		wxTextCtrl* tStairs;
		wxStaticText* StaticText20;
		wxTextCtrl* tDesigner;
		wxStaticText* StaticText18;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
		wxStaticText* StaticText16;
		wxTextCtrl* tDescription;
		wxStaticText* StaticText3;
		wxTextCtrl* tObjects;
		wxTextCtrl* tInShaft;
		wxStaticText* StaticText23;
		wxTextCtrl* tPolygons;
		wxTextCtrl* tRunningTime;
		wxTextCtrl* tActions;
		wxTextCtrl* tBasements;
		wxStaticText* StaticText8;
		wxStaticText* StaticText12;
		wxTextCtrl* tFrontendVersion;
		wxStaticText* StaticText7;
		wxTextCtrl* tFloors;
		wxTextCtrl* tInElevator;
		wxTextCtrl* tName;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxTextCtrl* tLocation;
		wxTextCtrl* tWalls;
		wxTextCtrl* tMeshes;
		wxTextCtrl* tPlatform;
		wxStaticText* StaticText26;
		wxStaticText* StaticText6;
		wxTextCtrl* tShafts;
		wxButton* bOK;
		wxStaticText* StaticText19;
		wxStaticText* StaticText9;
		wxStaticText* StaticText11;
		wxStaticText* StaticText25;
		//*)
		void Loop();

	protected:

		//(*Identifiers(Stats)
		static const long ID_STATICTEXT18;
		static const long ID_tName;
		static const long ID_STATICTEXT17;
		static const long ID_tDesigner;
		static const long ID_STATICTEXT16;
		static const long ID_tLocation;
		static const long ID_STATICTEXT15;
		static const long ID_tDescription;
		static const long ID_STATICTEXT14;
		static const long ID_tVersion;
		static const long ID_STATICTEXT5;
		static const long ID_tFloors;
		static const long ID_STATICTEXT6;
		static const long ID_tBasements;
		static const long ID_STATICTEXT7;
		static const long ID_tElevators;
		static const long ID_STATICTEXT8;
		static const long ID_tShafts;
		static const long ID_STATICTEXT9;
		static const long ID_tStairs;
		static const long ID_STATICTEXT1;
		static const long ID_tMeshes;
		static const long ID_STATICTEXT3;
		static const long ID_tTextures;
		static const long ID_STATICTEXT4;
		static const long ID_tActions;
		static const long ID_STATICTEXT21;
		static const long ID_tSounds;
		static const long ID_STATICTEXT22;
		static const long ID_tObjects;
		static const long ID_STATICTEXT24;
		static const long ID_tWalls;
		static const long ID_STATICTEXT25;
		static const long ID_tPolygons;
		static const long ID_STATICTEXT10;
		static const long ID_tInStairwell;
		static const long ID_STATICTEXT11;
		static const long ID_tInElevator;
		static const long ID_STATICTEXT12;
		static const long ID_tInShaft;
		static const long ID_STATICTEXT13;
		static const long ID_tRunningTime;
		static const long ID_STATICTEXT19;
		static const long ID_tStartTime;
		static const long ID_STATICTEXT20;
		static const long ID_tFrontendVersion;
		static const long ID_STATICTEXT23;
		static const long ID_tSBSVersion;
		static const long ID_STATICTEXT26;
		static const long ID_tPlatform;
		static const long ID_bOK;
		//*)
		void OnInit();

	private:

		//(*Handlers(Stats)
		void On_bOK_Click(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
