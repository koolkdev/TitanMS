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
#include "Map.h"
#include "MapReactors.h"
#include "Reactor.h"
#include "Player.h"

void PlayerHandler::hitReactorHandle(PacketReader& packet){
	int reactorid = packet.readInt();
	int pos = packet.readInt();
	short stance = packet.readShort();

	Reactor* reactor = player->getMap()->getReactor(reactorid);
	if(reactor == NULL)
		return;
	player->getMap()->getReactors()->hitReactor(reactor, player, stance, pos);
}
