#ifndef EQUIPDATA_H
#define EQUIPDATA_H

#include "DataStruct.h"

class EquipData : public DataStruct {
private:
	int price;
	char slots;
	short str, dex, intt, luk, hp, mp, watk, matk, wdef, mdef, acc, avo, hand, jump, speed;
	bool cash;
	bool quest;


public:
	EquipData(int id){
		this->id = id;
		price = 0;
		slots = 0;
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
		cash = 0;
		quest = 0;
	}
	void setPrice(int price){
		this->price = price;
	}
	int getPrice(){
		return price;
	}
	void setSlots(char slots){
		this->slots = slots;
	}
	char getSlots(){
		return slots;
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
	void setCash(bool cash){
		this->cash = cash;
	}
	bool getCash(){
		return cash;
	}
	void setQuest(bool quest){
		this->quest = quest;
	}
	bool getQuest(){
		return quest;
	}
};

#endif