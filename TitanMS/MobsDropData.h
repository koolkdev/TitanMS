#ifndef MOBDROPDATA_H
#define MOBDROPDATA_H

#include "TopData.h"
#include "Initializing.h"

class MobsDropData : public TopData <MobsDropData, DropsData> { 
	DropsData* load(int id){
		return Initializing::loadMobDrops(id);
	}
};


#endif