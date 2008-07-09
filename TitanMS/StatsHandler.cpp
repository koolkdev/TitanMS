#include "PlayerHandler.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "PacketCreator.h"
#include "Skill.h"
#include "PlayerSkills.h"
#include "Tools.h"
#include "Player.h"
#include "SkillLevelData.h"
#include "SkillsData.h"
#include "SkillData.h"
using namespace Tools;

void PlayerHandler::addStatHandle(PacketReader* packet){
	// TODO add check that is less then 999
	packet->readInt();
	int type = packet->readInt();
	if(player->getAP() <= 0)
		return;
	Values values;
	player->addAP(-1, false);
	values.add(Value(Player::Update::AP, player->getAP()));
	values.add(player->addStat(type, true));
	player->send(PacketCreator().updateStats(&values, true));

}