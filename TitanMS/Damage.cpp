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
#include "Damage.h"
#include "PacketReader.h"

Damage::Damage(PacketReader& packet, bool range){
	packet.read();
	info = packet.read();
	mobCount = info/0x10;
	attackCount = info%0x10;
	skill = packet.readInt();
	packet.read();
	charge = 0;
	if (skill == 2121001 || skill == 2221001 || skill == 2321001)
		charge = packet.readInt();
	stance = packet.read();
	packet.read();
	speed = packet.read();

	packet.readInt();

	// Mesos Explosion - TODO ( or after stance?)
	mesoexplosion = (skill == 4211006);
	if(mesoexplosion)
		return;

	if(skill == 5201002)
		packet.readInt();

	if(range){
		itemslot = packet.readInt();
		packet.read();
	}
	else itemslot = 0;
	for(int i=0; i<mobCount; i++){
		int mobid = packet.readInt();
		packet.read(14);
		mobs.push_back(mobid);
		vector<int> damage;
		for(int j=0; j<attackCount; j++){
			damage.push_back(packet.readInt());
		}
		damages[mobid] = damage;
		packet.readInt();
	}
}