#ifndef PLAYERKEYS_H
#define PLAYERKEYS_H

#include <stdlib.h>
class Key;
class Player;

class PlayerKeys {
private:
	Player* player;
	Key* keys[90];
public:
	PlayerKeys(Player* player){
		this->player = player;
		for(int i=0; i<90; i++)
			keys[i] = NULL;
	}
	Key* getKey(int n){
		if(n >= 0 && n<90)
			return keys[n];
		return NULL;
	}
	void setKey(int n, Key* key);
	void deleteAll();
};

#endif