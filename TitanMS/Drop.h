#ifndef DROP_H
#define DROP_H

#include "MapObject.h"

class Map;
class Item;
class Player;

class Drop : public MapObject {
private:
	int owner;
	Map* map;
	int amount;
	bool mesos;
	Item* item;
	bool quest;
	int questid;
	int exptime;
public:
	Drop(Map* map, Item* item);
	Drop(Map* map, Item* item, Position pos);
	Drop(Map* map);
	void setIsQuest(bool is){
		quest = is;
	}
	void setOwner(int owner){
		this->owner = owner;
	}
	int getOwner(){
		return owner;
	}
	bool isQuest(){
		return quest;
	}
	void setQuest(int questid){
		this->questid = questid;
	}
	int getQuest(){
		return questid;
	}
	void setMap(Map* map){
		this->map = map;
	}
	Map* getMap(){
		return map;
	}
	void setMesos(bool mesos){
		this->mesos = mesos;
	}
	bool getMesos(){
		return mesos;
	}
	void setTime(int time){
		exptime=time;
	}
	int getTime(){
		return exptime;
	}
	bool isMesos(){
		return mesos;
	}
	Item* getItem(){
		return item;
	}
	void setAmount(int a){
		amount = a;
	}
	int getAmount(){
		return amount;
	}
	void drop(MapObject* dropper);
	void showDrop(Player* player);
	void takeDrop(Player* player);
	void removeDrop(bool explode = false);
};

#endif