/*
	This file is part of TitanMS.
	Copyright (C) 2008 koolk

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "DataProvider.h"
#include "PlayerHandler.h"
#include "PacketReader.h" 
#include "PacketCreator.h" 
#include "PacketWriter.h" 
#include "Player.h"
#include "PlayerBuffs.h"
#include "MapMobs.h"
#include "Map.h"
#include "Effect.h"
#include "PvP.h"
#include "MobsData.h"
#include "Mob.h"
#include "Maps.h"
#include "Item.h"
#include "Pet.h"
#include "Equip.h"
#include "ObjectMoving.h"
#include "Channel.h"
#include "MapPortalData.h"
#include "PlayerInventories.h"
#include "PacketHandlingError.h"
#include "Handler.h"
#include "Inventory.h"
#include "MapPlayers.h"
#include "AngelScriptEngine.h"
#include "Trade.h"
#include "Transportations.h"
#include <iostream>
#include "Tools.h"
using namespace Tools;

hash_map <short, PlayerHandler::handlerf> Handler<PlayerHandler>::handlers;

void PlayerHandler::loadPackets(){
	handlers[PING] = &pingHandle;
	handlers[CONNECT] = &connectionRequestHandle;
	handlers[NPC_CHAT] = &NPCChatHandle;
	handlers[HANDLE_NPC] = &NPCHandle;
	handlers[USE_SHOP] = &useShopHandle;
	handlers[CHANGE_CHANNEL] = &changeChannelHandle;
	handlers[TAKE_DAMAGE] = &playerTakeDamageHandle;
	handlers[PLAYER_MOVE] = &playerMovingHandle;
	handlers[CHANGE_MAP] = &changeMapHandle;
	handlers[PLAYER_INFO] = &getPlayerInformationHandle;
	handlers[FACE_EXP] = &useFaceExpressionHandle;
	handlers[RECOVERY] = &recoveryHealthHandle;
	handlers[DAMAGE_MOB_MAGIC] = &damageMobMagicHandle;
	handlers[DAMAGE_MOB] = &damageMobHandle;
	handlers[DAMAGE_MOB_RANGED] = &damageMobRangedHandle;
	handlers[CONTROL_MOB] = &controlMobHandle;
	handlers[CONTROL_MOB_SKILL] = &controlMobSkillHandle;
	handlers[ADD_SKILL] = &addSkillHandle;
	handlers[STOP_SKILL] = &stopSkillHandle;
	handlers[USE_SKILL] = &useSkillHandle;
	handlers[USE_CHAT] = &useChatHandle;
	handlers[CHAT_COMMAND] = &chatCommandHandle;
	handlers[MOVE_ITEM] = &moveItemHandle;
	handlers[USE_CHAIR] = &useChairHandle;
	handlers[CANCEL_CHAIR] = &cancelChairHandle;
	handlers[USE_ITEM_EFFECT] = &useItemEffectHandle;
	handlers[USE_CASH_ITEM] = &useCashItemHandle;
	handlers[USE_ITEM] = &useItemHandle;
	handlers[USE_SCROLL] = &useScrollHandle;
	handlers[USE_RETURN_SCROLL] = &useReturnScrollHandle;
	handlers[USE_SUMMON_BUG] = &useSummonBugHandle;
	handlers[CANCEL_ITEM_BUFF] = &cancelItemBuffHandle;
	handlers[AUTO_ARRANGEMENT] = &autoArrangementHandle;
	handlers[ADD_STAT] = &addStatHandle;
	handlers[DROP_MESOS] = &dropMesosHandle;
	handlers[LOOT_DROP] = &lootDropHandle;
	handlers[QUEST_HANDLE] = &questHandle;
	handlers[CHANGE_KEY] = &changeKeyHandle;
	handlers[USE_PET] = &usePetHandle;
	handlers[MOVE_PET] = &movePetHandle;
	handlers[PET_COMMAND] = &petCommandHandle;
	handlers[PET_COMMAND_TEXT] = &petCommandTextHandle;
	handlers[HIT_REACTOR] = &hitReactorHandle;
	handlers[HIT_BY_MOB] = &hitByMobHandle;
	handlers[TRANSPORTATION] = &itemsTransportationHandle;
	handlers[CHANGE_MAP_SPECIAL] = &changeMapSpecialHandle;
	handlers[USE_PET_FOOD] = &usePetFoodHandle;
	handlers[PARTY_OPERATION] = &partyOperationHandle;
	handlers[DENIED_PARTY] = &deniedPartyInviteHandle;
	handlers[SPECIAL_CHAT] = &specialChatHandle;
}

void PlayerHandler::pingHandle(PacketReader& packet){
}

void PlayerHandler::connectionRequestHandle(PacketReader& packet){
	player->setUserID(packet.readInt());
}
void PlayerHandler::playerTakeDamageHandle(PacketReader& packet){
	packet.readInt();
	char damagetype = packet.read(); 
	packet.read();
	int damage = packet.readInt();
	if(damage < 0)
		return;
	int mobid = 0;
	int objid = 0;
	if(player->getPvP() && player->getMap()->getPvP() != NULL)
		player->getMap()->getPvP()->hitPlayer(NULL, player, damage);
	if(damagetype != -2){ // Damage from monster...
		mobid = packet.readInt();
		if(MobsData::getInstance()->getDataByID(mobid) == NULL) return;
		objid = packet.readInt();
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
void PlayerHandler::playerMovingHandle(PacketReader& packet){
	Position pos = player->getPosition();
	packet.read(5);
	ObjectMoving moving = ObjectMoving(packet, player);
	player->getMap()->send(PacketCreator().showMoving(player->getID(), moving), player);
	if(player->getPvP() && player->getMap()->getPvP() != NULL)
		player->getMap()->getPvP()->movePlayer(player, pos, moving);
}
void PlayerHandler::changeMapHandle(PacketReader& packet){
	packet.read();
	int type = packet.readInt();
	if(type == 0){
		if(player->getHP() > 0) return;
		player->getBuffs()->cancelAll();
		player->setHP(50, false);
		int mapid = player->getMap()->getReturnMap();
		if(mapid == 999999999){
			player->changeMap(player->getMap());
		}
		else{
			Map* map = player->getChannel()->getMaps()->getMap(mapid);
			if(map == NULL){
				player->changeMap(player->getMap());
			}
			else{
				player->changeMap(map);
			}
		}
	}
	else{
		string toname = packet.readString();
		MapPortalData* portal = player->getMap()->getPortal(toname);
		if(portal != NULL && portal->getScript() != ""){
			AngelScriptEngine::handlePortal(player, portal);
			player->send(PacketCreator().enableAction());
		}
		else if(portal->getType() == 5 && !player->getMap()->getPortalStatus(portal->getFromPortal())){
			player->send(PacketCreator().showMessage("The portal is closed for now.", 5)); //?
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
void PlayerHandler::getPlayerInformationHandle(PacketReader& packet){
	packet.readInt();
	int playerid = packet.readInt();
	Player* to = player->getMap()->getPlayers()->getPlayerByID(playerid);
	if(to == NULL)
		to = player;
	player->send(PacketCreator().showInfo(to));
}
void PlayerHandler::useFaceExpressionHandle(PacketReader& packet){
	int face = packet.readInt();
	if(face >= 8){
		if(player->getInventories()->getInventory(CASH)->getItemByID(5160000 + face - 8) == NULL)
			return;
	}
	player->getMap()->send(PacketCreator().faceExpression(player->getID(), face), player);
}
void PlayerHandler::recoveryHealthHandle(PacketReader& packet){
	packet.read(4);

	short hp = packet.readShort();
	short mp = packet.readShort();
	if(hp > 150 || mp > 300){ // too much >_>, checks for passive skills and chairs would be nice
		return;
	}
	player->addHP(hp);
	player->addMP(mp);
}
void PlayerHandler::changeChannelHandle(PacketReader& packet){
		// TODO
}
void PlayerHandler::itemsTransportationHandle(PacketReader& packet){
	char type = packet.read();
	if(type == Transportations::OPEN){
		char create = packet.read();
		if(create == Transportations::TYPE_TRADE){ 
			if(player->getTrade() == NULL){
				Trade* trade = new Trade(player);
				trade->open();
			}
			else{
				player->send(PacketCreator().showMessage("You are already in a trade", 5));
			}
		}
		else if(create == Transportations::TYPE_SHOP){ 

		}
	}
	else if(type == Transportations::INVITE){ 
		Trade* trade = player->getTrade();
		if(trade != NULL && trade->getInviteID() == 0){
			int invitedid = packet.readInt();
			Player* invited = player->getMap()->getPlayer(invitedid);
			if(!trade->invite(invited)){
				trade->close();
				player->setTrade(NULL);
				delete trade;
			}
		}
	}
	else if(type == Transportations::DECLINE){ 
		int tradeid = packet.readInt();
		Trade* trade = player->getChannel()->getTransportations()->getTradeByInvite(tradeid);
		if(trade != NULL && trade->getInvited() == player){
			trade->decline();
			player->getChannel()->getTransportations()->closeTrade(trade);
		}
	}
	else if(type == Transportations::JOIN){ 
		int id = packet.readInt();
		Trade* trade = player->getChannel()->getTransportations()->getTradeByInvite(id);
		if(trade != NULL){ // trade
			if(trade->getInvited() == player){
				if(trade->getPlayer()->getMap() != player->getMap())
					player->send(PacketCreator().tradeError(9));
				else
					trade->accept();
			}
			
		}
		else if(false){ // shop
		}
		else{
			player->send(PacketCreator().showMessage("The room is already closed.", 1));
		}
	}
	else if(type == Transportations::CHAT){ 
		if(player->getTrade() != NULL){
			player->getTrade()->chat(packet.readString());
		}
	}
	else if(type == Transportations::CLOSE){ 
		Trade* trade = player->getTrade();
		if(trade != NULL){
			player->getChannel()->getTransportations()->closeTrade(trade);
		}
	}
	else if(type == Transportations::OPEN_SHOP){
	}
	else if(type == Transportations::TRADE_ADD_ITEM){
		Trade* trade = player->getTrade();
		if(trade != NULL){
			if(trade->isConfirmed()) return;
			char inv = packet.read();
			short islot = packet.readShort();
			Item* item = player->getInventories()->getItemBySlot(inv, islot);
			if(item == NULL)
				return;
			short amount = packet.readShort();
			char slot = packet.read();
			if(IS_STAR(item->getID()))
				amount = item->getAmount();
			if((amount >= 0 && amount <= item->getAmount())){
				Item* tItem = NULL;
				if(IS_EQUIP(item->getID())){
					tItem = new Equip(*(Equip*)item);				
				}
				else if(IS_PET(item->getID())){
					tItem = new Pet(*(Pet*)item);
				}
				else{
					tItem = new Item(*item);
					tItem->setAmount(amount);
				}
				player->getInventories()->removeItemBySlot(inv, islot, amount);
				trade->addItem(tItem, slot);
			}
		}
	}
	else if(type == Transportations::ADD_MESOS){
		int mesos = packet.readInt();
		if(mesos <= 0 || mesos > player->getMesos())
			return;
		Trade* trade = player->getTrade();
		if(trade != NULL && !trade->isConfirmed()){
			trade->addMesos(mesos);
		}
	}
	else if(type == Transportations::CONFIRM){
		Trade* trade = player->getTrade();
		if(trade != NULL && !trade->isConfirmed() && trade->getTrader() != NULL){
			trade->confirm();
			if(trade->getTrader()->isConfirmed()){
				trade->complete();
				player->setTrade(NULL);
				trade->getTrader()->getPlayer()->setTrade(NULL);
				delete trade->getTrader();
				delete trade;
			}
		}
	}
	else if(type == Transportations::SHOP_ADD_ITEM){
	}
	else if(type == Transportations::BUY){
	}
	else if(type == Transportations::REMOVE_ITEM){ 
	}
}
void PlayerHandler::changeMapSpecialHandle(PacketReader& packet){
	packet.read();
	
	string toname = packet.readString();
	MapPortalData* portal = player->getMap()->getPortal(toname);
	if(portal == NULL)
		return;
	AngelScriptEngine::handlePortal(player, portal);
	player->send(PacketCreator().enableAction());
}