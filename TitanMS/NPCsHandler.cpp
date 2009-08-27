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
#include "PacketCreator.h"
#include "Player.h"
#include "Map.h"
#include "NPC.h"
#include "PlayerNPC.h"
#include "Item.h"
#include "DataProvider.h"
#include "Inventory.h"
#include "ShopData.h"
#include "ShopItemData.h"
#include "AngelScriptEngine.h"
#include "PlayerInventories.h"

void PlayerHandler::NPCHandle(PacketReader& packet){
	if(player->getNPC() != NULL || player->getNPCShop() != NULL)
		return;
	NPC* mapnpc = player->getMap()->getNPC(packet.readInt());
	if(mapnpc == NULL) return;
	int npcid = mapnpc->getNPCID();
	packet.readShort(); // x
	packet.readShort(); // y
	ShopData* shop = DataProvider::getInstance()->getShopData(npcid);
	if(shop != NULL){
		player->setNPCShop(shop);
		player->send(PacketCreator().showNPCShop(shop));
		return;
	}
	PlayerNPC* npc = new PlayerNPC(npcid, player);
	if(!AngelScriptEngine::handleNPC(npc)) npc->end();

	if(npc->isEnd())
		delete npc;
}
void PlayerHandler::NPCChatHandle(PacketReader& packet){

	PlayerNPC* npc = player->getNPC();
	if(npc == NULL)
		return;
	char type = packet.read();
	if(type == PlayerNPC::BACK_NEXT){
		char what = packet.read();
		if(what == 0)
			npc->setState(npc->getState()-1);
		else if(what == 1)
			npc->setState(npc->getState()+1);
		else if(what == -1)
			npc->end();
	}
	else if(type == PlayerNPC::YES_NO || type == PlayerNPC::ACCEPT_DECLINE){
		npc->setState(npc->getState()+1);
		char what = packet.read();
		if(what == 0)
			npc->setSelected(0);
		else if(what == 1)
			npc->setSelected(1);
		else if(what == -1)
			npc->end();
	}
	else if(type == PlayerNPC::GET_TEXT){
		npc->setState(npc->getState()+1);
		if(packet.read() != 0){
			npc->setGetText(packet.readString());
		}
		else
			npc->end();
	}
	else if(type == PlayerNPC::GET_NUMBER){
		npc->setState(npc->getState()+1);
		if(packet.read() == 1)
			npc->setGetNumber(packet.readInt());
		else
			npc->end();
	}
	else if(type == PlayerNPC::SIMPLE){
		npc->setState(npc->getState()+1);
		if(packet.read() == 0)
			npc->end();
		else
			npc->setSelected(packet.read());
	}
	else if(type == PlayerNPC::STYLE){
		npc->setState(npc->getState()+1);
		if(packet.read() == 1)
			npc->setSelected(packet.read());
		else 
			npc->end();
	}
	else
		npc->end();
	if(npc->isEnd()){
		delete npc;
		return;
	}
	if(!AngelScriptEngine::handleNPC(npc)) npc->end();
	if(npc->isEnd()){
		delete npc;
		return;
	}
}
void PlayerHandler::useShopHandle(PacketReader& packet){
	ShopData* shop = player->getNPCShop();
	if(shop == NULL)
		return;
	char action = packet.read();
	if(action == 0){ // Buy
		packet.readShort();
		int itemid = packet.readInt();
		short amount = packet.readShort();
		ShopItemData* item = shop->getDataByID(itemid);
		if(item == NULL)
			return;
		int price = item->getPrice();
		if(player->getMesos() < price*amount || price == 0)
			return;
		if(amount <= 0 || (INVENTORY(itemid) == EQUIP && amount > 1))
			return;
		if(player->getInventories()->giveItem(itemid, amount))
			player->addMesos(-amount*price);
		player->send(PacketCreator().bought());
	}
	else if(action == 1){ // Sell
		short slot = packet.readShort();
		int itemid = packet.readInt();
		short amount = packet.readShort();
		if(amount < 1) amount = 1;
		Item* item = player->getInventories()->getItemBySlot(INVENTORY(itemid), slot);
		if(item == NULL)
			return;
		if(item->getID() != itemid || (item->getAmount() < amount && !IS_STAR(itemid)))
			return;
		if(IS_STAR(itemid))
			player->getInventories()->getInventory(INVENTORY(itemid))->removeItem(slot);
		else
			player->getInventories()->getInventory(INVENTORY(itemid))->removeItemBySlot(slot, amount);
		player->addMesos(amount*DataProvider::getInstance()->getItemPrice(itemid));
		player->send(PacketCreator().bought());
	}
	else if(action == 2){ // recharge
		short slot = packet.readShort();
		Item* item = player->getInventories()->getItemBySlot(USE, slot);
		if(item == NULL || !IS_STAR(item->getID()) || shop->getDataByID(item->getID()) == NULL)
			return;
		// TODO skill 4100000
		int charge = DataProvider::getInstance()->getItemMaxPerSlot(item->getID()) - item->getAmount();
		if(charge <= 0)
			return;
		double dcost = charge*DataProvider::getInstance()->getItemUnitPrice(item->getID());
		int cost = (int)dcost;
		if(dcost != (double)cost) cost++; // round up
		int amount = DataProvider::getInstance()->getItemMaxPerSlot(item->getID());
		if(player->getMesos() < cost){
			cost = player->getMesos();
			int namount = item->getAmount() + (int)(cost/DataProvider::getInstance()->getItemUnitPrice(item->getID()));
			if(namount < amount) amount = namount;
		}
		player->addMesos(-cost);
		item->setAmount(amount);
		player->send(PacketCreator().updateSlot(item));
		player->send(PacketCreator().bought());
	}
	else if(action == 3){ // Close
		player->setNPCShop(NULL);
	}
}