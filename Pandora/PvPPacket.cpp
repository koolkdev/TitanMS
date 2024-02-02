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
#include "PacketCreator.h"
#include "ObjectMoving.h"
#include "Damage.h"
#include "PacketWriter.h"
#include "Player.h"
#include "Effect.h"

PacketWriter* PacketCreator::showPvPMoving(Player* player, Position& pos, ObjectMoving& moving){
	pw.writeShort(MOVE_MOB);
	
	pw.writeInt(player->getID());
	pw.write(-1);
	pw.writeInt(0);
	pw.write(0);
	pw.writeShort(pos.x);
	pw.writeShort(pos.y);
	pw.write(moving.getPacket()->getBytes(), moving.getPacket()->getLength());

	return &pw;
}
PacketWriter* PacketCreator::showPvPMovingForPlayer(Player* player, Position& start){
	pw.writeShort(MOVE_MOB);
	
	pw.writeInt(player->getID());
	pw.write(-1);
	pw.writeInt(0);
	pw.write(0);
	pw.writeShort(start.x);
	pw.writeShort(start.y);
	pw.write(1);
	pw.write(3);
	pw.writeShort(player->getPosition().x);
	pw.writeShort(player->getPosition().y);
	pw.writeInt(0);
	pw.write(player->getStance());

	return &pw;
}
PacketWriter* PacketCreator::startPvPMobForPlayer(Player* player){
	pw.writeShort(CONTROL_MOB);

	pw.write(1);
	pw.writeInt(player->getID());
	pw.write(1);
	pw.writeInt(player->getPvPMaskMob());
	pw.writeShort(0);
	pw.write(0);
	pw.write(8);
	pw.writeInt(0);
	pw.writeShort(player->getPosition().x);
	pw.writeShort(player->getPosition().y);
	pw.write(2);
	pw.writeInt(0);
	pw.writeShort(-3);
	pw.writeInt(0);
	pw.writeInt(0);
	return &pw;
}

PacketWriter* PacketCreator::startPvPMob(Player* player){
	pw.writeShort(SPAWN_MOB);

	pw.writeInt(player->getID());
	pw.write(1);
	pw.writeInt(player->getPvPMaskMob());
	pw.writeShort(0);
	pw.write(0);
	pw.write(8);
	pw.writeInt(0);
	pw.writeShort(player->getPosition().x);
	pw.writeShort(player->getPosition().y);
	pw.write(2);
	pw.writeInt(0);
	pw.writeShort(-3);
	pw.writeInt(0);
	pw.writeInt(0);
	return &pw;
}
PacketWriter* PacketCreator::startPvPMobEffect(Player* player, int wdef, int mdef){
	pw.writeShort(MOB_SKILL);

	pw.writeInt(player->getID());

	pw.writeInt(Effect::MOB_WDEF + Effect::MOB_MDEF);
	pw.writeShort(wdef*10);
	pw.writeShort(114);
	pw.writeShort(4);
	pw.writeShort(0);
	pw.writeShort(mdef*10);
	pw.writeShort(114);
	pw.writeShort(4);
	pw.writeShort(0);
	pw.writeShort(0);
	pw.write(1);
	
	return &pw;
}
PacketWriter* PacketCreator::stopPvP(Player* player){
	pw.writeShort(REMOVE_MOB);

	pw.writeInt(player->getID());
	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::stopPvPForPlayer(Player* player){
	pw.writeShort(CONTROL_MOB);

	pw.write(0);
	pw.writeInt(player->getID());

	return &pw;
}
PacketWriter* PacketCreator::updatePvPHP(Player* player){
	pw.writeShort(PARTY_HP);

	pw.writeInt(player->getID());
	pw.writeInt(player->getHP());
	pw.writeInt(player->getMaxHP());

	return &pw;
}