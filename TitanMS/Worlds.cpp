#include "Worlds.h"
#include "World.h"
#include "Channels.h"

Worlds* Worlds::instance = NULL;
int Worlds::openPort = 8888;
const char* Worlds::names [] = {"Scania", "Bera", "Broa", "Windia", "Khaini", "Bellocan", "Mardia", "Kradia", "Yellonde", "Demethos", "Elnido", "Kastia", "Judis", "Arkenia", "Plana", "Galicia", "Kalluna", "Stius", "Croa", "Zenith", "Tespia"};

void Worlds::newWorld(int id, int channels){
	worlds.push_back(new World(id, channels));
}

Channel* Worlds::getChannelByPort(int port){
	for(int i=0; i<(int)worlds.size(); i++){
		Channels* channels = worlds[i]->getChannels();
		for(int i=0; i<channels->getChannelsCount(); i++)
			if(channels->getChannelPort(i) == port)
				return channels->getChannel(i);
	}
	return NULL;
}

bool Worlds::isPlayerConnected(int charid){
	for(int i=0; i<(int)worlds.size(); i++)
		if(worlds[i]->isPlayerConnected(charid))
			return true;
	return false;
}