#ifndef MAPNPCS_H
#define MAPNPCS_H

#include <vector>

using namespace std;

class NPC;
class Map;
class Player;

class MapNPCs {
private:
	Map* map;
	vector <NPC*> npcs;
public:
	MapNPCs(Map* map);
	NPC* getNPCByID(int objid);
	void show(Player* player);

};

#endif