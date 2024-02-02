#include "Events.h"
#include "Player.h"
#include "Channel.h"
#include "Maps.h"
#include "Map.h"
#include "Event.h"
#include "EventsData.h"
#include "EventData.h"
#include "Players.h"
#include "Party.h"
#include "Timer.h"
#include "Run.h"
#include "Tools.h"
using namespace std;
using namespace stdext;

Event* Events::getEvent(string& name){
	if(events.find(name) != events.end())
		return events[name];
	return NULL;
}
bool Events::createEvent(std::string name){
	EventData* eventd = EventsData::getInstance()->getDataByID(name);
	if(eventd == NULL) return false;
	if(getEvent(name) != NULL) return false;
	if(last.find(name) != last.end() && last[name] > Tools::getServerTime() - 10000) return false;
	events[name] = new Event(channel, name);
	return true;
}
bool Events::addPlayer(std::string& name, Player* player){
	Event* pevent = getEvent(name);
	if(pevent == NULL) return false;
	pevent->start(player);
	return true;

}
void Events::deleteEvent(std::string name, bool timer){
	Event* pevent = getEvent(name);
	if(pevent == NULL) return;
	if(pevent->getDeleted()) return;
	pevent->setDeleted(true);
	pevent->end(timer);
	vector <int>* maps = EventsData::getInstance()->getDataByID(name)->getMapsData()->getData();
	for(int i=0; i<(int)maps->size(); i++){
		Map* map = channel->getMaps()->getMap((*maps)[i]);
		if(map != NULL){
			map->setEvent(NULL);
		}
	}
	last[name] = Tools::getServerTime();
	if(!timer) Timers::getInstance()->cancelTimer(pevent->getTimer());
	events.erase(name);
	delete pevent;
}
void Events::deleteEventForPlayer(std::string name, Player* player, bool logoff){
	Event* pevent = getEvent(name);
	if(pevent == NULL) return;
	if(pevent->getDeleted()) return;
	if(player->getParty() != NULL && pevent->getParty() && player->getParty()->getLeader() == player->getID()){
		deleteEvent(pevent->getName());
		return;
	}
	if(pevent->getPlayers()->getPlayersCount() == 1){
		vector <int>* maps = EventsData::getInstance()->getDataByID(name)->getMapsData()->getData();
		for(int i=0; i<(int)maps->size(); i++){
			Map* map = channel->getMaps()->getMap((*maps)[i]);
			if(map != NULL){
				map->setEvent(NULL);
			}
		}
	}
	pevent->end(player, !logoff);
	if(pevent->getPlayers()->getPlayersCount() == 0){
		last[name] = Tools::getServerTime();
		Timers::getInstance()->cancelTimer(pevent->getTimer());
		events.erase(name);
		delete pevent;
	}
}