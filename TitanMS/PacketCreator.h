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

using namespace std;


class PacketCreator{
private:

	static const short PING = 0x11;

	// Remote Control
	static const short REMOTE_LOGIN = 0x00;
	static const short COMMAND_REPLAY = 0x01;

	// Login
	static const short LOGIN_PROCESS = 0x00;
	static const short WORLD_STATUS = 0x03;
	static const short PIN = 0x06;
	static const short SHOW_WORLDS = 0x0A;
	static const short SHOW_CHARACTERS = 0x0B;
	static const short CHANNEL_INFO = 0x0C;
	static const short CHECK_NAME = 0x0D;
	static const short ADD_CHARACTER = 0x0E;
	static const short REMOVE_CHARACTER = 0x0F;
	static const short LOG_BACK = 0x16;
	// Drops
	static const short SHOW_DROP = 0xCC;
	static const short REMOVE_DROP = 0xCD;
	static const short SHOW_GAIN = 0x24;
	// Inventory
	static const short INVENTORY_CHANGE = 0x1A;
	static const short USE_PET = 0x7E;
	static const short PET_MOVE = 0x80;
	static const short PET_TEXT = 0x81;
	static const short PET_COMMAND_REPLAY = 0x84; 
	// Stats
	static const short STATS_UPDATE = 0x1C;
	static const short ADD_SKILL = 0x21;
	// Maps
	static const short CHANGE_MAP = 0x5B;
	static const short SHOW_PLAYER = 0x77;
	static const short REMOVE_PLAYER = 0x78;
	static const short MAP_EFFECT = 0x67;
	static const short MAP_WEATHER = 0x68;
	static const short SHOW_TIMER = 0x6D;
	static const short SHOW_SHIP1 = 0x6E;
	static const short SHOW_SHIP0 = 0x6F;
	// Mobs
	static const short DAMAGE_REGULAR = 0x8D;
	static const short DAMAGE_RANGED = 0x8E;
	static const short DAMAGE_MAGIC = 0x8F;
	static const short DAMAGE_MOB = 0x91; // to find
	static const short MOB_SKILL = 0x92; // to find
	static const short SPAWN_MOB = 0xAE;
	static const short REMOVE_MOB = 0xAF;
	static const short CONTROL_MOB = 0xB0;
	static const short MOVE_MOB = 0xB1;
	static const short MOVE_MOB_RESPONSE = 0xB2;
	static const short SHOW_MOB_HP = 0xBC;
	static const short DAMAGE_BY_MOB = 0xB8;
	// NPCs
	static const short SHOW_NPC = 0xC1;
	static const short NPC_TALK = 0xEB;
	static const short SHOW_SHOP = 0xEC;
	static const short BUY_RESONSE = 0xED;
	// Player
	static const short UPDATE_LOOK = 0x97;
	static const short SCROLL_EFFECT = 0x7D;
	static const short ITEM_EFFECT = 0x31;
	static const short SHOW_CHAIR = 0x96;
	static const short CANCEL_CHAIR = 0x9F;
	static const short SHOW_PLAYER_EFFECT = 0x98;
	static const short SHOW_PLAYER_BUFF = 0x1D;
	static const short SHOW_BUFF = 0x99;
	static const short CANCEL_PLAYER_BUFF = 0x1E;
	static const short CANCEL_BUFF = 0x9A;
	static const short MAKE_APPLE = 0x62;
	static const short SHOW_KEYS = 0x105;
	static const short DAMAGE_PLAYER = 0x93;
	static const short PLAYER_MOVE = 0x8C;
	static const short CHAT_MESSAGE = 0x79;
	static const short SHOW_MESSAGE = 0x41;
	static const short FACE_EEXPRESSION = 0x94;
	static const short PLAYER_INFO = 0x3A;
	static const short FIND_PLAYER = 0x64;
	static const short CHANGE_CHANNEL = 0x03;
	static const short SPECIAL_CHAT = 0x63;
	// Quests
	static const short ITEM_GAIN_CHAT = 0xA0;
	static const short QUEST_UPDATE = 0xA5;
	static const short COMPLETE_QUEST = 0x2E;
	// Reactors
	static const short UPDATE_REACTOR = 0xD5;
	static const short SHOW_REACTOR = 0xD7;
	static const short DESTROY_REACTOR = 0xD8;
	// Transportation
	static const short TRANSPORTATION = 0xF3;
	// Party
	static const short PARTY_OPERATION = 0x3B;
	static const short PARTY_HP = 0x9B;

