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
				if(DataProvider::getInstance()->getReactorDelay(map->getID(), mapid) > 0)
					destroyed = true;
				else
					map->send(PacketCreator().updateReactor(this, stance));
			}
			else{
				map->send(PacketCreator().updateReactor(this, stance));
			}
			AngelScriptEngine::handleReactor(player, this);
		}
		else{
			map->send(PacketCreator().updateReactor(this, stance));
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
void Reactor::drop(){
	map->getDrops()->dropFromReactor(this);
}
bool Reactor::inArea(Position fpos){
	Position lt = DataProvider::getInstance()->getReactorLT(rid, state);
	Position rb = DataProvider::getInstance()->getReactorRB(rid, state);
	
	//TODO fix
	return fpos.x > pos.x + lt.x && fpos.y > pos.y + lt.y - 10 && fpos.x < pos.x + rb.x && fpos.y < pos.y + rb.y + 10;
		
}