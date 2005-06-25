//Skyscraper 1.1 Alpha - Main form class
//Copyright ©2005 Ryan Thoryk
//http://www.tliquest.net/skyscraper
//http://sourceforge.net/projects/skyscraper
//Contact - ryan@tliquest.net

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef __MainFrm_HPP_
#define __MainFrm_HPP_

// For compilers that support precompilation, includes "wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
////Header Include End

#include <wx/frame.h>

////Dialog Style Start
	#define THIS_DIALOG_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxMAXIMIZE_BOX |  wxCLOSE_BOX
////Dialog Style End

class MainFrm : public wxFrame
{
private:
    DECLARE_EVENT_TABLE()
public:
    MainFrm( wxWindow *parent, wxWindowID id = 1, const wxString &title = _T("Skyscraper"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = THIS_DIALOG_STYLE);
    virtual ~MainFrm();
public:
  //Do not add custom Control Declarations here.
  //wx-devcpp will remove them. Try adding the custom code 
  //after the block.
  ////GUI Control Declaration Start
  ////GUI Control Declaration End
public:
	//Note: if you receive any error with these enums, then you need to
    //change your old form code that are based on the #define control ids.
	//It may replace a numeric value in the enums names.
	enum {
////GUI Enum Control ID Start
////GUI Enum Control ID End
   ID_DUMMY_VALUE_ //Dont Delete this DummyValue
   }; //End of Enum
public:
    void MainFrmClose(wxCloseEvent& event);
    void CreateGUIControls(void);

};

#endif
 
 
 
 
