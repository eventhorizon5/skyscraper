//Scalable Building Simulator - Console Dialog
//The Skyscraper Project - Version 1.1 Alpha
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

#include "console.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include "console.xpm"
////Header Include End



//----------------------------------------------------------------------------
// SBS_Console
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp 
    ////Event Table Start
BEGIN_EVENT_TABLE(Console,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(Console::ConsoleClose)
END_EVENT_TABLE()
    ////Event Table End



Console::Console( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxDialog( parent, id, title, position, size, style)
{
    CreateGUIControls();
}

Console::~Console()
{

} 

void Console::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	this->SetSize(8,8,425,334);
	this->SetTitle(_("Console"));
	this->Center();
	wxIcon Console_ICON (Console_XPM);
	this->SetIcon(Console_XPM);
	

	WxMemo1 =  new wxTextCtrl(this, ID_WXMEMO1, "" , wxPoint(11,8),wxSize(394,283)  , wxTE_READONLY |  wxTE_MULTILINE);
	WxMemo1->SetMaxLength(0);
    ////GUI Items Creation End
}

void Console::ConsoleClose(wxCloseEvent& event)
{
    // --> Don't use Close with a wxDialog,
    // use Destroy instead.
    Destroy();
}
 
