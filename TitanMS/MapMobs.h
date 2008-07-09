#ifndef MAPMOBS_H
#define MAPMOBS_H

#include <vector>

using namespace std;

class Mob;
class Map;
class Player;
class Damage;
class Item;

class MapMobs {
private:
	static int ids;
	Map* map;
	vector <Mob*> mobs;
	hash_map <int, int> last;
	bool timeron;
public:
	MapMobs(Map* map);
	Mob* getMobByID(int objid);
	void show(Player* player);
	void checkSpawn(bool timer = false);
	void show(Mob* mob, bool spawn = true);
	void damage(Player* player, Mob* mob, int damage);
	void damage(Player* player, Damage* dmg, Item* item = NULL);
	void kill(Mob* mob, Player* by);
	void summonFromMob(int mobid, int x, int y);
	Mob* spawnMob(int mobid, int x, int y, int orgID = -1, bool spawneffect = true, int fh = 0, bool flip = false);
	void killAll();
	int getMobsCount();

};

#endif