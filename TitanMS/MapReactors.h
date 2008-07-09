#ifndef MAPREACTOR_H
#define MAPREACTOR_H


#include <vector>

using namespace std;

class Mob;
class Reactor;
class Player;
class Drop;

class MapReactors {
private:
	static int ids;
	Map* map;
	vector <Reactor*> reactors;
	hash_map <int, int> last;
	bool timeron;
public:
	MapReactors(Map* map);
	Reactor* getReactorByID(int objid);
	void show(Player* player);
	void checkSpawn(bool timer = false);
	void show(Reactor* reactor);
	void hitReactor(Reactor* reactor);
	void hitReactor(Reactor* reactor, Player* player);
	void hitReactor(Reactor* reactor, Player* player, int stance, int pos);
	void destroy(Reactor* reactor);
	void checkDrop(Drop* drop, Player* player);
	Reactor* spawnReactor(int reactorid, int x, int y, int orgID = -1);
	Reactor* getReactorByReactorID(int id);
	void respawn();

};

#endif