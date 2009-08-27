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

#ifndef PLAYER_H
#define PLAYER_H

#include "../Connection/AbstractPlayer.h"

#include <string.h>
#include <math.h>
#include <vector>
#include <hash_map>

#include "LifeMapObject.h"

class Trade;
class PlayerNPC;
class PlayerSkills;
class PlayerQuests;
class PlayerInventories;
class PlayerBuffs;
class Map;
class Party;
class PlayerKeys;
class ShopData;
class Party;
class Channel;
class PacketWriter;
class Pet;
template <class T> class Handler;
class PlayerHandler;

struct SkillMapEnterActiveInfo;

class Player:public AbstractPlayer, public LifeMapObject{
public:
	Player (int port);

	~Player();


//

	int getID(){
		return id;
	}
	Position getPosition(){
		return pos;
	}
	short getFoothold(){
		return fh;
	}

//
	void handleRequest(unsigned char* buf, int len);

	void disconnect();
	void setName(string name){
		this->name = name;
	}
	string getName(){
		return this->name;
	}
	void setGender(char g){
		this->gender = g;
	}
	char getGender(){
		return this->gender;
	}
	void setSkin(char id, bool send = true);
	char getSkin(){
		return this->skin;
	}
	void setFace(int id, bool send = true);
	int getFace(){
		return this->face;
	}
	void setHair(int id, bool send = true);
	int getHair(){
		return this->hair;
	}
	void setMap(int mapid);
	void setMapPos(int mapid, int pos);
	void setMapPortal(int mapid, string& portalname);
	Map* getMap(){
		return this->map;
	}
	void setMappos(char pos){
		this->mappos = pos;
	}
	char getMappos(){
		return this->mappos;
	}
	void setNPCShop(ShopData* shop){
		this->npcShop=shop;
	}
	ShopData* getNPCShop(){
		return npcShop;
	}
	void setNPC(PlayerNPC* npc){
		this->npc = npc;
	}
	void setChair(int chair){
		this->chair = chair;
	}
	int getChair(){
		return chair;
	}
	void setItemEffect(int itemEffect){
		this->itemEffect = itemEffect;
	}
	int getItemEffect(){
		return itemEffect;
	}
	PlayerNPC* getNPC(){
		return npc;
	}
	void setTrade(Trade* trade){
		this->trade = trade;
	}
	Trade* getTrade(){
		return trade;
	}
	void setParty(Party* party){
		this->party = party;
	}
	Party* getParty(){
		return party;
	}
	bool isGM(){
		return gm == 1;
	}
	PlayerInventories* getInventories(){
		return inv;	
	}
	PlayerSkills* getSkills(){
		return skills;
	}
	PlayerQuests* getQuests(){
		return quests;
	}
	PlayerKeys* getKeys(){
		return keys;
	}
	PlayerBuffs* getBuffs(){
		return buffs;
	}
	void damage();
	void save();
	void setUserID(int id){
		this->id = id;
		playerConnect();
	}
	void addMaxHPAP(int a){
		hpap+=a;
	}
	void addMaxMPAP(int a){
		mpap+=a;
	}
	void setMaxHPAP(int a){
		hpap=a;
	}
	void setMaxMPAP(int a){
		mpap=a;
	}
	int getMaxHPAP(){
		return hpap;
	}
	int getMaxMPAP(){
		return mpap;
	}
	void setMesos(int m, bool send = true, bool item = false);
	int getMesos(){
		return mesos;
	}
	void setLevel(int level, bool send = true);
	unsigned char getLevel(){
		return this->level;
	}
	void setJob(short job, bool send = true);
	short getJob(){
		return this->job;
	}
	void setStr(short str, bool send = true);
	short getStr(){
		return this->str;
	}
	void setDex(short dex, bool send = true);
	short getDex(){
		return this->dex;
	}
	void setInt(short intt, bool send = true);
	short getInt(){
		return this->intt;
	}
	void setLuk(short luk, bool send = true);
	short getLuk(){
		return this->luk;
	}
	void setHP(int hp, bool send = true, bool item= false);
	unsigned short getHP(){
		return this->hp;
	}
	void setMP(int mp, bool send = true, bool item= false);
	unsigned short getMP(){
		return this->mp;
	}
	void setMaxHP(int mhp, bool send = true, bool item= false);
	unsigned short getMaxHP(){
		return this->mhp;
	}
	void setBaseMaxHP(int rmhp);
	unsigned short getBaseMaxHP(){
		return this->rmhp;
	}
	void setMaxMP(int mmp, bool send = true, bool item= false);
	unsigned short getMaxMP(){
		return this->mmp;
	}
	void setBaseMaxMP(int rmmp);
	unsigned short getBaseMaxMP(){
		return this->rmmp;
	}
	void setAP(short ap, bool send = true);
	short getAP(){
		return this->ap;
	}
	void setSP(short sp, bool send = true);
	short getSP(){
		return this->sp;
	}
	void setFame(short fame, bool send = true);
	short getFame(){
		return this->fame;
	}
	void setExp(int exp, bool send = true);
	int getExp(){
		return this->exp;
	}
	Channel* getChannel(){
		return channel;
	}
	void addMesos(int m, bool send = true, bool item=false){
		setMesos(mesos + m, send, item);
	}
	void addExp(int e, bool send = true){
		setExp(exp + e, send);
	}
	void addLevel(int l, bool send = true){
		setLevel(level + l, send);
	}
	void addJob(short j, bool send = true){
		setJob(job + j, send);
	}
	void addSTR(short s, bool send = true){
		setStr(str + s, send);
	}
	void addDEX(short d, bool send = true){
		setDex(dex + d, send);
	}
	void addINT(short i, bool send = true){
		setInt(intt + i, send);
	}
	void addLUK(short l, bool send = true){
		setLuk(luk+ l, send);
	}
	void addHP(int h, bool send = true, bool item= false){
		setHP(hp + h, send, item);
	}
	void addMP(int m, bool send = true, bool item= false){
		setMP(mp + m, send, item);
	}
	void addMaxHP(int m, bool send = true, bool item= false){
		setMaxHP(mhp + m, send, item);
	}
	void addBaseMaxHP(int r){
		setBaseMaxHP(rmhp + r);
	}
	void addMaxMP(int m, bool send = true, bool item= false){
		setMaxMP(mmp + m, send, item);
	}
	void addBaseMaxMP(int r){
		setBaseMaxMP(rmmp + r);
	}
	void addAP(short a, bool send = true){
		setAP(ap + a, send);
	}
	void addSP(short s, bool send = true){
		setSP(sp + s, send);
	}
	void addFame(short f, bool send = true){
		setFame(fame + f, send);
	}
	Pet* getPet(int id);
	vector <Pet*>* getPets(){
		return &pets;
	}
	void addPet(Pet* pet);
	void removePet(Pet* pet, bool die = false);
	void setEvent(string& name);
	void levelUP();
	void updateStat(int stat, int value, bool item = false, char pet=0);
	Value removeStat(int stat);
	Value addStat(int stat, bool random = false);
	void changeMap(Map* map, int portal = 0);
	void addQuest(short questid);
	void endQuest(short questid);
	void setStyle(int id);
	int getItemAmount(int itemid);
	bool giveItem(int itemid, short amount);
	void giveMesos(int amount);	
	void giveExp(int e);
	bool checkSlot(int inv);
	void setJobS(short j){
		setJob(j);
	}
	void setLevelS(unsigned char l){
		setLevel(l);
	}
	void setHPS(int hp){
		setHP(hp);
	}
	void setSPS(short sp){
		setSP(sp);
	}
	void setAPS(short ap){
		setAP(ap);
	}
	void setStrS(short s){
		setStr(s);
	}
	void setDexS(short s){
		setDex(s);
	}
	void setIntS(short s){
		setInt(s);
	}
	void setLukS(short s){
		setLuk(s);
	}
	void setPartyInvite(int i){
		partyInvite = i;
	}
	int getPartyInvite(){
		return partyInvite;
	}
	static class Update {
	public:
		static const int NOTHING = 0x0;
		static const int SKIN = 0x1;
		static const int FACE = 0x2;
		static const int HAIR = 0x4;
		static const int PET = 0x8;
		static const int LEVEL = 0x10;
		static const int JOB = 0x20;
		static const int STR = 0x40;
		static const int DEX = 0x80;
		static const int INT = 0x100;
		static const int LUK = 0x200;
		static const int HP = 0x400;
		static const int MAXHP = 0x800;
		static const int MP = 0x1000;
		static const int MAXMP = 0x2000;
		static const int AP = 0x4000;
		static const int SP = 0x8000;
		static const int EXP = 0x10000;
		static const int FAME = 0x20000;
		static const int MESOS = 0x40000;
	};
	static class Effects {
	public:
		static const int LEVEL_UP = 0x0;
		static const int PET_LEVEL_UP = 0x4;
		static const int JOB_CHANGE = 0x8;
		static const int QUEST_FINISH = 0x9;
		static const int HEAL = 0xA;
	};
	static const int exps[200];
	void send(PacketWriter* packet);
	/// PvP
		
