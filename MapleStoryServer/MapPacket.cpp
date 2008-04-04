#include "PacketCreator.h"
#include "Player.h"
#include "Maps.h"
#include "MapPacket.h"

Packet MapPacket::playerPacket(Player* player){
	Packet packet = Packet();
	packet.addHeader(0x65);
	packet.addInt(player->getPlayerid());
	packet.addShort(strlen(player->getName()));
	packet.addString(player->getName(), strlen(player->getName()));
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	packet.addByte(player->getGender());
	packet.addByte(player->getSkin());
	packet.addInt(player->getEyes());
	packet.addByte(1);
	packet.addInt(player->getHair());
	for(int i=0; i<player->inv->getEquipNum(); i++){
		Equip* equip = player->inv->getEquip(i);
		if(equip->pos<0){
			packet.addByte(equip->type);
			packet.addInt(equip->id);
		}
	}
	packet.addShort(-1);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(player->getChair());
	packet.addShort(player->getPos().x);
	packet.addShort(player->getPos().y);
	packet.addByte(player->getType());
	packet.addInt(0);
	packet.addShort(1);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	packet.addInt(0);
	return packet;
}

void MapPacket::showPlayer(Player* player, vector <Player*> players){
	Packet packet = playerPacket(player);
	for(unsigned int i=0; i<players.size(); i++){
		packet.packetSend(players[i]);
	}
}	

void MapPacket::removePlayer(Player* player, vector <Player*> players){
	Packet packet = Packet();
	packet.addHeader(0x70);
	packet.addInt(player->getPlayerid());
	for(unsigned int i=0; i<players.size(); i++){
		if(player->getPlayerid() != players[i]->getPlayerid())
			packet.packetSend(players[i]);
	}
}	

void MapPacket::showPlayers(Player* player, vector <Player*> players){
	for(unsigned int i=0; i<players.size(); i++){
		if(player->getPlayerid() != players[i]->getPlayerid()){
			Packet packet = playerPacket(players[i]);
			packet.packetSend(player);
		}
	}
}	

void MapPacket::changeMap(Player* player){
	Packet packet = Packet();
	packet.addHeader(0x4D);
	packet.addInt(0); // Channel
	packet.addShort(0); // 2?
	packet.addInt(player->getMap());
	packet.addByte(player->getMappos());
	if(player->getHP() <= 0)
		packet.addShort(5000);
	else
		packet.addShort(player->getHP());
	packet.addByte(0);
	packet.addInt(-1);
	packet.addShort(-1);
	packet.addByte(-1);
	packet.addByte(1);
	packet.packetSend(player);
}

void MapPacket::makeApple(Player* player){
	Packet packet = Packet();
	packet.addHeader(0x5B);  
	packet.packetSend(player);
}