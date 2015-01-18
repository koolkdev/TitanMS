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

#include "../Connection/PacketHandler.h"
#include "Player.h"
#include "Worlds.h"
#include "Channel.h"
#include "Players.h"
#include "PlayerHandler.h"
#include "PacketReader.h"
#include "Pet.h"
#include "PacketWriter.h"
#include "Transportations.h"
#include "PartyMember.h"
#include "PartyMembers.h"
#include "Party.h"
#include "PlayerNPC.h"
#include "MySQLM.h"
#include "Map.h"
#include "Maps.h"
#include "MapsData.h"
#include "MapData.h"
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
#include "PacketHandlingError.h"
#include "Trade.h"
#include "Event.h"
#include "Events.h"
#include <algorithm>
#include "Timer.h"
#include "World.h"
#include "Parties.h"
#include "Tools.h"
using namespace Tools;
using namespace mysqlpp;

int strval(string& str);
__int64 strval64(string& num);

struct ComparePets : public std::binary_function<Pet*, Pet*, bool>
{
	bool operator()(Pet* x, Pet* y) const
	{
		return x->getPetSlot() < y->getPetSlot();
	}
};

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
	loaded = false;
	stance = 0;
	chair = 0;
	itemEffect = 0;
	npcShop = NULL;
	npc = NULL;
	trade = NULL;
	party = NULL;
	partyInvite = 0;
	handler = new PlayerHandler(this);
}
Player::~Player(){
	if(isconnect){
		if(map->getEvent() != NULL) channel->getEvents()->deleteEventForPlayer(map->getEvent()->getName(), this, true);
		if(trade != NULL) channel->getTransportations()->closeTrade(trade);
		if(party != NULL){
			party->getMembers()->removeMember(id);
			party->getMembers()->send(PacketCreator().updateParty(party));
		}
		map->getPlayers()->remove(this);
		channel->getPlayers()->remove(this);
		if(loaded) save();
		buffs->cancelAll();
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


void Player::disconnect(){
	packetHandler->disconnect();
	for(int i=0; i<100000; i++)
	send(PacketCreator().ping());
}
void Player::send(PacketWriter* packet){
	unsigned char bytes [MAX_LENGTH];
	memcpy_s(bytes, packet->getLength(), packet->getBytes(), packet->getLength());
	sendPacket(bytes, packet->getLength());
}

void Player::handleRequest(unsigned char* buf, int len){
	try{
		handler->handle(PacketReader(buf, len, id));
	}
#ifdef _DEBUG
	catch(PacketHandlingError ph){
		cout << ph.getError();
	}
	catch (BadQuery er ) {
		printf("%s\n", er.what());
	}
	catch (char* er ) {
		printf("%s\n", er);
	}
	catch( const std::exception &e ) {
		std::cerr << "Exception catch: " << e.what() << std::endl;
	}
#endif
	catch(...){
		//TODO
	}
}

void Player::playerConnect(){
	string values[29];
	MySQL::getInstance()->getCharacter(id, values);
	if(values[25] != getIP() && values[25] != "127.0.0.1" && getIP() != "127.0.0.1" && stringToIP(getIP()) != *Worlds::getInstance()->getIP() && stringToIP(values[25]) != *Worlds::getInstance()->getIP()){ // localhost
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
	if(MapsData::getInstance()->getDataByID(map->getID())->getForcedReturn() != 999999999){
		Map* fmap = channel->getMaps()->getMap(MapsData::getInstance()->getDataByID(map->getID())->getForcedReturn());
		if(fmap != NULL)
			map = fmap;
	}
	mappos = strval(values[19]);
	gender = strval(values[20]);
	skin = strval(values[21]);
	face = strval(values[22]);
	hair = strval(values[23]);
	mesos = strval(values[24]);
	hpap = strval(values[26]);
	mpap = strval(values[27]);
	int partyid = strval(values[28]);
	if(partyid != 0)
		party = channel->getWorld()->getParties()->getParty(partyid);
	gm = MySQL::getInstance()->getInt("users", MySQL::getInstance()->getInt("characters", id, "userid"), "gm");
	Query query = MySQL::getInstance()->getDataBase()->query();
	StoreQueryResult res;
	inv = new PlayerInventories(this);
	skills = new PlayerSkills(this);
	quests = new PlayerQuests(this);
	keys = new PlayerKeys(this);
	buffs = new PlayerBuffs(this);
	isconnect = true;
	query << "SELECT * FROM equips WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		Equip* equip = new Equip();
		equip->setID(strval(string(res[i][0])));
		equip->setSlots(strval(string(res[i][3])));
		equip->setScrolls(strval(string(res[i][4])));
		equip->setSlot(strval(string(res[i][2])));
		equip->setStr(strval(string(res[i][5])));
		equip->setDex(strval(string(res[i][6])));
		equip->setInt(strval(string(res[i][7])));
		equip->setLuk(strval(string(res[i][8])));
		equip->setHP(strval(string(res[i][9])));
		equip->setMP(strval(string(res[i][10])));
		equip->setWAtk(strval(string(res[i][11])));
		equip->setMAtk(strval(string(res[i][12])));
		equip->setWDef(strval(string(res[i][13])));
		equip->setMDef(strval(string(res[i][14])));
		equip->setAcc(strval(string(res[i][15])));
		equip->setAvo(strval(string(res[i][16])));
		equip->setHand(strval(string(res[i][17])));
		equip->setSpeed(strval(string(res[i][18])));
		equip->setJump(strval(string(res[i][19])));
		equip->setOwner(string(res[i][20]));
		equip->setLocked(string(res[i][21]) == "1");
		inv->getInventory(equip->getSlot()>0)->addItem(equip, false, false, false);
	}
	query << "SELECT * FROM items WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		Item* item = new Item();
		item->setID(strval(string(res[i][0])));
		item->setSlot(strval(string(res[i][3])));
		item->setAmount(strval(string(res[i][4])));
		inv->getInventory(strval(string(res[i][2])))->addItem(item, false, false, false);
	}
	query << "SELECT * FROM skills WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		Skill* skill = new Skill(strval(string(res[i][1])), strval(string(res[i][2])), strval(string(res[i][3])));
		skills->addSkill(skill);
	}

	query << "SELECT * FROM keymap WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		Key* key = new Key(strval(string(res[i][2])), strval(string(res[i][3])));
		keys->setKey(strval(string(res[i][1])), key);
	}
	query << "SELECT * FROM pets WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		Pet* pet = new Pet(strval(string(res[i][0])));
		pet->setSlot(strval(string(res[i][2])));
		pet->setPetSlot(strval(string(res[i][3])));
		pet->setName(string(res[i][4]));
		pet->setLevel(strval(string(res[i][5])));
		pet->setCloseness(strval(string(res[i][6])));
		pet->setFullness(strval(string(res[i][7])));
		pet->setTime(strval64(string(res[i][8])));
		inv->getInventory(CASH)->addItem(pet, false, false, false);
		if(pet->getPetSlot() >= 0)
			pets.push_back(pet);	
	}
	query << "SELECT * FROM vars WHERE charid=" << id;
	res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		setGlobalVariable(string(res[i][1]), strval(string(res[i][2])));
	}
	setPosition(map->getPortalPosition(mappos));
	stance = 0;
	send(PacketCreator().connectionData(this));
	loaded = true;
	send(PacketCreator().showKeys(keys));
	channel->addPlayer(this);
	if(party != NULL){
		party->getMembers()->addMember(this);
		party->getMembers()->send(PacketCreator().updateParty(party));
		party->getMembers()->send(PacketCreator().updatePartyHP(this), map, this);
		hash_map <int, Player*>* players = party->getMembers()->getPlayers();
			for(hash_map<int, Player*>::iterator iter = players->begin(); iter != players->end(); iter++)
				if(iter->second->getMap() == map && iter->second != this)
					send(PacketCreator().updatePartyHP(iter->second));
	}
	sort<vector <Pet*>::iterator, ComparePets>(pets.begin(), pets.end(), ComparePets());
	for(int i=0; i<(int)pets.size(); i++){
		pets[i]->setPosition(pos);
		pets[i]->setStance(0);
		pets[i]->startTimer(this);
		map->send(PacketCreator().showPet(id, pets[i]));
	}
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
		if(party != NULL){
			party->getMembers()->getMember(id)->setLevel(level);
			party->getMembers()->send(PacketCreator().updateParty(party));
		}
	}
}
void Player::setJob(short job, bool send){
	this->job = job;
	if(send){
		updateStat(Update::JOB, this->job);
		map->send(PacketCreator().showEffect(id, Effects::JOB_CHANGE), this);
		if(party != NULL){
			party->getMembers()->getMember(id)->setJob(job);
			party->getMembers()->send(PacketCreator().updateParty(party));
		}
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
		if(party != NULL){
			party->getMembers()->send(PacketCreator().updatePartyHP(this), map);
		}
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
	if(mhp > 30000)
		mhp = 30000;
	this->mhp=mhp;
	if(party != NULL){
		party->getMembers()->send(PacketCreator().updatePartyHP(this), map);
	}
	if(send){
		updateStat(Update::MAXHP, this->mhp, item);
	}
}
void Player::setBaseMaxHP(int rmhp){
	if(rmhp > 30000)
		rmhp = 30000;
	this->rmhp=rmhp;
	mhp=rmhp;
}
void Player::setMaxMP(int mmp, bool send, bool item){
	if(mmp > 30000)
		mmp = 30000;
	this->mmp=mmp;
	if(send){
		updateStat(Update::MAXMP, this->mmp, item);
	}
}
void Player::setBaseMaxMP(int rmmp){
	if(rmmp > 30000)
		rmmp = 30000;
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
	if(party != NULL){
		party->getMembers()->getMember(id)->setLevel(level);
		party->getMembers()->send(PacketCreator().updateParty(party));
	}
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
	rmhp = (rmhp > 30000) ? 30000 : rmhp;
	rmmp = (rmmp > 30000) ? 30000 : rmmp;
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
	send(PacketCreator().updateStats(values));


}
void Player::updateStat(int stat, int value, bool item, char pet){
	Values v = Values();
	v.add(Value(stat, value));
	send(PacketCreator().updateStats(v, item, pet));
}

void Player::setMap(int mapid){
	Map* map = channel->getMaps()->getMap(mapid);
	if(map != NULL)
		changeMap(map);
}
void Player::setMapPos(int mapid, int pos){
	Map* map = channel->getMaps()->getMap(mapid);
	if(map != NULL)
		if(map->getPortal(pos) != NULL)
			changeMap(map, pos);
}
void Player::setMapPortal(int mapid, string& portalname){
	Map* map = channel->getMaps()->getMap(mapid);
	if(map != NULL){
		MapPortalData* portal = map->getPortal(portalname);
		if(portal != NULL)
			changeMap(map, portal->getID());
	}
}
void Player::changeMap(Map* map, int portal){
	Map* oldmap = this->map;
	this->map = map;
	if(oldmap->getEvent() != NULL && map->getEvent() != oldmap->getEvent()){
		channel->getEvents()->deleteEventForPlayer(oldmap->getEvent()->getName(), this);
	}
	else if(map->getEvent() != NULL) map->getEvent()->getPlayers()->add(this);
	oldmap->removePlayer(this);
	mappos = portal;
	send(PacketCreator().changeMap(this));
	MapPortalData* tportal = map->getPortal(portal);
	this->pos.x = tportal->getX();
	this->pos.y = tportal->getY();
	if(hp == 0) hp = 50;
	stance = 0;
	fh = 0;
	chair = 0;
	if(pvp)
		pvp = false;
	map->addPlayer(this);
	if(party != NULL){
		party->getMembers()->getMember(id)->setMap(map->getID());
		party->getMembers()->send(PacketCreator().updateParty(party));
		party->getMembers()->send(PacketCreator().updatePartyHP(this), map, this);
		hash_map <int, Player*>* players = party->getMembers()->getPlayers();
			for(hash_map<int, Player*>::iterator iter = players->begin(); iter != players->end(); iter++)
				if(iter->second->getMap() == map && iter->second != this)
					send(PacketCreator().updatePartyHP(iter->second));
	}
	for(int i=0; i<(int)pets.size(); i++){
		pets[i]->setPosition(getPosition());
		pets[i]->setStance(getStance());
		map->send(PacketCreator().showPet(id, pets[i]));
	}
		
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
		//if(getStr() >= 999)
		//	return Value(0, 0);
		addSTR(1, false);
		value = getStr();
	}
	else if(stat == Update::DEX){
		//if(getDex() >= 999)
		//	return Value(0, 0);
		addDEX(1, false);
		value = getDex();
	}
	else if(stat == Update::LUK){
		//if(getLuk() >= 999)
		//	return Value(0, 0);
		addLUK(1, false);
		value = getLuk();
	}
	else if(stat == Update::INT){
		//if(getInt() >= 999)
		//	return Value(0, 0);
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

void Player::addPet(Pet* pet){
	if(pets.size() >= 3){  // instead the first pet
		pets[0]->stopTimer();
		pets[0]->setPetSlot(-1);
		pet->setPetSlot(0);
		pets[0] = pet;
	}
	else{
		pet->setPetSlot(pets.size());
		pets.push_back(pet);
	}
	pet->setStance(0);
	pet->setPosition(getPosition());
	pet->startTimer(this);
	map->send(PacketCreator().showPet(id, pet));
	updateStat(Update::PET, (pet == NULL) ? 0 : pet->getItemID(), false, 3-pets.size());
	send(PacketCreator().enableAction());
	map->send(PacketCreator().updatePlayer(this), this);
}
void Player::removePet(Pet* pet, bool die){
	for(int i=0; i<(int)pets.size(); i++){
		if(pets[i] == pet){
			map->send(PacketCreator().removePet(id, pet->getPetSlot(), die));
			pet->stopTimer();
			pet->setPetSlot(-1);
			updateStat(Update::PET, (pet == NULL) ? 0 : pet->getItemID(), false);
	send(PacketCreator().enableAction());
			pets.erase(pets.begin()+i);
			break;
		}
	}
	for(int i=0; i<(int)pets.size(); i++){
		pets[i]->setPetSlot(i);
	}
}
Pet* Player::getPet(int id){
	for(int i=0; i<(int)pets.size(); i++){
		if(pets[i]->getObjectID() == id){
			return pets[i];
		}
	}
	return NULL;	
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
		updateStat(Update::FACE, face, true);
		map->send(PacketCreator().updatePlayer(this), this);
	}
}
void Player::setHair(int id, bool send){
	hair = id;
	if(send){
		updateStat(Update::HAIR, hair, true);
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
	if(amount < 0){
		if(-amount > inv->getItemAmount(itemid))
			return 0;
		inv->getInventory(INVENTORY(itemid))->removeItem(itemid, -amount);
		send(PacketCreator().itemGainChat(itemid, amount));
	}
	else{
		if(inv->giveItem(itemid, amount))
			send(PacketCreator().itemGainChat(itemid, amount));
		else
			return false;
	}
	return true;
	
}
void Player::giveMesos(int amount){
	addMesos(amount);
	send(PacketCreator().mesosGainChat(amount));
}
void Player::giveExp(int e){
	addExp(e);
	send(PacketCreator().gainEXP(e, true));
}
bool Player::checkSlot(int inv){
	return this->inv->getInventory(inv)->getItems()->size() < (unsigned int)this->inv->getInventory(inv)->getSlots();

}
void Player::save(){
	try {
		Connection* con = MySQL::getInstance()->getDataBase();
		stringstream q;

		q << "update characters set level=" << (int)level << " ,job=" << (int)job << ", str=" << (int)str << ", dex=" << (int)dex << ", intt=" << (int)intt << ", luk=" << (int)luk << ", chp=" << (int)hp << ", mhp=" << (int)rmhp << ", cmp=" << (int)mp << ", mmp=" << (int)rmmp << ", ap=" << (int)ap << ", sp=" << (int)sp << ", exp=" << (int)exp << ", fame=" << (int)fame << ", map=" << map->getID() << ", pos=" << (int)map->getClosestSpawnPos(pos) << ", gender=" << (int)gender << ", face=" << (int)face << ", hair=" << (int)hair << ", mesos=" << mesos << ", hpap=" << (int)hpap << ", mpap=" << (int)mpap << ", party=" << (int)((party == NULL) ? 0 : party->getID()) << " where ID="<< id;
		con->query(q.str()).execute();
		q.str("");
		q << "delete from equips where charid=" << id;
		con->query(q.str()).execute();
		q.str("");
		bool first = true;
		for(int i=EQUIPPED; i<=EQUIP; i++){
			hash_map <int, Item*>* items = inv->getInventory(i)->getItems();
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				Item* item = iter->second;
				if(IS_EQUIP(item->getID())){
					if(!first)
						q << ",";
					else{
						q << "insert into equips values";
						first = false;
					}
					Equip* equip = (Equip*)item;
					q << "(" << equip->getID() << "," << id << "," << (int)equip->getSlot() << "," << (int)equip->getSlots() << "," << (int)equip->getScrolls() << "," << (int)equip->getStr() << "," << (int)equip->getDex() << "," << (int)equip->getInt() << "," << (int)equip->getLuk() << "," << (int)equip->getHP() << "," << (int)equip->getMP() << "," << (int)equip->getWAtk() << "," << (int)equip->getMAtk() << "," << (int)equip->getWDef() << "," << (int)equip->getMDef() << "," << (int)equip->getAcc() << "," << (int)equip->getAvo() << "," << (int)equip->getHand() << "," << (int)equip->getSpeed() << "," << (int)equip->getJump() << ",'" << equip->getOwner() << "'," << ((equip->getLocked()) ? 1 : 0) << ")";
				}
			}
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
		q << "delete from items where charid=" << id;
		con->query(q.str()).execute();
		q.str("");
		first = true;
		for(int i=USE; i<=CASH; i++){
			hash_map <int, Item*>* items = inv->getInventory(i)->getItems();
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				Item* item = iter->second;

				if(!IS_PET(item->getID())){
					if(!first)
						q << ",";
					else{
						q << "insert into items values";
						first = false;
					}
					q << "(" << item->getID() << "," << id << "," << (int)item->getType() << "," << (int)item->getSlot() << "," << (int)item->getAmount() << ")";
				}
			}
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
		q << "delete from pets where charid=" << id;
		MySQL::getInstance()->getDataBase()->query(q.str()).execute();
		q.str("");
		first = true;
		for(int i=CASH; i<=CASH; i++){
			hash_map <int, Item*>* items = inv->getInventory(i)->getItems();
			for(hash_map<int, Item*>::iterator iter = items->begin(); iter!=items->end(); iter++){
				Item* item = iter->second;
				if(IS_PET(item->getID())){
					if(!first)
						q << ",";
					else{
						q << "insert into pets values";
						first = false;
					}
					Pet* pet = (Pet*)item;
					char number[50];
					sprintf_s(number, 50, "%I64d", pet->getTime());
					q << "(" << pet->getItemID() << "," << id << "," << (int)pet->getSlot() << "," << (int)pet->getPetSlot() << ",'" << pet->getName() << "'," << (int)pet->getLevel() << "," << (int)pet->getCloseness() << "," << (int)pet->getFullness() << "," << number << ")";
				}
			}
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
		first = true;
		q << "delete from keymap where charid=" << id;
		con->query(q.str()).execute();
		q.str("");
		for(int i=0; i<90; i++){
			Key* key = keys->getKey(i);
			if(key != NULL){
				if(!first)
					q << ",";
				else{
					q << "insert into keymap values";
					first = false;
				}
				q << "(" << id << "," << i << "," << (int)key->getType() << "," << key->getAction() << ")";
			}
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
		q << "delete from skills where charid=" << id;
		con->query(q.str()).execute();
		q.str("");
		hash_map <int, Skill*>* skillsd = skills->getSkillsInfo();
		first = true;
		for(hash_map<int, Skill*>::iterator iter = skillsd->begin(); iter!=skillsd->end(); iter++){
			Skill* skill = iter->second;
			if(skill->getLevel() > 0 || skill->getMasterLevel() > 0){
				if(!first)
					q << ",";
				else{
					q << "insert into skills values";
					first = false;
				}
				q << "(" << id << "," << skill->getID() << "," << (int)skill->getLevel() << "," << (int)skill->getMasterLevel() << ")";
			}
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
		q << "delete from vars where charid=" << id;
		con->query(q.str()).execute();
		q.str("");
		first = true;
		for(hash_map<string, int>::iterator iter = global.begin(); iter!=global.end(); iter++){
			if(!first)
				q << ",";
			else{
				q << "insert into vars values";
				first = false;
			}
			q << "(" << id << ", '" << iter->first << "' , '" << iter->second << "')";
		}
		if(!first) con->query(q.str()).execute();
		q.str("");
	}
	catch (BadQuery er ) {
		printf("%s\n", er.what());
	}
}

int Player::getWDef(){
	int wdef = 0;
	hash_map<int, Item*>* items = inv->getInventory(EQUIPPED)->getItems();
	for(hash_map<int, Item*>::iterator iter = items->begin(); iter != items->end(); iter++){
		wdef += ((Equip*)iter->second)->getWDef();
	}
	return wdef;
}
int Player::getMDef(){
	int mdef = 0;
	hash_map<int, Item*>* items = inv->getInventory(EQUIPPED)->getItems();
	for(hash_map<int, Item*>::iterator iter = items->begin(); iter != items->end(); iter++){
		mdef += ((Equip*)iter->second)->getMDef();
	}
	return mdef + intt;
}
void Player::setEvent(string& name){
	channel->getEvents()->addPlayer(name, this);
}