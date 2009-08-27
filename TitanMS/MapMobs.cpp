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

#include "Player.h"
#include "MapMobs.h"
#include "MapsData.h"
#include "MapData.h"
#include "PvP.h"
#include "MobData.h"
#include "MobsData.h"
#include "MapMobData.h"
#include "MapMobsData.h"
#include "Map.h"
#include "Mob.h"
#include "Timer.h"
#include "PacketCreator.h"
#include "DataProvider.h"
#include "MobSummonData.h"
#include "MapPlayers.h"
#include "Item.h"
#include "MapDrops.h"
#include "Damage.h"
#include "Timer.h"
#include "Run.h"
#include "PlayerInventories.h"
#include "Inventory.h"

int MapMobs::ids = 0x100000;

class MapMobsTimer : public Run {
private:
	MapMobs* map;
public:
	MapMobsTimer(MapMobs* map):map(map){}
	void run(){
		map->checkSpawn(true);
	}
};

class MobDropTimer : public Run {
private:
	Mob* mob;
public:
	int delay;
	MobDropTimer(Mob* mob, int delay):mob(mob), delay(delay) {}
	void run(){
		mob->getMap()->getDrops()->dropFromMob(mob);
		mob->setDropTimer(Timers::getInstance()->startTimer(delay, new MobDropTimer(mob, delay)));
	}
};

Mob* MapMobs::getMobByID(int id){
	for(int i=0; i<(int)mobs.size(); i++)
		if(mobs[i]->getID() == id)
			return mobs[i];
	return NULL;	
}

MapMobs::MapMobs(Map* map){
	this->map = map;
	timeron = false;

}
void MapMobs::show(Player* player){
	for(int i=0; i<(int)mobs.size(); i++){
		player->send(PacketCreator().showMob(mobs[i], MOB_SHOW));
	}
}
void MapMobs::show(Mob* mob, bool spawn){
	map->send(PacketCreator().showMob(mob, spawn ? MOB_SPAWN : MOB_SHOW));
}

void MapMobs::checkSpawn(bool timer){
	if(!map->getSpawn()){
		timeron = false;
		return;
	}
	if(map->getPlayers()->getPlayers()->empty()){
		for(int i=0; i<(int)mobs.size(); i++){
			mobs[i]->setControl(NULL, false, false);
		}
		timeron = false;
		return;
	}
	if(!timeron){
		timeron = true;
		checkSpawn(true);
		return;
	}
	if(timer){
		MapData* md = MapsData::getInstance()->getDataByID(map->getID());
		if(md == NULL)
			return;
		vector <MapMobData*>* smobs = md->getMobsData()->getData();
		for(int i=0; i<(int)smobs->size(); i++){
			bool spawned = false;
			for(int j=0; j<(int)mobs.size(); j++){
				if((*smobs)[i]->getInMapID() == mobs[j]->getOriginalID()){
					spawned = true;
					break;
				}
			}	
			if(spawned) continue;
			if(last.find(i) == last.end()) last[i] = 0;
			else if((*smobs)[i]->getDelay() == -1) continue;
			else if(last[i] + (int)((*smobs)[i]->getDelay()*1000*md->getMobRate()) > (int)GetTickCount()) continue;
			spawnMob((*smobs)[i]->getID(), (*smobs)[i]->getX(), (*smobs)[i]->getCY(), (*smobs)[i]->getInMapID(), MOB_SPAWN, -1, (*smobs)[i]->getFH(),  (*smobs)[i]->getFlip(), false);	
		}
		Timers::getInstance()->startTimer(10000, new MapMobsTimer(this));
	}
	for(int i=0; i<(int)mobs.size(); i++){
		if(mobs[i]->getControl() == NULL){
			mobs[i]->findControl(false);
		}
	}
}

