#ifndef EQUIP_H
#define EQUIP_H

#include "Item.h"

class Equip : public Item {
private:
	static short getRandomStat(int stat);
	char slots, scrolls;
	short str, dex, intt, luk, hp, mp, watk, matk, wdef, mdef, acc, avo, hand, jump, speed;
	string owner;
public:
	Equip();
	Equip(int itemid, bool rstats = false);

	void setSlots(char slots){
		this->slots = slots;
	}
	char getSlots(){
		return slots;
	}
	void setScrolls(char scrolls){
		this->scrolls = scrolls;
	}
	char getScrolls(){
		return scrolls;
	}
	void setStr(short str){
		if(str < 0)
			str = 0;
		this->str = str;
	}
	short getStr(){
		return str;
	}
	void setDex(short dex){
		if(dex < 0)
			dex = 0;
		this->dex = dex;
	}
	short getDex(){
		return dex;
	}
	void setInt(short intt){
		if(intt < 0)
			intt = 0;
		this->intt = intt;
	}
	short getInt(){
		return intt;
	}
	void setLuk(short luk){
		if(luk < 0)
			luk = 0;
		this->luk = luk;
	}
	short getLuk(){
		return luk;
	}
	void setHP(short hp){
		if(hp < 0)
			hp = 0;
		this->hp = hp;
	}
	short getHP(){
		return hp;
	}
	void setMP(short mp){
		if(mp < 0)
			mp = 0;
		this->mp = mp;
	}
	short getMP(){
		return mp;
	}
	void setWAtk(short watk){
		if(watk < 0)
			watk = 0;
		this->watk = watk;
	}
	short getWAtk(){
		return watk;
	}
	void setMAtk(short matk){
		if(matk < 0)
			matk = 0;
		this->matk = matk;
	}
	short getMAtk(){
		return matk;
	}
	void setWDef(short wdef){
		if(wdef < 0)
			wdef = 0;
		this->wdef = wdef;
	}
	short getWDef(){
		return wdef;
	}
	void setMDef(short mdef){
		if(mdef < 0)
			mdef = 0;
		this->mdef = mdef;
	}
	short getMDef(){
		return mdef;
	}
	void setAcc(short acc){
		if(acc < 0)
			acc = 0;
		this->acc = acc;
	}
	short getAcc(){
		return acc;
	}
	void setAvo(short avo){
		if(avo < 0)
			avo = 0;
		this->avo = avo;
	}
	short getAvo(){
		return avo;
	}
	void setHand(short hand){
		if(hand < 0)
			hand = 0;
		this->hand = hand;
	}
	short getHand(){
		return hand;
	}
	void setSpeed(short speed){
		if(speed < 0)
			speed = 0;
		this->speed = speed;
	}
	short getSpeed(){
		return speed;
	}
	void setJump(short jump){
		if(jump < 0)
			jump = 0;
		this->jump = jump;
	}
	short getJump(){
		return jump;
	}
	void setOwner(string name){
		owner = name;
	}
	string getOwner(){
		return owner;
	}
};

#endif