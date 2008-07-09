#include "PlayerHandler.h"
#include "PacketReader.h" 
#include "PacketCreator.h" 
#include "PacketWriter.h" 
#include "Player.h"
#include "PlayerBuffs.h"
#include "MapMobs.h"
#include "Map.h"
#include "Effect.h"
#include "Mob.h"
#include "Maps.h"
#include "ObjectMoving.h"
#include "Channel.h"
#include "MapPortalData.h"
#include "PlayerInventories.h"
#include "PacketHandlingError.h"
#include "Inventory.h"
#include "MapPlayers.h"
#include <iostream>
#include "Tools.h"
using namespace Tools;

void PlayerHandler::handle(PacketReader* packet){
	//int time = (int)(getLongTime()/10000);
	try{
		switch(packet->getHeader()){
			case PING: pingHandle(packet); break;
			case CONNECT: connectionRequestHandle(packet); break;
			case NPC_CHAT: NPCChatHandle(packet); break;
			case HANDLE_NPC: NPCHandle(packet); break;
			case USE_SHOP: useShopHandle(packet); break;
			case CHANGE_CHANNEL: changeChannelHandle(packet); break;
			case TAKE_DAMAGE: playerTakeDamageHandle(packet); break;
			case PLAYER_MOVE: playerMovingHandle(packet); break;
			case CHANGE_MAP: changeMapHandle(packet); break;
			case PLAYER_INFO: getPlayerInformationHandle(packet); break;
			case FACE_EXP: useFaceExpressionHandle(packet); break;
			case RECOVERY: recoveryHealthHandle(packet); break;
			case DAMAGE_MOB_MAGIC: damageMobMagicHandle(packet); break;
			case DAMAGE_MOB: damageMobHandle(packet); break;
			case DAMAGE_MOB_RANGED: damageMobRangedHandle(packet); break;
			case CONTROL_MOB: controlMobHandle(packet); break;
			case CONTROL_MOB_SKILL: controlMobSkillHandle(packet); break;
			case ADD_SKILL: addSkillHandle(packet); break;
			case STOP_SKILL: stopSkillHandle(packet); break;
			case USE_SKILL: useSkillHandle(packet); break;
			case USE_CHAT: useChatHandle(packet); break;
			case CHAT_COMMAND: chatCommandHandle(packet); break;
			case MOVE_ITEM: moveItemHandle(packet); break;
			case USE_CHAIR: useChairHandle(packet); break;
			case CANCEL_CHAIR: cancelChairHandle(packet); break;
			case USE_ITEM_EFFECT: useItemEffectHandle(packet); break;
			case USE_CASH_ITEM: useCashItemHandle(packet); break;
			case USE_ITEM: useItemHandle(packet); break;
			case USE_SCROLL: useScrollHandle(packet); break;
			case USE_RETURN_SCROLL: useReturnScrollHandle(packet); break;
			case USE_SUMMON_BUG: useSummonBugHandle(packet); break;
			case CANCEL_ITEM_BUFF: cancelItemBuffHandle(packet); break;
			case ADD_STAT: addStatHandle(packet); break;
			case DROP_MESOS: dropMesosHandle(packet); break;
			case LOOT_DROP: lootDropHandle(packet); break;
			case QUEST_HANDLE: questHandle(packet); break;
			case CHANGE_KEY: changeKeyHandle(packet); break;
			case USE_PET: usePetHandle(packet); break;
			case MOVE_PET: movePetHandle(packet); break;
			case PET_COMMAND: petCommandHandle(packet); break;
			case PET_COMMAND_TEXT: petCommandTextHandle(packet); break;
			case HIT_REACTOR: hitReactorHandle(packet); break;
			case HIT_BY_MOB: hitByMobHandle(packet);
			case 0x11: break; // stupid check or something
			default: printf("Unk header: %x\n", packet->getHeader()); packet->show();
		}
	}
	catch(PacketHandlingError ph){
		cout << ph.getError();
	}
	catch(...){

	}
	//printf("%d ", getLongTime()/10000 - time);
}

void PlayerHandler::pingHandle(PacketReader* packet){
}

