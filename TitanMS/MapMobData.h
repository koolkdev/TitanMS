#ifndef MAPMOBDATA_H
#define MAPMOBDATA_H

#include "DataStruct.h"

class MapMobData : public DataStruct {
private:
	int mapid;
	int x;
	int cy;
	int fh;
	int delay;
	bool flip;
public:
	MapMobData(){
		id=-1;
		mapid=0;
		x = 0;
		cy = 0;
		fh = 0;
		delay = 0;
		flip = 0;
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
	void setCY(int cy){
		this->cy = cy;
	}
	int getCY(){
		return cy;
	}
	void setFH(int fh){
		this->fh = fh;
	}
	int getFH(){
		return fh;
	}
	void setDelay(int delay){
		this->delay = delay;
	}
	int getDelay(){
		return delay;
	}
	void setFlip(bool b){
		flip = b;
	}
	bool getFlip(){
		return flip;
	}

};

#endif