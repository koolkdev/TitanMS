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

#include "PlayerSkills.h"
#include "Skill.h"
#include "DataProvider.h"
#include "SkillData.h"
#include "SkillLevelData.h"

using namespace stdext;

void PlayerSkills::addSkill(Skill* skill){
	skills[skill->getID()] = skill;
}
Skill* PlayerSkills::getSkill(int skillid){
	if(skills.find(skillid) != skills.end())
		return skills[skillid];
	SkillData* skilld = DataProvider::getInstance()->getSkillData(skillid);
	if(skilld == NULL)
		return NULL;
	Skill* nskill = new Skill(skillid, 0, (FOURTH_JOB(skillid) || skillid == 1004 || skillid == 1003) ? 0 : skilld->getCount());
	skills[skillid] = nskill;
	return nskill;
}
int PlayerSkills::getSkillLevel(int skillid){
	Skill* skill = getSkill(skillid);
	if(skill != NULL)
		return skill->getLevel();
	return 0;
}	
void PlayerSkills::deleteAll(){
	for (hash_map<int,Skill*>::iterator iter = skills.begin(); iter != skills.end(); iter++)
		delete iter->second;
}