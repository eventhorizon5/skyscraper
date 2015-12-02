/* $Id$ */

/*
	Skyscraper 1.10 Alpha - Object Information Dialog
	Copyright (C)2003-2015 Ryan Thoryk
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

#ifndef OBJECTINFO_H
#define OBJECTINFO_H

//(*Headers(ObjectInfo)
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
//*)

namespace Skyscraper {

class ObjectInfo: public wxDialog
{
	public:

		ObjectInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ObjectInfo();

		//(*Declarations(ObjectInfo)
		wxTextCtrl* tType;
		wxButton* bReset;
		wxTextCtrl* tScriptCommand;
		wxTextCtrl* tParent;
		wxTextCtrl* tScriptCommand2;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
		wxTextCtrl* tNumber;
		wxTreeCtrl* ObjectTree;
		wxStaticText* StaticText3;
		wxTextCtrl* tContext;
		wxTextCtrl* tLineNum;
		wxStaticLine* StaticLine1;
		wxStaticText* StaticText8;
		wxStaticText* StaticText12;
		wxTextCtrl* tParentType;
		wxStaticText* StaticText7;
		wxButton* bSave;
		wxTextCtrl* tPermanent;
		wxTextCtrl* tName;
		wxButton* bViewScript;
		wxStaticText* StaticText4;
		wxButton* bDelete;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxButton* bCreate;
		wxStaticText* StaticText6;
		wxButton* bOK;
		wxTextCtrl* tParentName;
		wxStaticText* StaticText9;
		wxTextCtrl* tIncludeFile;
		wxStaticText* StaticText11;
		wxButton* bMove;
		//*)
		void Loop();
		void PopulateTree();
		int oldobject;
		int oldcamobject;
		bool deleted;

	protected:

		//(*Identifiers(ObjectInfo)
		static const long ID_ObjectTree;
		static const long ID_bDelete;
		static const long ID_bMove;
		static const long ID_bCreate;
		static const long ID_bViewScript;
		static const long ID_bReset;
		static const long ID_bOK;
		static const long ID_bSave;
		static const long ID_STATICTEXT1;
		static const long ID_tNumber;
		static const long ID_STATICTEXT5;
		static const long ID_tName;
		static const long ID_STATICTEXT2;
		static const long ID_tType;
		static const long ID_STATICTEXT11;
		static const long ID_tPermanent;
		static const long ID_STATICTEXT3;
		static const long ID_tParent;
		static const long ID_STATICTEXT6;
		static const long ID_tParentName;
		static const long ID_STATICTEXT4;
		static const long ID_tParentType;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT7;
		static const long ID_tLineNum;
		static const long ID_STATICTEXT12;
		static const long ID_tIncludeFile;
		static const long ID_STATICTEXT10;
		static const long ID_tContext;
		static const long ID_STATICTEXT8;
		static const long ID_tScriptCommand;
		static const long ID_STATICTEXT9;
		static const long ID_tScriptCommand2;
		//*)
		//void OnInit();

	private:

		//(*Handlers(ObjectInfo)
		void On_bOK_Click(wxCommandEvent& event);
		void On_ObjectTree_SelectionChanged(wxTreeEvent& event);
		void On_bDelete_Click(wxCommandEvent& event);
		void On_bCreate_Click(wxCommandEvent& event);
		void On_bModify_Click(wxCommandEvent& event);
		void On_bSave_Click(wxCommandEvent& event);
		void On_bViewScript_Click(wxCommandEvent& event);
		void On_bMove_Click(wxCommandEvent& event);
		void On_bReset_Click(wxCommandEvent& event);
		//*)
		void AddChildren(SBS::Object *parent, const wxTreeItemId& treeparent);
		bool changed;
		int lastcount;
		SBS::SBS *Simcore;

		DECLARE_EVENT_TABLE()
};

class TreeItemData : public wxTreeItemData
{
public:
    TreeItemData(const wxString& desc) : m_desc(desc) { }
    const wxChar *GetDesc() const { return m_desc.c_str(); }

private:
    wxString m_desc;
};

}

#endif
