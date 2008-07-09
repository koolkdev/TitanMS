#ifndef CHANNEL_H
#define CHANNEL_H

class Maps;
class Map;
class Players;
class Player;
class World;
#include <string>

using namespace std;

class Channel {
private:
	int id;
	Maps* maps;
	Players* players;
	World* world;
	string notice;
	int port;
public:
	Channel(World* world, int id, int port);
	World* getWorld(){
		return world;
	}
	Players* getPlayers(){
		return players;
	}
	Maps* getMaps(){
		return maps;
	}
	Map* getMap(int id);
	int getID(){
		return id;
	}
	int getPort(){
		return port;
	}
	void addPlayer(Player* player);
};

#endif

