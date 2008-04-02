#include "SkillsPacket.h"
#include "Player.h"
#include "PacketCreator.h"

void SkillsPacket::addSkill(Player* player, int skillid, int level){
	Packet packet = Packet();
	packet.addHeader(0x1D);
	packet.addByte(1);
	packet.addShort(1);
	packet.addInt(skillid);
	packet.addInt(level);
	packet.addInt(0);
	packet.addByte(1);
	packet.packetSend(player);
	packet = Packet();
	packet.addHeader(0x5A);
	packet.addByte(0);
	packet.packetSend(player);
}

void SkillsPacket::useSkill(Player* player, vector <Player*> players, int skillid, int time, char type1, char type2, char type3, char type4, char type5, char type6, char type7, char type8, vector <int> values , bool is){
	Packet packet = Packet();
	packet.addHeader(0x1B);
	packet.addByte(type1);
	packet.addByte(type2);
	packet.addByte(type3);
	packet.addByte(type4);
	packet.addByte(type5);
	packet.addByte(type6);
	packet.addByte(type7);
	packet.addByte(type8);
	for(unsigned int i=0; i<values.size(); i++){
		packet.addShort(values[i]);
		packet.addInt(skillid);
		packet.addInt(time);
	}
	packet.addShort(0);
	packet.addByte(0);
	packet.packetSend(player);
	packet = Packet();
	if(is){
		packet = Packet();
		packet.addHeader(0x86);
		packet.addInt(player->getPlayerid());
		packet.addByte(type1);
		packet.addByte(type2);
		packet.addByte(type3);
		packet.addByte(type4);
		packet.addByte(type5);
		packet.addByte(type6);
		packet.addByte(type7);
		packet.addByte(type8);
		for(unsigned int i=0; i<values.size(); i++)
			packet.addShort(values[i]);
		packet.addByte(0);
		packet.sendTo(player, players, 0);
	}
	packet.addHeader(0x85);
	packet.addInt(player->getPlayerid());
	packet.addByte(1);
	packet.addInt(skillid);
	packet.addByte(1);
	packet.sendTo(player, players, 0);
}

void SkillsPacket::healHP(Player* player, short hp){
	Packet packet = Packet();
	packet.addHeader(0x8C);
	packet.addByte(0xA);
	packet.addShort(hp);
	packet.packetSend(player);
}

void SkillsPacket::endSkill(Player* player, vector <Player*> players, char type1, char type2, char type3, char type4, char type5, char type6, char type7, char type8, bool is){
	Packet packet = Packet();
	packet.addHeader(0x1C);
	packet.addByte(type1);
	packet.addByte(type2);
	packet.addByte(type3);
	packet.addByte(type4);
	packet.addByte(type5);
	packet.addByte(type6);
	packet.addByte(type7);
	packet.addByte(type8);
	packet.addByte(0);
	packet.packetSend(player);
	if(is){
		packet = Packet();
		packet.addHeader(0x87);
		packet.addInt(player->getPlayerid());
		packet.addByte(type1);
		packet.addByte(type2);
		packet.addByte(type3);
		packet.addByte(type4);
		packet.addByte(type5);
		packet.addByte(type6);
		packet.addByte(type7);
		packet.addByte(type8);
		packet.addByte(0);
		packet.sendTo(player, players, 0);

	}
}