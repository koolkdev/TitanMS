#ifndef WORLD_H
#define WORLD_H

class Channels;
class World;

class World {
private:
	int id;
	Channels* channels;
public:
	World(int id, int channels);
	Channels* getChannels(){
		return channels;
	}
	int getID(){
		return id;
	}
	bool isPlayerConnected(int charid);
};

#endif

