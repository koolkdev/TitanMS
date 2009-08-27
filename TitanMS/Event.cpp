#include "Event.h"
#include "Players.h"
#include "Player.h"
#include "Channel.h"
#include "Map.h"
#include "Party.h"
#include "Maps.h"
#include "Events.h"
#include "Timer.h"
#include "Run.h"
#include "PacketCreator.h"
#include "EventsData.h"
#include "EventData.h"
using namespace std;

class EventTimer : public Run {
private:
	Events* events;
	string name;
public:
	EventTimer(Events* events, string& name) : events(events), name(string(name)) {}
	void run(){
		events->deleteEvent(name, true);
	}
};

Event::Event(Channel* channel, string name) : channel(channel), name(name), players(new Players()) {
	timer = Timers::getInstance()->startTimer(EventsData::getInstance()->getDataByID(name)->getTime()*1000, new EventTimer(channel->getEvents(), name));
	party = false;
	deleted = false;
	vector <int>* maps = EventsData::getInstance()->getDataByID(name)->getMapsData()->getData();
	for(int i=0; i<(int)maps->size(); i++){
		Map* map = channel->getMaps()->getMap((*maps)[i]);
		if(map != NULL){
			map->clear();
			map->setEvent(this);
		}
	}
}
Event::~Event(){
	if(party){
		rparty->clear();
	}
	delete players;
}
void Event::end(bool timer){
	// TODO: if(timer) 
	struct ChangeMapEvent { 
		void operator()(Player* player){
			if(player->getMap()->getEvent() == pevent)
				player->setMap(map);
		}
		int map;
		Event* pevent;
	} changeMap = {EventsData::getInstance()->getDataByID(name)->getEndMap(), this};
	players->run(&changeMap);
}
void Event::end(Player* player, bool timer){
	if(player->getMap()->getEvent() != NULL && timer) player->setMap(EventsData::getInstance()->getDataByID(name)->getEndMap());
	players->remove(player);
}
void Event::start(Player* player){
	player->setMap(EventsData::getInstance()->getDataByID(name)->getStartMap());
	players->add(player);
	player->send(PacketCreator().showTimer(timer->getTime()/1000));
}
int Event::getTime(){
	return timer->getTime()/1000;
}