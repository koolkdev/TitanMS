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
#include "PacketCreator.h"
#include "Skill.h"
#include "PlayerSkills.h"
#include "Tools.h"
#include "Player.h"
#include "SkillLevelData.h"
#include "SkillsData.h"
#include "SkillData.h"
using namespace Tools;

void PlayerHandler::addStatHandle(PacketReader& packet){
	// TODO add check that is less then 999
	packet.readInt();
	int type = packet.readInt();
	if(player->getAP() <= 0)
		return;
	Values values;
	player->addAP(-1, false);
	values.add(Value(Player::Update::AP, player->getAP()));
	values.add(player->addStat(type, true));
	player->send(PacketCreator().updateStats(values, true));

}