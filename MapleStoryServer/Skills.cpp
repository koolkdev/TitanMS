 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#include "Skills.h"
#include "Player.h"
#include "SkillsPacket.h"
#include "Inventory.h"
#include "Maps.h"

hash_map <int, SkillsLevelInfo> Skills::skills;
hash_map <int, SkillsInfo> Skills::skillsinfo;
#include "Timer.h"

#define BEGINNER_SKILL(x) (x<10000)

class SkillTimer: public Timer::TimerHandler {
public:
	void setSkillTimer(Player* player, int skill, int time){
		STimer timer;
		timer.id = Timer::timer->setTimer(time, this);
		timer.player = player;
		timer.skill = skill;
		timer.time = time;
		timers.push_back(timer);
		act[timer.id] = false;
	}
	void setSkillTimer(Player* player, int skill, char* name, short value, int time){
		SActTimer timer;
		timer.id = Timer::timer->setTimer(time, this);
		timer.player = player;
		timer.skill = skill;
		strcpy_s(timer.act, 50, name);
		timer.time = time;
		timer.value = value;
		acttimers.push_back(timer);
		act[timer.id] = true;
	}
	void stop (Player* player, int skill){
		for(unsigned int i=0; i<timers.size(); i++){
			if(player == timers[i].player && timers[i].skill == skill){
				Timer::timer->cancelTimer(timers[i].id);
				break;
			}
		}
	}
	void stop (Player* player, int skill, char* name){
		for(unsigned int i=0; i<acttimers.size(); i++){
			if(player == acttimers[i].player && strcmp(acttimers[i].act, name) == 0 && skill == acttimers[i].skill){
				Timer::timer->cancelTimer(acttimers[i].id);
				break;
			}
		}
	}
	void stop (Player* player){
        for(unsigned int i=timers.size(); i>0; i--){
            if(player == timers[i-1].player){
                Timer::timer->cancelTimer(timers[i-1].id);
            }
        } 
        for(unsigned int i=acttimers.size(); i>0; i--){ 
            if(player == acttimers[i-1].player){
                Timer::timer->cancelTimer(acttimers[i-1].id);
            }
        } 
	}
	void stopKill (Player* player){
        for(unsigned int i=timers.size(); i>0; i--){
            if(player == timers[i-1].player){
				Skills::endSkill(player, timers[i].id);
                Timer::timer->cancelTimer(timers[i-1].id);
            }
        } 
        for(unsigned int i=acttimers.size(); i>0; i--){ 
            if(player == acttimers[i-1].player){
                Timer::timer->cancelTimer(acttimers[i-1].id);
            }
        } 
	}
private:
	struct STimer {
		int id;
		Player* player;
		int skill;
		int time;
	};
	struct SActTimer {
		int id;
		Player* player;
		int skill;
		int time;
		char act[50];
		short value;
	};
	static vector <STimer> timers;
	static vector <SActTimer> acttimers;
	static hash_map <int, bool> act;
	void handle (Timer* timer, int id) {
		int skill;
		Player* player;
		if(act[id]){
			char name[50];
			short value;
			for(unsigned int i=0; i<acttimers.size(); i++){
				if(acttimers[i].id == id){
					player = acttimers[i].player;
					skill = acttimers[i].skill;
					strcpy_s(name, 50, acttimers[i].act);
					value = acttimers[i].value;
					break;
				}
			}
			if(strcmp(name, "heal") == 0) Skills::heal(player, value, skill);
			// else if(...
		}
		else{
			for(unsigned int i=0; i<timers.size(); i++){
				if(timers[i].id == id){
					player = timers[i].player;
					skill = timers[i].skill;
					break;
				}
			}
			Skills::endSkill(player, skill);
		}
	}
	void remove (int id){
		if(act[id]){
			for(unsigned int i=0; i<acttimers.size(); i++){
				if(acttimers[i].id == id){	
					acttimers.erase(acttimers.begin()+i);	
					return;
				}
			}
		}
		else{
			for(unsigned int i=0; i<timers.size(); i++){
				if(timers[i].id == id){	
					timers.erase(timers.begin()+i);	
					return;
				}
			}
		}
		if(act.find(id) != act.end())
			act.erase(id);
	}
};

