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

#ifndef MAPNPCS_H
#define MAPNPCS_H

#include <vector>

using namespace std;

class NPC;
class Map;
class Player;

class MapNPCs {
private:
	Map* map;
	vector <NPC*> npcs;
public:
	MapNPCs(Map* map);
	NPC* getNPCByID(int objid);
	void show(Player* player);
	int getNewID(){
		return npcs.size()+1;
	}
	void spawn(NPC* npc);

};

#endif