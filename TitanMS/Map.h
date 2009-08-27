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

#ifndef MAP_H
#define MAP_H

#include <hash_map>
#include <string>
#include "Tools.h"
using namespace std;
using namespace stdext;

class MapMobs;
class MapNPCs;
class MapPlayers;
class MapPortalData;
class MapDrops;
class MapReactors;

class Drop;
class Reactor;
class NPC;
class Mob;
class PacketWriter;
class Player;
class PvP;
class Event;
class MapShipData;
class Channel;
struct Position;


class Map {
private:
	int id;
	bool spawn;
	bool ship;
	int type;
	string music;
	MapMobs* mobs;
	MapPlayers* players;
	MapNPCs* npcs;
	MapDrops* drops;
	MapReactors* reactors;
	MapShipData* shipData;
	Event* mevent;
	Channel* channel;
	PvP* pvp;
	hash_map <string, MapPortalData*> portals;
	hash_map <string, bool> portalsstatus;
public:
	Map(Channel* channel, int id);
	int getID(){
		return id;
	}
	MapPlayers* getPlayers(){
		return players;
	}
	MapMobs* getMobs(){
		return mobs;
	}
	MapNPCs* getNPCs(){
		return npcs;
	}
	MapReactors* getReactors(){
		return reactors;
	}
	MapDrops* getDrops(){
		return drops;
	}
	MapPortalData* getPortal(string name){
		if(portals.find(name) != portals.end())
			return portals[name];
		return NULL;

	}
	MapPortalData* getPortal(int pid);
	Position getPortalPosition(int pid);
	Drop* getDrop(int id);
	Mob* getMob(int id);
	NPC* getNPC(int id);
	Player* getPlayer(int id);
	Reactor* getReactor(int id);
	void addPlayer(Player* player);
	void removePlayer(Player* player);
	void send(PacketWriter* pw);
	void send(PacketWriter* pw, Player* player);
	void send(PacketWriter* pw, Player* player, Position pos);
	int getReturnMap();
	Mob* spawnMob(int id, int x, int y);
	Reactor* spawnReactor(int id, int x, int y);
	void killMob(Mob* mob);
	void destroyReactor(Reactor* reactor);
	int getMobsCount();
	Reactor* getReactorByReactorID(int id);
	void setSpawn(bool spawn);
	bool getSpawn(){
		return spawn;
	}
	void clearReactors();
	void clearDrops();
	void clearMobs();
	void clear();
	int getClosestSpawnPos(Position pos);
	Position findFoothold(Position pos, int* fh = NULL);
	void startPvP();
	void stopPvP();
	void playSound(string& name);
	void showEffect(string& name);
	void showObject(string& name);
	void setMusic(string& name);
	Position getShipPosition();
	string& getMusic(){
		return music;
	}
	PvP* getPvP(){
		return pvp;
	}
	Event* getEvent(){
		return mevent;
	}
	Channel* getChannel(){
		return channel;
	}
	void setEvent(Event* e);
	int getType(){
		return type;
	}
	bool getShip(){
		return ship;
	}
	void setShip(bool s);
	bool getPortalStatus(string& name){
		if(portalsstatus.find(name) == portalsstatus.end())
			return false;
		return portalsstatus[name];
	}
	void enablePortal(string& name){
		if(portalsstatus.find(name) != portalsstatus.end())
			portalsstatus[name] = true;
	}


};

#endif
