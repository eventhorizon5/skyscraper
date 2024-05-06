/*
	Skyscraper 3.0 Alpha - Server Core
	Copyright (C)2023-2024 Ryan Thoryk
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

#ifndef SERVER_H
#define SERVER_H

namespace Skyscraper {

class Server
{
public:

	Server(Skyscraper *frontend);
	~Server();
	bool Loop();
	void Handler();
	EngineContext* GetEngine(int number);
	EngineContext* CreateEngine(EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	bool DeleteEngine(EngineContext *engine);
	void DeleteEngines();
	int GetEngineCount();
	int GetEngineListSize() { return (int)engines.size(); }
	EngineContext* GetActiveEngine() { return active_engine; }
	EngineContext* FindActiveEngine();
	void SetActiveEngine(EngineContext *engine, bool switch_engines = false);
	bool IsEngineLoading();
	bool IsValidEngine(EngineContext *engine);
	bool IsValidSystem(::SBS::SBS *sbs);
	int RegisterEngine(EngineContext *engine);
	EngineContext* GetFirstValidEngine();
	bool Load(const std::string &filename, EngineContext *parent = 0, const Vector3 &position = Vector3::ZERO, Real rotation = 0.0, const Vector3 &area_min = Vector3::ZERO, const Vector3 &area_max = Vector3::ZERO);
	void Connect(Client* client);

	std::vector<Client*> clients;

private:

	bool RunEngines();
	void SwitchEngines();
	void HandleEngineShutdown();
	void HandleReload();
	int GetFreeInstanceNumber();

	Skyscraper *frontend;

	EngineContext *active_engine;

	std::vector<EngineContext*> engines;
};

}

#endif
