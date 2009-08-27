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
#include "Effect.h"
#include "Party.h"
#include "PartyMembers.h"
#include "ItemData.h"
#include "SkillData.h"
#include "DataProvider.h"
#include "SkillLevelData.h"
#include "PlayerBuffs.h"
#include "Buff.h"
#include "Player.h"
#include "Timer.h"
#include "Mob.h"
#include "PacketCreator.h"
#include "Inventory.h"
#include "Map.h"
#include "PlayerInventories.h"
#include "Run.h"

hash_map <int, Effect*> Effect::effects;
hash_map <int, hash_map<int, Effect*>> Effect::seffects;

class SkillTimer : public Run {
private:
	int id;
	Player* player;
public:
	SkillTimer(Player* player, int id):id(id), player(player){}
	void run(){
		player->getBuffs()->cancelBuff(id, true);
	}
};

class RecoverySkillTimer : public Run {
private:
	Player* player;
	int rec;
	int time;
	Timer** timer;
public:
	RecoverySkillTimer(Player* player, int rec, int time, Timer** timer):player(player), rec(rec), time(time), timer(timer){}
	void run(){
		if(rec < 0 && player->getHP() < -rec){
			*timer = NULL;
			player->getBuffs()->cancelBuff(1311008); // Drgoon blood
			return;
		}
		if(player->getHP() > 0){
			player->addHP(rec);
			if(rec > 0 && player->getHP() != player->getMaxHP())
				player->send(PacketCreator().showPlayerEffect(Player::Effects::HEAL, rec));
		}
		*timer = Timers::getInstance()->startTimer(time, new RecoverySkillTimer(player, rec, time, timer));
	}
};

Effect* Effect::getEffect(int id, int level){
	if(level > 0){
		if(seffects.find(id) != seffects.end() && seffects[id].find(level) != seffects[id].end())
			return seffects[id][level];
		SkillData* sd = DataProvider::getInstance()->getSkillData(id);
		if(sd == NULL)
			return NULL;
		SkillLevelData* seffect = sd->getDataByID(level);
		if(seffect == NULL)
			return NULL;
		Effect* eb = new Effect(id, seffect);
		seffects[id][level] = eb;
		return eb;
	}
	if(effects.find(id) != effects.end())
		return effects[id];
	ItemData* item = DataProvider::getInstance()->getItemData(id);
	ItemEffectData* effect = NULL;
	Effect* eb = NULL;
	if(item == NULL)
		return NULL;
	effect = item->getEffectData();
	eb = new Effect(id, effect);
	if(eb == NULL)
		return NULL;
	effects[id] = eb;
	return eb;
}

