#ifndef REACTORDROPDATA_H
#define REACTORDROPDATA_H

#include "TopData.h"
#include "Initializing.h"

class ReactorsDropData : public TopData <ReactorsDropData, DropsData> { 
	DropsData* load(int id){
		return Initializing::loadReactorDrops(id);
	}
};


#endif