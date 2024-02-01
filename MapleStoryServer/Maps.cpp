#include "Maps.h"
#include "Player.h"
#include "MapPacket.h"
#include "NPCs.h"
#include "Mobs.h"
#include "Drops.h"
#include "Timer.h"


hash_map <int, MapInfo> Maps::info;
short getShort(unsigned char* buf);
int getInt(unsigned char* buf);
void getString(unsigned char* buf, int len, char* out);

class MapTimer: public Timer::TimerHandler {
public:
	void setMapTimer(int mapid){
		if(ctimer.find(mapid) != ctimer.end())
			if(ctimer[mapid])
			return;
		Maps::mapTimer(mapid);
		timers[Timer::timer->setTimer(10000, this)] = mapid;
		ctimer[mapid] = 1;
	}
	void next(int mapid, int is){
		if(is)
			timers[Timer::timer->setTimer(10000, this)] = mapid;
		else
			ctimer[mapid] = 0;
	}
private:
	static hash_map <int, int> timers;
	static hash_map <int, int> ctimer;
	void handle (Timer* timer, int id) {
		Maps::mapTimer(timers[id]);
	}
	void remove (int id){
		for (hash_map<int, int>::iterator iter = timers.begin();
			 iter != timers.end(); iter++){
			if(iter->first == id){
				timers.erase(iter);
				break;
			 }
		}
	}
};
hash_map <int,int> MapTimer::timers;
hash_map <int,int> MapTimer::ctimer;
MapTimer* Maps::timer;

void Maps::addMap(int id, MapInfo map){
	info[id] = map;
}

void Maps::addPlayer(Player* player){
	if(player->getMap() == 1 || player->getMap() == 2)
		MapPacket::makeApple(player);
	info[player->getMap()].Players.push_back(player);
	MapPacket::showPlayers(player, info[player->getMap()].Players);
	MapPacket::showPlayer(player, info[player->getMap()].Players);
}

void Maps::removePlayer(Player* player){
	for(unsigned int i=0; i<info[player->getMap()].Players.size(); i++)
		if(info[player->getMap()].Players[i]->getPlayerid() == player->getPlayerid()){
			info[player->getMap()].Players.erase(info[player->getMap()].Players.begin()+i);
		}
	MapPacket::removePlayer(player, info[player->getMap()].Players);
	for(unsigned int i=0; i<Mobs::mobs[player->getMap()].size(); i++)
		if(Mobs::mobs[player->getMap()][i]->getControl() == player)
			Mobs::mobs[player->getMap()][i]->setControl(NULL);
	Mobs::updateSpawn(player->getMap());
}

void Maps::moveMap(Player* player, unsigned char* packet){
	if(getInt(packet+1) == 0){
		int tomap;
		if(info.find(player->getMap())==info.end())
			tomap = player->getMap();
		else
			tomap = info[player->getMap()].rm;
		player->setHP(50, 0);
		changeMap(player, tomap, 0); // TODO - Random
		return;
	}
	int portalsize = packet[5];
	char portalname[10];
	getString(packet+7, portalsize, portalname);;   
	PortalInfo portal;
	for(unsigned int i=0; i<info[player->getMap()].Portals.size(); i++)
		if(strcmp(info[player->getMap()].Portals[i].from, portalname) == 0){
			portal = info[player->getMap()].Portals[i];
			break;
		}
	int tonum = 0;
	if(info.find(portal.toid) != info.end()){
		for(unsigned int i=0; i<info[portal.toid].Portals.size(); i++){
			if(strcmp(portal.to, info[portal.toid].Portals[i].from) ==0){
				tonum = info[portal.toid].Portals[i].id;
				break;
			}
		}
	}
	changeMap(player, portal.toid, tonum);
}

void Maps::changeMap(Player* player, int mapid, int pos){
	removePlayer(player);
	player->setMap(mapid);
	player->setMappos(pos);
	player->setType(0);
	Pos cpos;
	if((unsigned int)pos<info[mapid].Portals.size()){
		cpos.x = info[mapid].Portals[pos].x;
		cpos.y = info[mapid].Portals[pos].y;
	}
	else if(info[mapid].Portals.size() > 0){
		cpos.x = info[mapid].Portals[0].x;
		cpos.y = info[mapid].Portals[0].y;
	}
	else{
		cpos.x = 0;
		cpos.y = 0;
	}
	player->setPos(cpos);
	MapPacket::changeMap(player);
	newMap(player);
}

void Maps::mapTimer(int mapid){
	if(info[mapid].Players.size() == 0){
		timer->next(mapid, 0);
		return;
	}
	else
		timer->next(mapid, 1);
	Mobs::checkSpawn(mapid);
	Drops::checkDrops(mapid);
}
void Maps::startTimer(){
	timer = new MapTimer();
}

void Maps::newMap(Player* player){
	Players::addPlayer(player);
	NPCs::showNPCs(player);
	addPlayer(player);
	Mobs::showMobs(player);
	Drops::showDrops(player);
	timer->setMapTimer(player->getMap());
}