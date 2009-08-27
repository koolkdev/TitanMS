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

#ifndef MOB_H
#define MOB_H

#define MOB_SHOW		-1
#define MOB_SPAWN		-2
#define MOB_TRANSFORM	-3

#include "LifeMapObject.h"
class Player;
class Map;
class Timer;
#include <hash_map>
using namespace std;
using namespace stdext;

class Mob : public LifeMapObject {
public:
	Mob (Map* map){
		control=NULL;
		this->map = map;
		aggressive = 0;
		dropTimer = NULL;
	}
	void setMobID(int mobid){
		this->mobid=mobid;
	}
	int getMobID(){
		return mobid;
	}
	void setOriginalID(int mapid){
		this->mapid=mapid;
	}
	int getOriginalID(){
		return mapid;
	}
	void setStartFH(short fh){
		sfh = fh;
	}
	short getStartFH(){
		return sfh;
	}
	void setHP(int hp){
		this->hp=hp;
		if(this->hp<0)
			this->hp=0;
	}
	int getHP(){
		return hp;
	}
	void setMP(int mp){
		this->mp=mp;
		if(this->mp<0)
			this->mp=0;
	}
	int getMaxMP();
	int getMaxHP();
	bool isBoss();
	int getMP(){
		return mp;
	}
	void setControl(Player* control, bool agrs = false, bool switchc = true);
	Player* getControl();
	Map* getMap(){
		return map;
	}
	void takeDamage(Player* player, int damage);
	void giveExp();
	void findControl(bool spawn);
	void setAggressive(char ag){
		aggressive = ag;
	}
	char getAggressive(){
		return aggressive;
	}
	int getKillerID();
	void setDropTimer(Timer* timer){
		dropTimer = timer;
	}
	Timer* getDropTimer(){
		return dropTimer;
	}
private:
	char aggressive;
	int mapid;
	int mobid;
	int hp;
	int mp;
	short sfh;
	Player* control;
	Map* map;
	Timer* dropTimer;
	hash_map<int, int> playersa;
};

#endif