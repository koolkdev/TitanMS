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

#ifndef SKILLS_H
#define SKILLS_H

#include <hash_map>


#define BEGINNER_SKILL(x) (x == 1000 || x == 1001 || x == 1002)
#define SKILL_JOB(x) (x/10000)
#define SKILL_JOB_N(x) ((x/100000%10 == 0) ? ((x/1000000 == 0) ? 0 : 1) : ((x/10000)%10+2))
#define FOURTH_JOB(x) ((x/10000)%10 == 2)

using namespace std;
using namespace stdext;

class Player;
class Skill;


class PlayerSkills{
private:
	Player* player;
	hash_map <int, Skill*> skills;
public:
	PlayerSkills(Player* player){
		this->player = player;
	}
	hash_map<int, Skill*>* getSkillsInfo(){
		return &skills;
	}
	void useAttackSkill(int skillid);
	void addSkill(Skill* skill);
	int getSkillsCount();
	int getSkillLevel(int skillid);
	void deleteAll();
	Skill* getSkill(int skillid);
};

#endif