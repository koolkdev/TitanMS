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

void PlayerHandler::NPCHandle(PacketReader* packet){
	if(player->getNPC() != NULL || player->getShop() != NULL)
		return;
	int npcid = player->getMap()->getNPC(packet->readInt())->getNPCID();
	ShopData* shop = DataProvider::getInstance()->getShopData(npcid);
	if(shop != NULL){
		player->setShop(shop);
		player->send(PacketCreator().showShop(shop));
		return;
	}
	PlayerNPC* npc = new PlayerNPC(npcid, player);
	if(!AngelScriptEngine::handleNPC(npc)) npc->end();

	if(npc->isEnd())
		delete npc;
}
void PlayerHandler::NPCChatHandle(PacketReader* packet){

	PlayerNPC* npc = player->getNPC();
	if(npc == NULL)
		return;
	char type = packet->read();
	if(type == 0){
		char what = packet->read();
		if(what == 0)
			npc->setState(npc->getState()-1);
		else if(what == 1)
			npc->setState(npc->getState()+1);
		else if(what == -1)
			npc->end();
	}
	else if(type == 1 || type == 2){
		npc->setState(npc->getState()+1);
		char what = packet->read();
		if(what == 0)
			npc->setSelected(0);
		else if(what == 1)
			npc->setSelected(1);
		else if(what == -1)
			npc->end();
	}
	else if(type == 3){
		npc->setState(npc->getState()+1);
		if(packet->read() != 0){
			npc->setGetText(packet->readString());
		}
		else
			npc->end();
	}
	else if(type == 4){
		npc->setState(npc->getState()+1);
		if(packet->read() == 1)
			npc->setGetNumber(packet->readInt());
		else
			npc->end();
	}
	else if(type == 5){
		npc->setState(npc->getState()+1);
		if(packet->read() == 0)
			npc->end();
		else
			npc->setSelected(packet->read());
	}
	else if(type == 7){
		npc->setState(npc->getState()+1);
		if(packet->read() == 1){
			npc->setSelected(packet->readShort());
		}
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
void PlayerHandler::useShopHandle(PacketReader* packet){
	ShopData* shop = player->getShop();
	if(shop == NULL)
		return;
	char action = packet->read();
	if(action == 0){ // Buy
		packet->readShort();
		int itemid = packet->readInt();
		short amount = packet->readShort();
		ShopItemData* item = shop->getDataByID(itemid);
		if(item == NULL)
			return;
		int price = item->getPrice();
		if(player->getMesos() < price*amount)
			return;
		if(amount <= 0 || (INVENTORY(itemid) == EQUIP && amount > 1))
			return;
		if(player->getInventories()->giveItem(itemid, amount))
			player->addMesos(-amount*price);
		player->send(PacketCreator().bought());
	}
	else if(action == 1){ // Sell
		short slot = packet->readShort();
		int itemid = packet->readInt();
		short amount = packet->readShort();
		if(amount < 1) amount = 1;
		Item* item = player->getInventories()->getItemBySlot(INVENTORY(itemid), slot);
		if(item == NULL)
			return;
		if(item->getID() != itemid || item->getAmount() < amount)
			return;
		player->getInventories()->getInventory(INVENTORY(itemid))->removeItem(item, false, true);
		player->addMesos(amount*DataProvider::getInstance()->getItemPrice(itemid));
	}
	else if(action == 2){ // recharge
		// TODO
	}
	else if(action == 3){ // Close
		player->setShop(NULL);
	}
}