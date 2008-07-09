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

using namespace std;


class PacketCreator{
private:
	// Login
	static const short LOGIN_PROCESS = 0x00;
	static const short PIN = 0x0D;
	static const short CHECK_NAME = 0x06;
	static const short SHOW_WORLDS = 0x05;
	static const short WORLD_STATUS = 0x12;
	static const short SHOW_CHARACTERS = 0x13;
	static const short ADD_CHARACTER = 0x07;
	static const short REMOVE_CHARACTER = 0x08;
	static const short CHANNEL_INFO = 0x0C;
	static const short LOG_BACK = 0x15;
	/*static const short LOGIN_PROCESS = 0x00;
	static const short PIN = 0x06;
	static const short CHECK_NAME = 0x0D;
	static const short SHOW_WORLDS = 0x0A;
	static const short WORLD_STATUS = 0x03;
	static const short SHOW_CHARACTERS = 0x0B;
	static const short ADD_CHARACTER = 0x0E;
	static const short REMOVE_CHARACTER = 0x0F;
	static const short CHANNEL_INFO = 0x0C;
	static const short LOG_BACK = 0x16;*/
	// Drops
	static const short SHOW_DROP = 0xB9;
	static const short REMOVE_DROP = 0xBA;
	static const short SHOW_GAIN = 0x32;
	// Inventory
	static const short INVENTORY_CHANGE = 0x18;
	static const short USE_PET = 0x7D; // 7E
	static const short PET_MOVE = 0x82; // 80
	static const short PET_COMMAND_REPLAY = 0x83; // 84
	static const short PET_TEXT = 0x81; // 81
	// Stats
	static const short STATS_UPDATE = 0x23;
	static const short ADD_SKILL = 0x2F;
	// Maps
	static const short CHANGE_MAP = 0x4E; // 5B
	static const short SHOW_PLAYER = 0x66;
	static const short REMOVE_PLAYER = 0x71;
	static const short MAP_CHANGE = 0x54;
	// Mobs
	static const short CONTROL_MOB = 0xA5;
	static const short SPAWN_MOB = 0x97;
	static const short MOVE_MOB = 0x98;
	static const short MOVE_MOB_RESPONSE = 0x9D;
	static const short SHOW_MOB_HP = 0x99;
	static const short REMOVE_MOB = 0xA6;
	static const short DAMAGE_MOB = 0x9E;
	static const short DAMAGE_REGULAR = 0x88;
	static const short DAMAGE_MAGIC = 0x94;
	static const short DAMAGE_RANGED = 0x8E;
	static const short BOSS_MOB = 0x54;
	// NPCs
	static const short SHOW_NPC = 0xA8;
	static const short BUY_RESONSE = 0xD8;
	static const short SHOW_SHOP = 0xD7;
	static const short NPC_TALK = 0xC3;
	// Player
	static const short UPDATE_LOOK = 0x93;
	static const short SCROLL_EFFECT = 0x7B;
	static const short ITEM_EFFECT = 0x8F;
	static const short SHOW_CHAIR = 0x92;
	static const short CANCEL_CHAIR = 0x66;
	static const short SHOW_PLAYER_EFFECT = 0x86; // 94
	static const short SHOW_PLAYER_BUFF = 0x3B;
	static const short SHOW_BUFF = 0x87;
	static const short CANCEL_PLAYER_BUFF = 0x24;
	static const short CANCEL_BUFF = 0x8B;
	static const short MAKE_APPLE = 0x5C;
	static const short SHOW_KEYS = 0xF7;
	static const short DAMAGE_PLAYER = 0x8A;
	static const short PLAYER_MOVE = 0x85;
	static const short CHAT_MASSAGE = 0x72;
	static const short SHOW_MASSAGE = 0x2D;
	static const short FACE_EEXPRESSION = 0x8D;
	static const short PLAYER_INFO = 0x2A;
	static const short FIND_PLAYER = 0x5F;
	static const short CHANGE_CHANNEL = 0x03;
	// Quests
	static const short ITEM_GAIN_CHAT = 0x68;
	static const short QUEST_UPDATE = 0x6D;
	static const short COMPLETE_QUEST = 0x1F;
	// Reactors
	static const short SHOW_REACTOR = 0xB3;
	static const short UPDATE_REACTOR = 0xB4;
	static const short DESTROY_REACTOR = 0xB5;

