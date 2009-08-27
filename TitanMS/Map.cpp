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

#include "DataProvider.h"
#include "Map.h"
#include "MapPlayers.h"
#include "MapData.h"
#include "MapMobs.h"
#include "MapReactors.h"
#include "MapNPCs.h"
#include "MapDrops.h"
#include "MapsData.h"
#include "MapData.h"
#include "MapFootholdsData.h"
#include "MapFootholdData.h"
#include "MapPortalsData.h"
#include "PacketCreator.h"
#include "DataProvider.h"
#include "PvP.h"
#include "Event.h"
#include "Mob.h"
#include "Reactor.h"
#include "MapPortalData.h"
using namespace Tools;

Map::Map(Channel* channel, int id){
	this->id = id;
	this->channel = channel;
	players = new MapPlayers(this);
	mobs = new MapMobs(this);
	npcs = new MapNPCs(this);
	drops = new MapDrops(this);
	reactors = new MapReactors(this);
	this->spawn = spawn; 

	pvp = NULL;
	mevent = NULL;
	type = MapsData::getInstance()->getDataByID(id)->getFieldType();
	music = MapsData::getInstance()->getDataByID(id)->getMusic();

	ship = false;
	shipData = MapsData::getInstance()->getDataByID(id)->getShip();

	vector <MapPortalData*>* pl = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getData();
	for(int i=0; i<(int)pl->size(); i++){
		portals[(*pl)[i]->getFromPortal()] = (*pl)[i];
		if((*pl)[i]->getType() == 5) portalsstatus[(*pl)[i]->getFromPortal()] = false;
	}
}

Drop* Map::getDrop(int id){
	return drops->getDropByID(id);
}
Mob* Map::getMob(int id){
	return mobs->getMobByID(id);
}
NPC* Map::getNPC(int id){
	return npcs->getNPCByID(id);
}
Player* Map::getPlayer(int id){
	return players->getPlayerByID(id);
}
Reactor* Map::getReactor(int id){
	return reactors->getReactorByID(id);
}

void Map::addPlayer(Player* player){
	players->add(player);
	mobs->show(player);
	mobs->checkSpawn();
	npcs->show(player);
	reactors->show(player);
	reactors->checkSpawn();
	drops->show(player);
	drops->checkDrops();
	if(pvp != NULL)
		pvp->showPvP(player);
}
void Map::removePlayer(Player* player){
	players->remove(player);
	mobs->checkSpawn();
}

Position Map::getPortalPosition(int pid){
	Position p;
	p.x = 0;
	p.y = 0;
	MapPortalData* pd = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getDataByID(pid);
	if(pd != NULL){
		p.x = pd->getX();
		p.y = pd->getY();
	}
	return p;
}
MapPortalData* Map::getPortal(int pid){
	return MapsData::getInstance()->getDataByID(id)->getPortalsData()->getDataByID(pid);
}
void Map::send(PacketWriter* pw){
	players->send(pw);
}
void Map::send(PacketWriter* pw, Player* player){
	players->send(pw, player);
}
void Map::send(PacketWriter* pw, Player* player, Position pos){
	players->send(pw, player, pos);
}
int Map::getReturnMap(){
	return DataProvider::getInstance()->getReturnMap(id);
}

Mob* Map::spawnMob(int id, int x, int y){
	Position pos;
	int fh;
	pos.x = x;
	pos.y = y;
	pos = findFoothold(pos, &fh);
	return mobs->spawnMob(id, pos.x, pos.y, -1, -1, -1, fh);
}
Reactor* Map::spawnReactor(int id, int x, int y){
	Position pos;
	pos.x = x;
	pos.y = y;
	return reactors->spawnReactor(id, pos.x, pos.y);
}
void Map::killMob(Mob* mob){
	if(mob->getMap() == this)
		mobs->kill(mob, NULL);
}
void Map::destroyReactor(Reactor* reactor){
	if(reactor->getMap() == this)
		reactors->destroy(reactor);
}
int Map::getMobsCount(){
	return mobs->getMobsCount();
}
Reactor* Map::getReactorByReactorID(int id){
	return reactors->getReactorByReactorID(id);
}

void Map::setSpawn(bool spawn){
	this->spawn = spawn;
	if(spawn)
		mobs->checkSpawn();
}
void Map::clearDrops(){
	drops->clear();
}
void Map::clearReactors(){
	reactors->clear();
}
void Map::clearMobs(){
	mobs->clear();
}
void Map::clear(){
	clearMobs();
	clearDrops();
	clearReactors();

	for(hash_map<string, bool>::iterator iter = portalsstatus.begin(); iter != portalsstatus.end(); iter++)
		iter->second = false;
}
int Map::getClosestSpawnPos(Position pos){
	vector <MapPortalData*>* pl = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getData();
	if(pl->size() <= 0) return 0;
	Position ppos;
	ppos.x = (*pl)[0]->getX();
	ppos.y = (*pl)[0]->getY();
	int close = 0; 
	int closed = getDistance(ppos, pos);
	for(int i=1; i<(int)pl->size(); i++){
		if((*pl)[i]->getType() == 0){
			ppos.x = (*pl)[i]->getX();
			ppos.y = (*pl)[i]->getY();
			int dis = getDistance(ppos, pos);
			if(dis < closed){
				close = i;
				closed = dis;
			}
		}
	}
	return close;
}
Position Map::findFoothold(Position pos, int* fh){
	short x = pos.x;
	short y = pos.y;
	bool first=1;
	short maxy;
	MapData* md = MapsData::getInstance()->getDataByID(id);
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
				if(fh != NULL)
					*fh = data->getID();
				if(maxy >= y)
					first=0;
			}
			else{
				if(x1 == x2){
					continue;
				}
				short cmax = (short) ( (double)( y1 - y2 ) / ( x1 - x2 ) * (x - x1) + y1 );
				if(cmax < maxy && cmax >= y){
					if(fh != NULL)
						*fh = data->getID();
					maxy = cmax;
				}
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

void Map::setMusic(string& name){
	music = name;
	send(PacketCreator().changeMusic(music));
}

void Map::startPvP(){
	if(pvp == NULL){
		pvp = new PvP(this);
		pvp->startPvP();
	}
}

void Map::stopPvP(){
	if( pvp == NULL) return;

	pvp->endPvP();
	delete pvp;
	pvp = NULL;
}

void Map::setShip(bool s){
	if(shipData != NULL){
		ship = s;
		send(PacketCreator().showShip((shipData->getType() == 0) ? (s ? 0 : 2) : (s ? 4 : 5), shipData->getType()));
	}
}
void Map::setEvent(Event* e){
	this->mevent = e;
	if(e != NULL){
		send(PacketCreator().showTimer(e->getTime()));
	}
}
Position Map::getShipPosition(){
	Position pos;
	if(shipData == NULL){
		pos.x = 0;
		pos.y = 0;
		return pos;
	}
	pos.x = shipData->getX();
	pos.y = shipData->getY();
	return pos;
}
void Map::playSound(string& name){
	send(PacketCreator().playSound(name));
}
void Map::showEffect(string& name){
	send(PacketCreator().showEffect(name));
}
void Map::showObject(string& name){
	send(PacketCreator().showObject(name));
}