#include "Player.h"
#include "Drops.h"
#include "Inventory.h"
#include "Maps.h"
#include "Mobs.h"
#include "NPCs.h"
#include "Players.h"
#include "MySQLM.h"
#include "PlayerPacket.h"
#include "Levels.h"
#include "Skills.h"
#include "Quests.h"

int distPos(Pos pos1, Pos pos2){
	return (int)sqrt(pow((float)(pos1.x+pos2.x), 2)+pow((float)(pos1.y+pos2.y), 2));
}

Player::~Player(){
	if(isconnect){
		Maps::removePlayer(this);
		Players::deletePlayer(this);
		char sql[2000];
		sprintf_s(sql, 2000, "update keymap set ");
		for(int i=0; i<90; i++){
			char temp[100];
			if(i!=89)
				sprintf_s(temp, 100, "pos%d=%d, ", i, keys[i]);
			else
				sprintf_s(temp, 100, "pos%d=%d ", i, keys[i]);
			strcat_s(sql, 2000, temp);
		}
		char temp[100];
		sprintf_s(temp, 100, "where charid=%d;", getPlayerid());
		strcat_s(sql, 2000, temp);
		MySQL::insert(sql);
		sprintf_s(sql, 2000, "delete from equip where charid=%d;", getPlayerid());
		MySQL::insert(sql);
		for(int i=0; i<inv->getEquipNum(); i++){
			sprintf_s(sql, 2000, "insert into equip values(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d);", inv->getEquip(i)->id, Drops::equips[inv->getEquip(i)->id].type ,getPlayerid(), inv->getEquipPos(i), inv->getEquip(i)->slots, inv->getEquip(i)->scrolls,
				inv->getEquip(i)->istr, inv->getEquip(i)->idex, inv->getEquip(i)->iint, inv->getEquip(i)->iluk, inv->getEquip(i)->ihp, inv->getEquip(i)->imp, inv->getEquip(i)->iwatk, inv->getEquip(i)->imatk, inv->getEquip(i)->iwdef, 
				inv->getEquip(i)->imdef, inv->getEquip(i)->iacc, inv->getEquip(i)->iavo, inv->getEquip(i)->ihand, inv->getEquip(i)->ijump, inv->getEquip(i)->ispeed);
			MySQL::insert(sql);
		}
		sprintf_s(sql, 2000, "delete from items where charid=%d;", getPlayerid());
		MySQL::insert(sql);
		for(int i=0; i<inv->getItemNum(); i++){
			sprintf_s(sql, 2000, "insert into items values(%d, %d, %d, %d, %d);", inv->getItem(i)->id, getPlayerid() ,inv->getItem(i)->inv, inv->getItem(i)->pos, inv->getItem(i)->amount);
			MySQL::insert(sql);
		}
		Skills::stopTimerPlayer(this);
		isconnect = false;
	}
}

void Player::handleRequest(unsigned char* buf, int len){
	short header = buf[0] + buf[1]*0x100;	
	printf("*%x*",header);
	switch(header){
		case 0x10: getUserID(buf+2); break;
		case 0x20: Maps::moveMap(this ,buf+2); break;
		case 0x23: Players::handleMoving(this ,buf+2, len-2); break;
		case 0x24: Inventory::stopChair(this ,buf+2); break;
		case 0x25: Inventory::useChair(this ,buf+2); break;
		case 0x26: Mobs::damageMob(this ,buf+2); break;
		case 0x27: Mobs::damageMobS(this ,buf+2, len-2); break;
		case 0x28: Mobs::damageMobSkill(this ,buf+2); break;
		case 0x29: Players::damagePlayer(this ,buf+2); break;
		case 0x2C: Players::faceExperiment(this ,buf+2); break;
		case 0x2A: Players::chatHandler(this ,buf+2); break;
		case 0x31: NPCs::handleNPC(this, buf+2); break;
		case 0x32: NPCs::handleNPCIn(this ,buf+2); break;
		case 0x33: Inventory::buyItem(this ,buf+2); break;
		case 0x3A: Inventory::itemMove(this ,buf+2); break;
		case 0x3B: Inventory::useItem(this, buf+2); break;
		case 0x48: Levels::addStat(this, buf+2); break;
		case 0x49: Players::healPlayer(this, buf+2); break;
		case 0x4A: Skills::addSkill(this, buf+2); break;
		case 0x4B: Skills::useSkill(this, buf+2); break;
		case 0x4C: Skills::cancelSkill(this, buf+2); break;
		case 0x4E: Drops::dropMesos(this ,buf+2); break;
		case 0x5A: Quests::getQuest(this, buf+2); break;
		case 0x75: chaneKey(buf+2);
		case 0x8A: Mobs::monsterControl(this ,buf+2, len-2); break;
		case 0x8B: Mobs::monsterControlSkill(this ,buf+2); break;
		case 0x96: Drops::lootItem(this ,buf+2); break;
	}
}

