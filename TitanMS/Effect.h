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

#ifndef EFFECT_H
#define EFFECT_H

#include <hash_map>
using namespace std;
using namespace stdext;

#include "Tools.h"
class ItemEffectData;
class SkillLevelData;
class Mob;
class Player;

class Effect {
private:
	int x,y;
	int id, level;
	bool skill, buff;
	short hp, mp, hpCon, mpCon, hpR, mpR;
	int time;
	int prop;
	int itemCon, itemConNo;
	Position lt, rb;
	Values values;
	Values mobvals;
	static hash_map <int, Effect*> effects;
	static hash_map <int, hash_map<int, Effect*>> seffects;
public:
	static const __int64 WATK = 			0x1;
	static const __int64 WDEF = 			0x2;
	static const __int64 MATK = 			0x4;
	static const __int64 MDEF = 			0x8;
	static const __int64 ACC = 				0x10;
	static const __int64 AVO = 				0x20;
	static const __int64 HAND =				0x40;
	static const __int64 SPEED =			0x80;
	static const __int64 JUMP =				0x100;
	static const __int64 MAGIC_GUARD =		0x200;
	static const __int64 DARK_SIGHT =		0x400;
	static const __int64 BOOSTER =			0x800;
	static const __int64 POWER_GUARD =		0x1000;
	static const __int64 HYPERBODY_HP =		0x2000;
	static const __int64 HYPERBODY_MP =		0x4000;
	static const __int64 INVINCIBLE =		0x8000;
	static const __int64 SOUL_ARROW =		0x10000;
	static const __int64 COMBO =			0x200000;
	static const __int64 CHARGE =			0x400000;
	static const __int64 DRAGON_BLOOD =		0x800000;
	static const __int64 HOLY_SYMBOL =		0x1000000;
	static const __int64 MESO_UP =			0x8000000;
	static const __int64 SHADOW_PARTNER =	0x4000000;
	static const __int64 PICK_POCKET =		0x8000000;
	static const __int64 MESO_GUARD =		0x10000000;
	static const __int64 THAW =				0x20000000;
	static const __int64 MORPH =			0x200000000;
	static const __int64 RECOVERY =			0x400000000;
	static const __int64 SHARP_EYES =		0x2000000000;
	static const __int64 MAPLE_WARRIOR =	0x8000000000;
	static const __int64 SHADOW_CLAW =		0x10000000000;
	static const __int64 INFINITY =			0x20000000000;
	static const __int64 HOLY_SHIELD =		0x40000000000;
	static const __int64 MONSTER_RIDING =	0x400000000000;

	static const __int64 MOB_WATK = 0x1;
	static const __int64 MOB_WDEF = 0x2; 
	static const __int64 MOB_MATK = 0x4; //?
	static const __int64 MOB_MDEF = 0x8; //?
	static const __int64 MOB_ACC = 0x10; //?
	static const __int64 MOB_AVO = 0x20; //?
	static const __int64 MOB_SPEED = 0x40;
	static const __int64 MOB_STUN = 0x80;
	static const __int64 MOB_FREEZE = 0x100;
	static const __int64 MOB_POISON = 0x200; 
	static const __int64 MOB_SEAL = 0x400; 
	static const __int64 MOB_WATK_UP = 0x1000; //?
	static const __int64 MOB_WDEF_UP = 0x2000; //?
	static const __int64 MOB_MATK_UP = 0x4000; //?
	static const __int64 MOB_MDEF_UP = 0x8000;
	static const __int64 MOB_DOOM = 0x10000;
	static const __int64 MOB_SHADOW_WEB = 0x20000;

	static const char VALUE_X = -1;
	static const char VALUE_Y = -2;
	static const char VALUE_SPEED = -3;
	static const char VALUE_JUMP = -4;
	static const char VALUE_WATK = -5;
	static const char VALUE_WDEF = -6;
	static const char VALUE_MATK = -7;
	static const char VALUE_MDEF = -8;
	static const char VALUE_ACC = -9;
	static const char VALUE_AVO = -10;
	static const char VALUE_SHARPEYES = -11;
	
	static Effect* getEffect(int id, int level = 0);

	Effect(int id, ItemEffectData* data);
	Effect(int id, SkillLevelData* data);
	int getID(){
		return id;
	}
	bool isSkill(){
		return skill;
	}
	bool isBuff(){
		return buff;
	}
	short getHP(){
		return hp;
	}
	short getMP(){
		return mp;
	}
	short getHPCon(){
		return hpCon;
	}
	short getMPCon(){
		return mpCon;
	}
	short getHPR(){
		return hpR;
	}	
	short getMPR(){
		return mpR;
	}
	int getTime(){
		return time;
	}
	void use(Player* player, bool by = false);
	void use(Mob* mob);
	Values* getValues(){
		return 
			&values;
	}
};

#endif