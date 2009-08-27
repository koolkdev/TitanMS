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
#include "Channel.h"
#include "Maps.h"
#include "Players.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Transportations.h"
#include "Map.h"
#include "Party.h"
#include "Worlds.h"
#include "ChannelEventsData.h"
#include "ChannelEventData.h"
#include "EventsData.h"
#include "EventData.h"
#include "AngelScriptEngine.h"
#include "Events.h"
#include "Event.h"
#include "Timer.h"
#include "Run.h"
#include "Tools.h"
using namespace Tools;

class ChannelEventTimer : public Run {
private:
	Channel* channel;
public:
	ChannelEventTimer(Channel* channel): channel(channel) {}
	void run(){
		int time = getMinute();
		vector <ChannelEventData*>* events = ChannelEventsData::getInstance()->getData();
		for(int i=0; i<(int)events->size(); i++){
			ChannelEventData* eventd = (*events)[i];
			if(eventd->getTime() > 0 && (time-eventd->getFirst())%eventd->getTime() == 0){
				AngelScriptEngine::handleChannelEvent(channel, eventd->getName());
			}
		}	
		Timers::getInstance()->startTimer(getTimeUntilNextMinute(), new ChannelEventTimer(channel));
	}
};

Channel::Channel(World* world, int id, int port){
	this->world = world;
	this->id = id;
	maps = new Maps(this);
	players = new Players();
	trans = new Transportations();
	events = new Events(this);
	notice = Worlds::getInstance()->getDefaultNotice();
	this->port = port;
	Timers::getInstance()->startTimer(getTimeUntilNextMinute(), new ChannelEventTimer(this));
}
void Channel::addPlayer(Player* player){
	players->add(player);
	player->getMap()->addPlayer(player);
	player->send(PacketCreator().showMessage(notice, 4, 0, true));

}
Map* Channel::getMap(int id){
	return maps->getMap(id);
}

bool Channel::createEventForPlayer(std::string& name, Player* player){
	EventData* eventd = EventsData::getInstance()->getDataByID(name);
	if(eventd == NULL) return false;
	if(isEventActive(name)) return false;
	if(!events->createEvent(name)) return false;
	events->addPlayer(name, player);
	return true;
}
bool Channel::createEventForPlayers(std::string& name, Players* players){
	EventData* eventd = EventsData::getInstance()->getDataByID(name);
	if(eventd == NULL) return false;
	if(isEventActive(name)) return false;
	if(!events->createEvent(name)) return false;
	struct AddToEvent {
		void operator()(Player* player){
			events->addPlayer(name, player);
		}
		Events* events;
		string name;
	} addToEvent = {events, name};
	players->run(&addToEvent);
	return true;
}
bool Channel::createEventForParty(std::string& name, Party* party){
	bool ret = createEventForPlayers(name, (Players*)party->getMembers());
	if(ret && events->getEvent(name) != NULL){
		events->getEvent(name)->setParty(party);
	}
	return ret;
}
bool Channel::createEvent(std::string& name){
	EventData* eventd = EventsData::getInstance()->getDataByID(name);
	if(eventd == NULL) return false;
	if(isEventActive(name)) return false;
	if(!events->createEvent(name)) return false;
	return true;
}
void Channel::deleteEvent(std::string& name){
	events->deleteEvent(name);
}
void Channel::deleteEventForPlayer(std::string& name, Player* player){
	events->deleteEventForPlayer(name, player);
}
bool Channel::isEventActive(string& name){
	return events->isEventActive(name);
}
