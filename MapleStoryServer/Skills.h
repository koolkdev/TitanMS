#ifndef SKILLS_H
#define SKILLS_H

#include <hash_map>


using namespace std;
using namespace stdext;

class Player;
class SkillTimer;

struct SkillLevelInfo {
	int time;
	short mp;
	short hp;
	int item;
	int itemcount;
	short x;
	short y;
	short speed;
	short watk;
	short wdef;
	short matk;
	short mdef;
	short acc;
	short avo;
};

typedef hash_map <int, SkillLevelInfo> SkillInfo;

class Skills{
public:
	static SkillTimer* timer;
	static void startTimer();
	static hash_map <int, SkillInfo> skills;
	static void addSkill(int id, SkillInfo skill){
		skills[id] = skill;
	}
	static void addSkill(Player* player, unsigned char* packet);
	static void cancelSkill(Player* player, unsigned char* packet);
	static void useSkill(Player* player, unsigned char* packet);
	static void useAttackSkill(Player* player, int skillid);
	static void stopTimerPlayer(Player* player);
	static void end1001(Player* player);
	static void heal1001(Player* player);
	static void end1002(Player* player);
	static void end1001003(Player* player);
	static void end2001002(Player* player);
	static void end2001003(Player* player);
	static void end3001003(Player* player);
	static void end4001003(Player* player);
};

class PlayerSkills {
public:
	void addSkillLevel(int skillid, int level){
		for (hash_map<int,int>::iterator iter = playerskills.begin(); iter != playerskills.end(); iter++){
			if(iter->first == skillid){
				iter->second += level;
				return;
			}
		}
		playerskills[skillid] = level;
	}
	int getSkillLevel(int skillid){
		for (hash_map<int,int>::iterator iter = playerskills.begin(); iter != playerskills.end(); iter++){
			if(iter->first == skillid){
				return iter->second;
			} 
		}
		return 0;
	}
	int getSkillsNum(){
		return playerskills.size();
	}
	int getSkillID(int i){
		int j=0;
		for (hash_map<int,int>::iterator iter = playerskills.begin(); iter != playerskills.end(); iter++){
			if(j == i){
				return iter->first;
			} 
			j++;
		}
		return 0;
	}
private:
	hash_map <int, int> playerskills;
};

#endif