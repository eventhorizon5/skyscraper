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

//(*InternalHeaders(ObjectInfo)
#include <wx/string.h>
#include <wx/intl.h>
//*)

#include <wx/filedlg.h>
#include "debugpanel.h"
#include "globals.h"
#include "sbs.h"
#include "skyscraper.h"
#include "objectinfo.h"
#include "createobject.h"
#include "parameterviewer.h"
#include "textwindow.h"
#include "moveobject.h"

using namespace SBS;

namespace Skyscraper {

CreateObject *createobject;
ParameterViewer *modifyobject;
MoveObject *moveobject;

//(*IdInit(ObjectInfo)
const long ObjectInfo::ID_ObjectTree = wxNewId();
const long ObjectInfo::ID_bDelete = wxNewId();
const long ObjectInfo::ID_bMove = wxNewId();
const long ObjectInfo::ID_bCreate = wxNewId();
const long ObjectInfo::ID_bViewScript = wxNewId();
const long ObjectInfo::ID_bReset = wxNewId();
const long ObjectInfo::ID_bOK = wxNewId();
const long ObjectInfo::ID_bSave = wxNewId();
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
const long ObjectInfo::ID_STATICTEXT12 = wxNewId();
const long ObjectInfo::ID_tIncludeFile = wxNewId();
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
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer2;
	wxFlexGridSizer* FlexGridSizer4;
	wxFlexGridSizer* FlexGridSizer6;
	wxFlexGridSizer* FlexGridSizer3;
	wxBoxSizer* BoxSizer1;
	wxFlexGridSizer* FlexGridSizer5;

