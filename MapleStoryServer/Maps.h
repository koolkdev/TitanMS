#ifndef MAPS_H
#define MAPS_H

#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

class Player;



struct PortalInfo {
	int id;
	char from[10];
	int toid;
	char to[10];
	int type;
	short x;
	short y;
};
typedef vector<PortalInfo> PortalsInfo;

struct MapInfo {
	int rm;
	PortalsInfo Portals;
	vector<Player*> Players;
};

class Maps {
public:
	static hash_map <int, MapInfo> info;
	static void addMap(int id, MapInfo map);
	static void addPlayer(Player* player);
	static void moveMap(Player* player, unsigned char* packet);
	static void removePlayer(Player* player);
	static void changeMap(Player* player, int mapid, int pos);
};

#endif