#include "Mob.h"
#include "DataProvider.h"
#include "Player.h"
#include "MapPlayers.h"
#include "Map.h"
#include "PacketCreator.h"
#include "Tools.h"

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
	if(playersa.find(player->getID()) == playersa.end())
		playersa[player->getID()] = ndamage;
	else
		playersa[player->getID()] += ndamage;
	// TODO party

	char per = (char)(hp * 100.0 / getMaxHP());
	if(isBoss()){
		char color = DataProvider::getInstance()->getMobTagColor(mobid);
		if(color != 0){
			map->send(PacketCreator().showBossHP(id, hp, getMaxHP(), per, color, DataProvider::getInstance()->getMobTagBgColor(mobid)));
		}
	}
	else if(per > 0){
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
	for (hash_map<int,int>::iterator iter = playersa.begin(); iter != playersa.end(); iter++){
		Player* p = map->getPlayers()->getPlayerByID(iter->first);
		if(p != NULL && p->getMap() == map){
			int hpk = iter->second;
			double fexp = hpk * 100.0 * DataProvider::getInstance()->getMobExp(mobid) / 100.0 / getMaxHP();
			int exp = round(fexp);
			exp *= 10; // Drop Rate
			if(exp != 0){
				p->send(PacketCreator().gainEXP(exp, false));
				p->addExp(exp);
			}
		}
	}
}

int getDistance(Position pos1, Position pos2){
	return (int)(pow((double)(pos1.x+pos2.x), 2)+pow((double)(pos1.y+pos2.y), 1));
}

void Mob::findControl(bool spawn){
	vector <Player*>* players = map->getPlayers()->getPlayers();
	if(players->size() == 0){
		control = NULL;
		return;
	}
	Player* p = (*players)[0];
	int d = getDistance(getPosition(), (*players)[0]->getPosition());
	for(int i=1; i<(int)(*players).size(); i++){
		int nd = getDistance(getPosition(), (*players)[i]->getPosition());
		if(nd < d){	
			d = nd;
			p = (*players)[i];
		}
	}
	if(spawn)
		setControl(p, spawn, DataProvider::getInstance()->getMobFirstAttack(mobid), false);
	else
		setControl(p, spawn, false, false);

}

void Mob::setControl(Player* control, bool spawn, bool agrs, bool switchc){
	if(this->control != NULL && switchc)
		this->control->send(PacketCreator().endControlMob(id));
	this->control = control;
	if(control != NULL)
		control->send(PacketCreator().controlMob(this, spawn, agrs));
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