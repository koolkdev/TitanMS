#ifndef DAMAGE_H
#define DAMAGE_H

class PacketReader;

#include <vector>
#include <hash_map>

using namespace std;
using namespace stdext;

class Damage {
private:
	char stance;
	int mobCount;
	int attackCount;
	int skill;
	int itemslot;
	hash_map <int, vector <int>> damages;
	vector<int> mobs;
	unsigned char info;
	bool mesoexplosion;
public:
	Damage(PacketReader *packet, bool range = false);
	int getMobsCount(){
		return mobCount;
	}
	int getAttacksCount(){
		return attackCount;
	}
	vector<int>* getMobs(){
		return &mobs;
	}
	vector<int>* getDamageForMob(int mobid){
		return &damages[mobid];
	}	
	char getStance(){
		return stance;
	}
	int getSkill(){
		return skill;
	}
	int getItemSlot(){
		return itemslot;
	}
	char getInfoByte(){
		return info;
	}	
};

#endif