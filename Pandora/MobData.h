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

#ifndef MOBDATA_H
#define MOBDATA_H

#include "MobSummonData.h"
#include "DataStruct.h"

class MobData : public DataStruct {
private:
	int maxhp;
	int maxmp;
	unsigned char level;
	int exp;
	int watk, wdef, matk, mdef;
	bool boss;
	char color;
	char bgcolor;
	int removeAfter;
	int hpRecovery;
	int mpRecovery;
	bool firstAttack;
	bool damagedByMob;
	int dropItemDelay;
	MobSummonData* msd;
public:
	MobData(int id){
		this->id = id;
		maxhp=0;
		maxmp=0;
		level=0;
		watk=0;
		wdef=0;
		matk=0;
		mdef=0;
		boss = false;
		color = 0;
		bgcolor = 0;
		removeAfter = 0;
		hpRecovery = 0;
		mpRecovery = 0;
		firstAttack = false;
		damagedByMob = false;
		dropItemDelay = 0;
		msd = new MobSummonData();
	}

	void setMaxHP(int maxhp){
		this->maxhp = maxhp;
	}
	void setMaxMP(int maxmp){
		this->maxmp = maxmp;
	}
	void setLevel(unsigned char level){
		this->level = level;
	}
	void setExp(int exp){
		this->exp = exp;
	}
	void setWAtk(int watk){
		this->watk = watk;
	}
	void setWDef(int wdef){
		this->wdef = wdef;
	}
	void setMAtk(int matk){
		this->matk = matk;
	}
	void setMDef(int mdef){
		this->matk = matk;
	}
	int getMaxHP(){
		return maxhp;
	}
	int getMaxMP(){
		return maxmp;
	}
	unsigned char getLevel(){
		return level;
	}
	int getExp(){
		return exp;
	}
	int getWAtk(){
		return watk;
	}
	int getWDef(){
		return wdef;
	}
	int getMAtk(){
		return matk;
	}
	int getMDef(){
		return mdef;
	}
	void setIsBoss(bool i){
		boss = i;
	}
	void setColor(char color){
		this->color = color;
	}
	void setBgColor(char bg){
		bgcolor = bg;
	}
	bool getIsBoss(){
		return boss;
	}
	char getColor(){
		return color;
	}
	char getBgColor(){
		return bgcolor;
	}
	void setRemoveAfter(int r){
		removeAfter = r;
	}
	void setHPRecovery(int hp){
		hpRecovery = hp;
	}
	void setMPRecovery(int mp){
		mpRecovery = mp;
	}
	int getRemoveAfter(){
		return removeAfter;
	}
	int getHPRecovery(){
		return hpRecovery;
	}
	int getMPRecovery(){
		return mpRecovery;
	}
	void setFirstAttack(bool b){
		firstAttack = b;
	}
	bool getFirstAttack(){
		return firstAttack;
	}
	void setDamagedByMob(bool d){
		damagedByMob = d;
	}
	bool getDamagedByMob(){
		return damagedByMob;
	}
	void setDropDelay(int d){
		dropItemDelay = d;
	}
	int getDropDelay(){
		return dropItemDelay;
	}
	MobSummonData* getSummonData(){
		return msd;
	}

};

#endif