vector <SkillTimer::STimer> SkillTimer::timers;
vector <SkillTimer::SActTimer> SkillTimer::acttimers;
hash_map <int, bool> SkillTimer::act;
SkillTimer* Skills::timer;

void Skills::stopTimerPlayer(Player* player){
	timer->stop(player);
}

void Skills::startTimer(){
	// NOTE: type can be only 0x1/0x2/0x4/0x8/0x10/0x20/0x40/0x80.
	timer = new SkillTimer();
	SkillPlayerInfo player;
	SkillMapInfo map;
	SkillAct act;
	//Booster
	player.type = 0x08;
	player.byte = 2;
	player.value = SKILL_X;;
	//
	skillsinfo[1101004].player.push_back(player); // Sword Booster
	skillsinfo[1101005].player.push_back(player); // Axe Booster
	skillsinfo[1201004].player.push_back(player); // Sword Booster
	skillsinfo[1201005].player.push_back(player); // Blunt Booster
	skillsinfo[1301004].player.push_back(player); // Spear Booster
	skillsinfo[1301005].player.push_back(player); // Polearm Booster
	skillsinfo[2111005].player.push_back(player); // Spell Booster
	skillsinfo[2211005].player.push_back(player); // Spell Booster
	skillsinfo[3101002].player.push_back(player); // Bow Booster
	skillsinfo[3201002].player.push_back(player); // Crossbow Booster
	skillsinfo[4101003].player.push_back(player); // Claw Booster
	skillsinfo[4201002].player.push_back(player); // Dagger Booster
	// 1001 - Recovery
	player.type = 0x4;
	player.byte = 5;
	player.value = SKILL_X;
	strcpy_s(act.name, 50, "heal");
	act.time = 4900;
	act.value = SKILL_X;
	skillsinfo[1001].player.push_back(player);
	skillsinfo[1001].act = act;
	skillsinfo[1001].bact.push_back(true);
	// 1002 - Nimble Feet
	player.type = 0x80;
	player.byte = 1;
	player.value = SKILL_SPEED;
	map.type = 0x80;
	map.byte = 1;
	map.value = SKILL_SPEED;
	map.val = true;
	skillsinfo[1002].player.push_back(player);
	skillsinfo[1002].map.push_back(map);
	// 1001003 - Iron Body
	player.type = 0x02;
	player.byte = 1;
	player.value = SKILL_WDEF;
	skillsinfo[1001003].player.push_back(player);
	// 2001002 - Magic Guard
	player.type = 0x02;
	player.byte = 2;
	player.value = SKILL_X;
	skillsinfo[2001002].player.push_back(player);
	// 2001003 - Magic Armor
	player.type = 0x02;
	player.byte = 1;
	player.value = SKILL_WDEF;
	skillsinfo[2001003].player.push_back(player);
	// 3001003 - Focus
	player.type = 0x10;
	player.byte = 1;
	player.value = SKILL_ACC;
	skillsinfo[3001003].player.push_back(player);
	player.type = 0x20;
	player.byte = 1;
	player.value = SKILL_AVO;
	skillsinfo[3001003].player.push_back(player);
	// 4001003 - Dark Sight
	player.type = 0x80;
	player.byte = 1;
	player.value = SKILL_SPEED;
	skillsinfo[4001003].player.push_back(player);
	player.type = 0x04;
	player.byte = 2;
	player.value = SKILL_X;
	skillsinfo[4001003].player.push_back(player);
	map.type = 0x80;
	map.byte = 1;
	map.value = SKILL_SPEED;
	map.val = true;
	skillsinfo[4001003].map.push_back(map);
	map.type = 0x04;
	map.byte = 2;
	map.value = SKILL_X;
	map.val = false;
	skillsinfo[4001003].map.push_back(map);
	// 1101006 - Rage
	player.type = 0x1;
	player.byte = 1;
	player.value = SKILL_WATK;
	skillsinfo[1101006].player.push_back(player);
	player.type = 0x2;
	player.byte = 1;
	player.value = SKILL_WDEF;
	skillsinfo[1101006].player.push_back(player);
	// 1101007 & 1201007 - Power Guard
	player.type = 0x10;
	player.byte = 2;
	player.value = SKILL_X;
	skillsinfo[1101007].player.push_back(player);
	skillsinfo[1201007].player.push_back(player);
	map.type = 0x10;
	map.byte = 2;
	map.value = SKILL_X;
	map.val = false;
	skillsinfo[1101007].map.push_back(map);
	skillsinfo[1201007].map.push_back(map);
	// 1301006 - Iron Will
	player.type = 0x2;
	player.byte = 1;
	player.value = SKILL_WDEF;
	skillsinfo[1301006].player.push_back(player);
	player.type = 0x8;
	player.byte = 1;
	player.value = SKILL_MDEF;
	skillsinfo[1301006].player.push_back(player);
	// 1301007 - Hyper Body
	player.type = 0x20;
	player.byte = 2;
	player.value = SKILL_X;
	skillsinfo[1301007].player.push_back(player);
	player.type = 0x40;
	player.byte = 2;
	player.value = SKILL_Y;
	skillsinfo[1301007].player.push_back(player);
	// 2101001 & 2201001 - Meditation
	player.type = 0x04;
	player.byte = 1;
	player.value = SKILL_MATK;
	skillsinfo[2101001].player.push_back(player);
	skillsinfo[2201001].player.push_back(player);
	// 2301003 - Invincible
	player.type = 0x80;
	player.byte = 2;
	player.value = SKILL_X;
	skillsinfo[2301003].player.push_back(player);
	// 2301004 - Bless
	player.type = 0x2;
	player.byte = 1;
	player.value = SKILL_WDEF;
	skillsinfo[2301004].player.push_back(player);
	player.type = 0x8;
	player.byte = 1;
	player.value = SKILL_MDEF;
	skillsinfo[2301004].player.push_back(player);
	player.type = 0x10;
	player.byte = 1;
	player.value = SKILL_ACC;
	skillsinfo[2301004].player.push_back(player);
	player.type = 0x20;
	player.byte = 1;
	player.value = SKILL_AVO;
	skillsinfo[2301004].player.push_back(player);
	// 3101004 & 3201004 - Soul Arrow
	player.type = 0x1;
	player.byte = 3;
	player.value = SKILL_X;
	skillsinfo[3101004].player.push_back(player);
	skillsinfo[3201004].player.push_back(player);
	map.type = 0x1;
	map.byte = 3;
	map.value = SKILL_X;
	map.val = false;
	skillsinfo[3101004].map.push_back(map);
	skillsinfo[3201004].map.push_back(map);
	// 4101004 & 4201003 - Haste
	player.type = 0x80;
	player.byte = 1;
	player.value = SKILL_SPEED;
	skillsinfo[4101004].player.push_back(player);
	skillsinfo[4201003].player.push_back(player);
	player.type = 0x1;
	player.byte = 2;
	player.value = SKILL_JUMP;
	skillsinfo[4101004].player.push_back(player);
	skillsinfo[4201003].player.push_back(player);
	map.type = 0x80;
	map.byte = 1;
	map.value = SKILL_SPEED;
	map.val = true;
	skillsinfo[4101004].map.push_back(map);
	skillsinfo[4201003].map.push_back(map);

}