	PacketWriter pw;
public:
	PacketCreator(){
		pw = PacketWriter();
	}
	PacketWriter* ping();
	// Remote Control
	PacketWriter* loginReplay(bool success);
	PacketWriter* commandReplay(string replay);
	// Login
	PacketWriter* loginError(short errorid);
	PacketWriter* loginProcess(char type);
	PacketWriter* loginConnect(char gender, string username, bool firstlogin = false);
	PacketWriter* processOk();
	PacketWriter* showWorld(World* world);
	PacketWriter* endWorlds();
	PacketWriter* showChannels();
	PacketWriter* showCharacters(vector <Character*>& chars);
	void showCharacter(Character* character);
	PacketWriter* addCharacter(Character* character);
	PacketWriter* checkName(string name, bool ok);
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
	PacketWriter* showPetText(int playerid, int petSlot, string text, int act);
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
	PacketWriter* showChatMessage(int playerid, string msg, bool macro, bool gm = false);
	PacketWriter* damagePlayer(int playerid, int skill, int dmg, int obj);
	PacketWriter* showMessage(string msg, char type, int channel = 0, bool server = false);
	PacketWriter* showInfo(Player* player);
	PacketWriter* findPlayerReplay(string name, bool success = true);
	PacketWriter* findPlayerMap(string name, int map);
	PacketWriter* findPlayerChannel(string name, int channel);
	PacketWriter* whisperPlayer(string name, string msg, int channel);
	PacketWriter* changeChannel(char channelid, short port);
	PacketWriter* showSpecialChat(char type, string& msg, string& sender);
	// Stats
	PacketWriter* gainEXP(int exp, bool chat, bool yellow=false, int partybonus=0);
	PacketWriter* enableAction();
	PacketWriter* updateStats(Values& stats, bool item=false, char pets = 0);
	// Map
	PacketWriter* showPlayer(Player* player);
	PacketWriter* removePlayer(int playerid);
	PacketWriter* changeMap(Player* player);
	PacketWriter* changeMusic(string sound);
	PacketWriter* showEffect(string effect);
	PacketWriter* playSound(string sound);
	PacketWriter* showObject(string object);
	PacketWriter* mapChange(char mode, string& name);
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
	void damage(int playerid, Damage& damage, int itemid = 0);
	PacketWriter* damageMob(int playerid, Damage& dmg);
	PacketWriter* damageMob(int mobid, int damage);
	PacketWriter* damageMobMagic(int playerid, Damage& dmg);
	PacketWriter* damageMobRanged(int playerid, Damage& dmg, int itemid = 0);
	PacketWriter* damageByMob(int mobid, int damage, int hp, int maxhp);
	// NPCs
	PacketWriter* showNPC(NPC* npc);
	PacketWriter* bought();
	PacketWriter* showNPCShop(ShopData* data);
	void npcPacket(int npcid, string text, char type);
	PacketWriter* sendSimple(int npcid, string text);
	PacketWriter* sendYesNo(int npcid, string text);
	PacketWriter* sendBackNext(int npcid, string text, bool back, bool next);
	PacketWriter* sendAcceptDecline(int npcid, string text);
	PacketWriter* sendGetText(int npcid, string text);
	PacketWriter* sendGetNumber(int npcid, string text, int def, int min, int max);
	PacketWriter* sendStyle(int npcid, string text, int styles[], char size);
	// Party
	PacketWriter* createParty(int leader);
	PacketWriter* removeParty(int partyid, int leader);
	PacketWriter* inviteParty(int party, string& invite);
	PacketWriter* partyError(char error, string& err = string(""));
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
	PacketWriter* updateSkill(vector <Skill*>& skills);
	// Reactors
	PacketWriter* showReactor(Reactor* reactor);
	PacketWriter* updateReactor(Reactor* reactor, short stance);
	PacketWriter* destroyReactor(Reactor* reactor);

	// Transportation
	PacketWriter* showTrade(Player* right, Player* left = NULL);
	PacketWriter* inviteTrade(Player* player, int inviteid);
	PacketWriter* joinTrade(Player* player);
	PacketWriter* tradeError(char s);
	PacketWriter* showTradeChat(string msg, bool by);
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
