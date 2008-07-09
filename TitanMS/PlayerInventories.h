#ifndef PLAYERINVENTORIES_H
#define PLAYERINVENTORIES_H

#include <hash_map>
using namespace std;
using namespace stdext;

#define INVENTORY(x) (x/1000000)
#define IS_EQUIP(x) (INVENTORY(x) == 1)
#define VALID_INVENTORY(x) (x>=1 && x<=5)
#define TYPE(x) (x/1000000)
#define MAJOR_TYPE(x) (x/10000)
#define MINOR_TYPE(x) ((x/1000)%10)
#define IS_STAR(x) (MAJOR_TYPE(x) == 207)
#define IS_PET(x) (MAJOR_TYPE(x) == 500)

#define EQUIPPED 0
#define EQUIP 1
#define USE 2
#define SETUP 3
#define ETC 4
#define CASH 5

class Inventory;
class Item;
class Player;

class PlayerInventories {
private:
	hash_map <int, Inventory*> invs;
	Player* player;
	Inventory* empty;
public:
	PlayerInventories(Player* player);
	Inventory* getInventory(int n){
		if(invs.find(n) != invs.end())
			return invs[n];
		return empty;
	}
	bool giveItem(int itemid, int amount=1, bool rands = false);
	bool giveItem(Item* item, bool drop = false);
	Item* getItemBySlot(int inv, int slot);
	Item* getItemByID(int id);
	//void removeItem(int inv, int slot);
	void removeItem(int itemid);
	//void removeItem(Item* item);
	static bool validID(int id);
	int getItemAmount(int id);
	void removeItemBySlot(int inv, int slot, int amount, bool send = true);
	void deleteAll();
};

#endif