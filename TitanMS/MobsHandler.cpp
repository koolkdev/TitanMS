#include "PlayerHandler.h"
#include "Player.h"
#include "Mob.h"
#include "Map.h"
#include "PacketCreator.h"
#include "PacketReader.h"
#include "ObjectMoving.h"
#include "PlayerInventories.h"
#include "Inventory.h"
#include "Item.h"
#include "Damage.h"
#include "Map.h"
#include "MapMobs.h"
#include <vector>
using namespace std;

void PlayerHandler::damageMobMagicHandle(PacketReader* packet){
	Damage damage = Damage(packet);
	player->getMap()->send(PacketCreator().damageMobMagic(player->getID(), &damage), player);
	player->getMap()->getMobs()->damage(player, &damage);
}
void PlayerHandler::damageMobHandle(PacketReader* packet){
	Damage damage = Damage(packet);
	player->getMap()->send(PacketCreator().damageMob(player->getID(), &damage), player);
	player->getMap()->getMobs()->damage(player, &damage);
}
void PlayerHandler::damageMobRangedHandle(PacketReader* packet){
	Damage damage = Damage(packet, true);
	short slot = damage.getItemSlot();
	Item* item = player->getInventories()->getItemBySlot(USE, slot);
	Item* touse = item;
	if(item == NULL)
		return;
	int itemid = 0;
	Item* weapon = player->getInventories()->getInventory(EQUIPPED)->getItemByType(147);
	if(weapon != NULL){
		Item* titem = player->getInventories()->getInventory(USE)->getItemByType(502);
		if(titem != NULL)
			item = titem;
	}
	itemid = item->getID();
	if(!item->isBullet()){
		return;
	}
	player->getMap()->send(PacketCreator().damageMobRanged(player->getID(), &damage, itemid), player);
	player->getMap()->getMobs()->damage(player, &damage, item);
}
void PlayerHandler::controlMobHandle(PacketReader* packet){
	int mobid = packet->readInt();
	Mob* mob = player->getMap()->getMob(mobid);
	if(mob == NULL)
		return;
	short type =  packet->readShort();
	char skillType = packet->read();
	int skill = packet->readInt();
	packet->read(6);
	Position pos;
	pos.x = packet->readShort();
	pos.y = packet->readShort();
	ObjectMoving moving = ObjectMoving(packet);
	if(player != mob->getControl())
		mob->setControl(player, false, true);
	else if(skill == 255 && mob->getAggressive() == 2)
		mob->setAggressive(0);
	player->send(PacketCreator().moveMobResponse(mob, type, (mob->getAggressive() != 0)));
	if(moving.isError())
		return;
	mob->setPosition(moving.getPosition());
	mob->setStance(moving.getStance());
	player->getMap()->send(PacketCreator().moveMob(mob->getID(), pos, &moving, skillType, skill), player, mob->getPosition());


}
void PlayerHandler::controlMobSkillHandle(PacketReader* packet){
	// Not sure..
}
void PlayerHandler::hitByMobHandle(PacketReader* packet){
	// TODO get packets (henesysPQ)
	int byid = packet->readInt();
	int mobid = packet->readInt();
	Mob* by = player->getMap()->getMob(byid);
	Mob* mob = player->getMap()->getMob(mobid);
	if(by == NULL || mob == NULL)
		return;
	//if(mob->getID()//if mob can be hitted by a mob TODO
}