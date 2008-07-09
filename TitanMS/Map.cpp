#include "Map.h"
#include "MapPlayers.h"
#include "MapData.h"
#include "MapMobs.h"
#include "MapReactors.h"
#include "MapNPCs.h"
#include "MapDrops.h"
#include "MapData.h"
#include "MapsData.h"
#include "MapPortalsData.h"
#include "PacketWriter.h"
#include "DataProvider.h"
#include "Mob.h"
#include "Reactor.h"
#include "MapPortalData.h"

Map::Map(int id){
	this->id = id;
	players = new MapPlayers(this);
	mobs = new MapMobs(this);
	npcs = new MapNPCs(this);
	drops = new MapDrops(this);
	reactors = new MapReactors(this);
	this->spawn = spawn; 

	vector <MapPortalData*>* pl = MapsData::getInstance()->getDataByID(id)->getPortalsData()->getData();
	for(int i=0; i<(int)pl->size(); i++)
		portals[(*pl)[i]->getFromPortal()] = (*pl)[i];
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
Reactor* Map::getReactor(int id){
	return reactors->getReactorByID(id);
}

void Map::addPlayer(Player* player){
	players->add(player);
	mobs->show(player);
	mobs->checkSpawn();
	npcs->show(player);
	reactors->checkSpawn();
	reactors->show(player);
	drops->show(player);
	drops->checkDrops();
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
	pos.x = x;
	pos.y = y;
	pos = drops->findFoothold(pos);
	return mobs->spawnMob(id, pos.x, pos.y);
}
Reactor* Map::spawnReactor(int id, int x, int y){
	Position pos;
	pos.x = x;
	pos.y = y;
	pos = drops->findFoothold(pos);
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
void Map::killMobs(){
	mobs->killAll();
}
int Map::getMobsCount(){
	return mobs->getMobsCount();
}
Reactor* Map::getReactorByReactorID(int id){
	return reactors->getReactorByReactorID(id);
}
void Map::respawnReactors(){
	reactors->respawn();
}

void Map::setSpawn(bool spawn){
	this->spawn = spawn;
	if(spawn)
		mobs->checkSpawn();
}
void Map::clearDrops(){
	drops->clear();
}