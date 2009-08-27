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

#ifndef MAPDROPs_H
#define MAPDROPs_H

#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

#include "Players.h"
#include "Player.h"

class Player;
class Mob;
class MapObject;
class Item;
class Drop;
class DropsData;
class Reactor;

class MapDrops {
private:
	Map* map;
	hash_map <int, Drop*> drops;
	bool timeron;
public:
	static int ids;
	MapDrops(Map* map){
		this->map = map;
		timeron = false;
	}
	void addDrop(Drop* drop);
	Drop* getDropByID(int objid);
	void removeDrop(Drop* drop);
	void showDropsToPlayer(Player* player);
	void checkDrops(bool timer = true);
	void show(Player* player);
	void dropFromMob(Mob* mob, Player* killer = 0);
	void dropFromReactor(Reactor* reactor, Player* player);
	void dropFromObject(DropsData* drops, MapObject* obj, Player* owner);
	void dropFromObject(Item* item, Position pos, MapObject* obj, int own = 0, Player* player = NULL, int delay = 0);
	void dropMesosFromObject(int amount, Position pos, MapObject* obj, int own, int delay = 0);
	void lootDrop(Player* player, Drop* drop);
	bool findDrop(Drop* drop){
		for (hash_map<int,Drop*>::iterator iter = drops.begin(); iter != drops.end(); iter++)
			if(iter->second == drop)
				return true;
		return false;
	}
	void clear(){
		hash_map<int,Drop*> drops = hash_map<int,Drop*>(this->drops);
		for (hash_map<int,Drop*>::iterator iter = drops.begin(); iter != drops.end(); iter++)
			removeDrop(iter->second);
	}
};

#endif