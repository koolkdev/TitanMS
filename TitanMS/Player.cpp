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

#include "Player.h"
#include "Worlds.h"
#include "Channel.h"
#include "Players.h"
#include "PlayerHandler.h"
#include "PacketReader.h"
#include "Pet.h"
#include "PacketWriter.h"
#include "PlayerNPC.h"
#include "MySQLM.h"
#include "Map.h"
#include "Maps.h"
#include "MapPlayers.h"
#include "PlayerInventories.h"
#include "MapPortalData.h"
#include "Inventory.h"
#include "PlayerQuests.h"
#include "PlayerKeys.h"
#include "PlayerBuffs.h"
#include "PlayerSkills.h"
#include "SkillsData.h"
#include "SkillData.h"
#include "SkillLevelData.h"
#include "Skill.h"
#include "Equip.h"
#include "Item.h"
#include "Quest.h"
#include "Key.h"
#include "PacketCreator.h"
#include "Tools.h"
using namespace Tools;

int strval(string& str);

const int Player::exps[200] = {15, 34, 57, 92, 135, 372, 560, 840, 1242, 1716, 2360, 3216, 4200,
	5460, 7050, 8840, 11040, 13716, 16680, 20216, 24402, 28980, 34320, 40512, 47216, 54900,
	63666, 73080, 83720, 95700, 108480, 122760, 138666, 155540, 174216, 194832, 216600, 240500,
	266682, 294216, 324240, 356916, 391160, 428280, 468450, 510420, 555680, 604416, 655200,
	709716, 748608, 789631, 832902, 878545, 926689, 977471, 1031036, 1087536, 1147032, 1209994,
	1276301, 1346242, 1420016, 1497832, 1579913, 1666492, 1757815, 1854143, 1955750, 2062925,
	2175973, 2295216, 2420993, 2553663, 2693603, 2841212, 2996910, 3161140, 3334370, 3517093,
	3709829, 3913127, 4127566, 4353756, 4592341, 4844001, 5109452, 5389449, 5684790, 5996316,
	6324914, 6671519, 7037118, 7422752, 7829518, 8258575, 8711144, 9188514, 9692044, 10223168,
	10783397, 11374327, 11997640, 12655110, 13348610, 14080113, 14851703, 15665576, 16524049,
	17429566, 18384706, 19392187, 20454878, 21575805, 22758159, 24005306, 25320796, 26708375,
	28171993, 29715818, 31344244, 33061908, 34873700, 36784778,  8800583, 40926854, 43169645,
	45535341, 48030677, 50662758, 53439077, 56367538, 59456479, 62714694, 66151459, 69776558,
	73600313, 77633610, 81887931, 86375389, 91108760, 96101520, 101367883, 106992842, 112782213,
	118962678, 125481832, 132358236, 139611467, 147262175, 155332142, 163844343, 172823012,
	182293713, 192283408, 202820538, 213935103, 225658746, 238024845, 251068606, 264827165,
	279339639, 294647508, 310794191, 327825712, 345790561, 364739883, 384727628, 405810702,
	428049128, 451506220, 476248760, 502347192, 529875818, 558913012, 589541445, 621848316,
	655925603, 691870326, 729784819, 769777027, 811960808, 856456260, 903390063, 952895838,
	1005114529, 1060194805, 1118293480, 1244216724, 1312399800, 1384319309, 1460180007,
	1540197871, 1624600714, 1179575962, 1713628833, 1807535693, 1906558648, 2011069705, 0};
Player::Player(int port){
	channel = Worlds::getInstance()->getChannelByPort(port);
	isconnect = false;
	stance = 0;
	chair = 0;
	itemEffect = 0;
	shop = 0;
	npc = 0;
	pet = NULL;
	handler = new PlayerHandler(this);
}
Player::~Player(){
	if(isconnect){
		if(pet!=NULL)
			pet->stopTimer();
		buffs->cancelAll();
		map->getPlayers()->remove(this);
		channel->getPlayers()->remove(this);
		save();
		inv->deleteAll();
		skills->deleteAll();
		keys->deleteAll();
		quests->deleteAll();
		if(npc != NULL)
			delete npc;
		delete inv;
		delete buffs;
		delete keys;
		delete skills;
		delete quests;
		isconnect = false;
		delete handler;
	}
}

