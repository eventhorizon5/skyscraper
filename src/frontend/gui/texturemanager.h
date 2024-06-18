/*
	Skyscraper 1.12 Alpha - Texture Manager Dialog
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

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

//(*Headers(TextureManager)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)

namespace Skyscraper {

class TextureManager: public wxDialog
{
    public:

        TextureManager(DebugPanel* parent,wxWindowID id=wxID_ANY);
        virtual ~TextureManager();

        //(*Declarations(TextureManager)
        wxButton* bExport;
        wxButton* bOK;
        wxButton* bSave;
        wxButton* bUnload;
        wxCheckBox* chkEnableForce;
        wxCheckBox* chkForceMode;
        wxListBox* TextureList;
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText12;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* tAlpha;
        wxTextCtrl* tDependencies;
        wxTextCtrl* tFilename;
        wxTextCtrl* tHeight;
        wxTextCtrl* tHeightMult;
        wxTextCtrl* tMaterial;
        wxTextCtrl* tMemory;
        wxTextCtrl* tMipMaps;
        wxTextCtrl* tWidth;
        wxTextCtrl* tWidthMult;
        //*)
        void Loop();

    protected:

        //(*Identifiers(TextureManager)
        static const long ID_TextureList;
        static const long ID_STATICTEXT7;
        static const long ID_tDependencies;
        static const long ID_STATICTEXT4;
        static const long ID_tMaterial;
        static const long ID_STATICTEXT1;
        static const long ID_tFilename;
        static const long ID_STATICTEXT8;
        static const long ID_tWidth;
        static const long ID_STATICTEXT9;
        static const long ID_tHeight;
        static const long ID_STATICTEXT11;
        static const long ID_tAlpha;
        static const long ID_STATICTEXT12;
        static const long ID_tMipMaps;
        static const long ID_STATICTEXT10;
        static const long ID_tMemory;
        static const long ID_STATICTEXT5;
        static const long ID_tWidthMult;
        static const long ID_STATICTEXT6;
        static const long ID_tHeightMult;
        static const long ID_STATICTEXT2;
        static const long ID_chkEnableForce;
        static const long ID_STATICTEXT3;
        static const long ID_chkForceMode;
        static const long ID_bSave;
        static const long ID_bExport;
        static const long ID_bUnload;
        static const long ID_bOK;
        //*)

    private:

        //(*Handlers(TextureManager)
        void On_bUnload_Click(wxCommandEvent& event);
        void On_bOK_Click(wxCommandEvent& event);
        void On_TextureList_Select(wxCommandEvent& event);
        void On_bSave_Click(wxCommandEvent& event);
        void On_bExport_Click(wxCommandEvent& event);
        //*)

        std::string GetTextureName(const std::string &material);

        SBS::SBS *Simcore;
		DebugPanel *panel;
		int lastcount;

        DECLARE_EVENT_TABLE()
};

}

#endif
