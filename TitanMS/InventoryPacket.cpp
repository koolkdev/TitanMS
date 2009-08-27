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
#include "PlayerInventories.h"
#include "DataProvider.h"
#include "Item.h"
#include "Pet.h"
#include "ObjectMoving.h"
#include "Equip.h"
#include "Tools.h"
using namespace Tools;

PacketWriter* PacketCreator::emptyInventoryAction(){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(1);
	pw.write(0);
	return &pw;
}

void PacketCreator::itemInfo(Item* item, bool showPosition, bool showZeroPosition){
	char slot = item->getSlot();
	bool cash = false;
	if(showPosition){
		if(slot < 0){ // EQUIPED
			if(slot < -100){
				//cash = true;
				//pw.write(0);
				pw.write(-(slot+100));
			}
			else{
				pw.write(-slot);
			}
		}
		else{
			pw.write(slot);
		}
	}
	else if(showZeroPosition){
		pw.write(0);		
	}
	pw.write(item->getType() == EQUIP ? 1 : (IS_PET(item->getID()) ? 3 : 2)); // 1 for equips and 2 for items 3 for pets
	pw.writeInt(item->getID());
	if(IS_PET(item->getID())){
		pw.write(1); 
		pw.writeLong(((MapObject*)((Pet*)item))->getID()); 
		pw.write(0);
		pw.writeBytes("8005"); // Cash?

	}
	else if(cash){
		pw.write(1); 
		pw.writeLong(getTimeByDays(90)); // Time?
		pw.writeBytes("80206F"); // Cash?
	}
	else{
		pw.writeShort(0);
		pw.writeBytes("8005");
	}

	pw.writeBytes("BB46E617");
	pw.write(2);

	if(item->getType() == EQUIP){
		Equip* equip = (Equip*)item;
		pw.write(equip->getSlots());
		pw.write(equip->getScrolls());
		pw.writeShort(equip->getStr());
		pw.writeShort(equip->getDex());
		pw.writeShort(equip->getInt());
		pw.writeShort(equip->getLuk());
		pw.writeShort(equip->getHP());
		pw.writeShort(equip->getMP());
		pw.writeShort(equip->getWAtk());
		pw.writeShort(equip->getMAtk());
		pw.writeShort(equip->getWDef());
		pw.writeShort(equip->getMDef());
		pw.writeShort(equip->getAcc());
		pw.writeShort(equip->getAvo());
		pw.writeShort(equip->getHand());
		pw.writeShort(equip->getSpeed());
		pw.writeShort(equip->getJump());
		pw.writeString(equip->getOwner());
		pw.write(equip->getLocked());
		if(!cash){
			pw.write(0);
			pw.writeLong(0);	
		}
	}
	else if(IS_PET(item->getID())){
		Pet* pet = (Pet*)item;
		pw.writeString(pet->getName(), 13);
		pw.write(pet->getLevel());
		pw.writeShort(pet->getCloseness());
		pw.write(pet->getFullness());
		pw.writeLong(pet->getTime()); 
		pw.writeInt(1); // Or 0?
	}
	else{
		pw.writeShort(item->getAmount());
		pw.writeString(""); // owner, but items don't have owner
		pw.writeShort(item->getLocked());
		if(IS_STAR(item->getID())){
			pw.writeInt(2);
			pw.writeShort(0x54);
			pw.write(0);
			pw.write(0x34);
		}
	}
}

