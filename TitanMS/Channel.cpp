#include "Channel.h"
#include "Maps.h"
#include "Players.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Map.h"

Channel::Channel(World* world, int id, int port){
	this->id = id;
	maps = new Maps(this);
	players = new Players();
	notice = "Welcome To MapleStory!";
	this->port = port;
}
void Channel::addPlayer(Player* player){
	players->add(player);
	player->getMap()->addPlayer(player);
	player->send(PacketCreator().showMassage(notice, 4, 0, true));

}
Map* Channel::getMap(int id){
	return maps->getMap(id);
}