Effect::Effect(int id, ItemEffectData* data){
	this->id =- id;
	skill = false;
	level = 0;
	time = data->getTime();
	buff = (time > 0);
	if(data->getWAtk() != 0)	values.add(Value(WATK, data->getWAtk()));
	if(data->getMAtk() != 0)	values.add(Value(MATK, data->getMAtk()));
	if(data->getWDef() != 0)	values.add(Value(WDEF, data->getWDef()));
	if(data->getMDef() != 0)	values.add(Value(MDEF, data->getMDef()));
	if(data->getAcc() != 0)	values.add(Value(ACC, data->getAcc()));
	if(data->getAvo() != 0)	values.add(Value(AVO, data->getAvo()));
	if(data->getHand() != 0)	values.add(Value(HAND, data->getHand()));
	if(data->getSpeed() != 0)values.add(Value(SPEED, data->getSpeed()));
	if(data->getJump() != 0)	values.add(Value(JUMP, data->getJump()));
	if(data->getMorph() != 0)values.add(Value(MORPH, data->getMorph()));
	hp = data->getHealHP();
	mp = data->getHealMP();
	hpCon = 0;
	mpCon = 0;
	hpR = data->getHealHPPer();
	mpR = data->getHealMPPer();
	itemCon = 0;
	itemConNo = 0;
	prop = 0;
	x = 0;
	y = 0;
}
Effect::Effect(int id, SkillLevelData* data){
	this->id = id;
	skill = true;
	time = 1000*data->getTime();
	buff = (time > 0);
	level = data->getID();
	if(data->getWAtk() != 0)	values.add(Value(WATK, data->getWAtk()));
	if(data->getMAtk() != 0)	values.add(Value(MATK, data->getWDef()));
	if(data->getWDef() != 0)	values.add(Value(WDEF, data->getMAtk()));
	if(data->getMDef() != 0)	values.add(Value(MDEF, data->getMDef()));
	if(data->getAcc() != 0) 	values.add(Value(ACC, data->getAcc()));
	if(data->getAvo() != 0) 	values.add(Value(AVO, data->getAvo()));
	//if(data->getHand() != 0)	values.add(Value(HAND, data->getHand()));
	if(data->getSpeed() != 0)	values.add(Value(SPEED, data->getSpeed()));
	if(data->getJump() != 0)	values.add(Value(JUMP, data->getJump()));
	hp = 0;
	mp = 0;
	hpCon = data->getHP();
	mpCon = data->getMP();
	hpR = data->getHPPer();
	mpR = 0;
	itemCon = data->getItem();
	itemConNo = data->getItemCount();
	x = data->getX();
	y = data->getY();
	lt = data->getLT();
	rb = data->getRB();
	//TODO
	prop = 0;
	//lt, rb;
	switch(id){
		// Credit for OdinMS
		case 2001002: // magic guard
			values.add(Value(MAGIC_GUARD, x)); break;
		case 2301003: // invincible
			values.add(Value(INVINCIBLE, x)); break;
		case 9101004: // hide
		case 4001003: // darksight
			values.add(Value(DARK_SIGHT, x)); break;
		case 4211003: // pickpocket
			values.add(Value(PICK_POCKET, x)); break;
		case 4211005: // mesoguard
			values.add(Value(MESO_GUARD, x)); break;
		case 4111001: // mesoup
			values.add(Value(MESO_UP, x)); break;
		case 4111002: // shadowpartner
			values.add(Value(SHADOW_PARTNER, x)); break;
		case 3101004: // soul arrow
		case 3201004:
			values.add(Value(SOUL_ARROW, x)); break;
		case 1211006: // wk charges
		case 1211003:
		case 1211004:
		case 1211005:
		case 1211008:
		case 1211007:
			values.add(Value(CHARGE, x)); break;
		case 1101005: // booster
		case 1101004:
		case 1201005:
		case 1201004:
		case 1301005:
		case 1301004:
		case 3101002:
		case 3201002:
		case 4101003:
		case 4201002:
		case 2111005:
		case 2211005:
		case 5101006:
		case 5101003:
			values.add(Value(BOOSTER, x)); break;
		case 1101007: // power guard
		case 1201007:
			values.add(Value(POWER_GUARD, x)); break;
		case 9101008: // gm hyper body
		case 1301007: // hyper body
			values.add(Value(HYPERBODY_HP, x)); 
			values.add(Value(HYPERBODY_MP, y)); break;
		case 1001: // recovery
			values.add(Value(RECOVERY, x)); break;
		case 1111002: // combo
			values.add(Value(COMBO, 1)); break;
		case 1004: // monster riding
			values.add(Value(MONSTER_RIDING, 1)); break;
		case 1311008: // dragon blood
			values.add(Value(DRAGON_BLOOD, x)); break;
		case 1121000: // maple warrior
		case 1221000:
		case 1321000:
		case 2121000:
		case 2221000:
		case 2321000:
		case 3121000:
		case 3221000:
		case 4121000:
		case 4221000:
			values.add(Value(MAPLE_WARRIOR, x)); break;
		case 3121002: // sharp eyes
		case 3221002: 
			values.add(Value(SHARP_EYES, x << 8 || y)); break;
		case 4001002: // disorder
		case 1201006: // threaten
			mobvals.add(Value(MOB_WATK, x));
			mobvals.add(Value(MOB_WDEF, y)); break;
		case 1211002: // charged blow
		case 1111008: // shout
		case 4211002: // assaulter
		case 3101005: // arrow bomb
		case 1111005: // coma: sword
		case 1111006: // coma: axe
			mobvals.add(Value(MOB_STUN, 1)); break;
		case 2201004: // cold beam
		case 2211002: // ice strike
		case 3211003: // blizzard
		case 2211006: // il elemental compo
			time*=2; // ????
			mobvals.add(Value(MOB_FREEZE, 1)); break;
		case 2101003: // fp slow
		case 2201003: // il slow
			mobvals.add(Value(MOB_SPEED, x));break;
		case 2101005: // poison breath
		// case 2111003: // poison mist
		case 2111006: // fp elemental compo
			mobvals.add(Value(MOB_POISON, 1)); break;
		case 2311005:
			mobvals.add(Value(MOB_DOOM, 1)); break;
		/*
		case 3111002: // puppet ranger
		case 3211002: // puppet sniper
			statups.add(Value(new Pair<MapleBuffStat, Integer>(MapleBuffStat.PUPPET, Integer.valueOf(1)));
			break;	
		case 3211005: // golden eagle
		case 3111005: // golden hawk
			statups.add(Value(new Pair<MapleBuffStat, Integer>(MapleBuffStat.SUMMON, Integer.valueOf(1)));
			monsterStatus.put(MonsterStatus.STUN, Integer.valueOf(1));
			break;
		case 3221005: // frostprey
		case 2121005: // elquines
			statups.add(Value(new Pair<MapleBuffStat, Integer>(MapleBuffStat.SUMMON, Integer.valueOf(1)));
			monsterStatus.put(MonsterStatus.FREEZE, Integer.valueOf(1));
			break;
		case 2311006: // summon dragon
		case 3121006: // phoenix
		case 2221005: // ifrit
		case 2321003: // bahamut
			statups.add(Value(new Pair<MapleBuffStat, Integer>(MapleBuffStat.SUMMON, Integer.valueOf(1)));
			break;
			*/
		case 2311003: // hs
		case 5101002: // GM hs
			values.add(Value(HOLY_SYMBOL, x)); break;
		case 2211004: // il seal
		case 2111004: // fp seal
			mobvals.add(Value(MOB_SEAL, 1)); break;
		case 4111003: // shadow web
			mobvals.add(Value(MOB_SHADOW_WEB, 1)); break;
	}
}