void Player::send(PacketWriter* packet){
	unsigned char bytes [MAX_LENGTH];
	memcpy_s(bytes, packet->getLength(), packet->getBytes(), packet->getLength());
	sendPacket(bytes, packet->getLength());
}

void Player::handleRequest(unsigned char* buf, int len){
	try{
		int c = 0;
		for(int i=0; i<len; i++)
			if(buf[i] == 0x10)
				c++;
		if(c>1)
			printf("A");
		handler->handle(&PacketReader(buf, len, id));
	}
	catch (BadQuery er ) {
		printf("%s\n", er.what());
	}
	catch(...){
		//TODO
	}
}

void Player::playerConnect(){
	// to improve
	string values[28];
	MySQL::getInstance()->getCharacter(id, values);
	if(values[25] != getIP() && values[25] != "127.0.0.1"){ // localhost
		disconnect();
		return;
	}
	name = values[1];
	level = strval(values[4]);
	job = strval(values[5]);
	str = strval(values[6]);
	dex = strval(values[7]);
	intt = strval(values[8]);
	luk = strval(values[9]);
	hp = strval(values[10]);
	rmhp = mhp = strval(values[11]);
	mp = strval(values[12]);
	rmmp = mmp = strval(values[13]);
	ap = strval(values[14]);
	sp = strval(values[15]);
	exp = strval(values[16]);
	fame = strval(values[17]);
	map = channel->getMaps()->getMap(strval(values[18]));
	if(map == NULL){
		disconnect();
		return;
	}
	mappos = strval(values[19]);
	gender = strval(values[20]);
	skin = strval(values[21]);
	face = strval(values[22]);
	hair = strval(values[23]);
	mesos = strval(values[24]);
	mesos = strval(values[26]);
	mesos = strval(values[27]);
	gm = MySQL::getInstance()->getInt("users", MySQL::getInstance()->getInt("characters", id, "userid"), "gm");
	string equips[150][22];
	int many = MySQL::getInstance()->showEquipsIn(id, equips);
	inv = new PlayerInventories(this);
	skills = new PlayerSkills(this);
	quests = new PlayerQuests(this);
	keys = new PlayerKeys(this);
	buffs = new PlayerBuffs(this);
	isconnect = true;
	for(int i=0; i<many; i++){
		Equip* equip = new Equip();
		equip->setID(strval(equips[i][0]));
		equip->setSlots(strval(equips[i][3]));
		equip->setScrolls(strval(equips[i][4]));
		equip->setSlot(strval(equips[i][2]));
		equip->setStr(strval(equips[i][5]));
		equip->setDex(strval(equips[i][6]));
		equip->setInt(strval(equips[i][7]));
		equip->setLuk(strval(equips[i][8]));
		equip->setHP(strval(equips[i][9]));
		equip->setMP(strval(equips[i][10]));
		equip->setWAtk(strval(equips[i][11]));
		equip->setMAtk(strval(equips[i][12]));
		equip->setWDef(strval(equips[i][13]));
		equip->setMDef(strval(equips[i][14]));
		equip->setAcc(strval(equips[i][15]));
		equip->setAvo(strval(equips[i][16]));
		equip->setHand(strval(equips[i][17]));
		equip->setSpeed(strval(equips[i][18]));
		equip->setJump(strval(equips[i][19]));
		equip->setOwner(equips[i][20]);
		equip->setLocked(equips[i][21] == "1");
		inv->getInventory(equip->getSlot()>0)->addItem(equip, false);
	}
	int items[400][4];
	many = MySQL::getInstance()->getItems(id, items);
	for(int i=0; i<many; i++){
		Item* item = new Item();
		item->setID(items[i][0]);
		item->setSlot(items[i][2]);
		item->setAmount(items[i][3]);
		inv->getInventory(items[i][1])->addItem(item, false);
	}
	int skilld[200][3];
	many = MySQL::getInstance()->getSkills(id, skilld);
	for(int i=0; i<many; i++){
		Skill* skill = new Skill(skilld[i][0], skilld[i][1], skilld[i][2]);
		skills->addSkill(skill);
	}

	int keya[90][3];
	many = MySQL::getInstance()->getKeys(id, keya);
	for(int i=0; i<many; i++){
		Key* key = new Key(keya[i][1], keya[i][2]);
		keys->setKey(keya[i][0], key);
	}
	setPosition(map->getPortalPosition(mappos));
	stance = 0;
	send(PacketCreator().connectionData(this));
	send(PacketCreator().showKeys(keys));
	channel->addPlayer(this);
}


