/*
	Skyscraper 1.12 Alpha - Engine Manager Dialog
	Copyright (C)2003-2023 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@thoryk.com

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

#ifndef ENGINEMANAGER_H
#define ENGINEMANAGER_H

//(*Headers(EngineManager)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class EngineManager: public wxDialog
{
	public:

		EngineManager(DebugPanel* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~EngineManager();

		//(*Declarations(EngineManager)
		wxButton* bLoad;
		wxButton* bMove;
		wxButton* bOk;
		wxButton* bReload;
		wxButton* bSetActive;
		wxButton* bShutdown;
		wxCheckBox* chkCLoads;
		wxCheckBox* chkRender;
		wxListBox* EngineList;
		wxStaticLine* StaticLine2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxTextCtrl* tActive;
		wxTextCtrl* tBoundsMax;
		wxTextCtrl* tBoundsMin;
		wxTextCtrl* tPosition;
		wxTextCtrl* tState;
		//*)
		void Loop();

	protected:

		//(*Identifiers(EngineManager)
		static const long ID_EngineList;
		static const long ID_STATICTEXT4;
		static const long ID_tPosition;
		static const long ID_STATICTEXT3;
		static const long ID_tBoundsMin;
		static const long ID_STATICTEXT5;
		static const long ID_tBoundsMax;
		static const long ID_STATICTEXT1;
		static const long ID_tActive;
		static const long ID_STATICTEXT2;
		static const long ID_tState;
		static const long ID_STATICLINE2;
		static const long ID_CLoads;
		static const long ID_chkRender;
		static const long ID_bSetActive;
		static const long ID_bReload;
		static const long ID_bMove;
		static const long ID_bLoad;
		static const long ID_bShutdown;
		static const long ID_bOk;
		//*)
		void OnInit();

	private:

		//(*Handlers(EngineManager)
		void On_bSetActive_Click(wxCommandEvent& event);
		void On_bLoad_Click(wxCommandEvent& event);
		void On_bReload_Click(wxCommandEvent& event);
		void On_bShutdown_Click(wxCommandEvent& event);
		void On_chkCLoads_Click(wxCommandEvent& event);
		void On_chkRender_Click(wxCommandEvent& event);
		void On_bOk_Click(wxCommandEvent& event);
		void On_bMove_Click(wxCommandEvent& event);
		//*)

		EngineContext *engine;
		DebugPanel *panel;
		LoadDialog *loader;
		MoveObject *moveobject;
		int lastcount;

		DECLARE_EVENT_TABLE()
};

}

#endif

