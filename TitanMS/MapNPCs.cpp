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

#include "DataProvider.h"
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

void MapNPCs::spawn(NPC* npc){
	npcs.push_back(npc);
	map->send(PacketCreator().showNPC(npc));
}