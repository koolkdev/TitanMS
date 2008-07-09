#ifndef SKILLDATA_H
#define SKILLDATA_H

#include "TopDataStruct.h"

class SkillLevelData;

class SkillData : public TopDataStruct<SkillLevelData>, public DataStruct {
public:
	SkillData(int id){
		this->id = id;
	}	 
};


#endif