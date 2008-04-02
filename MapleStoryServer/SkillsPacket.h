#ifndef SKILLSPACK_H
#define SKILLSPACK_H

class Player;

#include <vector>
using namespace std;

short getShort(unsigned char* buf);
int getInt(unsigned char* buf);
void getString(unsigned char* buf, int len, char* out);

class SkillsPacket {
public:
	static void addSkill(Player* player, int skillid, int level);
	static void useSkill(Player* player, vector <Player*> players, int skillid, int time, char type1, char type2, char type3, char type4, char type5, char type6, char type7, char type8, vector <int> values, bool is);
	static void endSkill(Player* player, vector <Player*> players, char type1, char type2, char type3, char type4, char type5, char type6, char type7, char type8, bool is);
	static void healHP(Player* player, short hp);
};

#endif