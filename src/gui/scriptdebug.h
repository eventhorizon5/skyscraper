/*
    Skyscraper 2.1 - Script Debugger
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

#ifndef SCRIPTDEBUG_H
#define SCRIPTDEBUG_H

//(*Headers(ScriptDebug)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class ScriptDebug: public wxDialog
{
    public:

        ScriptDebug(DebugPanel* root, wxWindow* parent);
        virtual ~ScriptDebug();
        void Loop();

        //(*Declarations(ScriptDebug)
        wxButton* bGoto;
        wxButton* bReset;
        wxButton* bRun;
        wxButton* bStart;
        wxButton* bStop;
        wxListBox* lstFunctions;
        wxListBox* lstScript;
        wxListBox* lstVariables;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxTextCtrl* txtFilename;
        wxTextCtrl* txtLine;
        wxTextCtrl* txtSection;
        //*)
		void OnInit();

    protected:

        //(*Identifiers(ScriptDebug)
        static const long ID_lstScript;
        static const long ID_STATICTEXT1;
        static const long ID_txtFilename;
        static const long ID_STATICTEXT2;
        static const long ID_txtLine;
        static const long ID_STATICTEXT3;
        static const long ID_txtSection;
        static const long ID_lstFunctions;
        static const long ID_bGoto;
        static const long ID_bRun;
        static const long ID_bStart;
        static const long ID_bStop;
        static const long ID_bReset;
        static const long ID_lstVariables;
        //*)

    private:

        //(*Handlers(ScriptDebug)
        void On_bGoto_Click(wxCommandEvent& event);
        void On_bRun_Click(wxCommandEvent& event);
        void On_bStart_Click(wxCommandEvent& event);
        void On_bStop_Click(wxCommandEvent& event);
        void On_bReset_Click(wxCommandEvent& event);
        //*)
		SBS::SBS *Simcore;
        ScriptProcessor *scriptproc;
		DebugPanel *panel;
        size_t variable_count;
        size_t function_count;

        DECLARE_EVENT_TABLE()
};

}

#endif
