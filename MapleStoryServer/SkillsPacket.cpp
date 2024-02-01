#include "SkillsPacket.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Skills.h"

void SkillsPacket::addSkill(Player* player, int skillid, int level){
	Packet packet = Packet();
	packet.addHeader(0x2f);
	packet.addByte(1);
	packet.addShort(1);
	packet.addInt(skillid);
	packet.addInt(level);
	packet.addInt(0);
	packet.addByte(1);
	packet.packetSend(player);
	packet.packetSend(player);
}

void SkillsPacket::showSkill(Player* player, vector <Player*> players, int skillid){
	Packet packet = Packet();
	packet.addHeader(0x85);
	packet.addInt(player->getPlayerid());
	packet.addByte(1);
	packet.addInt(skillid);
	packet.addByte(1); //TODO
	packet.sendTo(player, players, 0);
}


void SkillsPacket::useSkill(Player* player, vector <Player*> players, int skillid, int time, SkillActiveInfo pskill, SkillActiveInfo mskill){
	Packet packet = Packet();
	packet.addHeader(0x3a);
	packet.addByte(pskill.types[0]);
	packet.addByte(pskill.types[1]);
	packet.addByte(pskill.types[2]);
	packet.addByte(pskill.types[3]);
	packet.addByte(pskill.types[4]);
	packet.addByte(pskill.types[5]);
	packet.addByte(pskill.types[6]);
	packet.addByte(pskill.types[7]);
	for(unsigned int i=0; i<pskill.vals.size(); i++){
		packet.addShort(pskill.vals[i]);
		packet.addInt(skillid);
		packet.addInt(time);
	}
	packet.addShort(0);
	packet.addByte(0);
	packet.packetSend(player);
	if(mskill.vals.size()>0){
		packet = Packet();
		packet.addHeader(0x86);
		packet.addInt(player->getPlayerid());
		packet.addByte(mskill.types[0]);
		packet.addByte(mskill.types[1]);
		packet.addByte(mskill.types[2]);
		packet.addByte(mskill.types[3]);
		packet.addByte(mskill.types[4]);
		packet.addByte(mskill.types[5]);
		packet.addByte(mskill.types[6]);
		packet.addByte(mskill.types[7]);
		for(unsigned int i=0; i<mskill.vals.size(); i++){
			packet.addShort(mskill.vals[i]);
		}
		packet.addByte(0);
		packet.sendTo(player, players, 0);
	}
}

void SkillsPacket::healHP(Player* player, short hp){
	Packet packet = Packet();
	packet.addHeader(0x67);
	packet.addByte(0xA);
	packet.addShort(hp);
	packet.packetSend(player);
}

void SkillsPacket::endSkill(Player* player, vector <Player*> players, SkillActiveInfo pskill, SkillActiveInfo mskill){
	Packet packet = Packet();
	packet.addHeader(0x24);
	packet.addByte(pskill.types[0]);
	packet.addByte(pskill.types[1]);
	packet.addByte(pskill.types[2]);
	packet.addByte(pskill.types[3]);
	packet.addByte(pskill.types[4]);
	packet.addByte(pskill.types[5]);
	packet.addByte(pskill.types[6]);
	packet.addByte(pskill.types[7]);
	packet.addByte(0);
	packet.packetSend(player);
	if(mskill.vals.size()>0){
		packet = Packet();
		packet.addHeader(0x8A);
		packet.addInt(player->getPlayerid());
		packet.addByte(mskill.types[0]);
		packet.addByte(mskill.types[1]);
		packet.addByte(mskill.types[2]);
		packet.addByte(mskill.types[3]);
		packet.addByte(mskill.types[4]);
		packet.addByte(mskill.types[5]);
		packet.addByte(mskill.types[6]);
		packet.addByte(mskill.types[7]);
		packet.sendTo(player, players, 0);

	}
}