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

#ifndef INVENTORY_H
#define INVENTORY_H

#define ISSTAR(x) (x/10000 == 207)

class Player;
struct Equip;
struct Item;

class InventoryOld {
public:
	static void useShop(Player* player, unsigned char* packet);
	static void itemMove(Player* player, unsigned char* packet);
	static int findSlot(Player* player, int itemid ,char inv, short amount);
	static Equip* setEquipStats(Player* player, int equipid);
	static void addEquip(Player* player, Equip* equip, bool is=0);
	static void addItem(Player* player, Item* item, bool is=0);
	static void addNewItem(Player* player, int item, int howmany);
	static void takeItem(Player* player, int item, int howmany);
	static void useItem(Player* player, unsigned char* packet);
	static void takeItemSlot(Player* player, short slot, char inv, short amount);
	static void useChair(Player* player, unsigned char* packet);
	static void stopChair(Player* player, unsigned char* packet);
	static int isCash(int itemid);
	static void useSummonBag(Player* player, unsigned char* packet);
	static void useReturnScroll(Player* player, unsigned char* packet);
	static void useScroll(Player* player, unsigned char* packet);
	static void useItemEffect(Player* player, unsigned char* packet);
};

#endif