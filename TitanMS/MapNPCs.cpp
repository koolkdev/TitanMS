#include "Map.h"
#include "NPC.h"
#include "MapNPCs.h"
#include "MapsData.h"
#include "MapData.h"
#include "MapNPCsData.h"
#include "MapNPCData.h"
#include "PacketCreator.h"
#include "Player.h"

MapNPCs::MapNPCs(Map* map){
	this->map = map;
	vector <MapNPCData*>* np = MapsData::getInstance()->getDataByID(map->getID())->getNPCsData()->getData();
	for(int i=0; i<(int)np->size(); i++){
		npcs.push_back(new NPC(npcs.size(), (*np)[i]));
	}	
}

NPC* MapNPCs::getNPCByID(int objid){
	if(objid < (int)npcs.size())
		return npcs[objid];
	return NULL;
}
void MapNPCs::show(Player* player){
	for(int i=0; i<(int)npcs.size(); i++)
		player->send(PacketCreator().showNPC(npcs[i]));
}
