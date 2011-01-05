/* $Id$ */

/*
	Skyscraper 1.8 Alpha - Object Information Dialog
	Copyright (C)2003-2011 Ryan Thoryk
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

//(*InternalHeaders(ObjectInfo)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "debugpanel.h"
#include "globals.h"
#include "sbs.h"
#include "objectinfo.h"

extern SBS *Simcore; //external pointer to the SBS engine

//(*IdInit(ObjectInfo)
const long ObjectInfo::ID_ObjectTree = wxNewId();
const long ObjectInfo::ID_bDelete = wxNewId();
const long ObjectInfo::ID_bOK = wxNewId();
const long ObjectInfo::ID_STATICTEXT1 = wxNewId();
const long ObjectInfo::ID_tNumber = wxNewId();
const long ObjectInfo::ID_STATICTEXT5 = wxNewId();
const long ObjectInfo::ID_tName = wxNewId();
const long ObjectInfo::ID_STATICTEXT2 = wxNewId();
const long ObjectInfo::ID_tType = wxNewId();
const long ObjectInfo::ID_STATICTEXT11 = wxNewId();
const long ObjectInfo::ID_tPermanent = wxNewId();
const long ObjectInfo::ID_STATICTEXT3 = wxNewId();
const long ObjectInfo::ID_tParent = wxNewId();
const long ObjectInfo::ID_STATICTEXT6 = wxNewId();
const long ObjectInfo::ID_tParentName = wxNewId();
const long ObjectInfo::ID_STATICTEXT4 = wxNewId();
const long ObjectInfo::ID_tParentType = wxNewId();
const long ObjectInfo::ID_STATICLINE1 = wxNewId();
const long ObjectInfo::ID_STATICTEXT7 = wxNewId();
const long ObjectInfo::ID_tLineNum = wxNewId();
const long ObjectInfo::ID_STATICTEXT10 = wxNewId();
const long ObjectInfo::ID_tContext = wxNewId();
const long ObjectInfo::ID_STATICTEXT8 = wxNewId();
const long ObjectInfo::ID_tScriptCommand = wxNewId();
const long ObjectInfo::ID_STATICTEXT9 = wxNewId();
const long ObjectInfo::ID_tScriptCommand2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ObjectInfo,wxDialog)
	//(*EventTable(ObjectInfo)
	//*)
END_EVENT_TABLE()

ObjectInfo::ObjectInfo(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(ObjectInfo)
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer5;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;
	
	Create(parent, wxID_ANY, _("Object Info"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	ObjectTree = new wxTreeCtrl(this, ID_ObjectTree, wxDefaultPosition, wxSize(300,350), wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_ObjectTree"));
	FlexGridSizer4->Add(ObjectTree, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bDelete = new wxButton(this, ID_bDelete, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDelete"));
	BoxSizer2->Add(bDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer2->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer4->Add(BoxSizer2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer4, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Number:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tNumber = new wxTextCtrl(this, ID_tNumber, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tNumber"));
	FlexGridSizer1->Add(tNumber, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tName = new wxTextCtrl(this, ID_tName, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tName"));
	FlexGridSizer1->Add(tName, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tType = new wxTextCtrl(this, ID_tType, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tType"));
	FlexGridSizer1->Add(tType, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("Permanent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	FlexGridSizer1->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tPermanent = new wxTextCtrl(this, ID_tPermanent, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tPermanent"));
	FlexGridSizer1->Add(tPermanent, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Parent:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParent = new wxTextCtrl(this, ID_tParent, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tParent"));
	FlexGridSizer1->Add(tParent, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Parent Name:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParentName = new wxTextCtrl(this, ID_tParentName, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tParentName"));
	FlexGridSizer1->Add(tParentName, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Parent Type:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tParentType = new wxTextCtrl(this, ID_tParentType, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tParentType"));
	FlexGridSizer1->Add(tParentType, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
	FlexGridSizer2->Add(StaticLine1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer3 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Object created on line:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	FlexGridSizer3->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tLineNum = new wxTextCtrl(this, ID_tLineNum, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tLineNum"));
	FlexGridSizer3->Add(tLineNum, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer3, 1, wxTOP|wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Script context:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer5->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tContext = new wxTextCtrl(this, ID_tContext, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tContext"));
	FlexGridSizer5->Add(tContext, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer5, 1, wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Script Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer2->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tScriptCommand = new wxTextCtrl(this, ID_tScriptCommand, wxEmptyString, wxDefaultPosition, wxSize(300,40), wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE|wxTE_WORDWRAP, wxDefaultValidator, _T("ID_tScriptCommand"));
	FlexGridSizer2->Add(tScriptCommand, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("Processed Script Command:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer2->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tScriptCommand2 = new wxTextCtrl(this, ID_tScriptCommand2, wxEmptyString, wxDefaultPosition, wxSize(-1,40), wxTE_MULTILINE|wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tScriptCommand2"));
	FlexGridSizer2->Add(tScriptCommand2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Center();
	
	Connect(ID_ObjectTree,wxEVT_COMMAND_TREE_SEL_CHANGED,(wxObjectEventFunction)&ObjectInfo::On_ObjectTree_SelectionChanged);
	Connect(ID_bDelete,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bDelete_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bOK_Click);
	//*)
	//OnInit();
	oldobject = -1;
	oldcamobject = -1;
	changed = false;
	populated = false;
}

ObjectInfo::~ObjectInfo()
{
	//(*Destroy(ObjectInfo)
	//*)
}


void ObjectInfo::On_bOK_Click(wxCommandEvent& event)
{
	this->Hide();
}

void ObjectInfo::Loop()
{
	int number;
	if (changed == false)
	{
		int newobject = Simcore->camera->GetClickedObjectNumber();;
		if (newobject != oldcamobject)
		{
			number = Simcore->camera->GetClickedObjectNumber();
			oldcamobject = Simcore->camera->GetClickedObjectNumber();
		}
		else
			number = oldobject;
	}
	else
	{
		TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(ObjectTree->GetSelection());
		wxString num;
		num = data->GetDesc();
		number = atoi(num.ToAscii());
		changed = false;
	}
	if (number == oldobject)
		return;

	oldobject = number;
	Object *object = Simcore->GetObject(number);
	tNumber->SetValue(wxVariant(number).GetString());
	tLineNum->Clear();
	tScriptCommand->Clear();
	tScriptCommand2->Clear();
	tContext->Clear();
	tName->Clear();
	tType->Clear();
	tPermanent->Clear();
	tParent->Clear();
	tParentName->Clear();
	tParentType->Clear();

	if (object)
	{
		tName->SetValue(wxString::FromAscii(object->GetName()));
		tType->SetValue(wxString::FromAscii(object->GetType()));
		tLineNum->SetValue(wxVariant(object->linenum).GetString());
		tScriptCommand->SetValue(wxString::FromAscii(object->command.c_str()));
		tScriptCommand2->SetValue(wxString::FromAscii(object->command_processed.c_str()));
		tContext->SetValue(wxString::FromAscii(object->context.c_str()));
		tPermanent->SetValue(wxString::FromAscii(BoolToString(object->IsPermanent())));

		Object *parent = object->GetParent();
		if (parent)
		{
			tParent->SetValue(wxVariant(parent->GetNumber()).GetString());
			tParentName->SetValue(wxString::FromAscii(parent->GetName()));
			tParentType->SetValue(wxString::FromAscii(parent->GetType()));
		}
	}
}

void ObjectInfo::PopulateTree()
{
	if (populated == true)
		return;

	//populate object tree
	wxTreeItemId id = ObjectTree->AddRoot(wxString::FromAscii(Simcore->object->GetName()), -1, -1, new TreeItemData(wxVariant(Simcore->object->GetNumber()).GetString()));

	//add child objects
	AddChildren(Simcore->object, id);

	populated = true;
}

void ObjectInfo::AddChildren(Object *parent, const wxTreeItemId& treeparent)
{
	if (!parent)
		return;

	//add child objects of given SBS object to tree
	if (parent->GetChildrenCount() > 0)
	{
		for (int i = 0; i < parent->GetChildrenCount(); i++)
		{
			if (parent->GetChild(i))
			{
				//add child object
				wxTreeItemId id = ObjectTree->AppendItem(treeparent, wxString::FromAscii(parent->GetChild(i)->GetName()), -1, -1, new TreeItemData(wxVariant(parent->GetChild(i)->GetNumber()).GetString()));

				//if child object has children, call recursively to add those
				if (parent->GetChild(i)->GetChildrenCount() > 0)
					AddChildren(parent->GetChild(i), id);
			}
		}
	}
}

void ObjectInfo::On_ObjectTree_SelectionChanged(wxTreeEvent& event)
{
	changed = true;
}

void ObjectInfo::On_bDelete_Click(wxCommandEvent& event)
{
	TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(ObjectTree->GetSelection());
	wxString num;
	num = data->GetDesc();
	int number = atoi(num.ToAscii());

	//call SBS to delete object
	if (Simcore->DeleteObject(number))
	{
		//delete object from tree
		ObjectTree->Delete(ObjectTree->GetSelection());
	}
}