void Player::setMesos(int m, bool send, bool item){
	mesos = m;
	if(send){
		updateStat(Update::MESOS, mesos, item);
	}
}
void Player::setLevel(int level, bool send){
	this->level = level;
	if(send){
		updateStat(Update::LEVEL, this->level);
		map->send(PacketCreator().showEffect(id, Effects::LEVEL_UP), this);
	}
}
void Player::setJob(short job, bool send){
	this->job = job;
	if(send){
		updateStat(Update::JOB, this->job);
		map->send(PacketCreator().showEffect(id, Effects::JOB_CHANGE), this);
	}
}
void Player::setStr(short str, bool send){
	this->str=str;
	if(send){
		updateStat(Update::STR, this->str);
	}
}
void Player::setDex(short dex, bool send){
	this->dex=dex;
	if(send){
		updateStat(Update::DEX, this->dex);
	}
}
void Player::setInt(short intt, bool send){
	this->intt=intt;
	if(send){
		updateStat(Update::INT, this->intt);
	}
}
void Player::setLuk(short luk, bool send){
	this->luk=luk;
	if(send){
		updateStat(Update::LUK, this->luk);
	}
}
void Player::setHP(int hp, bool send, bool item){
	if(hp<0)
		this->hp=0;
	else if(hp>mhp)
		this->hp=mhp;
	else
		this->hp=hp;
	if(send){
		updateStat(Update::HP, this->hp, item);
	}
}
void Player::setMP(int mp, bool send, bool item){
	if(mp<0)
		this->mp=0;
	else if(mp>mmp)
		this->mp=mmp;
	else
		this->mp=mp;
	if(send){
		updateStat(Update::MP, this->mp, item);
	}
}
void Player::setMaxHP(int mhp, bool send, bool item){
	this->mhp=mhp;
	if(send){
		updateStat(Update::MAXHP, this->mhp, item);
	}
}
void Player::setBaseMaxHP(int rmhp){
	this->rmhp=rmhp;
	mhp=rmhp;
}
void Player::setMaxMP(int mmp, bool send, bool item){
	this->mmp=mmp;
	if(send){
		updateStat(Update::MAXMP, this->mmp, item);
	}
}
void Player::setBaseMaxMP(int rmmp){
	this->rmmp=rmmp;
	mmp=rmmp;
}
void Player::setAP(short ap, bool send){
	this->ap=ap;
	if(send){
		updateStat(Update::AP, this->ap);
	}
}
void Player::setSP(short sp, bool send){
	this->sp = sp;
	if(send){
		updateStat(Update::SP, this->sp);
	}
}
void Player::setFame(short fame, bool send){
	this->fame=fame;
	if(send){
		updateStat(Update::FAME, this->fame);
	}
}
void Player::setExp(int exp, bool send){
	if(exp >= exps[level-1]){
		if(level == 200){
			exp = 0;
		}
		else{
			exp-=exps[level-1];
			if(exp >= exps[level])
				exp = exps[level]-1;
			this->exp = exp;
			levelUP();
		}
	}
	else{
		this->exp = exp;
		if(send){
			updateStat(Update::EXP, this->exp);
		}
	}
}
void Player::levelUP(){
	level++;
	int jobt = job/100;
	if(jobt == 0){
		rmhp += 15;
		rmmp += 10;
	}
	else if(jobt == 2){
		rmhp += 15;
		rmmp += 35;
	}
	else{
		rmhp += 25;
		rmmp += 15;			
	}
	Skill* skill = getSkills()->getSkill(1000001);
	if(skill != NULL && skill->getLevel() > 0){
		SkillLevelData* level = SkillsData::getInstance()->getDataByID(1000001)->getDataByID(skill->getLevel());
		if(level != NULL)
			rmhp += level->getY();
	}
	skill = getSkills()->getSkill(2000001);
	if(skill != NULL && skill->getLevel() > 0){
		SkillLevelData* level = SkillsData::getInstance()->getDataByID(2000001)->getDataByID(skill->getLevel());
		if(level != NULL)
			rmmp += level->getY();
	}
	mhp = rmhp;
	mmp = rmmp;
	hp = mhp;
	mp = mmp;
	addAP(5, false);
	if(job != 0) addSP(3, false);
	map->send(PacketCreator().showEffect(id, Effects::LEVEL_UP), this);
	Values values;
	values.add(Value(Update::AP, ap));
	values.add(Value(Update::SP, sp));
	values.add(Value(Update::MAXHP, mhp));
	values.add(Value(Update::MAXMP, mmp));
	values.add(Value(Update::HP, hp));
	values.add(Value(Update::MP, mp));
	values.add(Value(Update::EXP, exp));
	values.add(Value(Update::LEVEL, level));
	send(PacketCreator().updateStats(&values));


}
void Player::updateStat(int stat, int value, bool item){
	Values v = Values();
	v.add(Value(stat, value));
	send(PacketCreator().updateStats(&v, item));
}

