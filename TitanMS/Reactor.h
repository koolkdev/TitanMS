#ifndef REACTOR_H
#define REACTOR_H

#include "MapObject.h"

class Player;
class Map;

class Reactor : public MapObject {
private:
	Map* map;
	int rid;
	int state;
	bool destroyed;
	int mapid;
public:
	Reactor (Map* map){
		this->map = map;
		rid = 0;
		mapid = -1;
		state = 0;
		destroyed = false;
	}
	void setReactorID(int rid){
		this->rid = rid;
	}
	int getReactorID(){
		return rid;
	}
	void setOriginalID(int mapid){
		this->mapid=mapid;
	}
	int getOriginalID(){
		return mapid;
	}
	bool isDestroyed(){
		return destroyed;
	}
	void setDestroyed(){
		destroyed = true;
	}
	int getState(){
		return state;
	}
	Map* getMap(){
		return map;
	}
	void drop();
	void setState(int s);
	void hit(Player* player, short stance, char pos);
	bool inArea(Position pos);
};

#endif