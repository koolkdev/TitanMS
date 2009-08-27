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
	int dropTime;
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
	void setDropTime(int time){
		dropTime = time;
	}
	int getDropTime(){
		return dropTime;
	}
	void drop(MapObject* dropper, int delay = 0);
	void showDrop(Player* player);
	bool takeDrop(Player* player);
	void removeDrop(bool explode = false);
};

#endif