#include "PacketCreator.h"
#include "Player.h"
#include "Players.h"
#include "PlayersPacket.h"


void PlayersPacket::showMoving(Player* player, vector <Player*> players, unsigned char* packett, int size){
	Packet packet = Packet();
	packet.addHeader(0x7A);
	packet.addInt(player->getPlayerid());
	packet.addInt(0);
	packet.addBytesHex(packett+5, size);
	packet.sendTo(player, players, 0);
}	

void PlayersPacket::faceExperiment(Player* player, vector <Player*> players, int face){
	Packet packet = Packet();
	packet.addHeader(0x81);
	packet.addInt(player->getPlayerid());
	packet.addInt(face);
	packet.sendTo(player, players, 0);
}

void PlayersPacket::showChat(Player* player, vector <Player*> players, char* msg){
	Packet packet = Packet();
	packet.addHeader(0x67);
	packet.addInt(player->getPlayerid());
	packet.addByte(0);
	packet.addShort(strlen(msg));
	packet.addString(msg, strlen(msg));
	packet.sendTo(player, players, 1);
}

void PlayersPacket::damagePlayer(Player* player, vector <Player*> players, int dmg, int mob){
	// DC Other Players in map, needed to be fixed // 
	Packet packet = Packet();
	packet.addHeader(0x81);
	packet.addInt(player->getPlayerid());
	packet.addInt(0);
	//packet.sendTo(player, players, 0);
	packet = Packet();
	packet.addHeader(0x80);
	packet.addInt(player->getPlayerid());
	packet.addByte(-1);
	packet.addInt(0);
	packet.addInt(mob);
	packet.addShort(1);
	packet.addByte(0);
	packet.addInt(dmg);
	//packet.sendTo(player, players, 1);
}

void PlayersPacket::showMassage(char* msg, char type){
	Packet packet = Packet();
	packet.addHeader(0x37);
	packet.addByte(type);
	packet.addShort(strlen(msg));
	packet.addString(msg, strlen(msg));
	for (hash_map<int,Player*>::iterator iter = Players::players.begin();
		 iter != Players::players.end(); iter++){
			 packet.packetSend(iter->second);
	}
}
