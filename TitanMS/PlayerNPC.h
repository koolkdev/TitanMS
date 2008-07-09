#ifndef PLAYERNPC_H
#define PLAYERNPC_H


#include <hash_map>
#include <vector>
#include "angelscript.h"
#include <string>

using namespace std;
using namespace stdext;

#define YES 1
#define NO 0
#define ACCEPT 1
#define DECLINE 0

class Player;
class Packet;

class PlayerNPC {
private:
	int npcid;
	Player* player;
	string text;
	string sname;
	int state;
	int selected;
	bool cend;
	int getnum;
	char gettext[101];
	bool isquest;
	hash_map <string, int> vars;
public:
	PlayerNPC(int npcid, Player* player, bool isquest = 0, string sname = "");
	~PlayerNPC();
	void addText(string &atext){
		text += atext;
	}

	void addChar(char cha){
		char temp[2]={0};
		temp[0] = cha;
		addText(string(temp));
	}

	Packet npcPacket(char type);
	void setScriptName(string sname){
		this->sname = sname;
	}
	string getScriptName(){
		return sname;
	}
	void sendSimple();
	void sendYesNo();
	void sendNext();
	void sendBackNext();
	void sendBackOK();
	void sendOK();
	void sendAcceptDecline();
	void sendGetText();
	void sendGetNumber(int def, int min, int max);
	void sendStyle(asIScriptArray* styles);
	bool isQuest(){
		return isquest;
	}
	void setState(int state){
		this->state = state;
	}
	int getState(){
		return state;
	}
	int getSelected(){
		return selected;
	}
	void setSelected(int selected){
		this->selected=selected;
	}
	void setGetNumber(int num){
		this->getnum = num;
	}
	int getNumber(){
		return getnum;
	}
	void setGetText(string &text){
		strcpy_s(this->gettext, strlen((char*)text.c_str())+1, (char*)text.c_str());
	}
	char* getText(){
		return gettext;
	}
	void end(){
		cend=true;
	}
	bool isEnd(){
		return cend;
	}
	int getNpcID(){
		return npcid;
	}
	Player* getPlayer(){
		return player;
	}
	void teleport(int mapid);
	int getItemAmount(int itemid);
	int getMesos();
	void giveItem(int itemid, short amount);
	void giveMesos(int amount);
	int getPlayerMap();
	void setStyle(int id);
	void setVariable(string &name, int val){
		vars[name] = val;
	}
	int getVariable(string &name){
		if(vars.find(name) == vars.end())
			return 0;
		else
			return vars[name];
	}
	int getPlayerHair();
	int getPlayerFace();
	void setPlayerHP(short hp);
	short getPlayerHP();
	void addQuest(short questid);
	void endQuest(short questid);
	void giveEXP(int exp);
	int getPlayerLevel();
	void setPlayerLevel(int level);
	int getPlayerJob();
	void setPlayerJob(int job);
	int getPlayerAp();
	int getPlayerSp();
	void setPlayerAp(int a);
	void setPlayerSp(int a);
	int getPlayerStr();
	int getPlayerDex();
	int getPlayerInt();
	int getPlayerLuk();
	void setPlayerStr(int a);
	void setPlayerDex(int a);
	void setPlayerInt(int a);
	void setPlayerLuk(int a);


};

#endif