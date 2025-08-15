/*
	Skyscraper 2.1 - GUI Manager
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

#ifndef GUI_H

#include <string>
#include "vm.h"

//wxWidgets definitions
class wxCmdLineParser;
class wxProgressDialog;

namespace Skyscraper {

class EngineContext;
class Console;
class LoadDialog;
class ScriptProcessor;
class VM;
class HAL;
class DebugPanel;

class VMIMPEXP GUI
{
public:
	GUI(VM *parent);
	~GUI();
	void UnregisterDebugPanel() { dpanel = 0; }
	void Unload();
	void ShowProgress();
	std::string SelectBuilding(const std::string &data_path);
	std::string SelectBuildingNative(const std::string &data_path);
	void Resize();
	void CreateDebugPanel();
	void EnableConsole(bool value);
	void RaiseWindow();
	void EnableTimer(bool value);
	void ShowConsole(bool send_button = true);
	void ShowProgressDialog();
	void CreateProgressDialog(const std::string &message);
	void CloseProgressDialog();
	bool UpdateProgress(int percent);
	bool ProgressCancelled();
	bool ProgressSkipped();
	void ShowError(const std::string &message);
    void ShowMessage(const std::string &message);
	void RefreshConsole();
	void ShowDebugPanel();
	void ShowControlReference();
	void ShowLoadDialog();
	void WriteToConsole(const std::string &message, const std::string &color = "white");
	bool ReportMissingFiles(std::vector<std::string> &missing_files);
	bool IsConsoleVisible();

private:

	wxProgressDialog *progdialog;

	//control panel
	DebugPanel *dpanel;

	//console window
	Console *console;

	//load dialog window
	LoadDialog *loaddialog;

	//progress dialog initial data
	bool show_progress;
	std::string prog_text;

	VM *vm;
};

}

#endif