	void setPvP(bool pvp){
		this->pvp = pvp;
	}
	bool getPvP(){
		return pvp;
	}
	void setPvPMaskMob(int mobid){
		mask = mobid;
	}
	int getPvPMaskMob(){
		return mask;
	}	
	int getWDef();
	int getMDef();

	///

	
	void setVariable(string &name, int val){
		vars[name] = val;
	}
	int getVariable(string &name){
		if(vars.find(name) == vars.end())
			return -1;
		else
			return vars[name];
	}
	void deleteVariable(string &name){
		if(vars.find(name) != vars.end())
			vars.erase(name);
	}
	void setGlobalVariable(string &name, int val){
		global[name] = val;
	}
	int getGlobalVariable(string &name){
		if(global.find(name) == global.end())
			return -1;
		else
			return global[name];
	}
	void deleteGlobalVariable(string &name){
		if(global.find(name) != global.end())
			global.erase(name);
	}
private:	
	bool isconnect;
	bool loaded;
	string name;
	char gender;
	char skin;
	int face;
	int hair;
	Map* map;
	char mappos;
	int chair;
	int itemEffect;
	int gm;
	int mesos;
	int hpap;
	int mpap;
	int mask;
	int partyInvite;
	bool pvp;
	Channel* channel;
	std::vector <Pet*> pets;
	void playerConnect();

	PlayerInventories* inv;
	PlayerSkills* skills;
	PlayerQuests* quests;
	PlayerNPC* npc;
	PlayerKeys* keys;
	ShopData* npcShop;
	PlayerBuffs* buffs;
	Handler <PlayerHandler>* handler;
	Trade* trade;
	Party* party;

	unsigned char level;
	short job;
	short str;
	short dex;
	short intt;
	short luk;
	unsigned short hp;
	unsigned short mhp;
	unsigned short rmhp;
	unsigned short mp;
	unsigned short mmp;
	unsigned short rmmp;
	short ap;
	short sp;
	int exp;
	short fame;

	stdext::hash_map <string, int> global;
	stdext::hash_map <string, int> vars;
};

class PlayerFactory:public AbstractPlayerFactory {
public:
	AbstractPlayer* createPlayer(int port) {
		return new Player(port);
	}
};

#endif