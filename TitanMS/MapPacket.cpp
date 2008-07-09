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
#include "Channel.h"
#include "PlayerBuffs.h"
#include "Effect.h"
#include "Map.h"
#include "Pet.h"
#include "Player.h"


PacketWriter* PacketCreator::showPlayer(Player* player){
	pw.writeShort(SHOW_PLAYER);

	pw.writeInt(player->getID());
	pw.writeString(player->getName());

	// blablabla get guild info
	pw.writeString(""); // guild name
	pw.writeShort(0); // guild symbol background
	pw.write(0); // guild symbol background color
	pw.writeShort(0); // guild symbol
	pw.write(0); // guild symbol color

	__int64 var = 0;
	
	if(player->getBuffs()->isBuffActive(Effect::DARK_SIGHT)){
		var |= Effect::DARK_SIGHT;
	}
	if(player->getBuffs()->isBuffActive(Effect::POWER_GUARD)){
		var |= Effect::POWER_GUARD;
	}
	if(player->getBuffs()->isBuffActive(Effect::SHADOW_PARTNER)){
		var |= Effect::SHADOW_PARTNER;
	}
	if(player->getBuffs()->isBuffActive(Effect::COMBO)){
		var |= Effect::COMBO;
	}
	if(player->getBuffs()->isBuffActive(Effect::MONSTER_RIDING)){
		var |= Effect::MONSTER_RIDING;
	}
	if(player->getBuffs()->isBuffActive(Effect::MORPH)){
		var |= Effect::MORPH;
	}
	pw.writeLong(var);
	if(player->getBuffs()->isBuffActive(Effect::COMBO)){
		pw.write(1); // TODO
	}
	if(player->getBuffs()->isBuffActive(Effect::MORPH)){
		pw.writeShort(player->getBuffs()->getBuffValue(Effect::MORPH));
	}

	playerShow(player);

	pw.writeInt(0);
	pw.writeInt(player->getItemEffect());
	pw.writeInt(player->getChair());
	pw.writeShort(player->getPosition().x);
	pw.writeShort(player->getPosition().y);
	pw.write(player->getStance());
	
	pw.write(0); // unl
	pw.writeShort(0);
	if(player->getPet() != NULL){
		pw.write(1);
		Pet* pet = player->getPet();
		pw.writeInt(pet->getItemID());
		pw.writeString(pet->getName());
		pw.writeInt(pet->getObjectID());
		pw.writeInt(0);
		pw.writeShort(pet->getPosition().x);
		pw.writeShort(pet->getPosition().y);
		pw.write(pet->getStance());
		pw.writeInt(0); // unk
		pw.write(0);
	}
	else{
		pw.write(0);
	}
	pw.writeInt(1);

	pw.writeLong(0);

	// TODO a box top of the player
	pw.write(0);

	pw.writeInt(0);
	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::removePlayer(int playerid){
	pw.writeShort(REMOVE_PLAYER);

	pw.writeInt(playerid);

	return &pw;
}
PacketWriter* PacketCreator::changeMap(Player* player){
	pw.writeShort(CHANGE_MAP);

	pw.writeInt(player->getChannel()->getID());
	pw.writeShort(0); // 2?
	pw.writeInt(player->getMap()->getID());
	pw.write(player->getMappos());
	pw.writeShort(player->getHP());
	pw.write(0);
	pw.writeInt(-1);
	pw.writeShort(-1);
	pw.write(-1);
	pw.write(1);

	return &pw;
}
PacketWriter* PacketCreator::changeSound(string sound){
	return mapChange(6, sound);
}
PacketWriter* PacketCreator::showEffect(string effect){
	return mapChange(3, effect);
}
PacketWriter* PacketCreator::playSound(string sound){
	return mapChange(4, sound);
}
PacketWriter* PacketCreator::mapChange(char mode, string name){
	pw.writeShort(MAP_CHANGE);

	pw.write(mode);
	pw.writeString(name);

	return &pw;
}