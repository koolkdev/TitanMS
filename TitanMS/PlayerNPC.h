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

#ifndef PLAYERNPC_H
#define PLAYERNPC_H


#include <hash_map>
#include <vector>
#include <string>

using namespace std;
using namespace stdext;
class asIScriptArray;

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
	string gettext;
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
		gettext = text;
	}
	string getText(){
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

	static const int BACK_NEXT = 0x0;
	static const int YES_NO = 0x1;
	static const int GET_TEXT = 0x2;
	static const int GET_NUMBER = 0x3;
	static const int SIMPLE = 0x4;
	static const int STYLE = 0x7;
	static const int ACCEPT_DECLINE = 0x12;


};

#endif