#ifndef WORLDS_H
#define WORLDS_H

#include <vector>

using namespace std;

class World;
class Channel;
class Selector;

class Worlds {
private:
	static const char* names [];
	vector <World*> worlds;
	static Worlds* instance;
	Selector* selector;
	static int openPort;
public:
	Worlds(Selector* s, int worlds, int channels){
		instance = this;
		selector = s;
		for(int i=0; i<worlds; i++)
			newWorld(i, channels);
	}
	void newWorld(int id, int channels);

	static Worlds* getInstance(){
		return instance;
	}
	int getWorldsCount(){
		return worlds.size();
	}	
	World* getWorld(unsigned int id){
		if(id > worlds.size())
			return NULL;
		return worlds[id];
	}
	static int getOpenPort(){
		return openPort++;
	}
	Selector* getSelector(){
		return selector;
	}
	Channel* getChannelByPort(int port);
	static string getName(int id){
		if(id>=0 && id < 21)
			return string(names[id]);
		return string("");
	}
	bool isPlayerConnected(int charid);
};

#endif