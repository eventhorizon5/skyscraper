/*
	Scalable Building Simulator - Card Reader Object
	The Skyscraper Project - Version 2.1
	Copyright (C)2004-2026 Ryan Thoryk
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

#ifndef _SBS_CARDREADER_H
#define _SBS_CARDREADER_H

namespace SBS {

class AccessController;

//a clickable badge reader; clicking presents the player's active badge,
//and right-clicking switches to the next badge in the wallet.
//Its light shows the lock state: red when locked, green when unlocked (by a badge or the
//controller), and alternating red/green for a moment when an invalid badge is presented.
class SBSIMPEXP CardReader : public Object
{
public:
	CardReader(Object *parent, AccessController *controller, const std::string &access_point, const std::string &grant_sound, const std::string &deny_sound, const std::string &idle_texture, const std::string &grant_texture, const std::string &deny_texture, const std::string &direction, Real width, Real height);
	~CardReader();

	const std::string& GetAccessPointName() { return access_point; }
	void ShowResult(bool granted);
	void Update(unsigned long now);
	void DetachController() { controller = 0; }
	bool Enabled(bool value);
	bool IsEnabled() { return is_enabled; }
	void OnClick(Vector3 &position, bool shift, bool ctrl, bool alt, bool right);
	void Report(const std::string &message);
	bool ReportError(const std::string &message);

private:
	AccessController *controller;
	MeshObject *ReaderMesh;
	Sound *sound;
	std::string access_point;
	std::string grant_sound;
	std::string deny_sound;
	std::string idle_texture;
	std::string grant_texture;
	std::string deny_texture;
	unsigned long flash_until; //invalid badge: alternate red/green until this time, 0 if not flashing
	unsigned long next_check; //next time to check the lock state
	std::string current_texture; //texture currently shown
	bool is_enabled;

	void SetTexture(const std::string &texture);
};

}

#endif
