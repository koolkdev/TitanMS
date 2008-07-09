#ifndef MAPSDATA_H
#define MAPSDATA_H

#include "TopData.h"
#include "Initializing.h"

class MapData;

class MapsData : public TopData<MapsData, MapData> {
	MapData* load(int id){
		return Initializing::loadMap(id);
	}
};


#endif