/*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef ITEMDATA_H
#define ITEMDATA_H

#include "ItemEffectData.h"
#include "DataStruct.h"

class ItemData : public DataStruct {
private:
	short maxslot;
	int price;
	double unitPrice;
	bool quest, consume, bullet, tradeBlock, notSale, only, expireOnLogout;
	string name;
	ItemEffectData* ied;
public:
	ItemData(int id){
		this->id = id;
		name = "";
		maxslot = 0;
		price = 0;
		quest = 0;
		tradeBlock = 0;
		notSale = 0;
		only = 0;
		expireOnLogout = 0;
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
	void setTradeBlock(bool b){
		this->tradeBlock = b;
	}
	void setNotSale(bool b){
		this->notSale = b;
	}
	void setOnly(bool b){
		this->only = b;
	}
	void setExpireOnLogout(bool b){
		this->expireOnLogout = b;
	}
	void setName(string name){
		this->name = name;
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
	bool getTradeBlock(){
		return tradeBlock;
	}
	bool getNotSale(){
		return notSale;
	}
	bool getOnly(){
		return only;
	}
	bool getExpireOnLogout(){
		return expireOnLogout;
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
	string getName(){
		return name;
	}

};

#endif