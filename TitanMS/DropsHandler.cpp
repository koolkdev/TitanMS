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
#include "PacketReader.h"
#include "PacketWriter.h"
#include "Player.h"
#include "Drop.h"
#include "Map.h"
#include "MapDrops.h"
#include "PacketCreator.h"

void PlayerHandler::dropMesosHandle(PacketReader& packet){
	packet.readInt();
	int amount = packet.readInt();
	if(amount < 10)
		return;
	player->addMesos(-amount, true, true);
	player->getMap()->getDrops()->dropMesosFromObject(amount, player->getPosition(), player, 0);
}
void PlayerHandler::lootDropHandle(PacketReader& packet){
	packet.read(9);
	int dropid = packet.readInt();
	Drop* drop = player->getMap()->getDrop(dropid);
	if(drop != NULL)
		player->getMap()->getDrops()->lootDrop(player, drop);
	else{
		player->send(PacketCreator().emptyInventoryAction());
		player->send(PacketCreator().lootError());
	}
}