#ifndef PETCHATCOMMANDDATA_H
#define PETCHATCOMMANDDATA_H

#include "DataStruct.h"

class PetCommandData : public DataStruct {
private:
	int closeness;
	int prob;
	int minLevel;
	int maxLevel;
public:
	PetCommandData(int id){
		this->id = id;
		closeness = 0;
		prob = 0;
		minLevel = 0;
		maxLevel = 0;
	}
	void setCloseness(int c){
		closeness = c;
	}
	void setProb(int p){
		prob = p;
	}
	void setMinLevel(int l){
		minLevel = l;
	}
	void setMaxLevel(int l){
		maxLevel = l;
	}
	int getCloseness(){
		return closeness;
	}
	int getProb(){
		return prob;
	}
	int getMinLevel(){
		return minLevel;
	}
	int getMaxLevel(){
		return maxLevel;
	}
	

};

#endif