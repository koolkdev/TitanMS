#include "Skills.h"
#include "Player.h"
#include "SkillsPacket.h"
#include "Inventory.h"
#include "Maps.h"

hash_map <int, SkillInfo> Skills::skills;
#include "Timer.h"

class SkillTimer: public Timer::TimerHandler {
public:
	void setSkillTimer(Player* player, char* name, int time){
		STimer timer;
		timer.id = Timer::timer->setTimer(time, this);
		timer.player = player;
		strcpy_s(timer.name, 50, name);
		timer.time = time;
		timers.push_back(timer);
	}
	void stop (Player* player, char* name){
		for(unsigned int i=0; i<timers.size(); i++){
			if(player == timers[i].player && strcmp(timers[i].name, name) == 0){
				Timer::timer->cancelTimer(timers[i].id);
				break;
			}
		}
	}
	void stop (Player* player){
		for(unsigned int i=0; i<timers.size(); i++){
			if(player == timers[i].player){
				Timer::timer->cancelTimer(timers[i].id);
				break;
			}
		}
	}
private:
	struct STimer {
		int id;
		Player* player;
		char name[50];
		int time;
	};
	static vector <STimer> timers;
	void handle (Timer* timer, int id) {
		char name[50];
		Player* player;
		for(unsigned int i=0; i<timers.size(); i++){
			if(timers[i].id == id){
				player = timers[i].player;
				strcpy_s(name, 50, timers[i].name);
			}
		}
		if(strcmp(name, "end1001") == 0) Skills::end1001(player);
		else if(strcmp(name, "heal1001") == 0) Skills::heal1001(player);
		else if(strcmp(name, "end1002") == 0) Skills::end1002(player);
		else if(strcmp(name, "end1001003") == 0) Skills::end1001003(player);
		else if(strcmp(name, "end2001002") == 0) Skills::end2001002(player);
		else if(strcmp(name, "end2001003") == 0) Skills::end2001003(player);
		else if(strcmp(name, "end3001003") == 0) Skills::end3001003(player);
		else if(strcmp(name, "end4001003") == 0) Skills::end4001003(player);

	}
	void remove (int id){
		for(unsigned int i=0; i<timers.size(); i++){
			if(timers[i].id == id){
				timers.erase(timers.begin()+i);
				break;
			}
		}

	}
};

vector <SkillTimer::STimer> SkillTimer::timers;
SkillTimer* Skills::timer;

void Skills::stopTimerPlayer(Player* player){
	timer->stop(player);
}

void Skills::startTimer(){
	timer = new SkillTimer();
}

void Skills::addSkill(Player* player, unsigned char* packet){
	int skillid = getInt(packet+4);
	if(skillid >= 10000)
		player->setSp(player->getSp()-1);
	else
		player->setSp(player->getSp());
	player->skills->addSkillLevel(skillid, 1);
	SkillsPacket::addSkill(player, skillid, player->skills->getSkillLevel(skillid));
}
void Skills::cancelSkill(Player* player, unsigned char* packet){
	int skillid = getInt(packet);
	if(skillid == 1001){
		timer->stop(player, "end1001");
		end1001(player);
	}
	else if(skillid == 1002){
		timer->stop(player, "end1002");
		end1002(player);
	}
	else if(skillid == 1001003){
		timer->stop(player, "end1001003");
		end1001003(player);
	}
	else if(skillid == 2001002){
		timer->stop(player, "end2001002");
		end2001002(player);
	}
	else if(skillid == 2001003){
		timer->stop(player, "end2001003");
		end2001003(player);
	}
	else if(skillid == 3001003){
		timer->stop(player, "end3001003");
		end3001003(player);
	}
	else if(skillid == 4001003){
		timer->stop(player, "end4001003");
		end4001003(player);
	}
}
void Skills::useSkill(Player* player, unsigned char* packet){
	int skillid = getInt(packet+4);
	short level = player->skills->getSkillLevel(skillid);
	if(skills[skillid][player->skills->getSkillLevel(skillid)].mp > 0){
		player->setMP(player->getMP()-skills[skillid][player->skills->getSkillLevel(skillid)].mp, 1);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].hp > 0){
		player->setHP(player->getHP()-skills[skillid][player->skills->getSkillLevel(skillid)].hp);
	}
	if(skills[skillid][player->skills->getSkillLevel(skillid)].item > 0){	
		Inventory::takeItem(player, skills[skillid][player->skills->getSkillLevel(skillid)].item, skills[skillid][player->skills->getSkillLevel(skillid)].itemcount);
	}
