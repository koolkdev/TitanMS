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
#include "PlayerBuffs.h"
#include "Tools.h"
#include "Player.h"
#include "PacketCreator.h"
#include "Map.h"
#include "Effect.h"
#include "Timer.h"
#include "Buff.h"

int PlayerBuffs::getBuffValue(__int64 buff){
	if(isBuffActive(buff)){
		return buffs[buff]->getEffect()->getValues()->getValueByType(buff);
	}
	return 0;
}
void PlayerBuffs::cancelBuff(int buffid, bool timer){
	if(idbuffs.find(buffid) == idbuffs.end()){
		return;		
	}
	Values* values = idbuffs[buffid]->getEffect()->getValues();
	player->send(PacketCreator().cancelPlayerBuff(values));
	Values m = Values();
	if(values->contain(Effect::DARK_SIGHT))m.add(Value(Effect::DARK_SIGHT, 0));
	else if(values->contain(Effect::POWER_GUARD))m.add(Value(Effect::POWER_GUARD, 0));
	else if(values->contain(Effect::SHADOW_PARTNER))m.add(Value(Effect::SHADOW_PARTNER, 0));
	else if(values->contain(Effect::COMBO))m.add(Value(Effect::COMBO, 0));
	else if(values->contain(Effect::MONSTER_RIDING))m.add(Value(Effect::MONSTER_RIDING, 0));
	else if(values->contain(Effect::MORPH))m.add(Value(Effect::MORPH, 0));
	if(m.getSize() != 0)
		player->getMap()->send(PacketCreator().cancelBuff(player->getID(), &m));
	deleteBuff(idbuffs[buffid]);
	idbuffs.erase(buffid);
}
void PlayerBuffs::cancelAll(){
	for (hash_map<int,Buff*>::iterator iter = idbuffs.begin(); iter != idbuffs.end(); iter++){
		deleteBuff(iter->second);
	}
	idbuffs.clear();
}
void PlayerBuffs::deleteBuff(Buff* buff, bool timer){
	if(!timer){
		Timers::getInstance()->cancelTimer(buff->getTimer());
	}
	if(*buff->getTimerPtr() != 0){
		Timers::getInstance()->cancelTimer(*buff->getTimerPtr());
	}
	vector <__int64> todel;
	for (hash_map<__int64,Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); iter++){
		if(iter->second == buff)
			todel.push_back(iter->first);
	}
	for(int i=0; i<(int)todel.size(); i++){
		buffs.erase(todel[i]);
	}
	delete buff;
}
void PlayerBuffs::addBuff(Buff* buff){
	int id = buff->getEffect()->getID();
	if(idbuffs.find(id) != idbuffs.end()){
		deleteBuff(idbuffs[id]);
	}
	vector <Value>* v = buff->getEffect()->getValues()->getValues();
	for(int i=0; i<(int)v->size(); i++){
		__int64 type = (*v)[i].getType();
		if(buffs.find(type) != buffs.end()){
			int count = 0;
			for (hash_map<__int64,Buff*>::iterator iter = buffs.begin(); iter != buffs.end(); iter++){
				if(type != iter->first && iter->second == buffs[type])
					count++;
			}
			if(count == 0)
				deleteBuff(buffs[type]);
			
		}
		buffs[type] = buff;
	}
	idbuffs[id] = buff;
}