void MapMobs::damage(Player* player, Mob* mob, int damage){
	if(damage > 0){
		mob->takeDamage(player, damage);
		if(mob->getDropTimer() != NULL){
			int delay = mob->getDropTimer()->otime;
			Timers::getInstance()->cancelTimer(mob->getDropTimer());	
			mob->setDropTimer(Timers::getInstance()->startTimer(delay, new MobDropTimer(mob, delay)));
		}
	
		if(mob->getHP() <= 0){
			kill(mob, player);
		}
	}
}
void MapMobs::damage(Player* player, Damage& dmg, Item* item){
	if(item != NULL){
		player->getInventories()->getInventory(INVENTORY(item->getID()))->removeItemBySlot(item->getSlot());
	}
	bool pvp = player->getPvP() && player->getMap()->getPvP() != NULL;
	vector <int>* mobs = dmg.getMobs();
	for(int i=0; i<(int)mobs->size(); i++){	
		if(pvp && map->getPlayer((*mobs)[i]) != NULL){
			Player* playern = map->getPlayer((*mobs)[i]);
			int tdamage = 0;
			vector<int>* damages = dmg.getDamageForMob((*mobs)[i]);
			for(int j=0; j<(int)damages->size(); j++){
				tdamage += (*damages)[j];
			}	
			player->getMap()->getPvP()->hitPlayer(player, playern, tdamage);
		}
		else{
			Mob* mob = getMobByID((*mobs)[i]);
			if(mob == NULL || DataProvider::getInstance()->getMobDamagedByMob(mob->getMobID())){
				continue;
			}
			int tdamage = 0;
			vector<int>* damages = dmg.getDamageForMob((*mobs)[i]);
			for(int j=0; j<(int)damages->size(); j++){
				tdamage += (*damages)[j];
			}
			if(mob->getAggressive() == 0){
				if(mob->getControl() == player)
					mob->setAggressive(1);
				else
					mob->setControl(player, true);
			}
			damage(player, mob, tdamage);
		}
	}

}
void MapMobs::kill(Mob* mob, Player* by){
	if(mob->getOriginalID() != -1){
		last[mob->getOriginalID()] = GetTickCount();
	}
	Player* control = mob->getControl();
	if(control != NULL && map->getPlayers()->find(control))
		control->send(PacketCreator().endControlMob(mob->getID()));
	if(by != NULL){
		mob->giveExp();
		map->getDrops()->dropFromMob(mob, mob->getMap()->getPlayers()->getPlayerByID(mob->getKillerID()));
		summonFromMob(mob);
	}
	mobs.erase(find(mobs.begin(), mobs.end(), mob));
	map->send(PacketCreator().killMob(mob->getID()));
	if(mob->getDropTimer() != NULL){
		Timers::getInstance()->cancelTimer(mob->getDropTimer());
	}
	delete mob;
}
void MapMobs::summonFromMob(Mob* from){
	MobData* mob = MobsData::getInstance()->getDataByID(from->getMobID());
	if(mob == NULL) return;
	vector <int>* summons = mob->getSummonData()->getData();
	for(int i=0; i<(int)summons->size(); i++){
		spawnMob((*summons)[i], from->getPosition().x, from->getPosition().y, -1, MOB_TRANSFORM, -1, from->getFoothold(), from->getStance()%2 == 0, false, from->getID());
	}
}
Mob* MapMobs::spawnMob(int mobid, int x, int y, int orgID, char type, char effect, int fh, bool flip, bool find, int from){
	MobData* data = DataProvider::getInstance()->getMobData(mobid);
	if(data == NULL)
		return NULL;
	Mob* mob = new Mob(map);
	mob->setID(ids++);
	Position pos;
	pos.x = x;
	pos.y = y;
	if(find) pos = map->findFoothold(pos);
	mob->setPosition(pos);
	mob->setMobID(mobid);
	mob->setHP(data->getMaxHP());
	mob->setMP(data->getMaxMP());
	mob->setOriginalID(orgID);
	mob->setStartFH(fh);
	mob->setFoothold(fh);
	mob->setStance(flip ? 3 : 2);
	map->send(PacketCreator().showMob(mob, type, from));
	mob->findControl(true);	
	mobs.push_back(mob);
	if(data->getDropDelay() != 0){
		mob->setDropTimer(Timers::getInstance()->startTimer(data->getDropDelay()*1000, new MobDropTimer(mob, data->getDropDelay()*1000)));
	}
	return mob;
}
void MapMobs::clear(){
	for(int i=0; i<(int)mobs.size();)
		kill(mobs[0], NULL);
	last.clear();
}
int MapMobs::getMobsCount(){
	return (int)mobs.size();
}