//	if(skill
//	if(Skills::
	vector <int> vals;
	if(skillid == 1001){
		vals.push_back(skills[skillid][level].x);
		timer->stop(player, "end1001");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 0, 0, 0, 0, 4, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "heal1001", 4900);
		timer->setSkillTimer(player, "end1001", skills[skillid][level].time*1000);
	}
	else if(skillid == 1002){
		vals.push_back(skills[skillid][level].speed);
		timer->stop(player, "end1002");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, (char)0x80, 0, 0, 0, 0, 0, 0, 0, vals, 1);
		timer->setSkillTimer(player, "end1002", skills[skillid][level].time*1000);
	}
	else if(skillid == 1001003){
		vals.push_back(skills[skillid][level].wdef);
		timer->stop(player, "end1001003");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 2, 0, 0, 0, 0, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "end1001003", skills[skillid][level].time*1000);
	}
	else if(skillid == 2001002){
		vals.push_back(skills[skillid][level].x);
		timer->stop(player, "end2001002");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 0, 2, 0, 0, 0, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "end2001002", skills[skillid][level].time*1000);
	}
	else if(skillid == 2001003){
		vals.push_back(skills[skillid][level].wdef);
		timer->stop(player, "end2001003");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 2, 0, 0, 0, 0, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "end2001003", skills[skillid][level].time*1000);
	}
	else if(skillid == 3001003){
		vals.push_back(skills[skillid][level].acc);
		vals.push_back(skills[skillid][level].avo);
		timer->stop(player, "end3001003");
		SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 0x30, 0, 0, 0, 0, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "end3001003", skills[skillid][level].time*1000);
	}
	else if(skillid == 4001003){
		if(skills[skillid][level].speed != 0)
			vals.push_back(skills[skillid][level].speed);
		vals.push_back(1);
		timer->stop(player, "end4001003");
		if(skills[skillid][level].speed != 0)
			SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, (char)0x80, 4, 0, 0, 0, 0, 0, 0, vals, 0);  // 0x1 - attack 0x2 - w.def 0x4 - m.atk 0x8 - m.def etc..
		else
			SkillsPacket::useSkill(player, Maps::info[player->getMap()].Players , skillid, skills[skillid][level].time*1000, 0, 4, 0, 0, 0, 0, 0, 0, vals, 0);
		timer->setSkillTimer(player, "end4001003", skills[skillid][level].time*1000);
	}
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

void Skills::end1001(Player* player){
	timer->stop(player, "heal1001");
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 0, 0, 0, 0, 4, 0, 0, 0, 0);
}

void Skills::end1002(Player* player){
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, (char)0x80, 0, 0, 0, 0, 0, 0, 0, 1);
}
void Skills::end1001003(Player* player){
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 2, 0, 0, 0, 0, 0, 0, 0, 0);
}

void Skills::end2001002(Player* player){
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 0, 2, 0, 0, 0, 0, 0, 0, 0);
}
void Skills::end2001003(Player* player){
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 2, 0, 0, 0, 0, 0, 0, 0, 0);
}
void Skills::end3001003(Player* player){
	SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 0x30, 0, 0, 0, 0, 0, 0, 0, 0);
}
void Skills::end4001003(Player* player){
	if(skills[4001003][player->skills->getSkillLevel(4001003)].speed != 0)
		SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, (char)0x80, 4, 0, 0, 0, 0, 0, 0, 1);	
	else
		SkillsPacket::endSkill(player, Maps::info[player->getMap()].Players, 0, 4, 0, 0, 0, 0, 0, 0, 1);	
}

void Skills::heal1001(Player* player){
	if(player->getHP() < player->getMHP()){
		player->setHP(player->getHP()+skills[1001][player->skills->getSkillLevel(1001)].x);
		SkillsPacket::healHP(player, skills[1001][player->skills->getSkillLevel(1001)].x);
	}
	timer->setSkillTimer(player, "heal1001", 5000);
}
