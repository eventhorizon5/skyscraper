//Skyscraper 1.1 Alpha - Debug/Control Panel form class
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

#include "debugpanel.h"

//Do not add custom headers.
//wx-dvcpp designer will remove them
////Header Include Start
#include "debugpanel.xpm"
////Header Include End


//----------------------------------------------------------------------------
// DebugPanel
//----------------------------------------------------------------------------
     //Add Custom Events only in the appropriate Block.
    // Code added in  other places will be removed by wx-dvcpp 
    ////Event Table Start
BEGIN_EVENT_TABLE(DebugPanel,wxFrame)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(DebugPanel::DebugPanelClose)
END_EVENT_TABLE()
    ////Event Table End



DebugPanel::DebugPanel( wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style )
    : wxFrame( parent, id, title, position, size, style)
{
    CreateGUIControls();
}

DebugPanel::~DebugPanel()
{
    
} 

void DebugPanel::CreateGUIControls(void)
{
    //Do not add custom Code here
    //wx-devcpp designer will remove them.
    //Add the custom code before or after the Blocks
    ////GUI Items Creation Start

	this->SetSize(8,8,549,407);
	this->SetTitle(_("Simulator Control Panel"));
	this->Center();
	wxIcon DebugPanel_ICON (DebugPanel_XPM);
	this->SetIcon(DebugPanel_XPM);
	
    ////GUI Items Creation End
}

void DebugPanel::DebugPanelClose(wxCloseEvent& event)
{
    // --> Don't use Close with a Frame,
    // use Destroy instead.
    Destroy();
}
 
