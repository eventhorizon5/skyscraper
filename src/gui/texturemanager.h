/*
	Skyscraper 2.1 - Texture Manager Dialog
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

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

//(*Headers(TextureManager)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
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
        wxStaticBitmap* bmpMain;
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
        static const wxWindowID ID_TextureList;
        static const wxWindowID ID_STATICTEXT7;
        static const wxWindowID ID_tDependencies;
        static const wxWindowID ID_STATICTEXT4;
        static const wxWindowID ID_tMaterial;
        static const wxWindowID ID_STATICTEXT1;
        static const wxWindowID ID_tFilename;
        static const wxWindowID ID_STATICTEXT8;
        static const wxWindowID ID_tWidth;
        static const wxWindowID ID_STATICTEXT9;
        static const wxWindowID ID_tHeight;
        static const wxWindowID ID_STATICTEXT11;
        static const wxWindowID ID_tAlpha;
        static const wxWindowID ID_STATICTEXT12;
        static const wxWindowID ID_tMipMaps;
        static const wxWindowID ID_STATICTEXT10;
        static const wxWindowID ID_tMemory;
        static const wxWindowID ID_STATICTEXT5;
        static const wxWindowID ID_tWidthMult;
        static const wxWindowID ID_STATICTEXT6;
        static const wxWindowID ID_tHeightMult;
        static const wxWindowID ID_STATICTEXT2;
        static const wxWindowID ID_chkEnableForce;
        static const wxWindowID ID_STATICTEXT3;
        static const wxWindowID ID_chkForceMode;
        static const wxWindowID ID_bSave;
        static const wxWindowID ID_bExport;
        static const wxWindowID ID_bUnload;
        static const wxWindowID ID_bmpMain;
        static const wxWindowID ID_bOK;
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
