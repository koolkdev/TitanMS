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
#include "Equip.h"
#include "Tools.h"
#include "EquipData.h"
#include "EquipsData.h"
#include "PlayerInventories.h"
using namespace Tools;

short Equip::getRandomStat(int stat){
	double r = stat/10;
	if(r != (double)((int)r)){
		r+=1;
	}
	int range = (int)r;
	range = (range > 5) ? 5 : range;
	return (short)(stat+random(range,-range));
}

Equip::Equip(){
	lock = false;
	type = 1;
	amount = 1;
	slot=0;
	owner = "";
}
Equip::Equip(int itemid, bool rstats){
	type = INVENTORY(itemid);
	lock = false;
	id = itemid;
	amount = 1;
	slot=0;
	owner = "";
	scrolls = 0;
	EquipData* data = EquipsData::getInstance()->getDataByID(itemid);
	if(data==NULL){
		//ERROR!
	}
	else{
		slots = data->getSlots();
		str = data->getStr();
		dex = data->getDex();
		intt = data->getInt();
		luk = data->getLuk();
		hp = data->getHP();
		mp = data->getMP();
		watk = data->getWAtk();
		matk = data->getMAtk();
		wdef = data->getWDef();
		mdef = data->getMDef();
		acc = data->getAcc();
		avo = data->getAvo();
		hand = data->getHand();
		jump = data->getJump();
		speed = data->getSpeed();
		if(rstats){
			str = getRandomStat(str);
			dex = getRandomStat(dex);
			intt = getRandomStat(intt);
			luk = getRandomStat(luk);
			hp = getRandomStat(hp);
			mp = getRandomStat(mp);
			watk = getRandomStat(watk);
			matk = getRandomStat(matk);
			wdef = getRandomStat(wdef);
			mdef = getRandomStat(mdef);
			acc = getRandomStat(acc);
			avo = getRandomStat(avo);
			hand = getRandomStat(hand);
			speed = getRandomStat(speed);
			jump = getRandomStat(jump);
		}
	}
}