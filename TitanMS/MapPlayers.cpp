#include "Player.h"
#include "Players.h"
#include "MapPlayers.h"
#include "PacketCreator.h"
#include "Map.h"

void MapPlayers::add(Player* player){
	players[player->getID()] = player;
	playersv.push_back(player);
	for(int i=0; i<(int)playersv.size(); i++){
		player->send(PacketCreator().showPlayer(playersv[i]));
		playersv[i]->send(PacketCreator().showPlayer(player));
	}

}
void MapPlayers::remove(Player* player){
	if(players.find(player->getID()) != players.end()){
		players.erase(player->getID());
	}
	for(int i=0; i<(int)playersv.size(); i++){
		if(playersv[i] == player){
			playersv.erase(playersv.begin()+i);
			break;
		}
	}
	map->send(PacketCreator().removePlayer(player->getID()), player);
}