	Create(parent, wxID_ANY, _("Object Manager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	FlexGridSizer4 = new wxFlexGridSizer(0, 1, 0, 0);
	ObjectTree = new wxTreeCtrl(this, ID_ObjectTree, wxDefaultPosition, wxSize(300,350), wxTR_DEFAULT_STYLE|wxSUNKEN_BORDER|wxVSCROLL, wxDefaultValidator, _T("ID_ObjectTree"));
	FlexGridSizer4->Add(ObjectTree, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	bDelete = new wxButton(this, ID_bDelete, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bDelete"));
	bDelete->SetToolTip(_("Delete selected object"));
	BoxSizer2->Add(bDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bMove = new wxButton(this, ID_bMove, _("Move"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bMove"));
	bMove->SetToolTip(_("Move selected object"));
	BoxSizer2->Add(bMove, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bCreate = new wxButton(this, ID_bCreate, _("Create"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bCreate"));
	bCreate->SetToolTip(_("Create a new object"));
	BoxSizer2->Add(bCreate, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer4->Add(BoxSizer2, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	bViewScript = new wxButton(this, ID_bViewScript, _("View Script"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bViewScript"));
	bViewScript->SetToolTip(_("View current running script"));
	BoxSizer3->Add(bViewScript, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bReset = new wxButton(this, ID_bReset, _("Reset State"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bReset"));
	bReset->SetToolTip(_("Reset the state of the current object"));
	BoxSizer3->Add(bReset, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	bOK = new wxButton(this, ID_bOK, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bOK"));
	BoxSizer3->Add(bOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	bSave = new wxButton(this, ID_bSave, _("Save Script"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_bSave"));
	bSave->Disable();
	bSave->Hide();
	BoxSizer3->Add(bSave, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
	FlexGridSizer4->Add(BoxSizer3, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
	FlexGridSizer6 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("In included file:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	FlexGridSizer6->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tIncludeFile = new wxTextCtrl(this, ID_tIncludeFile, wxEmptyString, wxDefaultPosition, wxSize(150,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tIncludeFile"));
	FlexGridSizer6->Add(tIncludeFile, 1, wxTOP|wxBOTTOM|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer2->Add(FlexGridSizer6, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	FlexGridSizer5 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Script context:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer5->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	tContext = new wxTextCtrl(this, ID_tContext, wxEmptyString, wxDefaultPosition, wxSize(125,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_tContext"));
	FlexGridSizer5->Add(tContext, 1, wxLEFT|wxRIGHT|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
	Connect(ID_bMove,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bMove_Click);
	Connect(ID_bCreate,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bCreate_Click);
	Connect(ID_bViewScript,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bViewScript_Click);
	Connect(ID_bReset,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bReset_Click);
	Connect(ID_bOK,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bOK_Click);
	Connect(ID_bSave,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ObjectInfo::On_bSave_Click);
	//*)
	OnInit();
}

ObjectInfo::~ObjectInfo()
{
	//(*Destroy(ObjectInfo)
	//*)

	if (createobject)
		createobject->Destroy();
	createobject = 0;
	if (modifyobject)
		modifyobject->Destroy();
	modifyobject = 0;
	if (moveobject)
		moveobject->Destroy();
	moveobject = 0;
}

void ObjectInfo::OnInit()
{
	oldobject = -1;
	oldcamobject = -1;
	createobject = 0;
	changed = false;
	lastcount = 0;
	deleted = false;
	Simcore = skyscraper->GetActiveEngine()->GetSystem();
}

void ObjectInfo::On_bOK_Click(wxCommandEvent& event)
{
	this->Hide();
}

void ObjectInfo::Loop()
{
	//if active engine has changed, refresh values
	if (skyscraper->GetActiveEngine())
	{
		if (Simcore != skyscraper->GetActiveEngine()->GetSystem())
			OnInit();
	}
	else
		return;

	if (Simcore->GetObjectCount() != lastcount)
	{
		lastcount = Simcore->GetObjectCount();
		if (deleted == false)
			PopulateTree();
		deleted = false;
	}

	if (moveobject)
		moveobject->Loop();

	int number = oldobject;
	if (changed == false)
	{
		int newobject = Simcore->camera->GetClickedObjectNumber();
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
		wxTreeItemId id = ObjectTree->GetSelection();
		if (id.IsOk())
		{
			TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(id);
			wxString num;
			num = data->GetDesc();
			number = atoi(num.ToAscii());
		}
		changed = false;
	}
	if (number == oldobject)
		return;

	oldobject = number;
	Object *object = Simcore->GetObject(number);
	tNumber->SetValue(wxVariant(number).GetString());
	tLineNum->Clear();
	tIncludeFile->Clear();
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
		tName->SetValue(wxString::FromAscii(object->GetName().c_str()));
		tType->SetValue(wxString::FromAscii(object->GetType().c_str()));
		tLineNum->SetValue(wxVariant(object->linenum).GetString());
		tIncludeFile->SetValue(wxString::FromAscii(object->includefile.c_str()));
		tScriptCommand->SetValue(wxString::FromAscii(object->command.c_str()));
		tScriptCommand2->SetValue(wxString::FromAscii(object->command_processed.c_str()));
		tContext->SetValue(wxString::FromAscii(object->context.c_str()));
		tPermanent->SetValue(wxString::FromAscii(BoolToString(object->IsPermanent()).c_str()));

		Object *parent = object->GetParent();
		if (parent)
		{
			tParent->SetValue(wxVariant(parent->GetNumber()).GetString());
			tParentName->SetValue(wxString::FromAscii(parent->GetName().c_str()));
			tParentType->SetValue(wxString::FromAscii(parent->GetType().c_str()));
		}
	}
}

void ObjectInfo::PopulateTree()
{
	//erase tree
	ObjectTree->DeleteAllItems();

	//populate object tree
	wxTreeItemId id = ObjectTree->AddRoot(wxString::FromAscii(Simcore->GetName().c_str()), -1, -1, new TreeItemData(wxVariant(Simcore->GetNumber()).GetString()));

	//add child objects
	AddChildren(Simcore, id);

	ObjectTree->Expand(id);
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
				wxTreeItemId id = ObjectTree->AppendItem(treeparent, wxString::FromAscii(parent->GetChild(i)->GetName().c_str()), -1, -1, new TreeItemData(wxVariant(parent->GetChild(i)->GetNumber()).GetString()));

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
	wxTreeItemId sel = ObjectTree->GetSelection();

	if (!sel.IsOk())
		return;

	TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(sel);
	wxString num;
	num = data->GetDesc();
	int number = atoi(num.ToAscii());

	//call SBS to delete object
	if (Simcore->DeleteObject(number))
	{
		//delete object from tree
		ObjectTree->Delete(sel);
		deleted = true;
	}
}

void ObjectInfo::On_bCreate_Click(wxCommandEvent& event)
{
	if (!createobject)
		createobject = new CreateObject(this, -1);
	if (createobject)
		createobject->Show();
}

void ObjectInfo::On_bModify_Click(wxCommandEvent& event)
{
	if (!modifyobject)
		modifyobject = new ParameterViewer(this, tType->GetValue(), tParentType->GetValue(), false, -1);
	if (modifyobject)
		modifyobject->Show();
}

void ObjectInfo::On_bSave_Click(wxCommandEvent& event)
{
	wxFileDialog *Selector = new wxFileDialog(0, _("Save Building Script"), _("buildings/"), _(""), _("Building files (*.bld *.txt)|*.bld;*.txt"), wxFD_SAVE);
	int result = Selector->ShowModal();
	if (result == wxID_CANCEL)
	{
		//delete dialog
		delete Selector;
		Selector = 0;
		//quit
		return;
	}
	wxString filename = wxT("buildings/") + Selector->GetFilename();
}

void ObjectInfo::On_bViewScript_Click(wxCommandEvent& event)
{
	TextWindow *twindow = new TextWindow(NULL, -1);
	twindow->SetMinSize(wxSize(640, 480));
	twindow->tMain->SetMinSize(wxSize(640, 480));
	twindow->Fit();
	twindow->Center();
	twindow->SetTitle(wxT("Current Script"));
	twindow->Show(true);
	std::vector<std::string> *data = skyscraper->GetActiveEngine()->GetScriptProcessor()->GetBuildingData();
	for (int i = 0; i < (int)data->size(); i++)
			twindow->tMain->WriteText(wxString::FromAscii(data->at(i).c_str()) + wxT("\n"));
	twindow->tMain->SetInsertionPoint(0);
}

void ObjectInfo::On_bMove_Click(wxCommandEvent& event)
{
	wxTreeItemId sel = ObjectTree->GetSelection();

	if (!sel.IsOk())
		return;

	TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(sel);

	wxString num;
	num = data->GetDesc();
	int number = atoi(num.ToAscii());

	if (moveobject)
		delete moveobject;
	moveobject = 0;

	moveobject = new MoveObject(this, -1, number);
	moveobject->Show();
}

void ObjectInfo::On_bReset_Click(wxCommandEvent& event)
{
	wxTreeItemId sel = ObjectTree->GetSelection();

	if (!sel.IsOk())
		return;

	TreeItemData *data = (TreeItemData*) ObjectTree->GetItemData(sel);
	wxString num;
	num = data->GetDesc();
	int number = atoi(num.ToAscii());

	//call SBS to reset object state
	Object *obj = Simcore->GetObject(number);
	if (obj)
		obj->ResetState();
}

}