	PacketWriter pw;
public:
	PacketCreator(){
		pw = PacketWriter();
	}
	// Login
	PacketWriter* loginError(short errorid);
	PacketWriter* loginProcess(char type);
	PacketWriter* loginConnect(char gender, string username, bool firstlogin = false);
	PacketWriter* processOk();
	PacketWriter* showWorld(World* world);
	PacketWriter* endWorlds();
	PacketWriter* showChannels();
	PacketWriter* showCharacters(vector <Character*>* chars);
	void showCharacter(Character* character);
	PacketWriter* addCharacter(Character* character);
	PacketWriter* checkName(string name, bool ok);
	PacketWriter* removeCharacter(int characterid);
	PacketWriter* connectChannel(int charid, short port);
	PacketWriter* logBack();
	// Drops
	PacketWriter* dropObject(Drop* drop, char mode,  MapObject* dropper);
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
	PacketWriter* removePet(int playerid);
	PacketWriter* movePet(int playerid, ObjectMoving* moving, Position pos);
	PacketWriter* updatePet(Pet* pet, bool drop = false);
	PacketWriter* petCommandReplay(int playerid, int id, bool success);
	PacketWriter* showPetText(int playerid, string text, int chatBalloon, int act);
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
	PacketWriter* showMoving(int playerid, ObjectMoving* moving);
	PacketWriter* faceExpression(int playerid, int face);
	PacketWriter* showChatMassage(int playerid, string msg, bool gm = false);
	PacketWriter* damagePlayer(int playerid, int skill, int dmg, int obj);
	PacketWriter* showMassage(string msg, char type, int channel = 0, bool server = false);
	PacketWriter* showInfo(Player* player);
	PacketWriter* findPlayerReplay(string name, bool success = true);
	PacketWriter* findPlayerMap(string name, int map, int channel);
	PacketWriter* findPlayerChannel(string name, int map, int channel);
	PacketWriter* whisperPlayer(string name, string msg, int channel);
	PacketWriter* changeChannel(char channelid, short port);
	// Stats
	PacketWriter* gainEXP(int exp, bool chat, bool yellow=false);
	PacketWriter* enableAction();
	PacketWriter* updateStats(Values* stats, bool item=false);
	// Map
	PacketWriter* showPlayer(Player* player);
	PacketWriter* removePlayer(int playerid);
	PacketWriter* changeMap(Player* player);
	PacketWriter* changeSound(string sound);
	PacketWriter* showEffect(string effect);
	PacketWriter* playSound(string sound);
	PacketWriter* mapChange(char mode, string name);
	// Mobs
	PacketWriter* controlMob(Mob* mob, bool agrs = false, bool spawn = false);
	PacketWriter* endControlMob(int mobid);
	PacketWriter* showMob(Mob* mob, bool spawn);
	PacketWriter* moveMob(int mobid, Position position, ObjectMoving* moving, char skill, int skillid);
	PacketWriter* moveMobResponse(Mob* mob, int type, bool agrs);
	PacketWriter* showHP(int mobid, char per);
	PacketWriter* showBossHP(int mobid, int hp, int maxhp, char per, int color, int bgcolor);
	PacketWriter* killMob(int mobid, bool animation = true);
	void damage(int playerid, Damage* damage, int itemid = 0);
	PacketWriter* damageMob(int playerid, Damage* dmg);
	PacketWriter* damageMob(int mobid, int damage);
	PacketWriter* damageMobMagic(int playerid, Damage* dmg);
	PacketWriter* damageMobRanged(int playerid, Damage* dmg, int itemid = 0);
	// NPCs
	PacketWriter* showNPC(NPC* npc);
	PacketWriter* bought();
	PacketWriter* showShop(ShopData* data);
	void npcPacket(int npcid, string text, char type);
	PacketWriter* sendSimple(int npcid, string text);
	PacketWriter* sendYesNo(int npcid, string text);
	PacketWriter* sendBackNext(int npcid, string text, bool back, bool next);
	PacketWriter* sendAcceptDecline(int npcid, string text);
	PacketWriter* sendGetText(int npcid, string text);
	PacketWriter* sendGetNumber(int npcid, string text, int def, int min, int max);
	PacketWriter* sendStyle(int npcid, string text, int styles[], char size);
	// Party
	/*
	PacketWriter* createParty(Player* player);
	PacketWriter* inviteParty(Player* player, Player* in);
	PacketWriter* partyError(Player* player, int error);
	PacketWriter* partyReplay(Player* player, char* to);
	*/
	//	Quests
	PacketWriter* updateQuest(short questid, int npcid, int nextquest=0);
	PacketWriter* updateQuestInfo(Quest* quest, bool forfeit = false);
	PacketWriter* questDone(int questid);
	PacketWriter* itemGainChat(int itemid, int amount);
	PacketWriter* mesosGainChat(int amount);
	// Skills
	PacketWriter* updateSkill(Skill* skill);
	PacketWriter* updateSkill(vector <Skill*>* skills);
	// Reactors
	PacketWriter* showReactor(Reactor* reactor);
	PacketWriter* updateReactor(Reactor* reactor, short stance);
	PacketWriter* destroyReactor(Reactor* reactor);




};

#endif
