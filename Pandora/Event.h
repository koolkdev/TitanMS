#ifndef EVENT_H
#define EVENT_H

#include <string>

class Players;
class Player;
class Timer;
class Channel;
class Party;

class Event  {
private:
	Players* players;
	Timer* timer;
	Channel* channel;
	bool party;
	bool deleted;
	Party* rparty;
	std::string name;
public:
	Event(Channel* channel, std::string name);
	~Event();
	void setTimer(Timer* timer){
		this->timer = timer;
	}
	Timer* getTimer(){
		return timer;
	}
	void start(Player* player);
	void end(bool timer = false);
	void end(Player* player, bool timer = false);
	Players* getPlayers(){
		return players;
	}
	std::string& getName(){
		return name;
	}
	int getTime();
	void setParty(Party* party){
		rparty = party;
		this->party = true;
	}
	bool getParty(){
		return party;
	}
	void setDeleted(bool b){
		deleted = b;
	}
	bool getDeleted(){
		return deleted;
	}
};

#endif