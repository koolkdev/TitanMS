#ifndef PLAYERBUFFS_H
#define PLAYERBUFFS_H

#include <vector>
#include <hash_map>

class Buff;
class Player;

using namespace std;
using namespace stdext;

class PlayerBuffs {
private:
	Player* player;
	hash_map <__int64, Buff*> buffs;
	hash_map <int, Buff*> idbuffs;
public:
	PlayerBuffs(Player* player){
		this->player = player;
	}
	bool isBuffActive(__int64 buff){
		return (buffs.find(buff) != buffs.end());
	}
	int getBuffValue(__int64 buff);
	void cancelBuff(int buffid, bool timer = false);
	void addBuff(Buff* buff);
	void deleteBuff(Buff* buff, bool timer = false);
	void cancelAll();
};

#endif