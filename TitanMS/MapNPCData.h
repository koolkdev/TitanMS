#ifndef MAPNPCDATA_H
#define MAPNPCDATA_H

#include "DataStruct.h"

class MapNPCData : public DataStruct {
private:
	int x;
	int cy;
	int fh;
	int rx0;
	int rx1;
	bool flip;
public:
	MapNPCData(){
		id=-1;
		x = 0;
		cy = 0;
		fh = 0;
		rx0 = 0;
		rx1 = 0;
		flip = 0;
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
	void setRX0(int rx0){
		this->rx0 = rx0;
	}
	int getRX0(){
		return rx0;
	}
	void setRX1(int rx1){
		this->rx1 = rx1;
	}
	int getRX1(){
		return rx1;
	}
	void setFlip(bool f){
		flip = f;
	}
	bool getFlip(){
		return flip;
	}

};

#endif