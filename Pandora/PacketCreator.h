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

#ifndef PACKETCREATOR_H
#define PACKETCREATOR_H

#include <vector>
#include "PacketWriter.h"

class Drop;
class Player;
class Players;
class MapObject;
class ObjectMoving;
class Values;
class World;
class Damage;
class Character;
class Item;
class NPC;
class Mob;
class PlayerKeys;
class Quest;
struct Position;
class ShopData;
class Skill;
class Effect;
class Pet;
class Reactor;
class Trade;
class Party;
class PartyMember;
struct Time;

class PacketCreator{
private:
	static const unsigned short PING = 0x11;

	// Remote Control
	static const unsigned short REMOTE_LOGIN = 0x00;
	static const unsigned short COMMAND_REPLAY = 0x01;

	// Login
	static const unsigned short LOGIN_PROCESS = 0x00;
	static const unsigned short WORLD_STATUS = 0x03;
	static const unsigned short PIN = 0x06;
	static const unsigned short SHOW_WORLDS = 0x0A;
	static const unsigned short SHOW_CHARACTERS = 0x0B;
	static const unsigned short CHANNEL_INFO = 0x0C;
	static const unsigned short CHECK_NAME = 0x0D;
	static const unsigned short ADD_CHARACTER = 0x0E;
	static const unsigned short REMOVE_CHARACTER = 0x0F;
	static const unsigned short LOG_BACK = 0x16;
	// Drops
	static const unsigned short SHOW_DROP = 0xEE;
	static const unsigned short REMOVE_DROP = 0xEF;
	static const unsigned short SHOW_GAIN = 0x24;
	// Inventory
	static const unsigned short INVENTORY_CHANGE = 0x1A;
	static const unsigned short USE_PET = 0x9E;
	static const unsigned short PET_MOVE = 0x9B;
	static const unsigned short PET_TEXT = 0x9C;
	static const unsigned short PET_COMMAND_REPLAY = 0x9F; 
	// Stats
	static const unsigned short STATS_UPDATE = 0x1C;
	static const unsigned short ADD_SKILL = 0x21;
	// Maps
	static const unsigned short CHANGE_MAP = 0x72;
	static const unsigned short SHOW_PLAYER = 0x91;
	static const unsigned short REMOVE_PLAYER = 0x92;
	static const unsigned short MAP_EFFECT = 0x7E;
	static const unsigned short MAP_WEATHER = 0x7F;
	static const unsigned short SHOW_TIMER = 0x87;
	static const unsigned short SHOW_SHIP1 = 0x88;
	static const unsigned short SHOW_SHIP0 = 0x89;
	// Mobs
	static const unsigned short DAMAGE_REGULAR = 0xA8;
	static const unsigned short DAMAGE_RANGED = 0xA9;
	static const unsigned short DAMAGE_MAGIC = 0xAA;
	static const unsigned short DAMAGE_MOB = 0xAE;
	static const unsigned short MOB_SKILL = 0xFFFF; // to find
	static const unsigned short SPAWN_MOB = 0xD0;
	static const unsigned short REMOVE_MOB = 0xD1;
	static const unsigned short CONTROL_MOB = 0xD2;
	static const unsigned short MOVE_MOB = 0xD3;
	static const unsigned short MOVE_MOB_RESPONSE = 0xD4;
	static const unsigned short SHOW_MOB_HP = 0xDE;
	static const unsigned short DAMAGE_BY_MOB = 0xFFFF; // to find
	// NPCs
	static const unsigned short SHOW_NPC = 0xE3;
	static const unsigned short NPC_TALK = 0x10E;
	static const unsigned short SHOW_SHOP = 0x10F;
	static const unsigned short BUY_RESONSE = 0x110;
	// Player
	static const unsigned short UPDATE_LOOK = 0xB3;
	static const unsigned short SCROLL_EFFECT = 0x98;
	static const unsigned short ITEM_EFFECT = 0x31;
	static const unsigned short SHOW_CHAIR = 0xB2;
	static const unsigned short CANCEL_CHAIR = 0xBB;
	static const unsigned short SHOW_PLAYER_EFFECT = 0xB4;
	static const unsigned short SHOW_PLAYER_BUFF = 0x1D;
	static const unsigned short SHOW_BUFF = 0xB5;
	static const unsigned short CANCEL_PLAYER_BUFF = 0x1E;
	static const unsigned short CANCEL_BUFF = 0xB6;
	static const unsigned short MAKE_APPLE = 0x79;
	static const unsigned short SHOW_KEYS = 0x12A;
	static const unsigned short DAMAGE_PLAYER = 0xAE;
	static const unsigned short PLAYER_MOVE = 0xA7;
	static const unsigned short CHAT_MESSAGE = 0x93;
	static const unsigned short SHOW_MESSAGE = 0x41;
	static const unsigned short FACE_EEXPRESSION = 0xAF;
	static const unsigned short PLAYER_INFO = 0x3A;
	static const unsigned short FIND_PLAYER = 0x7B;
	static const unsigned short CHANGE_CHANNEL = 0x10;
	static const unsigned short SPECIAL_CHAT = 0x7A;
	// Quests
	static const unsigned short ITEM_GAIN_CHAT = 0xBC;
	static const unsigned short QUEST_UPDATE = 0xC1;
	static const unsigned short COMPLETE_QUEST = 0x2E;
	// Reactors
	static const unsigned short UPDATE_REACTOR = 0xF7;
	static const unsigned short SHOW_REACTOR = 0xF9;
	static const unsigned short DESTROY_REACTOR = 0xFA;
	// Transportation
	static const unsigned short TRANSPORTATION = 0x118;
	// Party
	static const unsigned short PARTY_OPERATION = 0x3B;
	static const unsigned short PARTY_HP = 0xB7;

