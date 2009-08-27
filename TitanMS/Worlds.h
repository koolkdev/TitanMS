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

#ifndef WORLDS_H
#define WORLDS_H

#include <vector>
#include <string>
#include "Tools.h"

using namespace std;

class World;
class Channel;
class Selector;

class Worlds {
private:
	static const char* names [];
	vector <World*> worlds;
	static Worlds* instance;
	int channelc;
	int worldc;
	string password;
	string defaultNotice;
	Selector* selector;
	IP ip;
	int exp;
	static int openPort;
public:
	Worlds(){
		instance = this;
	}
	void start(Selector* s){
		for(int i=0; i<worldc; i++)
			newWorld(i, channelc);
	}
	void newWorld(int id, int channels);

	static Worlds* getInstance(){
		if(instance == NULL)
			return new Worlds();
		return instance;
	}
	void setChannelsCount(int c){
		channelc = c;
	}
	void setWorldsCount(int c){
		worldc = c;
	}
	void setIP(IP ip){
		this->ip = ip;
	}
	IP* getIP(){
		return &ip;
	}
	int getWorldsCount(){
		return worlds.size();
	}	
	void setEXP(int e){
		exp = e;
	}
	int getEXP(){
		return exp;
	}
	World* getWorld(unsigned int id){
		if(id > worlds.size())
			return NULL;
		return worlds[id];
	}
	static int getOpenPort(){
		return openPort++;
	}
	Selector* getSelector(){
		return selector;
	}
	Channel* getChannelByPort(int port);
	static string getName(int id){
		if(id>=0 && id < 21)
			return string(names[id]);
		return string("");
	}
	void setPassword(string pass){
		password = pass;
	}
	string getPassword(){
		return password;
	}
	void setDefaultNotice(string a){
		defaultNotice = a;
	}
	string getDefaultNotice(){
		return defaultNotice;
	}
	void commandListener();
	bool isPlayerConnected(int charid);
	string handleCommand(string cmd);
};

#endif