#ifndef ITEM_H
#define ITEM_H

#include <string>

using namespace std;

class Item {
protected:
	int id;
	int amount;
	int slot;
	int type;
	bool lock;
public:
	Item();
	Item(int itemid, int amount=1);
	void setID(int id);
	int getID(){
		return id;
	}
	void setAmount(int amount){
		this->amount = amount;
	}
	int getAmount(){
		return amount;
	}
	void setSlot(int slot){
		this->slot = slot;
	}
	int getSlot(){
		return slot;
	}
	int getType(){
		return type;
	}
	void setLocked(bool b){
		lock = b;
	}
	bool getLocked(){
		return lock;
	}
	bool isBullet();
};

#endif