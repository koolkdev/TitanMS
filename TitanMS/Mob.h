#ifndef MOB_H
#define MOB_H

#include "MapObject.h"
class Player;
class Map;
#include <hash_map>
using namespace std;
using namespace stdext;

class Mob : public MapObject {
public:
	Mob (Map* map){
		control=NULL;
		this->map = map;
		aggressive = 0;
	}
	void setMobID(int mobid){
		this->mobid=mobid;
	}
	int getMobID(){
		return mobid;
	}
	void setOriginalID(int mapid){
		this->mapid=mapid;
	}
	int getOriginalID(){
		return mapid;
	}
	void setFH(short fh){
		this->fh=fh;
	}
	short getFH(){
		return fh;
	}
	void setHP(int hp){
		this->hp=hp;
		if(this->hp<0)
			this->hp=0;
	}
	int getHP(){
		return hp;
	}
	void setMP(int mp){
		this->mp=mp;
		if(this->mp<0)
			this->mp=0;
	}
	int getMaxMP();
	int getMaxHP();
	bool isBoss();
	int getMP(){
		return mp;
	}
	void setStance(char stance){
		this->stance=stance;
	}
	char getStance(){
		return stance;
	}
	void setControl(Player* control, bool spawn = false, bool agrs = false, bool switchc = true);
	Player* getControl(){
		return control;
	}
	Map* getMap(){
		return map;
	}
	void takeDamage(Player* player, int damage);
	void giveExp();
	void findControl(bool spawn);
	void setAggressive(char ag){
		aggressive = ag;
	}
	char getAggressive(){
		return aggressive;
	}
	int getKillerID();
private:
	char aggressive;
	int mapid;
	int mobid;
	short fh;
	int hp;
	int mp;
	char stance;
	Player* control;
	Map* map;
	hash_map<int, int> playersa;
};

#endif