void PlayerHandler::connectionRequestHandle(PacketReader* packet){
	player->setUserID(packet->readInt());
}
void PlayerHandler::playerTakeDamageHandle(PacketReader* packet){
	packet->readInt();
	char damagetype = packet->read(); 
	int damage = packet->readInt();
	if(damage < 0)
		return;
	int mobid = 0;
	int objid = 0;
	if(damagetype != -2){ // Damage from monster...
		mobid = packet->readInt();
		objid = packet->readInt();
	}
	

	if(damage > 0){ 
		if(damagetype == -1 && player->getBuffs()->isBuffActive(Effect::POWER_GUARD)){ // only regular body attack from mobs..
			Mob* mob = player->getMap()->getMob(objid);
			if(mob != NULL && !mob->isBoss()){ // Not on bosses
				int mobdamage = (int) (damage* (player->getBuffs()->getBuffValue(Effect::POWER_GUARD)) / 100.0);
				if(mob->getMaxHP()/10 < mobdamage)
					mobdamage = mob->getMaxHP()/10;
				player->getMap()->getMobs()->damage(player, mob, mobdamage);
				damage -= mobdamage;
				player->send(PacketCreator().damageMob(objid, mobdamage));
			}
		}
		if(player->getBuffs()->isBuffActive(Effect::MAGIC_GUARD)){
			int mpdamage = (int)((damage * player->getBuffs()->getBuffValue(Effect::MAGIC_GUARD)) / 100.0);
			int hpdamage = damage - mpdamage;
			if (mpdamage >= player->getMP()) {
	            player->addHP(-(hpdamage + (mpdamage - player->getMP())));
				player->setMP(0);
			}
			else {
	            player->addMP(-mpdamage);
				player->addHP(-hpdamage);
			}
		}	
		else if(player->getBuffs()->isBuffActive(Effect::MESO_GUARD)){
			damage = damage / 2 + (damage % 2);
			int mesos = (int)((damage * player->getBuffs()->getBuffValue(Effect::MESO_GUARD)) / 100.0);
			if(player->getMesos() < mesos){
				player->addMesos(-player->getMesos());
				player->getBuffs()->cancelBuff(4211005);
			}
			else{
				player->addMesos(-mesos);
			}
			player->addHP(-damage);
		}
		else{
			player->addHP(-damage);
		}
	}
	player->getMap()->send(PacketCreator().damagePlayer(player->getID(), damagetype, damage, mobid), player); 
}
void PlayerHandler::playerMovingHandle(PacketReader* packet){
	packet->read(5);
	ObjectMoving moving = ObjectMoving(packet);
	if(moving.isError())
		return;
	player->getMap()->send(PacketCreator().showMoving(player->getID(), &moving), player);
	player->setStance(moving.getStance());
	player->setPosition(moving.getPosition());	
}
void PlayerHandler::changeMapHandle(PacketReader* packet){
	packet->read();
	int type = packet->readInt();
	if(type == 0){
		player->getBuffs()->cancelAll();
		player->setHP(50, false);
		Map* map = player->getChannel()->getMaps()->getMap(player->getMap()->getReturnMap());
		player->changeMap(map);
	}
	else{
		string toname = packet->readString();
		MapPortalData* portal = player->getMap()->getPortal(toname);
		if(portal != NULL && portal->getScript() != ""){
			if(!true)// Run script
				player->send(PacketCreator().enableAction());
		}
		else if(portal != NULL && portal->getToMapID() != 999999999){
			Map* map = player->getChannel()->getMaps()->getMap(portal->getToMapID());
			MapPortalData* tportal = map->getPortal(portal->getToPortal());
			player->changeMap(map, (tportal != NULL) ? tportal->getID() : 0);
		}
		else{
			player->send(PacketCreator().enableAction());
		}
	}

}
void PlayerHandler::getPlayerInformationHandle(PacketReader* packet){
	packet->readInt();
	int playerid = packet->readInt();
	Player* to = player->getMap()->getPlayers()->getPlayerByID(playerid);
	if(to == NULL)
		to = player;
	player->send(PacketCreator().showInfo(to));
}
void PlayerHandler::useFaceExpressionHandle(PacketReader* packet){
	int face = packet->readInt();
	if(face >= 8){
		if(player->getInventories()->getInventory(CASH)->getItemByID(5160000 + face - 8) == NULL)
			return;
	}
	player->getMap()->send(PacketCreator().faceExpression(player->getID(), face), player);
}
void PlayerHandler::recoveryHealthHandle(PacketReader* packet){
	packet->read(4);

	short hp = packet->readShort();
	short mp = packet->readShort();
	if(hp > 150 || mp > 300){ // too much >_>, checks for passive skills and chairs would be nice
		return;
	}
	player->addHP(hp);
	player->addMP(mp);
}
void PlayerHandler::changeChannelHandle(PacketReader* packet){
		// TODO
}