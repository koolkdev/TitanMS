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

#ifndef PLAYERS_H
#define PLAYERS_H

#include <hash_map>
#include <string>

using namespace std;
using namespace stdext;

class Player;
class PacketWriter;
struct Position;
class Map;

class Players {
protected:
	hash_map <int, Player*> players;
	hash_map <string, Player*> names;
public:
	Players(){ }
	int getPlayersCount(){
		return players.size();
	}
	hash_map <int, Player*>* getPlayers(){
		return &players;
	}
	Player* operator[](int index);
	void add(Player* player);
	void remove(Player* player);
	void changeMap(int mapid);
	void changeMapPos(int mapid, int pos);
	void changeMapPortal(int mapid, string& portalname);
	Player* getPlayerByID(int id);
	Player* getPlayerByName(string id);
	void send(PacketWriter* pw);
	void send(PacketWriter* pw, Player* player);
	void send(PacketWriter* pw, Player* player, Position pos);
	void send(PacketWriter* pw, Map* map);
	void send(PacketWriter* pw, Map* map, Player* player);
	template <class T>
	void run(T* act){
		hash_map<int,Player*> players = hash_map<int, Player*>(this->players);
		for (hash_map<int,Player*>::iterator iter = players.begin(); iter != players.end(); iter++){
			(*act)(iter->second);
		}
	}
};

#endif