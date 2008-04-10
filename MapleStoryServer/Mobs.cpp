#include "Mobs.h"
#include "Maps.h"
#include "Player.h"
#include "MobsPacket.h"
#include "Drops.h"
#include "Levels.h"
#include "Quests.h"
#include "Skills.h"

hash_map <int, MobInfo> Mobs::mobinfo;
hash_map <int, SpawnsInfo> Mobs::info;
hash_map <int, vector<Mob*>> Mobs::mobs;
int Mobs::mobscount=0x200;

void Mob::setControl(Player* control){
	if(this->control != NULL)
		MobsPacket::endControlMob(this->control, this);

	this->control = control;
	if(control != NULL)
		MobsPacket::controlMob(control, this);
}

void Mobs::monsterControl(Player* player, unsigned char* packet, int size){
	int mobid = getInt(packet);
	Mob* mob = NULL;
	for(unsigned int i=0; i<mobs[player->getMap()].size(); i++)
		if(mobs[player->getMap()][i]->getID() == mobid){
			mob = mobs[player->getMap()][i];
			break;
		}
	if(mob == NULL)
		return;	
	if(mob->getControl() == player){
	//if(mob->getControl() != player)
		//mob->setControl(player);
		Pos cpos;
		cpos.x = getShort(packet+size-4);
		cpos.y = getShort(packet+size-2);
		mob->setPos(cpos);
		mob->setType(packet[size-12]);
		MobsPacket::moveMob(player, mob, Maps::info[player->getMap()].Players, packet, size);
	}
}

void Mobs::checkSpawn(int mapid){
	for(unsigned int i=0; i<info[mapid].size(); i++){
		int check=0;
		for(unsigned int j=0; j<mobs[mapid].size(); j++){
			if(i == mobs[mapid][j]->getMapID()){
				check=1;
				break;
			}
		}
		if(!check){
			Mob* mob = new Mob();
			mobs[mapid].push_back(mob);
			Pos mobpos;
			mobpos.x = info[mapid][i].x;
			mobpos.y = info[mapid][i].cy;
			mob->setPos(mobpos);
			mob->setID(i+100);
			mob->setMobID(info[mapid][i].id);
			mob->setMapID(i);
			mob->setHP(mobinfo[info[mapid][i].id].hp);
			mob->setMP(mobinfo[info[mapid][i].id].mp);
			mob->setFH(info[mapid][i].fh);
			mob->setType(2);
		}
		int num;
		for(unsigned int j=0; j<mobs[mapid].size(); j++)
			if(i == mobs[mapid][j]->getMapID()){
				num = j;
				break;
			}
		if(Maps::info[mapid].Players.size() > 0 && mobs[mapid][num]->getControl()==0){
			int maxpos = distPos(mobs[mapid][num]->getPos(), Maps::info[mapid].Players[0]->getPos());
			int posi = 0;
			for(unsigned int k=0; k<Maps::info[mapid].Players.size(); k++){
				int curpos = distPos(mobs[mapid][num]->getPos(), Maps::info[mapid].Players[k]->getPos());
				if(curpos < maxpos){
					maxpos = curpos;
					posi = k;
				}
			}
			if(!check)
				MobsPacket::spawnMob(Maps::info[mapid].Players[posi], mobs[mapid][num], Maps::info[mapid].Players, 1);
			mobs[mapid][num]->setControl(Maps::info[mapid].Players[posi]);
		}
	}
}	

void Mobs::showMobs(Player* player){
	for(unsigned int i=0; i<mobs[player->getMap()].size(); i++){
		MobsPacket::showMob(player, mobs[player->getMap()][i]);
	}
}

