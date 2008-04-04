#ifndef PLAYER_H
#define PLAYER_H

#include "../Connection/AbstractPlayer.h"
#include "../Connection/PacketHandler.h"
#include "PlayerInventory.h"
#include <string.h>
#include <math.h>

struct Pos {
	short x;
	short y;
};

class NPC;
class PlayerSkills;
class PlayerQuests;

int distPos(Pos pos1, Pos pos2);

class Player:public AbstractPlayer {
public:
	Player () {
		isconnect=0;
		shop=0;
		npc=NULL;
	}

	~Player();

	void handleRequest(unsigned char* buf, int len);
	void sendPacket(unsigned char* buf, int len){ packetHandler->sendPacket(buf,len); }
	void setPlayerid(int id){
		this->id = id;
	}
	int getPlayerid(){
		return this->id;
	}
	char* getName(){
		return this->name;
	}
	char getGender(){
		return this->gender;
	}
	void setSkin(char id);
	char getSkin(){
		return this->skin;
	}
	void setEyes(int id);
	int getEyes(){
		return this->eyes;
	}
	void setHair(int id);
	int getHair(){
		return this->hair;
	}
	void setLevel(int level);
	unsigned char getLevel(){
		return this->level;
	}
	void setJob(short job);
	short getJob(){
		return this->job;
	}
	void setStr(short str){
		this->str=str;
	}
	short getStr(){
		return this->str;
	}
	void setDex(short dex){
		this->dex=dex;
	}
	short getDex(){
		return this->dex;
	}
	void setInt(short intt){
		this->intt=intt;
	}
	short getInt(){
		return this->intt;
	}
	void setLuk(short luk){
		this->luk=luk;
	}
	short getLuk(){
		return this->luk;
	}
	void setHP(short hp);
	short getHP(){
		return this->hp;
	}
	void setMP(short mp, bool is=0);
	short getMP(){
		return this->mp;
	}
	void setMHP(short mhp){
		this->mhp=mhp;
	}
	short getMHP(){
		return this->mhp;
	}
	void setMMP(short mmp){
		this->mmp=mmp;
	}
	short getMMP(){
		return this->mmp;
	}
	void setAp(short ap){
		this->ap=ap;
	}
	short getAp(){
		return this->ap;
	}
	void setSp(short sp);
	short getSp(){
		return this->sp;
	}
	void setFame(short fame){
		this->fame=fame;
	}
	short getFame(){
		return this->fame;
	}
	void setExp(int exp, bool is);
	int getExp(){
		return this->exp;
	}
	void setMap(int map){
		this->map=map;
	}
	int getMap(){
		return this->map;
	}
	void setMappos(char pos){
		this->mappos = pos;
	}
	char getMappos(){
		return this->mappos;
	}
	Pos getPos(){
		return this->pos;
	}
	void setPos(Pos pos){
		this->pos = pos;
	}
	void setType(char type){
		this->type=type;
	}
	char getType(){
		return this->type;
	}
	void setShop(int shopid){
		shop=shopid;
	}
	int getShop(){
		return shop;
	}
	void setNPC(NPC* npc){
		this->npc = npc;
	}
	NPC* getNPC(){
		return npc;
	}
	PlayerInventory* inv;
	PlayerSkills* skills;
	PlayerQuests* quests;
private:	
	bool isconnect;
	int id;
	char name[15];
	char gender;
	char skin;
	int eyes;
	int hair;
	unsigned char level;
	short job;
	short str;
	short dex;
	short intt;
	short luk;
	short hp;
	short mhp;
	short mp;
	short mmp;
	short ap;
	short sp;
	int exp;
	short fame;
	int map;
	char mappos;
	char type;
	int keys[90];
	int shop;
	Pos pos;
	NPC* npc;
	void getUserID(unsigned char* buf){
		isconnect=1;
		setPlayerid(buf[0] + buf[1]*0x100 + buf[2]*0x100*0x100 + buf[3]*0x100*0x100*0x100);
		playerConnect();
	}
	void playerConnect();
	void chaneKey(unsigned char* packet);
};

class PlayerFactory:public AbstractPlayerFactory {
public:
	AbstractPlayer* createPlayer() {
		return new Player();
	}
};

#endif