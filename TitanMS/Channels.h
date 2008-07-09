#ifndef CHANNELS_H
#define CHANNELS_H

#include <vector>

using namespace std;

class Channel;
class Selector;
class World;

class Channels {
private:
	vector <Channel*> channels;
	vector <int> channelsports;
	World* world;
public:
	Channels(World* world, int n){
		for(int i=0; i<n; i++)
			newChannel();
		this->world = world;
	}
	void newChannel();

	int getChannelsCount(){
		return channels.size();
	}
	Channel* getChannel(unsigned int id){
		if(id > channels.size())
			return NULL;
		return channels[id];
	}
	int getChannelPort(unsigned int id){
		if(id > channels.size())
			return -1;
		return channelsports[id];
		
	}

};

#endif