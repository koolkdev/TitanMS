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
#include "PacketCreator.h"
#include "ObjectMoving.h"
#include "Damage.h"
#include "PacketWriter.h"
#include "Mob.h"
#include "Tools.h"
#include "PlayerInventories.h"
#include "Inventory.h"
#include "Item.h"


PacketWriter* PacketCreator::controlMob(Mob* mob, char type, bool agrs, int from){
	pw.writeShort(CONTROL_MOB);

	pw.write((agrs)? 2 : 1);
	pw.writeInt(mob->getID());
	pw.write(1);
	pw.writeInt(mob->getMobID());
	pw.write(0);
	pw.writeShort(0);
	pw.write(8);
	pw.writeInt(0);
	pw.writeShort(mob->getPosition().x);
	pw.writeShort(mob->getPosition().y);
	pw.write(mob->getStance());
	pw.writeShort(mob->getFoothold()); // fh
	pw.writeShort(mob->getStartFH());//?
	pw.write(type);
	if(from != 0)
		pw.writeInt(from);
	pw.write(-1); // spawn effect(+ pw.writeInt(delay))
	pw.writeInt(0);

	return &pw;
}
PacketWriter* PacketCreator::endControlMob(int mobid){
	pw.writeShort(CONTROL_MOB);

	pw.write(0);
	pw.writeInt(mobid);	
	return &pw;
}
PacketWriter* PacketCreator::showMob(Mob* mob, char type, int from){
	pw.writeShort(SPAWN_MOB);

	pw.writeInt(mob->getID());
	pw.write(1);
	pw.writeInt(mob->getMobID());
	pw.write(0);
	pw.writeShort(0);
	pw.write(8);
	pw.writeInt(0);
	pw.writeShort(mob->getPosition().x);
	pw.writeShort(mob->getPosition().y);
	pw.write(mob->getStance());
	pw.writeShort(mob->getFoothold()); //fh
	pw.writeShort(mob->getStartFH()); // original FH?
	pw.write(type);
	if(from != 0)
		pw.writeInt(from);
	pw.write(-1); // spawn effect(+ pw.writeInt(delay))

	pw.writeInt(0);

	return &pw;
}
PacketWriter* PacketCreator::moveMob(int mobid, Position& pos, ObjectMoving& moving, char skill, int skillid){
	pw.writeShort(MOVE_MOB);
	
	pw.writeInt(mobid);
	pw.write(skill);
	pw.writeInt(skillid);
	pw.write(0);
	pw.writeShort(pos.x);
	pw.writeShort(pos.y);
	pw.write(moving.getPacket()->getBytes(), moving.getPacket()->getLength());

	return &pw;
}
PacketWriter* PacketCreator::moveMobResponse(Mob* mob, int count, bool agrs){
	pw.writeShort(MOVE_MOB_RESPONSE);

	pw.writeInt(mob->getID());
	pw.writeShort(count);
	pw.write(agrs);
	pw.writeInt(mob->getMP());

	return &pw;
}
PacketWriter* PacketCreator::showHP(int mobid, char per){
	pw.writeShort(SHOW_MOB_HP);
	
	pw.writeInt(mobid);
	pw.write(per);

	return &pw;
}
PacketWriter* PacketCreator::showBossHP(int mobid, int hp, int maxhp, char per, int color, int bgcolor){
	pw.writeShort(MAP_EFFECT);
	
	pw.write(5);
	pw.writeInt(mobid);
	pw.writeInt(hp);
	pw.writeInt(maxhp);
	pw.write(color);
	pw.write(bgcolor);

	return &pw;
}
PacketWriter* PacketCreator::killMob(int mobid, bool animation){
	pw.writeShort(REMOVE_MOB);

	pw.writeInt(mobid);
	pw.write(animation);

	return &pw;

}
void PacketCreator::damage(int playerid, Damage& damage, int itemid){
	pw.writeInt(playerid);
	pw.write(damage.getInfoByte());
	int skill = damage.getSkill();
	if(skill){
		pw.write(-1);
		pw.writeInt(skill);
	}
	else
		pw.write(0);
	pw.write(0);
	pw.write(damage.getStance());
	pw.write(damage.getSpeed());
	pw.write(10);
	pw.writeInt(itemid);
	vector <int>* mobs = damage.getMobs();

	for(int i=0; i<(int)mobs->size(); i++){
		pw.writeInt((*mobs)[i]);
		pw.write(-1);
		
		vector<int>* damages = damage.getDamageForMob((*mobs)[i]);

		for(int j=0; j<(int)damages->size(); j++){
			pw.writeInt((*damages)[j]);
		}
	}
	if(damage.getCharge() > 0)
		pw.writeInt(damage.getCharge());
}
PacketWriter* PacketCreator::damageMob(int playerid, Damage& dmg){
	pw.writeShort(DAMAGE_REGULAR);

	damage(playerid, dmg);

	return &pw;

}
PacketWriter* PacketCreator::damageMob(int mobid, int damage){
	pw.writeShort(DAMAGE_MOB);

	pw.writeInt(mobid);
	pw.write(0);
	pw.writeInt(damage);

	return &pw;

}
PacketWriter* PacketCreator::damageMobMagic(int playerid, Damage& dmg){
	pw.writeShort(DAMAGE_MAGIC);

	damage(playerid, dmg);

	return &pw;

}
PacketWriter* PacketCreator::damageMobRanged(int playerid, Damage& dmg, int itemid){
	pw.writeShort(DAMAGE_RANGED);

	damage(playerid, dmg, itemid);

	return &pw;
}


PacketWriter* PacketCreator::damageByMob(int mobid, int damage, int hp, int maxhp){
	pw.writeShort(DAMAGE_BY_MOB);
	
	pw.writeInt(mobid);
	pw.write(1);
	pw.writeInt(damage);
	pw.writeInt(hp);
	pw.writeInt(maxhp);

	return &pw;
}