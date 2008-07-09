#include "PlayerHandler.h"
#include "PacketReader.h"
#include "Map.h"
#include "MapReactors.h"
#include "Reactor.h"
#include "Player.h"

void PlayerHandler::hitReactorHandle(PacketReader* packet){
	int reactorid = packet->readInt();
	int pos = packet->readInt();
	short stance = packet->readShort();

	Reactor* reactor = player->getMap()->getReactor(reactorid);
	if(reactor == NULL)
		return;
	player->getMap()->getReactors()->hitReactor(reactor, player, stance, pos);
}
