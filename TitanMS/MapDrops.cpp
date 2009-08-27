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

#include "MapDrops.h"
#include "Map.h"
#include "Mob.h"
#include "MapPlayers.h"
#include "Player.h"
#include "MapPlayers.h"
#include "MapsData.h"
#include "MapData.h"
#include "MapFootholdsData.h"
#include "MapFootholdData.h"
#include "PacketCreator.h"
#include "Timer.h"
#include "Item.h"
#include "Equip.h"
#include "MobsDropData.h"
#include "ReactorsDropData.h"
#include "Drop.h"
#include "DropsData.h"
#include "DropData.h"
#include "Run.h"
#include "Tools.h"
#include "PlayerInventories.h"
#include "MapReactors.h"
#include "Reactor.h"
using namespace Tools;

int MapDrops::ids = 0;

class MapDropsTimer : public Run {
private:
	MapDrops* map;
public:
	MapDropsTimer(MapDrops* map):map(map){}
	void run(){
		map->checkDrops(true);
	}
};	
void MapDrops::addDrop(Drop* drop){
	drops[drop->getID()] = drop;
}
void MapDrops::removeDrop(Drop* drop){
	drop->removeDrop();
	drops.erase(drop->getID());
	delete drop;
}

Drop* MapDrops::getDropByID(int id){
	if(drops.find(id) != drops.end())
		return drops[id];
	return NULL;	
}

void MapDrops::lootDrop(Player* player, Drop* drop){
	if(drop->takeDrop(player)){
		drops.erase(drop->getID());
		delete drop;
	}
}

void MapDrops::show(Player* player){
	for (hash_map<int,Drop*>::iterator iter = drops.begin(); iter != drops.end(); iter++){
		player->send(PacketCreator().showDrop(iter->second));		
	}
}

void MapDrops::checkDrops(bool timer){
	if(map->getPlayers()->getPlayers()->empty()){
		timeron = false;
		return;
	}
	if(!timeron){
		timeron = true;
		checkDrops(true);
		return;
	}
	if(timer){
		hash_map<int,Drop*> tdrops(drops);
		for (hash_map<int,Drop*>::iterator iter = tdrops.begin(); iter != tdrops.end(); iter++)
			if(GetTickCount() > (unsigned int)(iter->second->getDropTime()  + 120000)) // 2 minutes
				removeDrop(iter->second);
		Timers::getInstance()->startTimer(10000, new MapDropsTimer(this));
	}
}

void MapDrops::dropFromMob(Mob* mob, Player* killer){
	DropsData* mdrops = MobsDropData::getInstance()->getDataByID(mob->getMobID());
	if(mdrops == NULL)
		return;
	dropFromObject(mdrops, mob, killer);

}
void MapDrops::dropFromReactor(Reactor* reactor, Player* player){
	DropsData* mdrops = ReactorsDropData::getInstance()->getDataByID(reactor->getReactorID());
	if(mdrops == NULL)
		return;
	dropFromObject(mdrops, reactor, player);

}
void MapDrops::dropFromObject(DropsData* drops, MapObject* obj, Player* owner){
	vector <DropData*>* ndrops = drops->getData();
	int count = 0;
	Position pos;
	for(int i=0; i<(int)ndrops->size(); i++){
		if(random(10000) < (*ndrops)[i]->getChance()){
			Item* item = NULL;
			if(IS_EQUIP((*ndrops)[i]->getID()))
				item = new Equip((*ndrops)[i]->getID(), true);
			else
				item = new Item((*ndrops)[i]->getID());
			pos.x = obj->getPosition().x + 25*((count+count%2)/2)*((count%2) ? 1 : -1);
			pos.y = obj->getPosition().y;
			dropFromObject(item, pos, obj, (owner == NULL) ? 0 : owner->getID(), NULL, 500);
			count++;
		}
		if(drops->getDropsLimit() != 0 && count >= drops->getDropsLimit())
			break;
	}
	int bags = random(drops->getMinBags(), drops->getMaxBags());
	for(int i=0; i<bags; i++){
		pos.x = obj->getPosition().x + 25*((count+count%2)/2)*((count%2) ? 1 : -1);
		pos.y = obj->getPosition().y;
		int mesos = random(drops->getMinMesos(), drops->getMaxMesos());
		if(mesos > 0)
			dropMesosFromObject(mesos, pos, obj, (owner == NULL) ? 0 : owner->getID(), 500);
	}
	/*
		vector <DropData*>* ndrops = drops->getData();
	Position pos;
	vector <pair<int, int>> droplist;
	int count = 0;
	for(int i=0; i<(int)ndrops->size() && (drops->getDropsLimit() > 0) ? ((int)droplist.size() < drops->getDropsLimit()) : true; i++){
		if(random(10000) < (*ndrops)[i]->getChance()){
			droplist.push_back(pair<int,int>((*ndrops)[i]->getID(), 1));
		}
	}
	for(int i=0; i<(int)droplist.size(); i++){
		
	}
			Item* item = NULL;
			if(IS_EQUIP((*ndrops)[i]->getID()))
				item = new Equip((*ndrops)[i]->getID(), true);
			else
				item = new Item((*ndrops)[i]->getID());
			pos.x = obj->getPosition().x + 25*((count+count%2)/2)*((count%2) ? 1 : -1);
			pos.y = obj->getPosition().y;
			dropFromObject(item, pos, obj, (owner == NULL) ? 0 : owner->getID());
			count++;
		}
		if(drops->getDropsLimit() != 0 && count >= drops->getDropsLimit())
			break;
	}
	int bags = random(drops->getMinBags(), drops->getMaxBags());
	for(int i=0; i<bags; i++){
		pos.x = obj->getPosition().x + 25*((count+count%2)/2)*((count%2) ? 1 : -1);
		pos.y = obj->getPosition().y;
		int mesos = random(drops->getMinMesos(), drops->getMaxMesos());
		if(mesos > 0)
			dropMesosFromObject(mesos, pos, obj, (owner == NULL) ? 0 : owner->getID(), count*500);
	}
	*/
}
void MapDrops::dropFromObject(Item* item, Position pos, MapObject* obj, int own, Player* player, int delay){
	pos.y -= 100;
	Drop* drop = new Drop(map, item, map->findFoothold(pos));
	drop->setID(ids++);
	if(own > 0)
		drop->setTime(GetTickCount()+30000);
	else
		drop->setTime(0);
	drop->setOwner(own);
	drop->drop(obj, delay);
	drop->setDropTime(GetTickCount());
	map->getReactors()->checkDrop(drop, player);
}
void MapDrops::dropMesosFromObject(int amount, Position pos, MapObject* obj, int own, int delay){
	Drop* drop = new Drop(map);
	drop->setID(ids++);
	drop->setMesos(true);
	drop->setAmount(amount);
	pos.y -= 100;
	drop->setPosition(map->findFoothold(pos));
	if(own > 0)
		drop->setTime(GetTickCount()+30000);
	else
		drop->setTime(0);
	drop->setOwner(own);
	drop->setDropTime(GetTickCount());
	drop->drop(obj, delay);
}