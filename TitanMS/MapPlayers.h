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

#ifndef MAPPLAYERS_H
#define MAPPLAYERS_H

#include <vector>
#include <algorithm>

using namespace std;

#include "Players.h"
class Player;
class Map;

class MapPlayers : public Players {
private:
	vector <Player*> playersv;
	Map* map;
public:
	MapPlayers(Map* map){
		this->map = map;
	}
	bool find(Player* player){
		if(playersv.empty()) return false;
		return std::find(playersv.begin(), playersv.end(), player) != playersv.end();
	}
	void add(Player* player);
	void remove(Player* player);
	vector <Player*>* getPlayersList(){
		return &playersv;
	}

};

#endif