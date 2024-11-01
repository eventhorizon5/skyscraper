/*
	Skyscraper 2.1 - GUI Manager
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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/dir.h"
#include "wx/progdlg.h"
#include "wx/cmdline.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/stdpaths.h"
#endif
#include "globals.h"
#include "sbs.h"
#include "vm.h"
#include "hal.h"
#include "enginecontext.h"
#include "gui/debugpanel.h"
#include "gui/console.h"
#include "gui/loaddialog.h"
#include "gui.h"

namespace Skyscraper {

GUI::GUI(VM *parent)
{
	this->vm = parent;
	console = 0;
	progdialog = 0;
	dpanel = 0;
	loaddialog = 0;
	show_progress = false;
}

GUI::~GUI()
{
	Unload();

	//delete building load dialog
	if (loaddialog)
		loaddialog->Destroy();
	loaddialog = 0;

	//delete progress dialog
	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	//delete console window
	if (console)
		console->Destroy();
	console = 0;

	CloseProgressDialog();
}

void GUI::Unload()
{
	//delete control panel object
	if(dpanel)
		delete dpanel;
	dpanel = 0;

	//disable console send button
	EnableConsole(false);
}

void GUI::ShowError(const std::string &message)
{
	//show error dialog
	wxMessageDialog dialog(0, message, _("Skyscraper"), wxOK | wxICON_ERROR);
	dialog.ShowModal();
}

void GUI::ShowMessage(const std::string &message)
{
	//show message dialog
	wxMessageDialog dialog(0, message, _("Skyscraper"), wxOK | wxICON_INFORMATION);
	dialog.ShowModal();
}

void GUI::ShowProgress()
{
	//show progress dialog if needed
	if (show_progress == true)
		ShowProgressDialog();
}

std::string GUI::SelectBuilding(const std::string &data_path)
{
	//choose a building from a script file

	std::string filename = "";

	//get listing of building files
	wxArrayString filelist;
	wxArrayString filelist2;
	wxDir::GetAllFiles(_("buildings/"), &filelist, _("*.bld"), wxDIR_FILES);
	wxDir::GetAllFiles(_(data_path + "buildings/"), &filelist2, _("*.bld"), wxDIR_FILES);

	//strip directory name and extension from entries
	for (size_t i = 0; i < filelist.size(); i++)
	{
		filelist[i] = filelist[i].substr(10);
		filelist[i] = filelist[i].substr(0, filelist[i].length() - 4);
	}
	for (size_t i = 0; i < filelist2.size(); i++)
	{
		filelist2[i] = filelist2[i].substr(data_path.length() + 10);
		filelist2[i] = filelist2[i].substr(0, filelist2[i].length() - 4);
	}

	for (size_t i = 0; i < filelist2.size(); i++)
	{
		bool found = false;
		for (size_t j = 0; j < filelist.size(); j++)
		{
			if (filelist[j] == filelist2[i])
			{
				found = true;
				break;
			}
		}
		if (found == false)
			filelist.Add(filelist2[i]);
	}

	//sort list
	filelist.Sort();

	//show selection dialog window
	wxSingleChoiceDialog Selector (0, _("Select a Building"), _("Load Building"), filelist);
	Selector.SetSize(wxSize(500, 400));
	Selector.CenterOnScreen();

	if (Selector.ShowModal() == wxID_OK)
	{
		filename = Selector.GetStringSelection();
		filename += ".bld";
	}

	return filename;
}

void GUI::CreateDebugPanel()
{
	if (!dpanel)
		dpanel = new DebugPanel(vm, vm->GetParent(), -1);
	dpanel->Show(true);
	HAL *hal = vm->GetHAL();
	dpanel->SetPosition(wxPoint(hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ControlPanelX", 10), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ControlPanelY", 25)));
}

void GUI::EnableConsole(bool value)
{
	if (console)
		console->bSend->Enable(value);
}

void GUI::EnableTimer(bool value)
{
	if(dpanel)
		dpanel->EnableTimer(value);
}

void GUI::ShowConsole(bool send_button)
{
	if (!console)
		console = new Console(vm, vm->GetParent(), -1);
	console->Show();
	console->Raise();
	HAL *hal = vm->GetHAL();
	console->SetPosition(wxPoint(hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ConsoleX", 10), hal->GetConfigInt(hal->configfile, "Skyscraper.Frontend.ConsoleY", 25)));
	console->bSend->Enable(send_button);
}

void GUI::CreateProgressDialog(const std::string &message)
{
	//don't create progress dialog if concurrent loading is enabled, and one engine is already running
	if (vm->GetEngineCount() > 1 && vm->ConcurrentLoads == true)
	{
		if (vm->GetFirstValidEngine()->IsRunning() == true)
			return;
	}

	if (!progdialog)
	{
		//show progress dialog in a queued fashion
		show_progress = true;
		prog_text = message;
	}
	else
	{
		wxString msg = progdialog->GetMessage();
		msg += "\n";
		msg += message;
		progdialog->Update(progdialog->GetValue(), msg);
	}

	//stop control panel timer
	EnableTimer(false);
}

void GUI::CloseProgressDialog()
{
	//close progress dialog
	if (progdialog)
		progdialog->Destroy();
	progdialog = 0;

	//start control panel timer
	EnableTimer(true);
}

void GUI::ShowProgressDialog()
{
	if (!progdialog)
		progdialog = new wxProgressDialog(wxT("Loading..."), prog_text, 100, vm->GetParent());

	show_progress = false;
}

void GUI::UpdateProgress()
{
	if (!progdialog)
		return;

	int total_percent = vm->GetEngineCount() * 100;
	int current_percent = 0;

	for (size_t i = 0; i < vm->GetEngineCount(); i++)
	{
		if (vm->GetEngine(i))
			current_percent += vm->GetEngine(i)->GetProgress();
	}

	int final = ((Real)current_percent / (Real)total_percent) * 100;
	progdialog->Update(final);
}

void GUI::RefreshConsole()
{
	if (console)
	{
		console->Refresh();
		console->Update();
	}
}

void GUI::RaiseWindow()
{
	vm->GetParent()->Raise();
    vm->GetParent()->SetFocus();
}

void GUI::ShowDebugPanel()
{
	if (dpanel)
		return;

	//show control panel if closed
	dpanel = new DebugPanel(vm, vm->GetParent(), -1);
	dpanel->Show(true);
	dpanel->SetPosition(wxPoint(vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.ControlPanelX", 10), vm->GetHAL()->GetConfigInt(vm->GetHAL()->configfile, "Skyscraper.Frontend.ControlPanelY", 25)));
}

void GUI::ShowControlReference()
{
	dpanel->ShowControlReference();
}

void GUI::ShowLoadDialog()
{
	if (!loaddialog)
		loaddialog = new LoadDialog(dpanel, vm->GetParent(), -1);
	loaddialog->CenterOnScreen();
	loaddialog->Show();
}

void GUI::WriteToConsole(const std::string &message)
{
	if (console)
	{
		console->Write(message);
		console->Update();
	}
}

}
