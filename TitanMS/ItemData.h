#ifndef ITEMDATA_H
#define ITEMDATA_H

#include "ItemEffectData.h"
#include "DataStruct.h"

class ItemData : public DataStruct {
private:
	short maxslot;
	int price;
	double unitPrice;
	bool quest, consume, bullet;
	ItemEffectData* ied;
public:
	ItemData(int id){
		this->id = id;
		maxslot = 0;
		price = 0;
		quest = 0;
		consume = 0;
		bullet = 0;
		unitPrice = 0;
		ied = NULL;
	}

	void setMaxPerSlot(short maxslot){
		this->maxslot= maxslot;
	}
	void setPrice(int price){
		this->price = price;
	}
	void setUnitPrice(double unitPrice){
		this->unitPrice = unitPrice;
	}
	void setQuest(bool quest){
		this->quest = quest;
	}
	void setConsume(bool consume){
		this->consume = consume;
		if(consume == true && ied == NULL)
			ied = new ItemEffectData();
	}
	short getMaxPerSlot(){
		return maxslot;
	}
	int getPrice(){
		return price;
	}
	double getUnitPrice(){
		return unitPrice;
	}
	bool getQuest(){
		return quest;
	}
	bool getConsume(){
		return consume;
	}
	ItemEffectData* getEffectData(){
		return ied;
	}
	void setBullet(bool b){
		bullet = b;
	}
	bool getBullet(){
		return bullet;
	}

};

#endif