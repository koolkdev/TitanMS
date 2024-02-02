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

#ifndef REACTOR_H
#define REACTOR_H

#include "MapObject.h"

class Player;
class Map;

class Reactor : public MapObject {
private:
	Map* map;
	int rid;
	int state;
	bool destroyed;
	int mapid;
public:
	Reactor (Map* map){
		this->map = map;
		rid = 0;
		mapid = -1;
		state = 0;
		destroyed = false;
	}
	void setReactorID(int rid){
		this->rid = rid;
	}
	int getReactorID(){
		return rid;
	}
	void setOriginalID(int mapid){
		this->mapid=mapid;
	}
	int getOriginalID(){
		return mapid;
	}
	bool isDestroyed(){
		return destroyed;
	}
	void setDestroyed(){
		destroyed = true;
	}
	int getState(){
		return state;
	}
	Map* getMap(){
		return map;
	}
	void drop(Player* player);
	void setState(int s);
	void hit(Player* player, short stance, char pos);
	bool inArea(Position pos);
};

#endif