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

#include "Mob.h"
#include "Party.h"
#include "DataProvider.h"
#include "Player.h"
#include "MapPlayers.h"
#include <algorithm>
#include "Map.h"
#include "Channel.h"
#include "World.h"
#include "Parties.h"
#include "PartyMembers.h"
#include "PacketCreator.h"
#include "Worlds.h"
#include "Timer.h"
#include "Tools.h"
using namespace Tools;

class ComparePlayersDist : public std::binary_function<Player*, Player*, bool>
{
	Position pos;
public:
	bool operator()(Player* x, Player* y) const
	{
		return getDistance((Position&)pos, x->getPosition()) < getDistance((Position&)pos, y->getPosition());
	}
	ComparePlayersDist(Position& pos){
		this->pos = pos;		
	}
};

int Mob::getMaxMP(){
	return DataProvider::getInstance()->getMobMaxMP(mobid);
}
int Mob::getMaxHP(){
	return DataProvider::getInstance()->getMobMaxHP(mobid);
}
bool Mob::isBoss(){
	return DataProvider::getInstance()->isMobBoss(mobid);
}

// DON'T call it directly, call damageMob in MapMobs.
void Mob::takeDamage(Player* player, int damage){
	hp-=damage;
	int ndamage = damage;
	if(hp < 0){
		ndamage += hp;
		hp = 0;
	}
	if(player != NULL){
		if(playersa.find(player->getID()) == playersa.end())
			playersa[player->getID()] = ndamage;
		else
			playersa[player->getID()] += ndamage;
	}
	// TODO party

	char per = (char)(hp * 100.0 / getMaxHP());
	if(isBoss()){
		char color = DataProvider::getInstance()->getMobTagColor(mobid);
		if(color != 0){
			map->send(PacketCreator().showBossHP(mobid, hp, getMaxHP(), per, color, DataProvider::getInstance()->getMobTagBgColor(mobid)));
		}
	}
	else if(DataProvider::getInstance()->getMobDamagedByMob(mobid)){
		map->send(PacketCreator().damageByMob(id, damage, hp, getMaxHP()));
	}
	else {
		for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
			Player* p = map->getPlayers()->getPlayerByID(iter->first);
			if(p != NULL && p->getMap() == map){
				p->send(PacketCreator().showHP(id, per));
			}
		}
	}
}
int round(double n){
	if(((int)n*10)%10 > 5)
		return (int)(n+1);
	else
		return (int)n;
}
void Mob::giveExp(){
	hash_map <int, int> maxdamage;
	hash_map <int, Player*> maxplayer;
	hash_map <int, int> damages;
	for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
		Player* p = map->getPlayers()->getPlayerByID(iter->first);
		if(p != NULL){
			if(p->getParty() == NULL){
				int hpk = iter->second;
				double fexp = hpk * Worlds::getInstance()->getEXP() * 100.0 * DataProvider::getInstance()->getMobExp(mobid) / 100.0 / getMaxHP();
				int exp = round(fexp);
				if(exp != 0){
					p->send(PacketCreator().gainEXP(exp, false));
					p->addExp(exp);
				}
			}
			else{
				int pid = p->getParty()->getID();
				if(maxdamage.find(pid) == maxdamage.end() || iter->second > maxdamage[pid]){
					if(maxdamage.find(pid) == maxdamage.end()){
						damages[pid] = 0;
					}
					maxdamage[pid] = iter->second;
					maxplayer[pid] = p;
					damages[pid] += iter->second;
				}
			}
		}
	}
	for (hash_map<int,int>::iterator d = damages.begin(); d != damages.end(); d++){
		Party* party = map->getChannel()->getWorld()->getParties()->getParty(d->first);
		if(party == NULL) continue;
		int hpk = d->second;
		double fexp = hpk * Worlds::getInstance()->getEXP() * 100.0 * DataProvider::getInstance()->getMobExp(mobid) / 100.0 / getMaxHP();
		int baseexp = round(fexp);
		vector <Player*> players;
		hash_map <int, Player*>* ps = party->getMembers()->getPlayers();
		int count = 0;
		for(hash_map<int,Player*>::iterator iter = ps->begin(); iter != ps->end(); iter++){
			if(iter->second->getMap() == map){
				count++;
				if(playersa.find(iter->second->getID()) != playersa.end() || iter->second->getLevel() >= maxplayer[party->getID()]->getLevel() - 5 || iter->second->getLevel() >= DataProvider::getInstance()->getMobLevel(mobid) - 5)
					players.push_back(iter->second);
			}
		}
		if(count == 1){
			maxplayer[party->getID()]->send(PacketCreator().gainEXP(baseexp, false));
			maxplayer[party->getID()]->addExp(baseexp);
			continue;
		}
		int levels = 0;
		for(int i=0; i<(int)players.size(); i++){
			levels += players[i]->getLevel();
		}
		int bonus = players.size()*5;
		for(int i=0; i<(int)players.size(); i++){
			// todo: improve the splitting
			int exp = round(baseexp*0.8*players[i]->getLevel()/levels);
			if(players[i] == maxplayer[party->getID()]) exp += round(baseexp*0.2);
			if(exp > 0){
				exp = (int)(exp*(1+bonus/100.0));
				players[i]->addExp(exp);
				players[i]->send(PacketCreator().gainEXP(exp, false, players[i] != maxplayer[party->getID()], bonus));
			}
		}
	}
}

int getDistance(Position& pos1, Position& pos2){
	return (int)(pow((double)(pos1.x+pos2.x), 2)+pow((double)(pos1.y+pos2.y), 1));
}

void Mob::findControl(bool spawn){
	vector <Player*> players = vector <Player*>(*map->getPlayers()->getPlayersList());
	if(players.empty()){
		control = NULL;
		return;
	}
	Player* player = *min_element<vector <Player*>::iterator, ComparePlayersDist>(players.begin(), players.end(), ComparePlayersDist(pos));
	setControl(*players.begin(), spawn && DataProvider::getInstance()->getMobFirstAttack(mobid), false);
}

void Mob::setControl(Player* control, bool agrs, bool switchc){
	if(getControl() != NULL && switchc)
		this->control->send(PacketCreator().endControlMob(id));
	this->control = control;
	if(control != NULL)
		control->send(PacketCreator().controlMob(this, MOB_SHOW, agrs));
	aggressive = agrs;
}
int Mob::getKillerID(){
	int md = 0;
	int mdid = 0;
	for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
		if(iter->second > md){
			md = iter->second;
			mdid = iter->first;
		}
	}
	return mdid;
}	
Player* Mob::getControl(){
	if(control != NULL && !map->getPlayers()->find(control))
		control = NULL;
	return control;
}