void Player::playerConnect(){
	MySQL::getStringI("characters", "ID", getPlayerid(), "name", name);
	gender = (char)MySQL::getInt("characters", getPlayerid(), "gender");
	skin = (char)MySQL::getInt("characters", getPlayerid(), "skin");
	eyes = MySQL::getInt("characters", getPlayerid(), "eyes");
	hair = MySQL::getInt("characters", getPlayerid(), "hair");
	level = (char)MySQL::getInt("characters", getPlayerid(), "level");
	job = (short)MySQL::getInt("characters", getPlayerid(), "job");
	str = (short)MySQL::getInt("characters", getPlayerid(), "str");
	dex = (short)MySQL::getInt("characters", getPlayerid(), "dex");
	intt = (short)MySQL::getInt("characters", getPlayerid(), "intt");
	luk = (short)MySQL::getInt("characters", getPlayerid(), "luk");
	hp = (short)MySQL::getInt("characters", getPlayerid(), "chp");
	mhp = (short)MySQL::getInt("characters", getPlayerid(), "mhp");
	mp = (short)MySQL::getInt("characters", getPlayerid(), "cmp");
	mmp = (short)MySQL::getInt("characters", getPlayerid(), "mmp");
	ap = (short)MySQL::getInt("characters", getPlayerid(), "ap");
	sp = (short)MySQL::getInt("characters", getPlayerid(), "sp");
	exp = (short)MySQL::getInt("characters", getPlayerid(), "exp");
	fame = (short)MySQL::getInt("characters", getPlayerid(), "fame");
	map = MySQL::getInt("characters", getPlayerid(), "map");
	mappos = (char)MySQL::getInt("characters", getPlayerid(), "pos");
	int equips[115][21];
	int many = MySQL::showEquipsIn(getPlayerid(), equips);
	inv = new PlayerInventory();
	skills = new PlayerSkills();
	quests = new PlayerQuests();
	quests->setPlayer(this);
	for(int i=0; i<many; i++){
		Equip* equip = new Equip;
		equip->id = equips[i][0];
		equip->slots = equips[i][4];
		equip->scrolls = equips[i][5];
		equip->type = equips[i][1];
		equip->pos = equips[i][3];
		equip->istr = equips[i][6];
		equip->idex = equips[i][7];
		equip->iint = equips[i][8];
		equip->iluk = equips[i][9];
		equip->ihp = equips[i][10];
		equip->imp = equips[i][11];
		equip->iwatk = equips[i][12];
		equip->imatk = equips[i][13];
		equip->iwdef = equips[i][14];
		equip->imdef = equips[i][15];
		equip->iacc = equips[i][16];
		equip->iavo = equips[i][17];
		equip->ihand = equips[i][18];
		equip->ijump = equips[i][19];
		equip->ispeed = equips[i][20];
		inv->addEquip(equip);
	}
	int items[400][4];
	many = MySQL::getItems(getPlayerid(), items);
	for(int i=0; i<many; i++){
		Item* item = new Item;
		item->id = items[i][0];
		item->inv = items[i][1];
		item->pos = items[i][2];
		item->amount = items[i][3];
		inv->addItem(item);
	}
	inv->setMesosStart(MySQL::getInt("characters", getPlayerid(), "mesos"));
	inv->setPlayer(this);
	MySQL::getKeys(getPlayerid(), keys);
	PlayerPacket::headerNotice(this);
	PlayerPacket::connectData(this);
	pos.x = Maps::info[map].Portals[0].x;
	pos.y = Maps::info[map].Portals[0].y;
	type=0;
	Players::addPlayer(this);
	NPCs::showNPCs(this);
	Maps::addPlayer(this);
	PlayerPacket::showKeys(this, keys);
	Mobs::showMobs(this);
	Drops::showDrops(this);

}

void Player::setHP(short hp){
	this->hp=hp;
	if(this->hp<0)
		this->hp=0;
	if(this->hp>mhp)
		this->hp=mhp;
	PlayerPacket::newHP(this, this->hp);
}

void Player::setMP(short mp, bool is){
	this->mp=mp;
	if(this->mp<0)
		this->mp=0;
	if(this->mp>mmp)
		this->mp=mmp;
	PlayerPacket::newMP(this, this->mp, is);
}

void Player::setSp(short sp){
	this->sp=sp;
	PlayerPacket::setSP(this);
}

void Player::setJob(short job){
	this->job=job;
	PlayerPacket::setJob(this);
}


void Player::setExp(int exp, bool is){
	this->exp=exp;
	if(is)
		PlayerPacket::newEXP(this, this->exp);
}

void Player::setLevel(int level){
	this->level=level;

}
int getIntg(unsigned char* buf){
	return buf[0] + buf[1]*0x100 + buf[2]*0x100*0x100 + buf[3]*0x100*0x100*0x100;
}

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

void Player::setHair(int id){
	this->hair = id;
	PlayerPacket::newHair(this);
}

void Player::setEyes(int id){
	this->eyes = id;
	PlayerPacket::newEyes(this);
}

void Player::setSkin(char id){
	this->skin = id;
	PlayerPacket::newSkin(this);
}