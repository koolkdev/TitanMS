#ifndef MAPFOOTHOLDDATA_H
#define MAPFOOTHOLDDATA_H

#include "DataStruct.h"

class MapFootholdData : public DataStruct {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	MapFootholdData(){
		id=-1;
		x1 = 0;
		y1 = 0;
		x2 = 0;
		y2 = 0;
	}

	void setX1(int x1){
		this->x1 = x1;
	}
	int getX1(){
		return x1;
	}
	void setY1(int y1){
		this->y1 = y1;
	}
	int getY1(){
		return y1;
	}
	void setX2(int x2){
		this->x2 = x2;
	}
	int getX2(){
		return x2;
	}
	void setY2(int y2){
		this->y2 = y2;
	}
	int getY2(){
		return y2;
	}
};

#endif