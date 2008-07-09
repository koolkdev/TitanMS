#ifndef PET_H
#define PET_H

#include "Item.h"
#include "MapObject.h"

class Player;

class Pet : public Item, public MapObject {
private:
	static int ids;
	static int levels[30];
	__int64 time;
	string name;
	char level;
	short closeness;
	char fullness;
	char stance;
	int lastTime;
	int timer;
public:
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
	void setFullness(char fullness){
		this->fullness = fullness;
	}
	char getFullness(){
		return fullness;
	}
	void addLevel(char level){
		setLevel(this->level + level);
	}
	void addFullness(char fullness){
		setFullness(this->fullness + fullness);
	}
	void addCloseness(char closeness, Player* player = NULL){
		setCloseness(this->closeness + closeness, player);
	}
	void setStance(char stance){
		this->stance = stance;
	}
	char getStance(){
		return stance;
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
	int* getTimer(){
		return &timer;
	}
	void startTimer(Player* Player);
	void stopTimer();
	Pet(int id);
	string getCommandReplay(string command);
};

#endif