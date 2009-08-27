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
#include "PacketWriter.h"
#include "PlayerInventories.h"
#include "Item.h"
#include "Trade.h"
#include "Player.h"
#include "Transportations.h"

PacketWriter* PacketCreator::showTrade(Player* right, Player* left){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::SHOW);
	pw.write(Transportations::TYPE_TRADE);
	pw.write(2);

	if(left == NULL){
		pw.writeShort(0);
	}
	else{
		pw.write(1);
		pw.write(0);
		playerShow(left);
		pw.writeString(left->getName());
		pw.write(1);
	}
	playerShow(right);
	pw.writeString(right->getName());
	pw.write(-1);

	return &pw;
}
PacketWriter* PacketCreator::inviteTrade(Player* player, int inviteid){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::INVITE);
	pw.write(Transportations::TYPE_TRADE);

	pw.writeString(player->getName());
	pw.writeInt(inviteid); 

	return &pw;
}

PacketWriter* PacketCreator::joinTrade(Player* player){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::JOIN);
	pw.write(1);

	playerShow(player);
	pw.writeString(player->getName());

	return &pw;
}

/* 2 - another player closed the trade
   6 - successful
   7 - unsuccessful
   8 - there is a item that you can't carry more then one
   9 - player is in another map
*/

PacketWriter* PacketCreator::tradeError(char s){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::CLOSE); 
	pw.write(0);
	pw.write(s);

	return &pw;
}
PacketWriter* PacketCreator::showTradeChat(string msg, bool by){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::CHAT);
	pw.write(8);
	pw.write(by);
	pw.writeString(msg);

	return &pw;
}

PacketWriter* PacketCreator::tradeMesos(int mesos, char side){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::ADD_MESOS); 
	pw.write(side);
	pw.writeInt(mesos);

	return &pw;
}
PacketWriter* PacketCreator::tradeItem(Item* item, char side){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::TRADE_ADD_ITEM); 
	pw.write(side);
	itemInfo(item);

	return &pw;
}
PacketWriter* PacketCreator::tradeConfirm(){
	pw.writeShort(TRANSPORTATION);

	pw.write(Transportations::CONFIRM); 

	return &pw;

}