#ifndef MAPPLAYERS_H
#define MAPPLAYERS_H

#include <vector>

using namespace std;

#include "Players.h"
class Player;
class Map;

class MapPlayers : public Players {
private:
	vector <Player*> playersv;
	Map* map;
public:
	MapPlayers(Map* map){
		this->map = map;
	}
	void add(Player* player);
	void remove(Player* player);
	vector <Player*>* getPlayers(){
		return &playersv;
	}

};

#endif