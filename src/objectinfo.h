/* $Id$ */

/*
	Skyscraper 1.7 Alpha - Object Information Dialog
	Copyright (C)2003-2010 Ryan Thoryk
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
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class ObjectInfo: public wxDialog
{
	public:

		ObjectInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ObjectInfo();

		//(*Declarations(ObjectInfo)
		wxStaticText* StaticText9;
		wxButton* bOK;
		wxTreeCtrl* ObjectTree;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxStaticText* StaticText8;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxTextCtrl* tType;
		wxTextCtrl* tNumber;
		wxTextCtrl* tName;
		wxTextCtrl* tLineNum;
		wxStaticText* StaticText5;
		wxStaticText* StaticText7;
		wxTextCtrl* tScriptCommand;
		wxTextCtrl* tScriptCommand2;
		wxTextCtrl* tParentType;
		wxStaticLine* StaticLine1;
		wxTextCtrl* tParent;
		wxTextCtrl* tParentName;
		wxStaticText* StaticText4;
		//*)
		void Loop();
		void PopulateTree();
		int oldobject;
		int oldcamobject;

	protected:

		//(*Identifiers(ObjectInfo)
		static const long ID_ObjectTree;
		static const long ID_bOK;
		static const long ID_STATICTEXT1;
		static const long ID_tNumber;
		static const long ID_STATICTEXT5;
		static const long ID_tName;
		static const long ID_STATICTEXT2;
		static const long ID_tType;
		static const long ID_STATICTEXT3;
		static const long ID_tParent;
		static const long ID_STATICTEXT6;
		static const long ID_tParentName;
		static const long ID_STATICTEXT4;
		static const long ID_tParentType;
		static const long ID_STATICLINE1;
		static const long ID_STATICTEXT7;
		static const long ID_tLineNum;
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
		//*)
		void AddChildren(Object *parent, const wxTreeItemId& treeparent);
		bool changed;
		bool populated;

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

#endif