void Player::setMap(int mapid){
	Map* map = channel->getMaps()->getMap(mapid);
	if(map != NULL)
		changeMap(map);
		
}
void Player::changeMap(Map* map, int portal){
	Map* oldmap = this->map;
	this->map = map;
	oldmap->removePlayer(this);
	mappos = portal;
	send(PacketCreator().changeMap(this));
	MapPortalData* tportal = map->getPortal(portal);
	Position npos;
	npos.x = tportal->getX();
	npos.y = tportal->getY();
	pos = npos;
	stance = 0;
	chair = 0;
	if(pet!=NULL){
		pet->setPosition(getPosition());
		pet->setStance(getStance());
		send(PacketCreator().showPet(id, pet));
	}
	itemEffect = 0;
	map->addPlayer(this);
		
}
Value Player::removeStat(int stat){
	int value = -1;
	if(stat == Update::STR){
		if(getStr() <= 4)
			return Value(0,0);
		addSTR(-1, false);
		value = getStr();
	}
	else if(stat == Update::DEX){
		if(getDex() <= 4)
			return Value(0,0);
		addDEX(-1, false);
		value = getDex();
	}
	else if(stat == Update::LUK){
		if(getLuk() <= 4)
			return Value(0,0);
		addLUK(-1, false);
		value = getLuk();
	}
	else if(stat == Update::INT){
		if(getInt() <= 4)
			return Value(0,0);
		addINT(-1, false);
		value = getInt();
	}
	else if(stat == Update::MAXHP){
		int add = 0;
		if(getMaxHPAP() <= 0)
			return Value(0,0);
		switch(getJob()/100){
			case 0: add = -8; break;
			case 1: add = -20; break;
			case 2: add = -6; break;
			default: add = -16; break;
		}
		addMaxHPAP(-1);
		addBaseMaxHP(add);
		value = getMaxHP();
	}
	else if(stat == Player::Update::MAXMP){
		int add = 0;
		if(getMaxMPAP() <= 0)
			return Value(0,0);
		switch(getJob()/100){
			case 0: add = -6; break;
			case 1: add = -2; break;
			case 2: add = -18; break;
			default: add = -10; break;
		}
		addMaxMPAP(-1);
		addBaseMaxMP(add);
		value = getMaxMP();
	}
	if(value == -1)
		return Value(0, 0);
	return Value(stat, value);
}
Value Player::addStat(int stat, bool rrandom){
	int value = -1;
	if(stat == Update::STR){
		if(getStr() >= 999)
			return Value(0, 0);
		addSTR(1, false);
		value = getStr();
	}
	else if(stat == Update::DEX){
		if(getDex() >= 999)
			return Value(0, 0);
		addDEX(1, false);
		value = getDex();
	}
	else if(stat == Update::LUK){
		if(getLuk() >= 999)
			return Value(0, 0);
		addLUK(1, false);
		value = getLuk();
	}
	else if(stat == Update::INT){
		if(getInt() >= 999)
			return Value(0, 0);
		addINT(1, false);
		value = getInt();
	}
	else if(stat == Player::Update::MAXHP){
		int add = 0;
		if(rrandom){
			switch(getJob()/100){
				case 0: add = random(8, 12); break;
				case 1:
				{
					add = random(24, 28);	
					Skill* skill = getSkills()->getSkill(1000001);
					if(skill != NULL && skill->getLevel() > 0){
						SkillLevelData* level = SkillsData::getInstance()->getDataByID(1000001)->getDataByID(skill->getLevel());
						if(level != NULL)
							add += level->getX();
					}
					break;
				}
				case 2: add = random(10, 14); break;
				default: add = random(16, 20); break;
			}
		}
		else{
			switch(getJob()/100){
				case 0: add = 8; break;
				case 1: add = 20; break;
				case 2: add = 6; break;
				default: add = 16; break;
			}
		}
		addMaxHPAP(1);
		addBaseMaxHP(add);
		value = getMaxHP();
	}
	else if(stat == Player::Update::MAXMP){
		int add = 0;
		if(rrandom){
			switch(getJob()/100){
				case 0: add = random(6, 8); break;
				case 1: add = random(2, 4); break;
				case 2:
				{
					add = random(18, 20);
					Skill* skill = getSkills()->getSkill(2000001);
					if(skill != NULL && skill->getLevel() > 0){
						SkillLevelData* level = SkillsData::getInstance()->getDataByID(2000001)->getDataByID(skill->getLevel());
						if(level != NULL)
							add += level->getX();
					}
					break;		   
				}
				default: add = random(10, 12); break;
			}
		}
		else{
			switch(getJob()/100){
				case 0: add = 6; break;
				case 1: add = 2; break;
				case 2: add = 18; break;
				default: add = 10; break;
			}
		}
		addMaxMPAP(1);
		addBaseMaxMP(add);
		value = getMaxMP();
	}
	if(value == -1)
		return Value(0, 0);
	return Value(stat, value);
}