PacketWriter* PacketCreator::moveItem(char inv, short slot1, short slot2, char equip){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(1);
	pw.write(1);
	pw.write(2);
	pw.write(inv);
	pw.writeShort(slot1);
	pw.writeShort(slot2);
	if(equip)
		pw.write(equip);
	return &pw;
}
PacketWriter* PacketCreator::updatePet(Pet* pet, bool drop){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(drop);
	pw.write(2);
	pw.write(3);
	pw.write(pet->getType());
	pw.writeShort(pet->getSlot());
	pw.write(0);
	pw.write(pet->getType());
	pw.writeShort(pet->getSlot());
	itemInfo(pet, false);

	return &pw;
}
PacketWriter* PacketCreator::updateSlot(Item* item, bool drop){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(drop);
	pw.write(1);
	pw.write(1);
	pw.write(item->getType());
	pw.writeShort(item->getSlot());
	pw.writeShort(item->getAmount());
	return &pw;
}/*
PacketWriter* PacketCreator::updateItem(Item* item, bool drop){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(drop);
	pw.write(1);
	pw.write(2);
	pw.write(item->getType());
	pw.writeShort(item->getSlot());
	itemInfo(item, true);
	return &pw;
}*/
PacketWriter* PacketCreator::newItem(Item* item, bool drop){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(drop);
	pw.write(1);
	pw.write(0);
	pw.write(item->getType());
	pw.writeShort(item->getSlot());
	itemInfo(item, false);
	return &pw;
}
PacketWriter* PacketCreator::removeItem(char inv, int slot, bool drop){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(drop);
	pw.write(1);
	pw.write(3);
	pw.write(inv);
	pw.writeShort(slot);
	if(slot < 0)
		pw.write(1);
	return &pw;
}
PacketWriter* PacketCreator::moveItemMerge(char inv, short slot1, short slot2, short amount){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(1);
	pw.write(2);
	pw.write(3);
	pw.write(inv);
	pw.writeShort(slot1);
	pw.write(1);
	pw.write(inv);
	pw.writeShort(slot2);
	pw.writeShort(amount);
	return &pw;
}
PacketWriter* PacketCreator::moveItemMergeTwo(char inv, short slot1, short amount1, short slot2, short amount2){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(1);
	pw.write(2);
	pw.write(1);
	pw.write(inv);
	pw.writeShort(slot1);
	pw.writeShort(amount1);
	pw.write(1);
	pw.write(inv);
	pw.writeShort(slot2);
	pw.writeShort(amount2);
	return &pw;
}
PacketWriter* PacketCreator::scrollItem(Item* scroll, Item* equip, bool destroyed){
	pw.writeShort(INVENTORY_CHANGE);

	pw.write(1);
	pw.write((destroyed) ? 2 : 3);
	pw.write((scroll->getAmount() > 1) ? 1 : 3);
	pw.write(USE);
	pw.writeShort(scroll->getSlot());
	if(scroll->getAmount() > 1)
		pw.writeShort(scroll->getAmount()-1);
	pw.write(3);
	if(!destroyed){
		pw.write(EQUIP);
		pw.writeShort(equip->getSlot());
		pw.write(0);
	}
	pw.write(EQUIP);
	pw.writeShort(equip->getSlot());
	if(!destroyed)
		itemInfo(equip, false);
	pw.write(1);

	return &pw;
}

PacketWriter* PacketCreator::showPet(int playerid, Pet* pet){
	pw.writeShort(USE_PET);
	// 7E 00 C4 E6 1F 00 00 01 01 67 4B 4C 00 09 00 50 6F 72 63 75 70 69 6E 65 3B FC 33 00 00 00 00 00 0F F3 5E 00 00 EE 00 00 00
	pw.writeInt(playerid);
	pw.write(pet->getPetSlot());
	pw.write(1); //
	pw.write(1); // 

	pw.writeInt(pet->getItemID());
	pw.writeString(pet->getName());
	pw.writeInt(pet->getObjectID());
	pw.writeInt(0);
	pw.writeShort(pet->getPosition().x);
	pw.writeShort(pet->getPosition().y);
	pw.write(pet->getStance());
	pw.writeInt(pet->getFoothold()); 

	return &pw;
}
PacketWriter* PacketCreator::removePet(int playerid, int petSlot, bool die){
	pw.writeShort(USE_PET);

	pw.writeInt(playerid);
	pw.write(petSlot);
	pw.write(0);
	pw.write(die);

	return &pw;
}

PacketWriter* PacketCreator::petCommandReplay(int playerid, int petSlot, int id, bool success){
	pw.writeShort(PET_COMMAND_REPLAY);

	pw.writeInt(playerid);
	pw.writeShort(petSlot);
	pw.write(id);
	pw.writeShort(success);

	return &pw;

}

PacketWriter* PacketCreator::movePet(int playerid, int petSlot, ObjectMoving& moving, Position& pos){
	pw.writeShort(PET_MOVE);
	
	pw.writeInt(playerid);
	pw.write(petSlot);
	pw.writeShort(pos.x);
	pw.writeShort(pos.y);
	pw.write(moving.getPacket()->getBytes(), moving.getPacket()->getLength());

	return &pw;

}

PacketWriter* PacketCreator::showPetText(int playerid, int petSlot, string text, int act){
	pw.writeShort(PET_TEXT);
	//81 00 C1 57 43 00 00 00 0F 0F 00 49 20 66 65 65 6C 20 73 70 65 63 69 61 6C 21 00
	pw.writeInt(playerid);
	pw.writeShort(petSlot);
	pw.write(act);
	pw.writeString(text);
	pw.write(0);

	return &pw;

}
