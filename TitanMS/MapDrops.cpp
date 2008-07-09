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

void MapDrops::show(Player* player){
	for (hash_map<int,Drop*>::iterator iter = drops.begin(); iter != drops.end(); iter++){
			player->send(PacketCreator().showDrop(iter->second));		
	}
}

void MapDrops::checkDrops(bool timer){
	if(map->getPlayers()->getPlayers()->size() == 0){
		timeron = false;
		return;
	}
	if(!timeron){
		timeron = true;
		checkDrops(true);
		return;
	}
	if(timer){
		// Check Drops
		Timer::getInstance()->setTimer(10000, new MapDropsTimer(this));
	}
}

Position MapDrops::findFoothold(Position pos){
	short x = pos.x;
	short y = pos.y-100;
	bool first=1;
	short maxy;
	MapData* md = MapsData::getInstance()->getDataByID(map->getID());
	if(md == NULL) return pos;
	vector <MapFootholdData*>* tdata = md->getFootholdsData()->getData();
	short x1, x2, y1, y2;
	for(int i=0; i<(int)tdata->size(); i++){
		MapFootholdData* data = (*tdata)[i];
		x1 = data->getX1();
		x2 = data->getX2();
		y1 = data->getY1();
		y2 = data->getY2();
		if((x>=x1 && x<=x2) || (x>=x2 && x<=x1)){
			if(first){
				if(x1 == x2){
					continue;
				}
				maxy = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(maxy >= y)
					first=0;
			}
			else{
				if(x1 == x2){
					continue;
				}
				short cmax = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(cmax < maxy && cmax >= y)
					maxy = cmax;
			}
		}
	}
	if(!first){
		Position newpos;
		newpos.x = x;
		newpos.y = maxy;
		return newpos;
	}
	return pos;
}
void MapDrops::dropFromMob(Mob* mob, int killer){
	DropsData* mdrops = MobsDropData::getInstance()->getDataByID(mob->getMobID());
	if(mdrops == NULL)
		return;
	dropFromObject(mdrops, mob, killer);

}
void MapDrops::dropFromReactor(Reactor* reactor){
	DropsData* mdrops = ReactorsDropData::getInstance()->getDataByID(reactor->getReactorID());
	if(mdrops == NULL)
		return;
	dropFromObject(mdrops, reactor, 0);

}
void MapDrops::dropFromObject(DropsData* drops, MapObject* obj, int owner){
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
			dropFromObject(item, pos, obj, owner);
			count++;
		}
		if(drops->getDropsLimit() != -1 && count >= drops->getDropsLimit())
			break;
	}
	int bags = random(drops->getMinBags(), drops->getMaxBags());
	for(int i=0; i<bags; i++){
		pos.x = obj->getPosition().x + 25*((count+count%2)/2)*((count%2) ? 1 : -1);
		pos.y = obj->getPosition().y;
		int mesos = random(drops->getMinMesos(), drops->getMaxMesos());
		if(mesos > 0)
			dropMesosFromObject(mesos, pos, obj, owner);
	}
}
void MapDrops::dropFromObject(Item* item, Position pos, MapObject* obj, int own, Player* player){
	Drop* drop = new Drop(map, item, findFoothold(pos));
	drop->setID(ids++);
	if(own > 0)
		drop->setTime(GetTickCount()+30000);
	else
		drop->setTime(0);
	drop->setOwner(own);
	drop->drop(obj);
	if(player != NULL){ // Dropped by a player...
		map->getReactors()->checkDrop(drop, player);
	}
}
void MapDrops::dropMesosFromObject(int amount, Position pos, MapObject* obj, int own){
	Drop* drop = new Drop(map);
	drop->setID(ids++);
	drop->setMesos(true);
	drop->setAmount(amount);
	drop->setPosition(findFoothold(pos));
	if(own > 0)
		drop->setTime(GetTickCount()+30000);
	else
		drop->setTime(0);
	drop->setOwner(own);
	drop->drop(obj);
}
/*
Drop* Drop::getDrop(int objid, int mapid){
	for(unsigned int i=0; i<Drops::drops[mapid].size(); i++){
		if(Drops::drops[mapid][i]->getObjID() == objid)
			return Drops::drops[mapid][i];
	}
	return NULL;
}

Pos Drops::findFoothold(Pos pos, int map){
	short x = pos.x;
	short y = pos.y-100;
	bool first=1;
	short maxy;
	for(unsigned int i=0; i<Drops::foots[map].size(); i++){
		if((x>=Drops::foots[map][i].x1 && x<=Drops::foots[map][i].x2) || (x>=Drops::foots[map][i].x2 && x<=Drops::foots[map][i].x1)){
			if(first){
				if(Drops::foots[map][i].x1 == Drops::foots[map][i].x2){
					continue;
				}
				maxy = (short) ( (float)( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) * x - Drops::foots[map][i].x1 * (float) ( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) + Drops::foots[map][i].y1 );
				if(maxy >= y)
					first=0;
			}
			else{
				if(Drops::foots[map][i].x1 == Drops::foots[map][i].x2){
					continue;
				}
				short cmax = (short) ( (float)( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) * x - Drops::foots[map][i].x1 * (float) ( Drops::foots[map][i].y1 - Drops::foots[map][i].y2 ) / ( Drops::foots[map][i].x1 - Drops::foots[map][i].x2 ) + Drops::foots[map][i].y1 );
				if(cmax < maxy && cmax >= y)
					maxy = cmax;
			}
		}
	}
	if(!first){
		Pos newpos;
		newpos.x = x;
		newpos.y = maxy;
		return newpos;
	}
	return pos;
}

void Drop::doDrop(Dropped dropped){
	setDropped(GetTickCount());
	setPos(Drops::findFoothold(getPos(), getMap()));
	if(!isQuest())
		DropsPacket::drop(Maps::info[getMap()].Players, this, dropped);
	else{
		if(Players::players.find(playerid) == Players::players.end())
			return;
		if(Players::players[playerid]->getMap() == getMap())
			DropsPacket::dropForPlayer(Players::players[playerid], this, dropped);
	}
}

void Drop::takeDrop(Player* player){
	for(unsigned int i=0; i<Drops::drops[player->getMap()].size(); i++){
		if(Drops::drops[player->getMap()][i] == this){
			Drops::drops[player->getMap()].erase(Drops::drops[player->getMap()].begin()+i);
			break;
		}
	}
	DropsPacket::takeDrop(player, Maps::info[player->getMap()].Players, this);
}

void Drop::showDrop(Player* player){
	if(isQuest() && player->getPlayerid() != playerid)
		return;
	DropsPacket::showDrop(player, this);
}

void Drop::removeDrop(){

}

void Drops::dropMob(Player* player, Mob* mob){
	MobDropsInfo drop = dropsinfo[mob->getMobID()];
	int d=0;
	for(unsigned int k=0; k<drop.size(); k++){
		if(rand()%10000 < drop[k].chance){
			if(drop[k].quest>0){
				if(!player->quests->isQuestActive(drop[k].quest))
					continue;
				int request=0;
				for(unsigned int i=0; i<Quests::quests[drop[k].quest].rewards.size(); i++){
					if(Quests::quests[drop[k].quest].rewards[i].id == drop[k].id){
						request = Quests::quests[drop[k].quest].rewards[i].count;
					}
				}
				if(player->inv->getItemAmount(drop[k].id) > request)
					continue;
			}
			Drop* drp = new Drop(player->getMap());
			if(drop[k].quest>0){
				drp->setIsQuest(1);
				drp->setPlayer(player->getPlayerid());
				drp->setQuest(drop[k].quest);
			}
			drp->setID(drop[k].id);
			drp->setAmount(1);
			drp->setOwner(player->getPlayerid());
			if(drop[k].id/1000000 == 1){
				drp->setEquip(true);
				EquipInfo ei = equips[drp->getID()];
				DropInfo dp;
				dp.scrolls = 0;
				dp.type =  ei.type;
				dp.slots = ei.slots;
				dp.scrolls = 0;
				if(ei.istr > 0)
					dp.istr = ei.istr + rand()%3-1;
				else
					dp.istr = 0;
				if(ei.idex > 0)
					dp.idex = ei.idex + rand()%3-1;
				else
					dp.idex = 0;
				if(ei.iint > 0)
					dp.iint = ei.iint + rand()%3-1;
				else
					dp.iint = 0;
				if(ei.iluk > 0)
					dp.iluk = ei.iluk + rand()%3-1;
				else
					dp.iluk = 0;
				if(ei.ihp > 0)
					dp.ihp = ei.ihp + rand()%11-5;
				else
					dp.ihp = 0;
				if(ei.imp > 0)
					dp.imp = ei.imp + rand()%11-5;
				else
					dp.imp = 0;
				if(ei.iwatk > 0)
					dp.iwatk = ei.iwatk + rand()%11-5;
				else
					dp.iwatk = 0;
				if(ei.imatk > 0)
					dp.imatk = ei.imatk + rand()%11-5;
				else
					dp.imatk = 0;
				if(ei.iwdef > 0)
					dp.iwdef = ei.iwdef + rand()%11-5;
				else
					dp.iwdef = 0;
				if(ei.imdef > 0)
					dp.imdef = ei.imdef + rand()%11-5;
				else
					dp.imdef = 0;
				if(ei.iacc > 0)
					dp.iacc = ei.iacc + rand()%3-1;
				else
					dp.iacc = 0;
				if(ei.iavo > 0)
					dp.iavo = ei.iavo + rand()%3-1;
				else
					dp.iavo = 0;
				dp.ihand = ei.ihand;
				if(ei.ijump > 0)
					dp.ijump = ei.ijump + rand()%3-1;
				else
					dp.ijump = 0;
				if(ei.ispeed > 0)
					dp.ispeed = ei.ispeed + rand()%3-1;	
				else
					dp.ispeed = 0;
				drp->setDropInfo(dp);
			}
			drp->setTime(100);
			Dropped dropper;
			dropper.id = mob->getID();
			dropper.pos = mob->getPos();
			Pos pos;
			if(d%2){
				pos.x = mob->getPos().x+25*((d+1)/2);
				pos.y = mob->getPos().y;
			}
			else{
				pos.x = mob->getPos().x-25*(d/2);
				pos.y = mob->getPos().y;
			}
			drp->setPos(pos);
			drp->setOwner(player->getPlayerid());
			drp->doDrop(dropper);
			d++;
		}
	}
	int nm = mesos[mob->getMobID()].min;
	int xm = mesos[mob->getMobID()].max;	
	if(xm > 0 && nm > 0){
		Drop* drp = new Drop(player->getMap());
		drp->setID(rand()%(xm-nm+1)+nm);
		drp->setMesos(true);
		drp->setOwner(player->getPlayerid());
		drp->setTime(100);
		Dropped dropper;
		dropper.id = mob->getID();
		dropper.pos = mob->getPos();
		Pos pos;
		if(d%2){
			pos.x = mob->getPos().x+25*((d+1)/2);
			pos.y = mob->getPos().y;
		}
		else{
			pos.x = mob->getPos().x-25*(d/2);
			pos.y = mob->getPos().y;
		}
		drp->setPos(pos);
		drp->doDrop(dropper);
	}
}

void Drops::showDrops(Player* player){
	for(unsigned int i=0; i<drops[player->getMap()].size(); i++){
		drops[player->getMap()][i]->showDrop(player);
	}
}

int getDistance(Pos a, Pos b);

void Drops::lootItem(Player* player, unsigned char*packet){
	int itemid = getInt(packet+9);
	Drop* drop = Drop::getDrop(itemid, player->getMap());
	if(drop == NULL){
		DropsPacket::dontTake(player);
		return;
	}
	if(getDistance(drop->getPos(), player->getPos()) > 300){
		player->addWarning();
	}
	if(drop->isQuest()){
		int request=0;
		for(unsigned int i=0; i<Quests::quests[drop->getQuest()].rewards.size(); i++){
			if(Quests::quests[drop->getQuest()].rewards[i].id == drop->getID()){
				request = Quests::quests[drop->getQuest()].rewards[i].count;
			}
		}
		if(player->inv->getItemAmount(drop->getID()) > request || !player->quests->isQuestActive(drop->getQuest())){
			DropsPacket::takeNote(player, 0, 0, 0);
			DropsPacket::dontTake(player);
			return;
		}
	}
	if(drop->getMesos()){
		player->inv->setMesos(player->inv->getMesos() + drop->getID(), 1);
		DropsPacket::takeNote(player, drop->getID(), 1, 0);
	}
	else{
		if(drop->getEquip()){
			Equip* equip = new Equip;
			DropInfo ei = drop->getDropInfo();
			equip->id = drop->getID();
			equip->slots = ei.slots;
			equip->scrolls = ei.scrolls;
			equip->type = ei.type;
			equip->pos = Inventory::findSlot(player, drop->getID(), 1, 1);
			if(equip->pos == 0){
				DropsPacket::takeNote(player, 0, 0, 0);
				DropsPacket::dontTake(player);
				delete equip;
				return;
			}
			equip->istr = ei.istr;
			equip->idex = ei.idex;
			equip->iint = ei.iint;
			equip->iluk = ei.iluk;
			equip->ihp = ei.ihp;
			equip->imp = ei.imp;
			equip->iwatk = ei.iwatk;
			equip->imatk = ei.imatk;
			equip->iwdef = ei.iwdef;
			equip->imdef = ei.imdef;
			equip->iacc = ei.iacc;
			equip->iavo = ei.iavo;
			equip->ihand = ei.ihand;
			equip->ijump = ei.ijump;
			equip->ispeed = ei.ispeed;	
			Inventory::addEquip(player, equip, 1);
		}
		else{
			char type = Drops::items[drop->getID()].type;
			Item* newitem = new Item;
			newitem->amount = drop->getAmount();
			newitem->id = drop->getID();
			newitem->inv = type;
			newitem->pos = Inventory::findSlot(player, drop->getID() , type, drop->getAmount());
			if(newitem->pos == 0){
				DropsPacket::takeNote(player, 0, 0, 0);
				DropsPacket::dontTake(player);	
				delete newitem;
				return;
			}
			Inventory::addItem(player, newitem, 1);
		}
		DropsPacket::takeNote(player, drop->getID(), 0, drop->getAmount());
	}
	drop->takeDrop(player);
	delete drop;
}

void Drops::dropMesos(Player* player, unsigned char* packet){
	int amount = getInt(packet+4);
	if(amount < 10 || amount >50000){
		// hacking
		return;
	}
	player->inv->setMesos(player->inv->getMesos()-amount, 1);
	Drop* drop = new Drop(player->getMap());
	drop->setID(amount);
	drop->setMesos(true);
	drop->setOwner(player->getPlayerid());
	drop->setTime(0);
	Dropped dropper;
	dropper.id = player->getPlayerid();
	dropper.pos = player->getPos();
	drop->setPos(player->getPos());
	drop->doDrop(dropper);
}

void Drops::checkDrops(int mapid){
	int t = GetTickCount() - 60000;
	for(unsigned int i=0; i<drops[mapid].size(); i++){
		if(drops[mapid][i]->getDropped() < t){
			DropsPacket::removeDrop(Maps::info[mapid].Players, drops[mapid][i]);
			drops[mapid].erase(drops[mapid].begin()+i);
			i--;
		}
	}
}*/