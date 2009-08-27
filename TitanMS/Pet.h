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

#ifndef PET_H
#define PET_H

#include "Item.h"
#include "LifeMapObject.h"

class Player;
class Timer;

class Pet : public Item, public LifeMapObject {
private:
	static int ids;
	static int levels[30];
	__int64 time;
	string name;
	char level;
	short closeness;
	unsigned char fullness;
	int lastTime;
	int petSlot;
	Timer* timer;
public:
	Pet(int id);
	Pet(const Pet &pet): LifeMapObject(), Item(pet), time(pet.time), name(pet.name), level(pet.level), closeness(pet.closeness), fullness(pet.fullness), lastTime(0), petSlot(-1), timer(NULL) { Item::Item(pet); }
	~Pet();
	void setTime(__int64 time){
		this->time = time;
	}
	__int64 getTime(){
		return time;
	}
	void setName(string name){
		this->name = name;
	}
	string getName(){
		return name;
	}
	void setLevel(char level){
		if(level > 30)
			level = 30;
		if(level < 1)
			level = 1;
		this->level = level;
	}
	char getLevel(){
		return level;
	}
	void setCloseness(short closeness, Player* player = NULL);
	short getCloseness(){
		return closeness;
	}
	void setFullness(unsigned char fullness){
		if(fullness > 100)
			this->fullness = 100;
		else
			this->fullness = fullness;
	}
	char getFullness(){
		return fullness;
	}
	void addLevel(char level){
		setLevel(this->level + level);
	}
	void addFullness(unsigned char fullness){
		setFullness(this->fullness + fullness);
	}
	void addCloseness(char closeness, Player* player = NULL){
		setCloseness(this->closeness + closeness, player);
	}
	int getObjectID(){
		return ((MapObject*)this)->getID();
	}
	int getItemID(){
		return ((Item*)this)->getID();
	}
	void setLastTime(int t){
		lastTime = t;
	}
	int getLastTime(){
		return lastTime;
	}
	Timer** getTimer(){
		return &timer;
	}
	void setPetSlot(int slot){
		petSlot = slot;
	}
	int getPetSlot(){
		return petSlot;
	}
	void startTimer(Player* Player);
	void stopTimer();
};

#endif