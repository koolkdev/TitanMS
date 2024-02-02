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
#include "SkillData.h"
#include "SkillLevelData.h"
#include "PlayerSkills.h"
#include "Effect.h"
#include "Skill.h"
#include <vector>
using namespace std;

// TODO: use skill

void PlayerHandler::damageMobMagicHandle(PacketReader& packet){
	Damage damage = Damage(packet);
	player->getMap()->send(PacketCreator().damageMobMagic(player, damage), player);
	player->getMap()->getMobs()->damage(player, damage);
}
void PlayerHandler::damageMobHandle(PacketReader& packet){
	Damage damage = Damage(packet);
	player->getMap()->send(PacketCreator().damageMob(player, damage), player);
	player->getMap()->getMobs()->damage(player, damage);
}
void PlayerHandler::damageMobRangedHandle(PacketReader& packet){
	Damage damage = Damage(packet, true);
	int itemid = 0;
	Item* item = NULL;
	short slot = damage.getItemSlot();
	if(slot > 0)
	{
		item = player->getInventories()->getItemBySlot(USE, slot);
		Item* touse = item;
		if(item == NULL || item->getAmount() <= 0)
			return;
		itemid = item->getID();
		// TODO: 
		if(damage.getCashSlot() > 0)
		{
			Item* titem = player->getInventories()->getInventory(CASH)->getItemBySlot(damage.getCashSlot());
			if(titem != NULL && MAJOR_TYPE(titem->getID()) == 502)
				itemid = titem->getID();
		}
		if(!item->isBullet()){
			return;
		}
	}
	if(damage.getSkill())
	{
		Skill* skill = player->getSkills()->getSkill(damage.getSkill());
		if(skill == NULL)
			return;
		if(skill->getLevel() < 1)
			return;
		Effect* effect = Effect::getEffect(damage.getSkill(), skill->getLevel());
		if(effect != NULL)
			if(!effect->use(player)) return;
	}
	player->getMap()->send(PacketCreator().damageMobRanged(player, damage, itemid), player);
	// TODO fix using items
	player->getMap()->getMobs()->damage(player, damage, item);
}
void PlayerHandler::controlMobHandle(PacketReader& packet){
	int mobid = packet.readInt();
	Mob* mob = player->getMap()->getMob(mobid);
	if(mob == NULL)
		return;
	short count =  packet.readShort();
	char skillType = packet.read();
	char skill = packet.readInt();
	packet.read(6);
	Position pos;
	pos.x = packet.readShort();
	pos.y = packet.readShort();
	ObjectMoving moving = ObjectMoving(packet, mob);
	if(player != mob->getControl())
		mob->setControl(player, false, true);
	else if(skill == -1 && mob->getAggressive() == 2)
		mob->setAggressive(0);

	player->send(PacketCreator().moveMobResponse(mob, count, (mob->getAggressive() != 0)));
	player->getMap()->send(PacketCreator().moveMob(mob->getID(), pos, moving, skillType, skill), player, mob->getPosition());
	if(mob->getAggressive() == 1)
		mob->setAggressive(2);

}
void PlayerHandler::controlMobSkillHandle(PacketReader& packet){
	// Not sure..
}
void PlayerHandler::hitByMobHandle(PacketReader& packet){
	// TODO get packets (henesysPQ)
	packet.show();
	int byid = packet.readInt();
	int playerid = packet.readInt();
	int mobid = packet.readInt();
	Mob* by = player->getMap()->getMob(byid);
	Mob* mob = player->getMap()->getMob(mobid);
	if(by == NULL || !DataProvider::getInstance()->getMobDamagedByMob(mob->getMobID()) || mob == NULL || playerid != player->getID())
		return;
	player->getMap()->getMobs()->damage(NULL, mob, (DataProvider::getInstance()->getMobLevel(by->getMobID())/(Tools::random(40,45)) + 1)*DataProvider::getInstance()->getMobWAtk(by->getMobID())); //?
	//if(mob->getID()//if mob can be hitted by a mob TODO
}