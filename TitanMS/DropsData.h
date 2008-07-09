#ifndef DROPSDATA_H
#define DROPSDATA_H


#include "TopDataStruct.h"

class DropData;

class DropsData: public TopDataStruct <DropData>, public DataStruct {
private:
	int dropslimit;
	int maxmesos;
	int minmesos;
	int maxbags;
	int minbags;
public:
	DropsData(int id){
		this->id = id;
		dropslimit=-1;
		maxmesos=0;
		minmesos=0;
		maxbags=1;
		minbags=1;
	}
	void setDropsLimit(int dl){
		dropslimit = dl;
	}
	int getDropsLimit(){
		return dropslimit;
	}
	void setMaxMesos(int max){
		maxmesos = max;
	}
	void setMinMesos(int min){
		minmesos = min;
	}
	int getMaxMesos(){
		return maxmesos;
	}
	int getMinMesos(){
		return minmesos;
	}
	void setMaxBags(int max){
		maxbags = max;
	}
	void setMinBags(int min){
		minbags = min;
	}
	int getMaxBags(){
		return maxbags;
	}
	int getMinBags(){
		return minbags;
	}
	 
};

#endif