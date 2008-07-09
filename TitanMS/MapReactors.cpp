#include "Player.h"
#include "MapReactors.h"
#include "MapsData.h"
#include "MapData.h"
#include "ReactorData.h"
#include "ReactorsData.h"
#include "ReactorStateData.h"
#include "MapReactorData.h"
#include "MapReactorsData.h"
#include "Map.h"
#include "Item.h"
#include "Reactor.h"
#include "Timer.h"
#include "PacketCreator.h"
#include "DataProvider.h"
#include "MapPlayers.h"
#include "MapDrops.h"
#include "Timer.h"
#include "AngelScriptEngine.h"
#include "Drop.h"
#include "Run.h"

int MapReactors::ids = 0x200000;

class MapReactorsTimer : public Run {
private:
	MapReactors* map;
public:
	MapReactorsTimer(MapReactors* map):map(map){}
	void run(){
		map->checkSpawn(true);
	}
};

class ReactorTakeItemTimer : public Run {
private:
	Reactor* reactor;
	Drop* drop;
	Player* player;
public:
	ReactorTakeItemTimer(Reactor* reactor, Drop* drop, Player* player):reactor(reactor), drop(drop), player(player){}
	void run(){
		if(reactor->getMap()->getDrops()->findDrop(drop)){
			reactor->getMap()->getDrops()->removeDrop(drop);
			reactor->hit(NULL, 0, 0);
		}
	}

};
MapReactors::MapReactors(Map* map){
	this->map = map;
	timeron = false;
}
Reactor* MapReactors::getReactorByID(int objid){
	for(int i=0; i<(int)reactors.size(); i++)
		if(reactors[i]->getID() == objid)
			return reactors[i];
	return NULL;	
}
void MapReactors::show(Player* player){
	for(int i=0; i<(int)reactors.size(); i++){
		player->send(PacketCreator().showReactor(reactors[i]));
	}
}
void MapReactors::checkSpawn(bool timer){
	if(map->getPlayers()->getPlayers()->size() == 0){
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
		vector <MapReactorData*>* sreactors = md->getReactorsData()->getData();
		for(int i=0; i<(int)sreactors->size(); i++){
			bool spawned = false;
			for(int j=0; j<(int)reactors.size(); j++){
				if((*sreactors)[i]->getInMapID() == reactors[j]->getOriginalID()){
					spawned = true;
					break;
				}
			}	
			if(spawned) continue;
			if(last.find(i) == last.end()) last[i] = 0;
			else if(last[i] + (*sreactors)[i]->getDelay()*1000 > (int)GetTickCount()) continue;
			spawnReactor((*sreactors)[i]->getID(), (*sreactors)[i]->getX(), (*sreactors)[i]->getY(), (*sreactors)[i]->getInMapID());	
		}
		if(sreactors->size() > 0)
			Timer::getInstance()->setTimer(10000, new MapReactorsTimer(this));
	}
}
void MapReactors::show(Reactor* reactor){
	map->send(PacketCreator().showReactor(reactor));

}
void MapReactors::hitReactor(Reactor* reactor){
	hitReactor(reactor, NULL, 0, 0);

}
void MapReactors::hitReactor(Reactor* reactor, Player* player){
	hitReactor(reactor, player, 0, 0);
}
void MapReactors::hitReactor(Reactor* reactor, Player* player, int stance, int pos){
	reactor->hit(player, stance, pos);
	if(reactor->isDestroyed()){
		destroy(reactor);
	}
}
void MapReactors::destroy(Reactor* reactor){
	if(reactor->getOriginalID() != -1){
		last[reactor->getOriginalID()] = GetTickCount();
	}
	map->send(PacketCreator().destroyReactor(reactor));
	for(int i=0; i<(int)reactors.size(); i++){
		if(reactors[i] == reactor){
			reactors.erase(reactors.begin() + i);
			break;
		}
	}
	delete reactor;
}
Reactor* MapReactors::spawnReactor(int reactorid, int x, int y, int orgID){
	ReactorData* data = DataProvider::getInstance()->getReactorData(reactorid);
	if(data == NULL)
		return NULL;
	Reactor* reactor = new Reactor(map);
	reactor->setID(ids++);
	reactor->setReactorID(reactorid);
	Position pos;
	pos.x = x;
	pos.y = y;
	reactor->setPosition(pos);
	reactor->setOriginalID(orgID);
	map->send(PacketCreator().showReactor(reactor));
	reactors.push_back(reactor);
	return reactor;
}
Reactor* MapReactors::getReactorByReactorID(int id){
	for(int i=0; i<(int)reactors.size(); i++){
		if(reactors[i]->getReactorID() == id)
			return reactors[i];
	}
	return NULL;
}
void MapReactors::checkDrop(Drop* drop, Player* player){
	if(drop->isMesos()) return;
	for(int i=0; i<(int)reactors.size(); i++){
		ReactorData* rd = ReactorsData::getInstance()->getDataByID(reactors[i]->getReactorID()); 
		if(rd == NULL) continue;
		ReactorStateData* s = rd->getDataByID(reactors[i]->getState());
		if(s == NULL) continue;
		if(s->getType() == 100){ 
			if(s->getItemID() == drop->getItem()->getID() && drop->getItem()->getAmount() >= s->getAmount()){
				if(reactors[i]->inArea(drop->getPosition())){
					Timer::getInstance()->setTimer(5000, new ReactorTakeItemTimer(reactors[i], drop, player));
					break;
				}
			}
		}
	}
}
void MapReactors::respawn(){
	for(int i=0; i<(int)reactors.size(); i++){
		destroy(reactors[0]);
	}
	MapData* md = MapsData::getInstance()->getDataByID(map->getID());
	if(md == NULL)
		return;
	vector <MapReactorData*>* sreactors = md->getReactorsData()->getData();
	for(int i=0; i<(int)sreactors->size(); i++){
			spawnReactor((*sreactors)[i]->getID(), (*sreactors)[i]->getX(), (*sreactors)[i]->getY(), (*sreactors)[i]->getInMapID());	
	}
}