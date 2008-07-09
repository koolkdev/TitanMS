#ifndef MAPDATA_H
#define MAPDATA_H

#include "TopDataStruct.h"
#include "MapMobsData.h"
#include "MapNPCsData.h"
#include "MapReactorsData.h"
#include "MapPortalsData.h"
#include "MapFootholdsData.h"

class MapData : public DataStruct {
private:
	MapMobsData* mobs;
	MapNPCsData* NPCs;
	MapReactorsData* reactors;
	MapPortalsData* portals;
	MapFootholdsData* footholds;
	int returnMap;
public:
	MapData(int id){
		this->id = id;
		mobs = new MapMobsData();
		NPCs = new MapNPCsData();
		reactors = new MapReactorsData();
		portals = new MapPortalsData();
		footholds = new MapFootholdsData();
	}

	MapMobsData* getMobsData(){
		return mobs;
	}
	MapNPCsData* getNPCsData(){
		return NPCs;
	}
	MapReactorsData* getReactorsData(){
		return reactors;
	}
	MapPortalsData* getPortalsData(){
		return portals;
	}
	MapFootholdsData* getFootholdsData(){
		return footholds;
	}
	void setReturnMap(int returnMap){
		this->returnMap = returnMap;
	}
	int getReturnMap(){
		return returnMap;
	}
};


#endif