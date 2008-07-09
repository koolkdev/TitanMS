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
struct Position;


class Map {
private:
	int id;
	bool spawn;
	MapMobs* mobs;
	MapPlayers* players;
	MapNPCs* npcs;
	MapDrops* drops;
	MapReactors* reactors;
	hash_map <string, MapPortalData*> portals;
public:
	Map(int id);
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
	void killMobs();
	int getMobsCount();
	Reactor* getReactorByReactorID(int id);
	void setSpawn(bool spawn);
	bool getSpawn(){
		return spawn;
	}
	void respawnReactors();
	void clearDrops();


};

#endif