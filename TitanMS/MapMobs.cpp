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

class MobKillDelay : public Run {
private:
	Map* map;
	Mob* mob;
public:
	MobKillDelay(Map* map, Mob* mob):map(map), mob(mob){}
	void run(){
		map->getMobs()->summonFromMob(mob->getID(), mob->getPosition().x, mob->getPosition().y);
		delete mob;
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
		player->send(PacketCreator().showMob(mobs[i], false));
	}
}
void MapMobs::show(Mob* mob, bool spawn){
	map->send(PacketCreator().showMob(mob, spawn));
}

void MapMobs::checkSpawn(bool timer){
	if(!map->getSpawn()){
		timeron = false;
		return;
	}
	if(map->getPlayers()->getPlayers()->size() == 0){
		for(int i=0; i<(int)mobs.size(); i++){
			mobs[i]->setControl(NULL, false, false, false);	
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
			else if(last[i] + (*smobs)[i]->getDelay()*1000 > (int)GetTickCount()) continue;
			spawnMob((*smobs)[i]->getID(), (*smobs)[i]->getX(), (*smobs)[i]->getCY(), (*smobs)[i]->getInMapID(), true, (*smobs)[i]->getFH(),  (*smobs)[i]->getFlip());	
		}
		Timer::getInstance()->setTimer(10000, new MapMobsTimer(this));
	}
	for(int i=0; i<(int)mobs.size(); i++){
		if(mobs[i]->getControl() == NULL || mobs[i]->getControl()->getMap() != map){
			mobs[i]->findControl(false);
		}
	}
}

