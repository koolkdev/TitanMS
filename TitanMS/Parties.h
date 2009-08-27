/*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

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
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef PARTIES_H
#define PARTIES_H

#include <hash_map>
#include <string>
class Party;
class Player;
class World;

class Parties {
private:
	stdext::hash_map <int, Party*> parties;
	World* world;
public:
	Parties(World* world){
		this->world = world;
	}
	Party* getParty(int id);
	void create(Player* player);
	void leave(Player* player);
	void join(Player* player, int partyid);
	void invite(Player* player, std::string& name);
	void expel(Player* player, int playerid);
	void changeLeader(Player* player, int playerid);
	void denied(Player* player, std::string& to, std::string& from, bool block = false);
};

#endif