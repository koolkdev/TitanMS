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

#ifndef SKILLLEVELDATA_H
#define SKILLLEVELDATA_H

#include "DataStruct.h"
#include "Tools.h"

class SkillLevelData : public DataStruct {
private:
	int time, hp, mp, item, itemCount, x, y, speed, jump, watk, wdef, matk, mdef,acc, avo, hpp, attackCount, mobCount, prop, cooltime;
	Position rb, lt;
public:
	SkillLevelData(int level){
		id=level;
		time = 0;
		hp = 0;
		mp = 0;
		item = 0;
		itemCount = 0;
		x = 0;
		y = 0;
		speed = 0;
		jump = 0;
		watk = 0;
		wdef = 0;
		matk = 0;
		mdef = 0;
		acc = 0;
		avo = 0;
		hpp = 0;
		attackCount = 0;
		mobCount = 0;
		prop = 0;
		cooltime = 0;
		rb.x = 0;
		rb.y = 0;
		lt.x = 0;
		lt.y = 0;
	}
	void setTime(int time){
		this->time = time;
	}
	int getTime(){
		return time;
	}
	void setItem(int item){
		this->item = item;
	}
	int getItem(){
		return item;
	}
	void setItemCount(int itemCount){
		this->itemCount = itemCount;
	}
	int getItemCount(){
		return itemCount;
	}
	void setX(int x){
		this->x = x;
	}
	int getX(){
		return x;
	}
	void setY(int y){
		this->y = y;
	}
	int getY(){
		return y;
	}
	void setHP(int hp){
		this->hp = hp;
	}
	int getHP(){
		return hp;
	}
	void setMP(int mp){
		this->mp = mp;
	}
	int getMP(){
		return mp;
	}
	void setWAtk(int watk){
		this->watk = watk;
	}
	int getWAtk(){
		return watk;
	}
	void setMAtk(int matk){
		this->matk = matk;
	}
	int getMAtk(){
		return matk;
	}
	void setWDef(int wdef){
		this->wdef = wdef;
	}
	int getWDef(){
		return wdef;
	}
	void setMDef(int mdef){
		this->mdef = mdef;
	}
	int getMDef(){
		return mdef;
	}
	void setAcc(int acc){
		this->acc = acc;
	}
	int getAcc(){
		return acc;
	}
	void setAvo(int avo){
		this->avo = avo;
	}
	int getAvo(){
		return avo;
	}
	void setSpeed(int speed){
		this->speed = speed;
	}
	int getSpeed(){
		return speed;
	}
	void setJump(int jump){
		this->jump = jump;
	}
	int getJump(){
		return jump;
	}
	void setHPPer(int hpp){
		this->hpp = hpp;
	}
	int getHPPer(){
		return hpp;
	}
	void setRBX(int x){
		rb.x = x;
	}
	void setRBY(int y){
		rb.y = y;
	}
	void setLTX(int x){
		lt.x = x;
	}
	void setLTY(int y){
		lt.y = y;
	}
	Position& getRB(){
		return rb;
	}
	Position& getLT(){
		return lt;
	}
	void setAttackCount(int a){
		attackCount = a;
	}
	int getAttackCount(){
		return attackCount;
	}
	void setMobCount(int m){
		mobCount = m;
	}
	int getMobCount(){
		return mobCount;
	}
	void setCooltime(int c){
		this->cooltime = c;
	}
	int getCooltime(){
		return cooltime;;
	}
	void setProp(int p){
		prop = p;
	}
	int getProp(){
		return prop;;
	}

};

#endif