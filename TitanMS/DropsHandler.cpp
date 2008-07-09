#include "PlayerHandler.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "Player.h"
#include "Drop.h"
#include "Map.h"
#include "MapDrops.h"
#include "PacketCreator.h"

void PlayerHandler::dropMesosHandle(PacketReader* packet){
	packet->readInt();
	int amount = packet->readInt();
	if(amount < 10)
		return;
	player->addMesos(-amount, true, true);
	player->getMap()->getDrops()->dropMesosFromObject(amount, player->getPosition(), player, 0);
}
void PlayerHandler::lootDropHandle(PacketReader* packet){
	packet->read(9);
	int dropid = packet->readInt();
	Drop* drop = player->getMap()->getDrop(dropid);
	if(drop != NULL)
		drop->takeDrop(player);
}