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
#include "PacketWriter.h"
#include "Drop.h"
#include "Tools.h"
#include "PlayerInventories.h"
#include "Item.h"

PacketWriter* PacketCreator::dropObject(Drop* drop, char mode, MapObject* dropper, int delay){
	pw.writeShort(SHOW_DROP);

	pw.write(mode);
	pw.writeInt(drop->getID());
	pw.write(drop->isMesos());
	if(drop->isMesos())
		pw.writeInt(drop->getAmount());
	else
		pw.writeInt(drop->getItem()->getID());
	pw.writeInt(drop->getOwner());
	pw.write(0);
	pw.writeShort(drop->getPosition().x);
	pw.writeShort(drop->getPosition().y);
	if(mode != 2){
		pw.writeInt((drop->getOwner() == 0) ? 0 : dropper->getID());
		pw.writeShort(dropper->getPosition().x);
		pw.writeShort(dropper->getPosition().y);
		pw.writeShort(delay);
	}
	else
		pw.writeInt(0);
	pw.write(drop->getMesos());
	if(!drop->isMesos()){
		pw.writeBytes("8005BB46E617");
		pw.write(2);
		pw.write(0);
	}
	
	return &pw;
}
PacketWriter* PacketCreator::showDrop(Drop* drop){
	return dropObject(drop, 2, NULL);
}
PacketWriter* PacketCreator::gainDrop(int itemid, int amount, bool mesos){

	pw.writeShort(SHOW_GAIN);
	pw.write(0);
	if(itemid == 0){
		pw.write(-1);
	}
	else{
		pw.write(mesos);
		pw.writeInt(itemid);
		if(mesos)
			pw.writeShort(0); // Internet Cafe Bonus
		else if(INVENTORY(itemid) != EQUIP)
			pw.writeShort(amount);
	}
	if(!mesos){
		pw.writeLong(0);
	}
	return &pw;

}
PacketWriter* PacketCreator::gainMesos(int mesos){
	return gainDrop(mesos, 0, true); 
}

PacketWriter* PacketCreator::lootError(){
	return gainDrop(0, 0, 0); 
}

PacketWriter* PacketCreator::removeDrop(int dropid, char type, int id){

	pw.writeShort(REMOVE_DROP);
	pw.write(type);
	pw.writeInt(dropid);
	switch(type){
		case 2: pw.writeInt(id); break;
		case 4: pw.writeShort(0); break;
	}
	return &pw;
}

PacketWriter* PacketCreator::explodeDrop(int dropid){
	return removeDrop(dropid, 4);
}
PacketWriter* PacketCreator::lootDrop(int dropid, int playerid){
	return removeDrop(dropid, 2, playerid);
}