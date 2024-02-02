#include "PacketCreator.h"
#include "PacketReader.h"
#include "PlayerHandler.h"
#include "Player.h"
#include "World.h"
#include "Channel.h"
#include "Parties.h"

void PlayerHandler::partyOperationHandle(PacketReader& packet){
	char op = packet.read();
	Parties* parties = player->getChannel()->getWorld()->getParties();
	switch(op){
		case 0x01: parties->create(player); break;
		case 0x02: parties->leave(player); break;
		case 0x03: parties->join(player, packet.readInt()); break;
		case 0x04: parties->invite(player, packet.readString()); break;
		case 0x05: parties->expel(player, packet.readInt()); break;
		case 0x06: parties->changeLeader(player, packet.readInt()); break; 
	}
}

void PlayerHandler::deniedPartyInviteHandle(PacketReader& packet){
	char op = packet.read();
	Parties* parties = player->getChannel()->getWorld()->getParties();
	switch(op){
		case 0x15: parties->denied(player, packet.readString(), packet.readString(), true); break;
		case 0x17: parties->denied(player, packet.readString(), packet.readString()); break;
	}
}