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

#ifndef ITEMEFFECTDATA_H
#define ITEMEFFECTDATA_H

#include "ItemSummonData.h"
#include <string>
using namespace std;

class ItemEffectData {
private:
	int time;
	short hhp;
	short hmp;
	short hpr;
	short mpr;
	int moveTo;
	short success;
	short cursed;
	short str;
	short dex;
	short intt;
	short luk;
	short hp;
	short mp;
	short watk;
	short matk;
	short wdef;
	short mdef;
	short acc;
	short avo;
	short hand;
	short jump;
	short speed;
	int itemOnMap;
	string msg;
	bool recover;
	bool randstat;
	int morph;
	int fullness;
	int thaw;
	int masterLevel;
	int reqSkillLevel;
	vector<int> skills;
	ItemSummonData* isd;
public:
	ItemEffectData(){
		hhp = 0;
		hmp = 0;
		hpr = 0;
		mpr = 0;
		moveTo = 0;
		success = 0;
		cursed = 0;
		str = 0;
		dex = 0;
		intt = 0;
		luk = 0;
		hp = 0;
		mp = 0;
		watk = 0;
		matk = 0;
		wdef = 0;
		mdef = 0;
		acc = 0;
		avo = 0;
		hand = 0;
		jump = 0;
		speed = 0;
		itemOnMap = 0;
		msg = "";
		recover=0;
		randstat=0;
		morph = 0;
		thaw = 0;
		fullness = 0;
		time = 0;
		masterLevel = 0;
		reqSkillLevel = 0;
		isd = new ItemSummonData();
	}

	void setHealHP(short hhp){
		this->hhp = hhp;
	}
	short getHealHP(){
		return hhp;
	}
	void setHealMP(short hmp){
		this->hmp = hmp;
	}
	short getHealMP(){
		return hmp;
	}
	void setHealHPPer(short hpr){
		this->hpr = hpr;
	}
	short getHealHPPer(){
		return hpr;
	}
	void setHealMPPer(short mpr){
		this->mpr = mpr;
	}
	short getHealMPPer(){
		return mpr;
	}
	void setMoveTo(int moveTo){
		this->moveTo = moveTo;
	}
	int getMoveTo(){
		return moveTo;
	}
	void setSuccess(short success){
		this->success = success;
	}
	short getSuccess(){
		return success;
	}
	void setCursed(short cursed){
		this->cursed = cursed;
	}
	short getCursed(){
		return cursed;
	}
	void setTime(int time){
		this->time = time;
	}
	int getTime(){
		return time;
	}
	void setStr(short str){
		this->str = str;
	}
	short getStr(){
		return str;
	}
	void setDex(short dex){
		this->dex = dex;
	}
	short getDex(){
		return dex;
	}
	void setInt(short intt){
		this->intt = intt;
	}
	short getInt(){
		return intt;
	}
	void setLuk(short luk){
		this->luk = luk;
	}
	short getLuk(){
		return luk;
	}
	void setHP(short hp){
		this->hp = hp;
	}
	short getHP(){
		return hp;
	}
	void setMP(short mp){
		this->mp = mp;
	}
	short getMP(){
		return mp;
	}
	void setWAtk(short watk){
		this->watk = watk;
	}
	short getWAtk(){
		return watk;
	}
	void setMAtk(short matk){
		this->matk = matk;
	}
	short getMAtk(){
		return matk;
	}
	void setWDef(short wdef){
		this->wdef = wdef;
	}
	short getWDef(){
		return wdef;
	}
	void setMDef(short mdef){
		this->mdef = mdef;
	}
	short getMDef(){
		return mdef;
	}
	void setAcc(short acc){
		this->acc = acc;
	}
	short getAcc(){
		return acc;
	}
	void setAvo(short avo){
		this->avo = avo;
	}
	short getAvo(){
		return avo;
	}
	void setHand(short hand){
		this->hand = hand;
	}
	short getHand(){
		return hand;
	}
	void setSpeed(short speed){
		this->speed = speed;
	}
	short getSpeed(){
		return speed;
	}
	void setJump(short jump){
		this->jump = jump;
	}
	short getJump(){
		return jump;
	}
	void setItemOnMap(int id){
		itemOnMap = id;
	}
	int getItemOnMap(){
		return itemOnMap;
	}
	void setMsg(string m){
		msg = m;
	}
	string getMsg(){
		return msg;
	}
	void setRecover(bool b){
		recover = b;
	}
	bool getRecover(){
		return recover;
	}
	void setRandStat(bool b){
		randstat = b;
	}
	bool getRandStat(){
		return randstat;
	}
	void setMorph(int m){
		morph = m;
	}
	int getMorph(){
		return morph;
	}
	void setFullness(int f){
		fullness = f;
	}
	int getFullness(){
		return fullness;
	}
	void setThaw(int t){
		thaw = t;
	}
	int getThaw(){
		return thaw;
	}
	void setMasterLevel(int ml){
		masterLevel = ml;
	}
	int getMasterLevel(){
		return masterLevel;
	}
	void setReqSkillLevel(int r){
		reqSkillLevel = r;
	}
	int getReqSkillLevel(){
		return reqSkillLevel;
	}
	void addSkill(int skillid){
		skills.push_back(skillid);
	}
	ItemSummonData* getSummonData(){
		return isd;
	}
};

#endif