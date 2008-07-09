#include "PlayerHandler.h"
#include "Player.h"
#include "PacketReader.h"
#include "PacketCreator.h"
#include "PlayerSkills.h"
#include "Skill.h"
#include "PlayerBuffs.h"
#include "Effect.h"

void PlayerHandler::addSkillHandle(PacketReader* packet){
	packet->readInt();
	int skillid = packet->readInt();
	if(BEGINNER_SKILL(skillid)){
		int cursp = (player->getLevel() > 6) ? 6 : player->getLevel()\
			- player->getSkills()->getSkillLevel(1000)\
			- player->getSkills()->getSkillLevel(1001)\
			- player->getSkills()->getSkillLevel(1002);
		if(cursp < 1)
			return;
	}
	else if(player->getSP() < 1)		
			return;
	Skill* skill = player->getSkills()->getSkill(skillid);
	if(skill == NULL){
		player->send(PacketCreator().enableAction());
		return;
	}
	// TODO: check that player can have this skill
	if(!(skill->getLevel() < skill->getMasterLevel()))
		return;
	skill->setLevel(skill->getLevel()+1);
	if(!BEGINNER_SKILL(skillid))
		player->addSP(-1);
	player->send(PacketCreator().updateSkill(skill));
}
void PlayerHandler::stopSkillHandle(PacketReader* packet){
	int skillid = packet->readInt();
	player->getBuffs()->cancelBuff(skillid);	
}
void PlayerHandler::useSkillHandle(PacketReader* packet){
	packet->readInt();
	int skillid = packet->readInt();
	Skill* skill = player->getSkills()->getSkill(skillid);
	if(skill == NULL)
		return;
	if(skill->getLevel() < 1)
		return;
	Effect* effect = Effect::getEffect(skillid, skill->getLevel());
	if(effect != NULL)
		effect->use(player);

}