void Skills::addSkill(Player* player, unsigned char* packet){
	int skillid = getInt(packet+4);
	if(!BEGINNER_SKILL(skillid) && player->getSp() == 0){
		// hacking
		return;
	}
	if(!BEGINNER_SKILL(skillid))
		player->setSp(player->getSp()-1);
	else
		player->setSp(player->getSp());
	player->skills->addSkillLevel(skillid, 1);
	SkillsPacket::addSkill(player, skillid, player->skills->getSkillLevel(skillid));
}
void Skills::cancelSkill(Player* player, unsigned char* packet){
	stopSkill(player, getInt(packet));
}
void Skills::stopSkill(Player* player, int skillid){
	timer->stop(player, skillid);
	endSkill(player, skillid);
}
void Skills::useSkill(Player* player, unsigned char* packet){
	int skillid = getInt(packet+4);
	short level = player->skills->getSkillLevel(skillid);
	if(level == 0){
		// hacking
		return;
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].mp > 0){
		player->setMP(player->getMP()-skills[skillid][player->skills->getSkillLevel(skillid)].mp, 1);
	}
	else
		player->setMP(player->getMP(), 1);
	if(skills[skillid][player->skills->getSkillLevel(skillid)].hp > 0){
		player->setHP(player->getHP()-skills[skillid][player->skills->getSkillLevel(skillid)].hp);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].item > 0){	
		Inventory::takeItem(player, skills[skillid][player->skills->getSkillLevel(skillid)].item, skills[skillid][player->skills->getSkillLevel(skillid)].itemcount);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].hpP > 0){	
		//TODO PARTY
		int healrate = skills[skillid][player->skills->getSkillLevel(skillid)].hpP/1;
		if(healrate>100)
			healrate=100;
		player->setHP(player->getHP() + healrate*player->getMHP()/100);
	}
	SkillsPacket::showSkill(player, Maps::info[player->getMap()].Players, skillid); 
	///
	if(skillid == 1301007){ // Hyper Body
		player->setMHP(player->getRMHP()*(100 + skills[skillid][player->skills->getSkillLevel(skillid)].x)/100);
		player->setMMP(player->getRMMP()*(100 + skills[skillid][player->skills->getSkillLevel(skillid)].y)/100);
	}
	///
	if(skillsinfo.find(skillid) == skillsinfo.end())
		return;
	SkillActiveInfo playerskill;
	SkillActiveInfo mapskill;
	vector <SkillMapActiveInfo> mapenterskill;
	playerskill.types[0] = 0;
	playerskill.types[1] = 0;
	playerskill.types[2] = 0;
	playerskill.types[3] = 0;
	playerskill.types[4] = 0;
	playerskill.types[5] = 0;
	playerskill.types[6] = 0;
	playerskill.types[7] = 0;
	mapskill.types[0] = 0;
	mapskill.types[1] = 0;
	mapskill.types[2] = 0;
	mapskill.types[3] = 0;
	mapskill.types[4] = 0;
	mapskill.types[5] = 0;
	mapskill.types[6] = 0;
	mapskill.types[7] = 0;
	for(unsigned int i=0; i<skillsinfo[skillid].player.size(); i++){
		playerskill.types[skillsinfo[skillid].player[i].byte-1]+= skillsinfo[skillid].player[i].type;
		char val = skillsinfo[skillid].player[i].value;
		short value=0;
		switch(val){
			case SKILL_X: value = skills[skillid][level].x; break;
			case SKILL_Y: value = skills[skillid][level].y; break;
			case SKILL_SPEED: value = skills[skillid][level].speed; break;
			case SKILL_JUMP: value = skills[skillid][level].jump; break;
			case SKILL_WATK: value = skills[skillid][level].watk; break;
			case SKILL_WDEF: value = skills[skillid][level].wdef; break;
			case SKILL_MATK: value = skills[skillid][level].matk; break;
			case SKILL_MDEF: value = skills[skillid][level].mdef; break;
			case SKILL_ACC: value = skills[skillid][level].acc; break;
			case SKILL_AVO: value = skills[skillid][level].avo; break;
		}
		playerskill.vals.push_back(value);
	}
	for(unsigned int i=0; i<skillsinfo[skillid].map.size(); i++){
		mapskill.types[skillsinfo[skillid].map[i].byte-1]+= skillsinfo[skillid].map[i].type;
		char val = skillsinfo[skillid].map[i].value;
		short value=0;
		switch(val){
			case SKILL_X: value = skills[skillid][level].x; break;
			case SKILL_Y: value = skills[skillid][level].y; break;
			case SKILL_SPEED: value = skills[skillid][level].speed; break;   
			case SKILL_JUMP: value = skills[skillid][level].jump; break; 
			case SKILL_WATK: value = skills[skillid][level].watk; break;
			case SKILL_WDEF: value = skills[skillid][level].wdef; break;
			case SKILL_MATK: value = skills[skillid][level].matk; break; 
			case SKILL_MDEF: value = skills[skillid][level].mdef; break;
			case SKILL_ACC: value = skills[skillid][level].acc; break;
			case SKILL_AVO: value = skills[skillid][level].avo; break;
		}
		mapskill.vals.push_back(value);
		SkillMapActiveInfo map;
		map.byte = skillsinfo[skillid].map[i].byte;
		map.type = skillsinfo[skillid].map[i].type;
		if(skillsinfo[skillid].map[i].val){
			map.value = (char)value;
			map.isvalue = true;
		}
		else{
			map.isvalue = false;
			map.value = 0;
		}
		map.skill = skillid;
		mapenterskill.push_back(map);
	}
	SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, playerskill, mapskill);
	player->skills->setSkillPlayerInfo(skillid, playerskill);
	player->skills->setSkillMapInfo(skillid, mapskill);
	player->skills->setSkillMapEnterInfo(skillid, mapenterskill);
	timer->stop(player, skillid);
	if(skillsinfo[skillid].bact.size()>0){
		timer->stop(player, skillid, skillsinfo[skillid].act.name);
	}
	if(skillsinfo[skillid].bact.size()>0){
		int value = 0;
		switch(skillsinfo[skillid].act.value){
			case SKILL_X: value = skills[skillid][level].x; break;
			case SKILL_Y: value = skills[skillid][level].y; break;
			case SKILL_SPEED: value = skills[skillid][level].speed; break;
			case SKILL_WATK: value = skills[skillid][level].watk; break;
			case SKILL_WDEF: value = skills[skillid][level].wdef; break;
			case SKILL_MATK: value = skills[skillid][level].matk; break;
			case SKILL_MDEF: value = skills[skillid][level].mdef; break;
			case SKILL_ACC: value = skills[skillid][level].acc; break;
			case SKILL_AVO: value = skills[skillid][level].avo; break;
		}
		timer->setSkillTimer(player, skillid, skillsinfo[skillid].act.name, value, skillsinfo[skillid].act.time);
	}
	player->setSkill(player->skills->getSkillMapEnterInfo());
	timer->setSkillTimer(player, skillid, skills[skillid][level].time*1000);
	player->skills->setActiveSkillLevel(skillid, level);
}
void Skills::useAttackSkill(Player* player, int skillid){
	if(skills.find(skillid) == skills.end())
		return;
	if(skills[skillid][player->skills->getSkillLevel(skillid)].mp > 0){
		player->setMP(player->getMP()-skills[skillid][player->skills->getSkillLevel(skillid)].mp);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].hp > 0){
		player->setHP(player->getHP()-skills[skillid][player->skills->getSkillLevel(skillid)].hp);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].item > 0){	
		Inventory::takeItem(player, skills[skillid][player->skills->getSkillLevel(skillid)].item, skills[skillid][player->skills->getSkillLevel(skillid)].itemcount);
	}

}

void Skills::endSkill(Player* player, int skill){
	/// 
	if(skill == 1301007){ // Hyper Body
		player->setMHP(player->getRMHP());
		player->setMMP(player->getRMMP());
		player->setHP(player->getHP());
		player->setMP(player->getMP());
	}
	///
	if(skillsinfo[skill].bact.size()>0){
		timer->stop(player, skill, skillsinfo[skill].act.name);
	}
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, player->skills->getSkillPlayerInfo(skill) ,player->skills->getSkillMapInfo(skill));
	player->skills->deleteSkillMapEnterInfo(skill);
	player->skills->setActiveSkillLevel(skill, 0);
}

void Skills::heal(Player* player, short value, int skillid){
	if(player->getHP() < player->getMHP()){
		player->setHP(player->getHP()+ value);
		SkillsPacket::healHP(player, value);
	}
	timer->setSkillTimer(player, skillid, "heal", value, 5000);
}
