#ifndef QUESTREQUESTDATA_H
#define QUESTREQUESTDATA_H

#include "DataStruct.h"

class QuestRequestData : public DataStruct {
private:
	bool item;
	bool mob;
	bool quest;
	int count;
public:
	QuestRequestData(){
		id=-1;
		item = 0;
		mob = 0;
		quest = 0;
		count = 0;
	}

	void setItem(bool item){
		this->item = item;
	}
	bool getItem(){
		return item;
	}
	void setMob(bool mob){
		this->mob = mob;
	}
	bool getMob(){
		return mob;
	}
	void setQuest(bool quest){
		this->quest = quest;
	}
	bool getQuest(){
		return quest;
	}
	void setCount(int count){
		this->count = count;
	}
	int getCount(){
		return count;
	}

};

#endif