void MapMobs::damage(Player* player, Mob* mob, int damage){
	if(damage > 0){
		mob->takeDamage(player, damage);
	
		if(mob->getHP() <= 0){
			kill(mob, player);
		}
	}
}
void MapMobs::damage(Player* player, Damage* dmg, Item* item){
	vector <int>* mobs = dmg->getMobs();
	for(int i=0; i<(int)mobs->size(); i++){	
		Mob* mob = getMobByID((*mobs)[i]);
		if(mob == NULL)
			continue;
		int tdamage = 0;
		vector<int>* damages = dmg->getDamageForMob((*mobs)[i]);
		for(int j=0; j<(int)damages->size(); j++){
			tdamage += (*damages)[j];
		}
		damage(player, mob, tdamage);
	}

}
void MapMobs::kill(Mob* mob, Player* by){
	if(mob->getOriginalID() != -1){
		last[mob->getOriginalID()] = GetTickCount();
	}
	map->send(PacketCreator().killMob(mob->getID()));
	mob->giveExp();
	map->getDrops()->dropFromMob(mob, mob->getKillerID());
	//if( TODO, only if needed
	Timer::getInstance()->setTimer(DataProvider::getInstance()->getMobSummonDelay(mob->getMobID()), new MobKillDelay(map, mob)); 
	for(int i=0; i<(int)mobs.size(); i++){
		if(mobs[i] == mob){
			mobs.erase(mobs.begin() + i);
			break;
		}
	}
}
void MapMobs::summonFromMob(int mobid, int x, int y){
	MobData* mob = MobsData::getInstance()->getDataByID(mobid);
	if(mob == NULL) return;
	vector <int> summons = mob->getSummonData()->getMobs();;
	for(int i=0; i<(int)summons.size(); i++){
		spawnMob(summons[i], x, y);
	}
}
Mob* MapMobs::spawnMob(int mobid, int x, int y, int orgID, bool spawneffect, int fh, bool flip){
	MobData* data = DataProvider::getInstance()->getMobData(mobid);
	if(data == NULL)
		return NULL;
	Mob* mob = new Mob(map);
	mob->setID(ids++);
	Position pos;
	pos.x = x;
	pos.y = y;
	mob->setPosition(pos);
	mob->setMobID(mobid);
	mob->setHP(data->getMaxHP());
	mob->setMP(data->getMaxHP());
	mob->setOriginalID(orgID);
	mob->setFH(0);
	mob->setStance(flip ? 3 : 2);
	map->send(PacketCreator().showMob(mob, spawneffect));
	mob->findControl(true);	
	mobs.push_back(mob);
	return mob;
}
void MapMobs::killAll(){
	for(int i=0; i<(int)mobs.size(); i++)
		kill(mobs[0], NULL);
}
int MapMobs::getMobsCount(){
	return (int)mobs.size();
}
/*
int getDistance(Pos a, Pos b){
	return (int)sqrt((double)(pow((double)(a.x-b.x), 2.0) + pow((double)(a.y-b.y), 2.0)));
}

void Mob::setControl(Player* control){
	if(this->control != NULL)
		MobsPacket::endControlMob(this->control, this);

	this->control = control;
	if(control != NULL)
		MobsPacket::controlMob(control, this);
}

void Mobs::monsterControl(Player* player, unsigned char* packet, int size){
	int mobid = getInt(packet);
	Mob* mob = getMobByID(mobid, player->getMap());
	if(mob == NULL)
		return;	
	if(mob->getControl() == player){
	//if(mob->getControl() != player)
		//mob->setControl(player);
		Pos cpos;
		cpos.x = getShort(packet+size-4);
		cpos.y = getShort(packet+size-2);
		if(getDistance(cpos, mob->getPos()) > 300){
			player->addWarning();
		}
		mob->setPos(cpos);
		mob->setType(packet[size-12]);
		MobsPacket::moveMob(player, mob, Maps::info[player->getMap()].Players, packet, size);
	}
}

Player* Mobs::getClosetPlayer(int mapid, Mob* mob){
	if(Maps::info[mapid].Players.size() > 0){
		int maxpos = distPos(mob->getPos(), Maps::info[mapid].Players[0]->getPos());
		int posi = 0;
		for(unsigned int k=0; k<Maps::info[mapid].Players.size(); k++){
			int curpos = distPos(mob->getPos(), Maps::info[mapid].Players[k]->getPos());
			if(curpos < maxpos){
				maxpos = curpos;
				posi = k;
			}
		}
		return Maps::info[mapid].Players[posi];
	}
	return NULL;
}
void Mobs::checkSpawn(int mapid){
	for(unsigned int i=0; i<info[mapid].size(); i++){
		int check=0;
		for(unsigned int j=0; j<mobs[mapid].size(); j++){
			if(i == mobs[mapid][j]->getMapID()){
				check=1;
				break;
			}
		}
		if(!check){
			spawnMob(mapid, i);
		}
		else{
			Mob* mob = NULL;
			for(unsigned int j=0; j<mobs[mapid].size(); j++)
				if(i == mobs[mapid][j]->getMapID()){
					mob = mobs[mapid][j];
					break;
				}

			if(mob == NULL)
				continue;
			if(mob->getControl() == NULL ){
				mob->setControl(getClosetPlayer(mapid, mob));
			}
		}
	}
}	

void Mobs::showMobs(Player* player){
	updateSpawn(player->getMap()); 
	for(unsigned int i=0; i<mobs[player->getMap()].size(); i++){
		MobsPacket::showMob(player, mobs[player->getMap()][i]);
	}
}

void Mobs::updateSpawn(int mapid){
	for(unsigned int i=0; i<mobs[mapid].size(); i++){
		if(Maps::info[mapid].Players.size() > 0 && mobs[mapid][i]->getControl()==NULL){
			int maxpos = distPos(mobs[mapid][i]->getPos(), Maps::info[mapid].Players[0]->getPos());
			int posi = 0;
			for(unsigned int k=0; k<Maps::info[mapid].Players.size(); k++){
				int curpos = distPos(mobs[mapid][i]->getPos(), Maps::info[mapid].Players[k]->getPos());
				if(curpos < maxpos){
					maxpos = curpos;
					posi = k;
				}
			}
			mobs[mapid][i]->setControl(Maps::info[mapid].Players[posi]);
		}
		else if(Maps::info[mapid].Players.size() == 0)
			mobs[mapid][i]->setControl(NULL);
	}
}
void Mobs::dieMob(Player* player, Mob* mob){
	MobsPacket::dieMob(player, Maps::info[player->getMap()].Players, mob , mob->getID());
	/*
		for(unsigned int l=0; l< WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].summon.size(); l++){
		SpawnedInfo mob;
		mob.id = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].summon[l];
			mob.spid = -1;
		mob.mid = WZInitialize::mobid++;
		mob.x = WZInitialize::MapsInfo[pd.map].Spawned[j].x;
		mob.cy = WZInitialize::MapsInfo[pd.map].Spawned[j].cy;	mob.fh = WZInitialize::MapsInfo[pd.map].Spawned[j].fh;
							mob.hp = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].hp;
							mob.mp = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].mp;
							mob.by = pd.userid;
							SpawnMob(mob, pd);
						}
						*//*
	Levels::giveEXP(player, mobinfo[mob->getMobID()].exp * 10);
	Drops::dropMob(player, mob);
	player->quests->updateQuestMob(mob->getMobID());
	for(unsigned int i=0; i<mobs[player->getMap()].size(); i++){
		if(mobs[player->getMap()][i] == mob){
			mobs[player->getMap()].erase(mobs[player->getMap()].begin()+i);
			break;
		}
	}
	delete mob;
}

Mob* Mobs::getMobByID(int mobid, int map){
	for(unsigned int i=0; i<mobs[map].size(); i++){
		if(mobs[map][i]->getID() == mobid){
			return mobs[map][i];
		}
	}
	return NULL;
}
void Mobs::damageMobSkill(Player* player, unsigned char* packet){
	MobsPacket::damageMobSkill(player, Maps::info[player->getMap()].Players, packet);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	int map = player->getMap();
	int skillid = getInt(packet+2);
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
		int mobid = getInt(packet+14+i*(22+4*(hits-1)));
		Mob* mob = getMobByID(mobid, map);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+32+i*(22+4*(hits-1))+k*4);
			if(mob!=NULL){
				mob->setHP(mob->getHP()-damage);
				int mhp=-1;
				mhp = mobinfo[mob->getMobID()].hp;
				MobsPacket::showHP(player, mobid ,mob->getHP()*100/mhp);
				if(mob->getHP() <= 0){
					dieMob(player, mob);
					break;
				}
			}
		}
	}	
}

void Mobs::damageMob(Player* player, unsigned char* packet){
	MobsPacket::damageMob(player, Maps::info[player->getMap()].Players, packet);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	int map = player->getMap();
	int skillid = getInt(packet+2);
	bool s4211006 = false;
	if(skillid == 4211006)
		s4211006 = true;
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
        int mobid = getInt(packet+14+i*(22-s4211006+4*(hits-1)));
		Mob* mob = getMobByID(mobid, map);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+32-s4211006+i*(22-s4211006+4*(hits-1))+k*4);
			if(mob!=NULL){
				if(getDistance(mob->getPos(), player->getPos()) > 300 && skillid == 0){
					player->addWarning();
				}
				mob->setHP(mob->getHP()-damage);
				int mhp=-1;
				mhp = mobinfo[mob->getMobID()].hp;
				MobsPacket::showHP(player, mobid ,mob->getHP()*100/mhp);
				if(mob->getHP() <= 0){
					dieMob(player, mob);
					break;
				}
			}
		}
	}
	if (s4211006){
        unsigned char howmanyitems = packet[(14+howmany*(21+4*(hits-1)))+4];
        for(int i=0; i<howmanyitems; i++){
            int objID = getInt(packet+(14+howmany*(21+4*(hits-1)))+5+(5*i));
            for(unsigned int i=0; i<Drops::drops[map].size(); i++){
                if(Drops::drops[map][i]->getObjID() == objID){
                    DropsPacket::explodeDrop(Maps::info[map].Players, Drops::drops[map][i]);
                    Drops::drops[map].erase(Drops::drops[map].begin()+i);
                    break;
                }
            }
        }
    }
}
void Mobs::damageMobS(Player* player, unsigned char* packet, int size){
	int itemid=0;
	int pos=getInt(packet+14);
	for(int i=0; i<player->inv->getItemNum(); i++){
		if(player->inv->getItem(i)->pos == pos && player->inv->getItem(i)->inv == 2){
			itemid = player->inv->getItem(i)->id;
			break;
		}
	}
	MobsPacket::damageMobS(player, Maps::info[player->getMap()].Players, packet, itemid);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	//Inventory::takeItemSlot(player, pos, 2, hits); // TODO
	int map = player->getMap();
	int skillid = getInt(packet+2);
	bool s3121004 = false;
	if(skillid == 3121004 || skillid == 3221001)
		s3121004 = true;
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
		int mobid = getInt(packet+19+4*s3121004+i*(22+4*(hits-1)));
		Mob* mob = getMobByID(mobid, map);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+37+4*s3121004+i*(22+4*(hits-1))+k*4);
			if(mob!=NULL){
				mob->setHP(mob->getHP()-damage);
				int mhp=-1;
				mhp = mobinfo[mob->getMobID()].hp;
				MobsPacket::showHP(player, mobid ,mob->getHP()*100/mhp);
				if(mob->getHP() <= 0){
					dieMob(player, mob);
					break;
				}
			}
		}
	}	
}

void Mobs::spawnMob(Player* player, int mobid){
	Mob* mob = new Mob();
	mobs[player->getMap()].push_back(mob);
	mob->setPos(player->getPos());
	mob->setID(mobscount++);
	mob->setMobID(mobid);
	mob->setMapID(-1);
	mob->setHP(mobinfo[mobid].hp);
	mob->setMP(mobinfo[mobid].mp);
	mob->setFH(0);
	mob->setType(2);
	mob->setControl(player);
	for(unsigned int j=0; j<Maps::info[player->getMap()].Players.size(); j++)
		MobsPacket::showMob(Maps::info[player->getMap()].Players[j], mob);
}

void Mobs::spawnMob(int mapid, int mobnum){
	Mob* mob = new Mob();
	mobs[mapid].push_back(mob);
	Pos mobpos;
	mobpos.x = info[mapid][mobnum].x;
	mobpos.y = info[mapid][mobnum].cy;
	mob->setPos(mobpos);
	mob->setID(mobscount++);
	mob->setMobID(info[mapid][mobnum].id);
	mob->setMapID(mobnum);
	mob->setHP(mobinfo[info[mapid][mobnum].id].hp);
	mob->setMP(mobinfo[info[mapid][mobnum].id].mp);
	mob->setFH(info[mapid][mobnum].fh);
	mob->setType(2); 
	MobsPacket::spawnMob(NULL, mob, Maps::info[mapid].Players, 1);
	mob->setControl(getClosetPlayer(mapid, mob));
}

void Mobs::spawnMobPos(Player* player, int mobid, int xx, int yy){
    Mob* mob = new Mob();
    mobs[player->getMap()].push_back(mob);
    Pos spawnPos;
    spawnPos.x = xx;
    spawnPos.y = yy;
    mob->setPos(spawnPos);
    mob->setID(mobscount++);
    mob->setMobID(mobid);
    mob->setMapID(-1);
    mob->setHP(mobinfo[mobid].hp);
    mob->setMP(mobinfo[mobid].mp);
    mob->setFH(0);
    mob->setControl(player);
    mob->setType(2);
	for(unsigned int j=0; j<Maps::info[player->getMap()].Players.size(); j++)
		MobsPacket::showMob(Maps::info[player->getMap()].Players[j], mob);
}*/