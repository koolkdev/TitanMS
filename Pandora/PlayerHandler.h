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

#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

class PacketReader;
class Player;
class PlayerHandler;
#include "Handler.h"

class PlayerHandler : public Handler<PlayerHandler> {
private:
	static const unsigned short CONNECT = 0x14;
	static const unsigned short PING = 0x18;
	// NPCs
	static const unsigned short HANDLE_NPC = 0x39;
	static const unsigned short NPC_CHAT = 0x3B;
	static const unsigned short USE_SHOP = 0x3C;
	// Channels
	static const unsigned short CHANGE_CHANNEL = 0x26;
	// Player
	static const unsigned short TAKE_DAMAGE = 0x2F;
	static const unsigned short PLAYER_MOVE = 0x28;
	static const unsigned short CHANGE_MAP = 0x25;
	static const unsigned short CHANGE_MAP_SPECIAL = 0x63;
	static const unsigned short PLAYER_INFO = 0x60;
	static const unsigned short FACE_EXP = 0x32;
	static const unsigned short RECOVERY = 0x58;
	static const unsigned short TRANSPORTATION = 0x79;        
	// Mobs
	static const unsigned short DAMAGE_MOB = 0x2B;
	static const unsigned short DAMAGE_MOB_RANGED = 0x2C;
	static const unsigned short DAMAGE_MOB_MAGIC = 0x2D;
	static const unsigned short CONTROL_MOB = 0xB2;
	static const unsigned short CONTROL_MOB_SKILL = 0xA0; // to find
	static const unsigned short HIT_BY_MOB = 0xB6;
	// Skills
	static const unsigned short ADD_SKILL = 0x59;
	static const unsigned short USE_SKILL = 0x5A;
	static const unsigned short STOP_SKILL = 0x5B;
	// Chat
	static const unsigned short USE_CHAT = 0x30;
	static const unsigned short SPECIAL_CHAT = 0x75;
	static const unsigned short CHAT_COMMAND = 0x76;
	// Inventory
	static const unsigned short CANCEL_CHAIR = 0x29;
	static const unsigned short USE_CHAIR = 0x2A;
	static const unsigned short USE_ITEM_EFFECT = 0x33;
	static const unsigned short AUTO_ARRANGEMENT = 0x44;
	static const unsigned short MOVE_ITEM = 0x46;
	static const unsigned short USE_ITEM = 0x47;
	static const unsigned short CANCEL_ITEM_BUFF = 0x48;
	static const unsigned short USE_SUMMON_BAG = 0x4A;
	static const unsigned short USE_PET_FOOD = 0x4B;
	static const unsigned short USE_CASH_ITEM = 0x4E;
	static const unsigned short USE_RETURN_SCROLL = 0x54;
	static const unsigned short USE_SCROLL = 0x55;
	// Pet
	static const unsigned short USE_PET = 0x61;
	static const unsigned short MOVE_PET = 0xA1; 
	static const unsigned short PET_COMMAND_TEXT = 0xA2;
	static const unsigned short PET_COMMAND = 0xA3;
	// Stats
	static const unsigned short ADD_STAT = 0x56;
	static const unsigned short ADD_AUTOSTAT = 0x57;
	// Drops
	static const unsigned short DROP_MESOS = 0x5D;
	static const unsigned short LOOT_DROP = 0xC0;
	// Quests
	static const unsigned short QUEST_HANDLE = 0x6A;
	// Keys
	static const unsigned short CHANGE_KEY = 0x85;
	// Reactors
	static const unsigned short HIT_REACTOR = 0xC3;
	// Party
	static const unsigned short PARTY_OPERATION = 0x7A;
	static const unsigned short DENIED_PARTY = 0x7B; //to find

	Player* player;
public:
	PlayerHandler(Player* player){
		this->player = player;
	}
	static void loadPackets();

	void connectionRequestHandle(PacketReader& packet);
	void pingHandle(PacketReader& packet);
	// NPCs
	void NPCHandle(PacketReader& packet);
	void NPCChatHandle(PacketReader& packet);
	// TODO: controlled npcs
	void useShopHandle(PacketReader& packet);
	// Channels - TODO
	void changeChannelHandle(PacketReader& packet);
	// Player
	void playerTakeDamageHandle(PacketReader& packet);
	void playerMovingHandle(PacketReader& packet);
	void changeMapHandle(PacketReader& packet);
	void changeMapSpecialHandle(PacketReader& packet);
	void getPlayerInformationHandle(PacketReader& packet);
	void useFaceExpressionHandle(PacketReader& packet);
	void recoveryHealthHandle(PacketReader& packet);
	void itemsTransportationHandle(PacketReader& packet);
	// Mobs
	void damageMobMagicHandle(PacketReader& packet);
	void damageMobHandle(PacketReader& packet);
	void damageMobRangedHandle(PacketReader& packet);
	void controlMobHandle(PacketReader& packet);
	void controlMobSkillHandle(PacketReader& packet); //?
	void hitByMobHandle(PacketReader& packet);
	// Skills
	void addSkillHandle(PacketReader& packet);
	void stopSkillHandle(PacketReader& packet);
	void useSkillHandle(PacketReader& packet);
	// Chat
	void chatCommandHandle(PacketReader& packet);
	void useChatHandle(PacketReader& packet);
	void specialChatHandle(PacketReader& packet);
	// Inventory
	void cancelChairHandle(PacketReader& packet);
	void useChairHandle(PacketReader& packet);
	void useItemEffectHandle(PacketReader& packet);
	void useCashItemHandle(PacketReader& packet);
	void moveItemHandle(PacketReader& packet);
	void useItemHandle(PacketReader& packet);
	void useReturnScrollHandle(PacketReader& packet);
	void useScrollHandle(PacketReader& packet);
	void useSummonBagHandle(PacketReader& packet);
	void cancelItemBuffHandle(PacketReader& packet);
	void autoArrangementHandle(PacketReader& packet);
	void usePetFoodHandle(PacketReader& packet);
	// Pet
	void usePetHandle(PacketReader& packet);
	void movePetHandle(PacketReader& packet);
	void petCommandHandle(PacketReader& packet);
	void petCommandTextHandle(PacketReader& packet);
	//Stats
	void addStatHandle(PacketReader& packet);
	void addAutoStatHandle(PacketReader& packet);
	// Drops
	void dropMesosHandle(PacketReader& packet);
	void lootDropHandle(PacketReader& packet);
	// Quests
	void questHandle(PacketReader& packet);
	// Keys
	void changeKeyHandle(PacketReader& packet);
	// Reactor
	void hitReactorHandle(PacketReader& packet);
	// Party
	void partyOperationHandle(PacketReader& packet);
	void deniedPartyInviteHandle(PacketReader& packet);
};

#endif