// by is false when a party member get the effect of the skill
void Effect::use(Player* player, bool by){
	if(player->getHP() == 0 && !by)
		return;
	int ahp = hp - ((by) ? 0 : hpCon) + player->getMaxHP()*hpR/100;
	int amp = mp - ((by) ? 0 : mpCon )+ player->getMaxMP()*mpR/100;
	Values toupdate;
	if(ahp != 0 && player->getHP() > 0){
		player->addHP(ahp, true, true);
	}
	if((amp < 0 && player->getMP() < -amp) || (ahp < 0 && player->getHP() < -ahp)){
		player->send(PacketCreator().enableAction());
		return;
	}
	if(amp != 0){
		player->addMP(amp, true, true);
	}
	if(amp == 0 && ahp == 0)
		player->send(PacketCreator().enableAction());
	// TODO: add check here
	if(!by && itemConNo != 0){
		player->getInventories()->getInventory(INVENTORY(itemCon))->removeItem(itemCon, itemConNo);
	}
	if(by && (id == 9101005 || id == 9001005 || id == 2321006)){ // esurrection
		player->setStance(0);
		player->setHP(player->getMaxHP());
	}
	if(skill){
		player->getMap()->send(PacketCreator().showBuffEffect(player->getID(), by ? 2 : 1, id), player);
		if(by)
			player->send(PacketCreator().showPlayerBuffEffect(2, id));
	}
	// TODO: hpR/people to use on
	if(!by && lt != 0 && rb != 0 && player->getParty() != NULL){
		hash_map <int, Player*>* ps = player->getParty()->getMembers()->getPlayers();
		for(hash_map<int,Player*>::iterator iter = ps->begin(); iter != ps->end(); iter++){
			if(iter->second != player && iter->second->getMap() == player->getMap()){	
				Position pos = player->getPosition();
				Position fpos = iter->second->getPosition();
				if(fpos.x > pos.x + lt.x && fpos.y > pos.y + lt.y && fpos.x < pos.x + rb.x && fpos.y < pos.y + rb.y){
					if(iter->second->getHP() > 0 || id == 9101005 || id != 9001005 || id == 2321006)
						use(iter->second, true);
				}
			}
		}
	}
	if(time > 0){
		// TODO hyper body
		Buff* buff = new Buff(this, Timers::getInstance()->startTimer(time, new SkillTimer(player, id)));
		Timer* stimer = NULL;
		switch(id){
			case 1001: stimer = Timers::getInstance()->startTimer(4900, new RecoverySkillTimer(player, x, 5000, buff->getTimerPtr())); break; //Recovery
			case 1311008: stimer = Timers::getInstance()->startTimer(3900, new RecoverySkillTimer(player, -x, 4000, buff->getTimerPtr())); break; //Dragon Blood
		}
		if(stimer != 0)
			*buff->getTimerPtr() = stimer;
		player->getBuffs()->addBuff(buff);
		player->send(PacketCreator().showPlayerBuff(&values, id, time));
		Values m = Values();
		if(values.contain(DARK_SIGHT))m.add(Value(DARK_SIGHT, 0));
		else if(values.contain(POWER_GUARD))m.add(Value(POWER_GUARD, 0));
		else if(values.contain(SHADOW_PARTNER))m.add(Value(SHADOW_PARTNER, 0));
		else if(values.contain(COMBO))m.add(Value(COMBO, 1));
		else if(values.contain(MONSTER_RIDING))m.add(Value(MONSTER_RIDING, 1));
		else if(values.contain(MORPH))m.add(Value(MORPH, values.getValueByType(MORPH)));
		if(m.getSize() != 0)
			player->getMap()->send(PacketCreator().showBuff(player->getID(), &m));
	}
	// TODO door
}
void Effect::use(Mob* mob){

}