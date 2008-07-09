#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

class PacketReader;
class Player;

class PlayerHandler {
private:
	static const short CONNECT = 0x14;
	static const short PING = 0xA;
	// NPCs
	static const short NPC_CHAT = 0x21;
	static const short HANDLE_NPC = 0x23;
	static const short USE_SHOP = 0x22;
	// Channels
	static const short CHANGE_CHANNEL = 0x27;
	// Player
	static const short TAKE_DAMAGE = 0x2A;
	static const short PLAYER_MOVE = 0x35;
	static const short CHANGE_MAP = 0x2F;
	static const short PLAYER_INFO = 0x44;
	static const short FACE_EXP = 0x5C;
	static const short RECOVERY = 0x67;
	// Mobs
	static const short DAMAGE_MOB_MAGIC = 0x2E;
	static const short DAMAGE_MOB = 0x59;
	static const short DAMAGE_MOB_RANGED = 0x36;
	static const short CONTROL_MOB = 0x9D;
	static const short CONTROL_MOB_SKILL = 0xA0;
	static const short HIT_BY_MOB = 0x9E;
	// Skills
	static const short ADD_SKILL = 0x4D;
	static const short STOP_SKILL = 0x4E;
	static const short USE_SKILL = 0x51;
	// Chat
	static const short USE_CHAT = 0x2C;
	static const short CHAT_COMMAND = 0x58;
	// Inventory
	static const short MOVE_ITEM = 0x62;
	static const short USE_CHAIR = 0x2D;
	static const short CANCEL_CHAIR = 0x2B;
	static const short USE_ITEM_EFFECT = 0x5D;
	static const short USE_CASH_ITEM = 0x53;
	static const short USE_ITEM = 0x63;
	static const short USE_SCROLL = 0x65;
	static const short USE_RETURN_SCROLL = 0x64;
	static const short USE_SUMMON_BUG = 0x4B;
	static const short CANCEL_ITEM_BUFF = 0x49;
	// Pet
	static const short USE_PET = 0x45;
	static const short MOVE_PET = 0x84;
	static const short PET_COMMAND = 0x80; 
	static const short PET_COMMAND_TEXT = 0x82; 
	// Stats
	static const short ADD_STAT = 0x66;
	// Drops
	static const short DROP_MESOS = 0x68;
	static const short LOOT_DROP = 0x89;
	// Quests
	static const short QUEST_HANDLE = 0x6B;
	// Keys
	static const short CHANGE_KEY = 0x75;
	// Reactors
	static const short HIT_REACTOR = 0x8C; 

	Player* player;
public:
	PlayerHandler(Player* player){
		this->player = player;
	}
	void handle(PacketReader* packet);

	void connectionRequestHandle(PacketReader* packet);
	void pingHandle(PacketReader* packet);
	// NPCs
	void NPCHandle(PacketReader* packet);
	void NPCChatHandle(PacketReader* packet);
	// TODO: controlled npcs
	void useShopHandle(PacketReader* packet);
	// Channels - TODO
	void changeChannelHandle(PacketReader* packet);
	// Player
	void playerTakeDamageHandle(PacketReader* packet);
	void playerMovingHandle(PacketReader* packet);
	void changeMapHandle(PacketReader* packet);
	void getPlayerInformationHandle(PacketReader* packet);
	void useFaceExpressionHandle(PacketReader* packet);
	void recoveryHealthHandle(PacketReader* packet);
	// Mobs
	void damageMobMagicHandle(PacketReader* packet);
	void damageMobHandle(PacketReader* packet);
	void damageMobRangedHandle(PacketReader* packet);
	void controlMobHandle(PacketReader* packet);
	void controlMobSkillHandle(PacketReader* packet); //?
	void hitByMobHandle(PacketReader* packet);
	// Skills
	void addSkillHandle(PacketReader* packet);
	void stopSkillHandle(PacketReader* packet);
	void useSkillHandle(PacketReader* packet);
	// Chat
	void chatCommandHandle(PacketReader* packet);
	void useChatHandle(PacketReader* packet);
	// Inventory
	void cancelChairHandle(PacketReader* packet);
	void useChairHandle(PacketReader* packet);
	void useItemEffectHandle(PacketReader* packet);
	void useCashItemHandle(PacketReader* packet);
	void moveItemHandle(PacketReader* packet);
	void useItemHandle(PacketReader* packet);
	void useReturnScrollHandle(PacketReader* packet);
	void useScrollHandle(PacketReader* packet);
	void useSummonBugHandle(PacketReader* packet);
	void cancelItemBuffHandle(PacketReader* packet);
	// Pet
	void usePetHandle(PacketReader* packet);
	void movePetHandle(PacketReader* packet);
	void petCommandHandle(PacketReader* packet);
	void petCommandTextHandle(PacketReader* packet);
	//Stats
	void addStatHandle(PacketReader* packet);
	// Drops
	void dropMesosHandle(PacketReader* packet);
	void lootDropHandle(PacketReader* packet);
	// Quests
	void questHandle(PacketReader* packet);
	// Keys
	void changeKeyHandle(PacketReader* packet);
	// Reactor
	void hitReactorHandle(PacketReader* packet);
};

#endif