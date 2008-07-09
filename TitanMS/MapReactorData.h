#ifndef MAPREACTORDATA_H
#define MAPREACTORDATA_H

#include "DataStruct.h"

class MapReactorData : public DataStruct {
private:
	int mapid;
	int x;
	int y;
	int delay;
public:
	MapReactorData(){
		id=-1;
		mapid=0;
		x = 0;
		y = 0;
		delay = 0;
	}
	void setInMapID(int mapid){
		this->mapid = mapid;
	}
	int getInMapID(){
		return mapid;
	}
	void setX(int x){
		this->x = x;
	}
	int getX(){
		return x;
	}
	void setY(int y){
		this->y = y;
	}
	int getY(){
		return y;
	}
	void setDelay(int delay){
		this->delay = delay;
	}
	int getDelay(){
		return delay;
	}

};

#endif