void Mobs::updateSpawn(int mapid){
	for(unsigned int i=0; i<mobs[mapid].size(); i++){
		if(Maps::info[mapid].Players.size() > 0 && mobs[mapid][i]->getControl()==NULL){
			int maxpos = distPos(mobs[mapid][i]->getPos(), Maps::info[mapid].Players[0]->getPos());
			int posi = 0;
			for(unsigned int k=0; k<Maps::info[mapid].Players.size(); k++){
				int curpos = distPos(mobs[mapid][i]->getPos(), Maps::info[mapid].Players[k]->getPos());
				if(curpos < maxpos){
					maxpos = curpos;
					posi = k;
				}
			}
			mobs[mapid][i]->setControl(Maps::info[mapid].Players[posi]);
		}
		else if(Maps::info[mapid].Players.size() == 0)
			mobs[mapid][i]->setControl(NULL);
	}
}
void Mobs::dieMob(Player* player, Mob* mob, int n){
	MobsPacket::dieMob(player, Maps::info[player->getMap()].Players, mob , mob->getID());
	/*
		for(unsigned int l=0; l< WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].summon.size(); l++){
		SpawnedInfo mob;
		mob.id = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].summon[l];
			mob.spid = -1;
		mob.mid = WZInitialize::mobid++;
		mob.x = WZInitialize::MapsInfo[pd.map].Spawned[j].x;
		mob.cy = WZInitialize::MapsInfo[pd.map].Spawned[j].cy;	mob.fh = WZInitialize::MapsInfo[pd.map].Spawned[j].fh;
							mob.hp = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].hp;
							mob.mp = WZInitialize::MobsInfo[WZInitialize::MapsInfo[pd.map].Spawned[j].id].mp;
							mob.by = pd.userid;
							SpawnMob(mob, pd);
						}
						*/
	Levels::giveEXP(player, mobinfo[mob->getMobID()].exp * 10);
	Drops::dropMob(player, mob);
	player->quests->updateQuestMob(mob->getMobID());
	mobs[player->getMap()].erase(mobs[player->getMap()].begin()+n);
	delete mob;
}
void Mobs::damageMobSkill(Player* player, unsigned char* packet){
	MobsPacket::damageMobSkill(player, Maps::info[player->getMap()].Players, packet);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	int map = player->getMap();
	int skillid = getInt(packet+2);
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
		int mobid = getInt(packet+14+i*22);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+32+i*22+k*4);
			for(unsigned int j=0; j<mobs[map].size(); j++){
				if(mobs[map][j]->getID() == mobid){
					mobs[map][j]->setHP(mobs[map][j]->getHP()-damage);
					int mhp=-1;
					mhp = mobinfo[mobs[map][j]->getMobID()].hp;
					MobsPacket::showHP(player, mobid ,mobs[map][j]->getHP()*100/mhp);
					if(mobs[map][j]->getHP() <= 0){
						dieMob(player, mobs[map][j], j);
					}
				break;
				}
			}
		}
	}	
}

void Mobs::damageMob(Player* player, unsigned char* packet){
	MobsPacket::damageMob(player, Maps::info[player->getMap()].Players, packet);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	int map = player->getMap();
	int skillid = getInt(packet+2);
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
		int mobid = getInt(packet+14+i*22);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+32+i*22+k*4);
			for(unsigned int j=0; j<mobs[map].size(); j++){
				if(mobs[map][j]->getID() == mobid){
					mobs[map][j]->setHP(mobs[map][j]->getHP()-damage);
					int mhp=-1;
					mhp = mobinfo[mobs[map][j]->getMobID()].hp;
					MobsPacket::showHP(player, mobid ,mobs[map][j]->getHP()*100/mhp);
					if(mobs[map][j]->getHP() <= 0){
						dieMob(player, mobs[map][j], j);
					}
				break;
				}
			}
		}
	}	
}
void Mobs::damageMobS(Player* player, unsigned char* packet, int size){
	int itemid=0;
	int pos=getInt(packet+14);
	for(int i=0; i<player->inv->getItemNum(); i++){
		if(player->inv->getItem(i)->pos == pos && player->inv->getItem(i)->inv == 2){
			itemid = player->inv->getItem(i)->id;
			break;
		}
	}
	MobsPacket::damageMobS(player, Maps::info[player->getMap()].Players, packet, itemid);
	int howmany = packet[1]/0x10;
	int hits = packet[1]%0x10;
	int map = player->getMap();
	int skillid = getInt(packet+2);
	if(skillid > 0)
		Skills::useAttackSkill(player, skillid);
	for(int i=0; i<howmany; i++){
		int mobid = getInt(packet+19+i*22);
		for(int k=0; k<hits; k++){
			int damage = getInt(packet+37+i*22+k*4);
			for(unsigned int j=0; j<mobs[map].size(); j++){
				if(mobs[map][j]->getID() == mobid){
					mobs[map][j]->setHP(mobs[map][j]->getHP()-damage);
					int mhp=-1;
					mhp = mobinfo[mobs[map][j]->getMobID()].hp;
					MobsPacket::showHP(player, mobid ,mobs[map][j]->getHP()*100/mhp);
					if(mobs[map][j]->getHP() <= 0){
						dieMob(player, mobs[map][j], j);
					}
				break;
				}
			}
		}
	}	
}

void Mobs::spawnMob(Player* player, int mobid){
	Mob* mob = new Mob();
	mobs[player->getMap()].push_back(mob);
	mob->setPos(player->getPos());
	mob->setID(mobscount++);
	mob->setMobID(mobid);
	mob->setMapID(-1);
	mob->setHP(mobinfo[mobid].hp);
	mob->setMP(mobinfo[mobid].mp);
	mob->setFH(0);
	mob->setControl(player);
	mob->setType(2);
	for(unsigned int j=0; j<Maps::info[player->getMap()].Players.size(); j++)
		MobsPacket::showMob(Maps::info[player->getMap()].Players[j], mob);
}