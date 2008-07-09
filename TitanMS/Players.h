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

using namespace std;
using namespace stdext;

class Player;
class PacketWriter;
struct Position;

class Players {
protected:
	hash_map <int, Player*> players;
	hash_map <string, Player*> names;
public:
	Players(){ }
	void add(Player* player);
	void remove(Player* player);
	Player* getPlayerByID(int id);
	Player* getPlayerByName(int id);
	void send(PacketWriter* pw);
	void send(PacketWriter* pw, Player* player);
	void send(PacketWriter* pw, Player* player, Position pos);
};

#endif