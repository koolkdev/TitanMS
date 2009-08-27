#ifndef EVENTS_H
#define EVENTS_H

#include <string>
#include <hash_map>
class Event;
class Player;
class Timer;
class Channel;

class Events {
private:
	stdext::hash_map <std::string, Event*> events;
	stdext::hash_map <std::string, int> last;
	Channel* channel;
public:
	Events(Channel* channel) : channel(channel) {}
	Event* getEvent(std::string& name);
	bool createEvent(std::string name);
	bool addPlayer(std::string& name, Player* player);
	//void addParty(std::string& name, PartyMembers* player);
	void deleteEvent(std::string name, bool timer = false);
	void deleteEventForPlayer(std::string name, Player* player, bool logoff = false);
	bool isEventActive(std::string& name){
		return events.find(name) != events.end();
	}
};

#endif