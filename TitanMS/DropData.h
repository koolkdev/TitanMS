#ifndef DROPDATA_H
#define DROPDATA_H

#include "DataStruct.h"

class DropData : public DataStruct {
private:
	int chance;
	int quest;
public:
	DropData(){
		id=-1;
		chance=0;
		quest=0;
	}

	void setChance(int chance){
		this->chance = chance;
	}
	void setQuest(int quest){
		this->quest = quest;
	}
	int getChance(){
		return chance;
	}
	int getQuest(){
		return quest;
	}

};

#endif