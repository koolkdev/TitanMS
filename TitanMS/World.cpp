#include "World.h"
#include "Channel.h"
#include "Channels.h"
#include "Players.h"

World::World(int id, int channels){
	this->id = id;
	this->channels = new Channels(this, channels);
}

bool World::isPlayerConnected(int charid){
	for(int i=0; i<channels->getChannelsCount(); i++){
		if(channels->getChannel(i)->getPlayers()->getPlayerByID(charid) != NULL)
			return true;
	}
	return false;
}	