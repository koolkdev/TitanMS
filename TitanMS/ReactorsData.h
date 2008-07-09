#ifndef REACTORSDATA_H
#define REACTORSDATA_H

#include "TopData.h"
#include "Initializing.h"

class ReactorData;

class ReactorsData : public TopData<ReactorsData, ReactorData> {
	ReactorData* load(int id){
		return Initializing::loadReactor(id);
	}
};



#endif