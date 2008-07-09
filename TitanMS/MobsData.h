#ifndef MOBSDATA_H
#define MOBSDATA_H

#include "TopData.h"
#include "Initializing.h"

class MobData;

class MobsData : public TopData<MobsData, MobData> {
	MobData* load(int id){
		return Initializing::loadMob(id);
	}
};



#endif