	PacketWriter pw;
public:
	PacketCreator(){
		pw = PacketWriter();
	}
	PacketWriter* ping();
	// Remote Control
	PacketWriter* loginReplay(bool success);
	PacketWriter* commandReplay(std::string replay);
	// Login
	PacketWriter* loginError(short errorid);
	PacketWriter* loginProcess(char type);
	PacketWriter* loginConnect(char gender, std::string username, bool firstlogin = false);
	PacketWriter* processOk();
	PacketWriter* showWorld(World* world);
	PacketWriter* endWorlds();
	PacketWriter* showChannels();
	PacketWriter* showCharacters(std::vector <Character*>& chars);
	void showCharacter(Character* character);
	PacketWriter* addCharacter(Character* character);
	PacketWriter* checkName(std::string name, bool ok);
	PacketWriter* removeCharacter(int characterid);
	PacketWriter* connectChannel(int charid, short port);
	PacketWriter* logBack();
	// Drops
	PacketWriter* dropObject(Drop* drop, char mode,  MapObject* dropper, int delay = 0);
	PacketWriter* showDrop(Drop* drop);
	PacketWriter* gainDrop(int itemid, int amount, bool mesos = false);
	PacketWriter* gainMesos(int mesos);
	PacketWriter* lootError();
	PacketWriter* removeDrop(int dropid, char type = 0, int id = 0);
	PacketWriter* explodeDrop(int dropid);
	PacketWriter* lootDrop(int dropid, int playerid);
	// Inventory
	PacketWriter* emptyInventoryAction();
	void itemInfo(Item* item, bool showPosition = true, bool showZeroPosition = false);
	PacketWriter* moveItem(char inv, short slot1, short slot2, char equip = 0);
	PacketWriter* updateSlot(Item* item, bool drop = false);
	PacketWriter* newItem(Item* item, bool drop);
	PacketWriter* removeItem(char inv, int slot, bool drop);
	PacketWriter* moveItemMerge(char inv, short slot1, short slot2, short amount);
	PacketWriter* moveItemMergeTwo(char inv, short slot1, short amount1, short slot2, short amount2);
	PacketWriter* scrollItem(Item* scroll, Item* equip, bool destroyed);
	PacketWriter* showPet(int playerid, Pet* pet);
	PacketWriter* removePet(int playerid, int petSlot, bool die = false);
	PacketWriter* movePet(int playerid, int petSlot, ObjectMoving& moving, Position& pos);
	PacketWriter* updatePet(Pet* pet, bool drop = false);
	PacketWriter* petCommandReplay(int playerid, int petSlot, int id, bool success);
	PacketWriter* showPetText(int playerid, int petSlot, std::string text, int act);
	// Player
	PacketWriter* updatePlayer(Player* player);
	PacketWriter* useScroll(int playerid, bool success, bool cursed = false);
	PacketWriter* useItemEffect(int playerid, int itemid);
	PacketWriter* useChair(int playerid, int chairid);
	PacketWriter* cancelChair();
	PacketWriter* showEffect(int playerid, char effect, int what = 0, bool buff = false);
	PacketWriter* showPlayerEffect(char effect, int what = 0, bool buff = true);
	PacketWriter* showBuffEffect(int playerid, char effect, int source);
	PacketWriter* showPlayerBuffEffect(char effect, int source);
	PacketWriter* showPlayerBuff(Values* values, int buffid, int time);
	PacketWriter* showBuff(int playerid, Values* values);
	PacketWriter* cancelPlayerBuff(Values* values);
	PacketWriter* cancelBuff(int playerid, Values* values);
	void playerInfo(Player* player);
	void playerShow(Player* player, bool smega = false);
	PacketWriter* makeApple();
	PacketWriter* connectionData(Player* player);
	PacketWriter* showKeys(PlayerKeys* keys);
	PacketWriter* showMoving(int playerid, ObjectMoving& moving);
	PacketWriter* faceExpression(int playerid, int face);
	PacketWriter* showChatMessage(int playerid, std::string msg, bool macro, bool gm = false);
	PacketWriter* damagePlayer(int playerid, int skill, int dmg, int obj);
	PacketWriter* showMessage(std::string msg, char type, int channel = 0, bool server = false);
	PacketWriter* showInfo(Player* player);
	PacketWriter* findPlayerReplay(std::string name, bool success = true);
	PacketWriter* findPlayerMap(std::string name, int map);
	PacketWriter* findPlayerChannel(std::string name, int channel);
	PacketWriter* whisperPlayer(std::string name, std::string msg, int channel);
	PacketWriter* changeChannel(char channelid, short port);
	PacketWriter* showSpecialChat(char type, std::string& msg, std::string& sender);
	// Stats
	PacketWriter* gainEXP(int exp, bool chat, bool yellow=false, int partybonus=0);
	PacketWriter* enableAction();
	PacketWriter* updateStats(Values& stats, bool item=false, char pets = 0);
	// Map
	PacketWriter* showPlayer(Player* player);
	PacketWriter* removePlayer(int playerid);
	PacketWriter* changeMap(Player* player);
	PacketWriter* changeMusic(std::string sound);
	PacketWriter* showEffect(std::string effect);
	PacketWriter* playSound(std::string sound);
	PacketWriter* showObject(std::string object);
	PacketWriter* mapChange(char mode, std::string& name);
	PacketWriter* showShip(char mode, char type);
	PacketWriter* showClock(Time& time);
	PacketWriter* showTimer(int seconds);
	// Mobs
	PacketWriter* controlMob(Mob* mob, char type, bool args = false, int from = 0);
	PacketWriter* endControlMob(int mobid);
	PacketWriter* showMob(Mob* mob, char type, int from = 0);
	PacketWriter* moveMob(int mobid, Position& position, ObjectMoving& moving, char skill, int skillid);
	PacketWriter* moveMobResponse(Mob* mob, int count, bool agrs);
	PacketWriter* showHP(int mobid, char per);
	PacketWriter* showBossHP(int mobid, int hp, int maxhp, char per, int color, int bgcolor);
	PacketWriter* killMob(int mobid, bool animation = true);
	void damage(Player *player, Damage& damage, int itemid = 0);
	PacketWriter* damageMob(Player *player, Damage& dmg);
	PacketWriter* damageMob(int mobid, int damage);
	PacketWriter* damageMobMagic(Player *player, Damage& dmg);
	PacketWriter* damageMobRanged(Player *player, Damage& dmg, int itemid = 0);
	PacketWriter* damageByMob(int mobid, int damage, int hp, int maxhp);
	// NPCs
	PacketWriter* showNPC(NPC* npc);
	PacketWriter* bought();
	PacketWriter* showNPCShop(ShopData* data);
	void npcPacket(int npcid, std::string text, char type);
	PacketWriter* sendSimple(int npcid, std::string text);
	PacketWriter* sendYesNo(int npcid, std::string text);
	PacketWriter* sendBackNext(int npcid, std::string text, bool back, bool next);
	PacketWriter* sendAcceptDecline(int npcid, std::string text);
	PacketWriter* sendGetText(int npcid, std::string text);
	PacketWriter* sendGetNumber(int npcid, std::string text, int def, int min, int max);
	PacketWriter* sendStyle(int npcid, std::string text, int styles[], char size);
	// Party
	PacketWriter* createParty(int leader);
	PacketWriter* removeParty(int partyid, int leader);
	PacketWriter* inviteParty(int party, std::string& invite);
	PacketWriter* partyError(char error, std::string& err = std::string(""));
	PacketWriter* changeLeader(int newleader);
	PacketWriter* updateParty(Party* party);
	PacketWriter* leaveParty(Party* party, PartyMember* member, bool expel = false);
	PacketWriter* joinParty(Party* party, PartyMember* member);
	void partyInfo(Party* party, int leave = -1);
	PacketWriter* updatePartyHP(Player* player);
	//	Quests
	PacketWriter* updateQuest(short questid, int npcid, int nextquest=0);
	PacketWriter* updateQuestInfo(Quest* quest, bool forfeit = false);
	PacketWriter* questDone(int questid);
	PacketWriter* itemGainChat(int itemid, int amount);
	PacketWriter* mesosGainChat(int amount);
	// Skills
	PacketWriter* updateSkill(Skill* skill);
	PacketWriter* updateSkill(std::vector <Skill*>& skills);
	// Reactors
	PacketWriter* showReactor(Reactor* reactor);
	PacketWriter* updateReactor(Reactor* reactor, short stance);
	PacketWriter* destroyReactor(Reactor* reactor);

	// Transportation
	PacketWriter* showTrade(Player* right, Player* left = NULL);
	PacketWriter* inviteTrade(Player* player, int inviteid);
	PacketWriter* joinTrade(Player* player);
	PacketWriter* tradeError(char s);
	PacketWriter* showTradeChat(std::string msg, bool by);
	PacketWriter* tradeMesos(int mesos, char side);
	PacketWriter* tradeItem(Item* item, char side);
	PacketWriter* tradeConfirm();

	// PvP
	PacketWriter* showPvPMovingForPlayer(Player* player, Position& start);
	PacketWriter* showPvPMoving(Player* player, Position& pos, ObjectMoving& obj);
	PacketWriter* startPvPMobForPlayer(Player* player);
	PacketWriter* startPvPMob(Player* player);
	PacketWriter* startPvPMobEffect(Player* player, int wdef, int mdef);
	PacketWriter* stopPvP(Player* player);
	PacketWriter* stopPvPForPlayer(Player* player);
	PacketWriter* updatePvPHP(Player* player);



};

#endif
