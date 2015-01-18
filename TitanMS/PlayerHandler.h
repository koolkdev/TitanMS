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

	static const short CONNECT = 0x14;
	static const short PING = 0x18;
	// NPCs
	static const short HANDLE_NPC = 0x36;
	static const short NPC_CHAT = 0x38;
	static const short USE_SHOP = 0x39;
	// Channels
	static const short CHANGE_CHANNEL = 0x24;
	// Player
	static const short TAKE_DAMAGE = 0x2D;
	static const short PLAYER_MOVE = 0x26;
	static const short CHANGE_MAP = 0x23;
	static const short CHANGE_MAP_SPECIAL = 0x5C;
	static const short PLAYER_INFO = 0x59;
	static const short FACE_EXP = 0x30;
	static const short RECOVERY = 0x51;
	static const short TRANSPORTATION = 0x6E;        
	// Mobs
	static const short DAMAGE_MOB = 0x29;
	static const short DAMAGE_MOB_RANGED = 0x2A;
	static const short DAMAGE_MOB_MAGIC = 0x2B;
	static const short CONTROL_MOB = 0x9A;
	static const short CONTROL_MOB_SKILL = 0xA0; // to find
	static const short HIT_BY_MOB = 0x9E; // to find
	// Skills
	static const short ADD_SKILL = 0x52;
	static const short USE_SKILL = 0x53;
	static const short STOP_SKILL = 0x54;
	// Chat
	static const short USE_CHAT = 0x2E;
	static const short SPECIAL_CHAT = 0x6A;
	static const short CHAT_COMMAND = 0x6B;
	// Inventory
	static const short CANCEL_CHAIR = 0x27;
	static const short USE_CHAIR = 0x28;
	static const short USE_ITEM_EFFECT = 0x31;
	static const short AUTO_ARRANGEMENT = 0x40;
	static const short MOVE_ITEM = 0x42;
	static const short USE_ITEM = 0x43;
	static const short CANCEL_ITEM_BUFF = 0x44;
	static const short USE_SUMMON_BUG = 0x46;
	static const short USE_PET_FOOD = 0x47;
	static const short USE_CASH_ITEM = 0x49;
	static const short USE_RETURN_SCROLL = 0x4E;
	static const short USE_SCROLL = 0x4F;
	// Pet
	static const short USE_PET = 0x5A;
	static const short MOVE_PET = 0x89; 
	static const short PET_COMMAND_TEXT = 0x8A;
	static const short PET_COMMAND = 0x8B;
	// Stats
	static const short ADD_STAT = 0x50;
	// Drops
	static const short DROP_MESOS = 0x56;
	static const short LOOT_DROP = 0xA8;
	// Quests
	static const short QUEST_HANDLE = 0x62;
	// Keys
	static const short CHANGE_KEY = 0x7A;
	// Reactors
	static const short HIT_REACTOR = 0xAB;
	// Party
	static const short PARTY_OPERATION = 0x6F;
	static const short DENIED_PARTY = 0x70;

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
	void useSummonBugHandle(PacketReader& packet);
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