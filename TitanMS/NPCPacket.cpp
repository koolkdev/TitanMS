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
#include "PlayerNPC.h"
#include "PacketWriter.h"
#include "PlayerInventories.h"
#include "NPC.h"
#include "ShopData.h"
#include "ShopItemData.h"
#include "DataProvider.h"
#include "Tools.h"

using namespace Tools;

PacketWriter* PacketCreator::showNPC(NPC* npc){
	pw.writeShort(SHOW_NPC);

	pw.writeInt(npc->getID());
	pw.writeInt(npc->getNPCID());
	pw.writeShort(npc->getPosition().x);
	pw.writeShort(npc->getPosition().y);
	pw.write(!npc->getFlip());
	pw.writeShort(npc->getFoothold());
	pw.writeShort(npc->getRX0());
	pw.writeShort(npc->getRX1());
	pw.write(1);

	return &pw;
}
PacketWriter* PacketCreator::bought(){
	pw.writeShort(BUY_RESONSE);

	pw.write(0);

	return &pw;	
}
PacketWriter* PacketCreator::showNPCShop(ShopData* data){
	pw.writeShort(SHOW_SHOP);

	pw.writeInt(data->getNPC());
	pw.writeShort(data->getCount());
	vector <ShopItemData*>* items = data->getData();
	for(int i=0; i<(int)items->size(); i++){
		ShopItemData* item = (*items)[i];
		pw.writeInt(item->getID());
		pw.writeInt(item->getPrice());
		if(IS_STAR(item->getID())){
			pw.writeLong(doubleAsLong(DataProvider::getInstance()->getItemUnitPrice(item->getID())));
		}
		else{
			pw.writeShort(1);
		}
		pw.writeShort(DataProvider::getInstance()->getItemMaxPerSlot(item->getID()));
	}
	return &pw;

}
void PacketCreator::npcPacket(int npcid, string text, char type){
	pw.writeShort(NPC_TALK);
	
	pw.write(4);
	pw.writeInt(npcid);
	pw.write(type);
	pw.writeString(text);
}
PacketWriter* PacketCreator::sendSimple(int npcid, string text){
	npcPacket(npcid, text, PlayerNPC::SIMPLE);
	return &pw;
}
PacketWriter* PacketCreator::sendYesNo(int npcid, string text){
	npcPacket(npcid, text, PlayerNPC::YES_NO);
	return &pw;
}
PacketWriter* PacketCreator::sendBackNext(int npcid, string text, bool back, bool next){
	npcPacket(npcid, text, PlayerNPC::BACK_NEXT);
	pw.write(back);
	pw.write(next);
	return &pw;
}
PacketWriter* PacketCreator::sendAcceptDecline(int npcid, string text){
	npcPacket(npcid, text, PlayerNPC::ACCEPT_DECLINE);
	return &pw;
}
PacketWriter* PacketCreator::sendGetText(int npcid, string text){
	npcPacket(npcid, text, PlayerNPC::GET_TEXT);
	pw.writeInt(0);
	pw.writeInt(0);
	return &pw;
}
PacketWriter* PacketCreator::sendGetNumber(int npcid, string text, int def, int min, int max){
	npcPacket(npcid, text, PlayerNPC::GET_NUMBER);
	pw.writeInt(def);
	pw.writeInt(min);
	pw.writeInt(max);
	pw.writeInt(0);
	return &pw;
}
PacketWriter* PacketCreator::sendStyle(int npcid, string text, int styles[], char size){
	npcPacket(npcid, text, PlayerNPC::STYLE);
	pw.write(size);
	for(int i=0; i<size; i++)
		pw.writeInt(styles[i]);
	return &pw;
}