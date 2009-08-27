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
#include "Player.h"
#include "Item.h"
#include "Trade.h"
#include "Channel.h"
#include "Transportations.h"
#include "PlayerInventories.h"
#include "Inventory.h"
#include "Item.h"

// note: checks are in playerHandler

Trade::Trade(Player* player){
	this->player = player;
	player->setTrade(this);
	trader = NULL;
	mesos = 0;
	inv = 0;
	locked = false;
}
void Trade::open(){
	if(trader != NULL)
		player->send(PacketCreator().showTrade(player, trader->getPlayer()));
	else
		player->send(PacketCreator().showTrade(player));
}
bool Trade::invite(Player* to){
	if(to->getTrade() != NULL){
		player->send(PacketCreator().showMessage("The other player is already trading with someone else.", 5));
		return false;
	}
	invited = to;
	inv = to->getChannel()->getTransportations()->getNewInviteID(this);
	to->send(PacketCreator().inviteTrade(player, inv));
	return true;
}
void Trade::close(){
	// TODO
	player->addMesos(mesos);
	for(int i=0; i<(int)items.size(); i++){
		items[i].second->setSlot(items[i].first);
		player->getInventories()->getInventory(INVENTORY(items[i].second->getID()))->addItem(items[i].second, false);
	}
	player->send(PacketCreator().tradeError(2));
	player->setTrade(NULL);
	if(trader != NULL){
		trader->setTrader(NULL);
		trader->close();
	}
}
void Trade::decline(){
	player->send(PacketCreator().showMessage(invited->getName() + " has declined your trade request", 5));
}
void Trade::accept(){
	player->send(PacketCreator().joinTrade(invited));
	invited->setTrade(new Trade(invited));
	trader = invited->getTrade();
	trader->setTrader(this);
	trader->open();
}
void Trade::chat(string msg){
	string chat = player->getName() + " : " + msg;
	player->send(PacketCreator().showTradeChat(chat, true));
	if(trader != NULL)
		trader->getPlayer()->send(PacketCreator().showTradeChat(chat, false));
}
void Trade::addMesos(int mesos){
	player->addMesos(-mesos, true, true);
	this->mesos += mesos;
	player->send(PacketCreator().tradeMesos(this->mesos, 0));
	if(trader != NULL)
		trader->getPlayer()->send(PacketCreator().tradeMesos(this->mesos, 1));
}
void Trade::addItem(Item* item, short slot){
	pair<int, Item*> tItem;
	tItem.first = item->getSlot();
	tItem.second = item;
	items.push_back(tItem);
	item->setSlot(slot);
	player->send(PacketCreator().tradeItem(item, 0));
	if(trader != NULL)
		trader->getPlayer()->send(PacketCreator().tradeItem(item, 1));
}
void Trade::complete(){
	if(trader != NULL){
		Player* tplayer = trader->getPlayer();
		int tmesos = trader->getMesos();
		vector <pair<int, Item*>>* pitems = trader->getItems();
		bool check = true;
		for(int i=0; i<(int)items.size() && check; i++){
			if(!tplayer->getInventories()->getInventory(INVENTORY(items[i].second->getID()))->addItem(items[i].second, true, false, false))
				check = false;
		}
		for(int i=0; i<(int)pitems->size() && check; i++){
			if(!player->getInventories()->getInventory(INVENTORY((*pitems)[i].second->getID()))->addItem((*pitems)[i].second, true, false, false))
				check = false;
		}
		if(!check){
			player->send(PacketCreator().tradeError(7));
			tplayer->send(PacketCreator().tradeError(7));
			return;
		}
		player->send(PacketCreator().tradeError(6));
		tplayer->send(PacketCreator().tradeError(6));
		player->addMesos(tmesos);
		tplayer->addMesos(mesos);
		for(int i=0; i<(int)items.size(); i++){
			tplayer->getInventories()->getInventory(INVENTORY(items[i].second->getID()))->addItem(items[i].second);
		}
		for(int i=0; i<(int)pitems->size() ; i++){
			player->getInventories()->getInventory(INVENTORY((*pitems)[i].second->getID()))->addItem((*pitems)[i].second);
		}
	}
}
void Trade::confirm(){
	locked = true;
	trader->getPlayer()->send(PacketCreator().tradeConfirm());
}