void Player::setPet(Pet* pet, bool send){
	if(pet == NULL){
		if(this->pet != NULL)
			this->pet->stopTimer();
	}
	else
		pet->startTimer(this);
	this->pet = pet;
	if(send){
		if(pet == NULL)
			map->send(PacketCreator().removePet(id));
		else
			map->send(PacketCreator().showPet(id, pet));
		updateStat(Update::PET, (pet == NULL) ? 0 : pet->getItemID(), true);
		map->send(PacketCreator().updatePlayer(this), this);
	}
}

void Player::setSkin(char id, bool send){
	skin = id;
	if(send){
		updateStat(Update::SKIN, skin, true);
		map->send(PacketCreator().updatePlayer(this), this);
	}
}
void Player::setFace(int id, bool send){
	face= id;
	if(send){
		updateStat(Update::SKIN, face, true);
		map->send(PacketCreator().updatePlayer(this), this);
	}
}
void Player::setHair(int id, bool send){
	hair = id;
	if(send){
		updateStat(Update::SKIN, hair, true);
		map->send(PacketCreator().updatePlayer(this), this);
	}
}

void Player::setStyle(int id){
	if(id/10000 == 0){
		setSkin((char)id);
	}
	else if(id/10000 == 2){
		setFace(id);
	}
	else if(id/10000 == 3){
		setHair(id);
	}
}

int Player::getItemAmount(int itemid){
	return inv->getItemAmount(itemid);
}

