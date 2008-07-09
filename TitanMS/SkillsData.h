#ifndef SKILLSDATA_H
#define SKILLSDATA_H

#include "TopData.h"
#include "Initializing.h"

class SkillData;

class SkillsData : public TopData<SkillsData, SkillData> {
	SkillData* load(int id){
		return Initializing::loadSkill(id);
	}
};


#endif