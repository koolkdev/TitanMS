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
#include "Reactor.h"
#include "DataProvider.h"
#include "PacketCreator.h"
#include "Map.h"
#include "MapDrops.h"
#include "AngelScriptEngine.h"
#include "ReactorData.h"
#include "ReactorsData.h"
#include "ReactorStateData.h"

// DON'T call it directly, call hitReactor in MapReactors.
void Reactor::hit(Player* player, short stance, char pos){
	// TODO: type 101 in 0.57
	char type = DataProvider::getInstance()->getReactorType(rid, state);
	if((type == 2 && pos != 0 && pos != 2) || type != 2){
		int newstance = DataProvider::getInstance()->getReactorNextState(rid, state);
		if(newstance == -1) return;
		state = newstance;
		int nstate = DataProvider::getInstance()->getReactorNextState(rid, state);
		if(nstate == -1){
			if(type < 100){ //
				//if(DataProvider::getInstance()->getReactorDelay(map->getID(), mapid) > 0)
					destroyed = true;
				//else
				//	map->send(PacketCreator().updateReactor(this, stance), player);
			}
			else{
				map->send(PacketCreator().updateReactor(this, stance), player);
			}
			AngelScriptEngine::handleReactor(player, this);
		}
		else{
			map->send(PacketCreator().updateReactor(this, stance), player);
			if(nstate == state)
				AngelScriptEngine::handleReactor(player, this);

		}
	}
}
void Reactor::setState(int s){
	ReactorData* r = ReactorsData::getInstance()->getDataByID(rid);
	if(r == NULL)
		return;
	ReactorStateData* sd = r->getDataByID(s);
	if(sd == NULL)
		return;
	state = s;
	map->send(PacketCreator().updateReactor(this, 0));
	if(r->getDataByID(s+1) == NULL){ // last
		AngelScriptEngine::handleReactor(NULL, this);
	}
}
void Reactor::drop(Player* player){
	map->getDrops()->dropFromReactor(this, player);
}
bool Reactor::inArea(Position fpos){
	Position lt = DataProvider::getInstance()->getReactorLT(rid, state);
	Position rb = DataProvider::getInstance()->getReactorRB(rid, state);
	
	return fpos.x > pos.x + lt.x && fpos.y > pos.y + lt.y && fpos.x < pos.x + rb.x && fpos.y < pos.y + rb.y;
		
}