bool Player::giveItem(int itemid, short amount){
	if(inv->giveItem(itemid, amount))
		send(PacketCreator().itemGainChat(itemid, amount));
	else
		return false;
	return true;
	
}
void Player::giveMesos(int amount){
	addMesos(amount);
	send(PacketCreator().mesosGainChat(amount));
}
void Player::save(){
}
/*
void Player::chaneKey(unsigned char* packet){
	int howmany = getIntg(packet+4);
	for(int i=0; i<howmany; i++){
		int pos = getIntg(packet+8+i*9);
		int key = getIntg(packet+12+i*9);
		if(packet[12+i*9] == 0) // TODO 1st type byte, than key int
			key=0;
		if(pos>=0 && pos<90)
			keys[pos] = key;
	}
}
void Player::addWarning(){
	int t = GetTickCount();
	// Deleting old warnings
	for(unsigned int i=0; i<warnings.size(); i++){
		if(warnings[i] + 300000 < t){
			warnings.erase(warnings.begin()+i);
			i--;
		}
	}
	warnings.push_back(t);
	if(warnings.size()>50){
		// Hacker - Temp DCing
		//int tmap = map;
		//Maps::changeMap(this, 999999999, 0);
		//Maps::changeMap(this, tmap, 0);
	}
}

void Player::save(){
	char sql[10000];
	sprintf_s(sql, 10000, "update keymap set ");
	for(int i=0; i<90; i++){
		char temp[100];
		if(i!=89)
			sprintf_s(temp, 100, "pos%d=%d, ", i, keys[i]);
		else
			sprintf_s(temp, 100, "pos%d=%d where charid=%d; ", i, keys[i], getPlayerid());
		strcat_s(sql, 10000, temp);
	}
	MySQL::insert(sql);
	sprintf_s(sql, 10000, "update characters set level=%d, job=%d, str=%d, dex=%d, intt=%d, luk=%d, chp=%d, mhp=%d, cmp=%d, mmp=%d, ap=%d, sp=%d, exp=%d, fame=%d, map=%d, gender=%d, skin=%d, eyes=%d, hair=%d, mesos=%d where id=%d", getLevel(), getJob(), getStr(), getDex(), getInt(), getLuk(), getHP(), getRMHP(), getMP(), getRMMP(), getAp(), getSp(), getExp(), getFame(), getMap(), getGender(), getSkin(), getEyes(), getHair(), inv->getMesos() ,getPlayerid());
	MySQL::insert(sql);
	char temp[1000];
	sprintf_s(sql, 10000, "delete from equip where charid=%d;", getPlayerid());
	MySQL::insert(sql);
	bool firstrun = true;
	for(int i=0; i<inv->getEquipNum(); i++){
		if(firstrun == true){
			sprintf_s(sql, 10000, "INSERT INTO equip VALUES (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", inv->getEquip(i)->id, Drops::equips[inv->getEquip(i)->id].type ,getPlayerid(), inv->getEquipPos(i), inv->getEquip(i)->slots, inv->getEquip(i)->scrolls,
				inv->getEquip(i)->istr, inv->getEquip(i)->idex, inv->getEquip(i)->iint, inv->getEquip(i)->iluk, inv->getEquip(i)->ihp, inv->getEquip(i)->imp, inv->getEquip(i)->iwatk, inv->getEquip(i)->imatk, inv->getEquip(i)->iwdef, 
				inv->getEquip(i)->imdef, inv->getEquip(i)->iacc, inv->getEquip(i)->iavo, inv->getEquip(i)->ihand, inv->getEquip(i)->ijump, inv->getEquip(i)->ispeed);
			firstrun = false;
		}
		else{
			sprintf_s(temp, 1000, ",(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", inv->getEquip(i)->id, Drops::equips[inv->getEquip(i)->id].type ,getPlayerid(), inv->getEquipPos(i), inv->getEquip(i)->slots, inv->getEquip(i)->scrolls,
				inv->getEquip(i)->istr, inv->getEquip(i)->idex, inv->getEquip(i)->iint, inv->getEquip(i)->iluk, inv->getEquip(i)->ihp, inv->getEquip(i)->imp, inv->getEquip(i)->iwatk, inv->getEquip(i)->imatk, inv->getEquip(i)->iwdef, 
				inv->getEquip(i)->imdef, inv->getEquip(i)->iacc, inv->getEquip(i)->iavo, inv->getEquip(i)->ihand, inv->getEquip(i)->ijump, inv->getEquip(i)->ispeed);
			strcat_s(sql, 10000, temp);
		}
	}
	MySQL::insert(sql);
	sprintf_s(sql, 10000, "delete from skills where charid=%d;", getPlayerid());
	MySQL::insert(sql);
	firstrun = true;
	for(int i=0; i<skills->getSkillsNum(); i++){
		if(firstrun == true){
			sprintf_s(sql, 10000, "INSERT INTO skills VALUES (%d, %d, %d)", getPlayerid(), skills->getSkillID(i), skills->getSkillLevel(skills->getSkillID(i)));
			firstrun = false;
		}
		else{
			sprintf_s(temp, 1000, ",(%d, %d, %d)", getPlayerid(), skills->getSkillID(i), skills->getSkillLevel(skills->getSkillID(i)));
			strcat_s(sql, 10000, temp);
		}
	}
	MySQL::insert(sql);
	sprintf_s(sql, 10000, "DELETE FROM items WHERE charid=%d;", getPlayerid());
	MySQL::insert(sql);
	firstrun = true;
	for(int i=0; i<inv->getItemNum(); i++){
		if(firstrun == true){
			sprintf_s(sql, 10000, "INSERT INTO items VALUES (%d, %d, %d, %d, %d)", inv->getItem(i)->id, getPlayerid() ,inv->getItem(i)->inv, inv->getItem(i)->pos, inv->getItem(i)->amount);
			firstrun = false;
		}
		else{
			sprintf_s(temp, 1000, ",(%d, %d, %d, %d, %d)", inv->getItem(i)->id, getPlayerid() ,inv->getItem(i)->inv, inv->getItem(i)->pos, inv->getItem(i)->amount);
			strcat_s(sql, 10000, temp);
		}
	}
	MySQL::insert(sql);
}*/