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

#include "DataProvider.h"
#include "Worlds.h"
#include "World.h"
#include "Channels.h"
#include "Channel.h"
#include "Players.h"
#include "Player.h"
#include "PacketCreator.h"
#include <iostream>
#include <string>
using namespace std;


Worlds* Worlds::instance = NULL;
int Worlds::openPort = 8888;
const char* Worlds::names [] = {"Scania", "Bera", "Broa", "Windia", "Khaini", "Bellocan", "Mardia", "Kradia", "Yellonde", "Demethos", "Elnido", "Kastia", "Judis", "Arkenia", "Plana", "Galicia", "Kalluna", "Stius", "Croa", "Zenith", "Tespia"};

void Worlds::newWorld(int id, int channels){
	worlds.push_back(new World(id, channels));
}

Channel* Worlds::getChannelByPort(int port){
	for(int i=0; i<(int)worlds.size(); i++){
		Channels* channels = worlds[i]->getChannels();
		for(int i=0; i<channels->getChannelsCount(); i++)
			if(channels->getChannelPort(i) == port)
				return channels->getChannel(i);
	}
	return NULL;
}

bool Worlds::isPlayerConnected(int charid){
	for(int i=0; i<(int)worlds.size(); i++)
		if(worlds[i]->isPlayerConnected(charid))
			return true;
	return false;
}
void Worlds::commandListener(){
	string cmd;
	while(getline(cin,cmd) || true){
		string rep = handleCommand(cmd);
		if(rep != ""){
			cout << rep << endl;
		}
	}
}
string Worlds::handleCommand(string cmd){
	string command = cmd.substr(0, cmd.find(" "));
	if(command == "commands" || command == "help"){
		return "Commands: \n" + \
			string("notice [msg]\n") + \
			"dc [player-name]";
	}	
	else if(command == "notice"){
		string notice;
		if(cmd.find(" ") == -1)
			notice = "";
		else
			notice = cmd.substr(cmd.find(" "));
		for(int i=0; i<(int)worlds.size(); i++){
			for(int j=0; j<worlds[i]->getChannels()->getChannelsCount(); j++){
				worlds[i]->getChannels()->getChannel(j)->setNotice(notice);
				worlds[i]->getChannels()->getChannel(j)->getPlayers()->send(PacketCreator().showMessage(notice, 4, 0, true));
			}
		}	
	}
	else if(command == "dc" || command == "kick"){
		if(cmd.find(" ") == -1) return "Useage:" + command + " [player-name]";
		string playername = cmd.substr(cmd.find(" ")+1);
		for(int i=0; i<(int)worlds.size(); i++){
			Player* player = worlds[i]->getPlayerByName(playername);
			if(player != NULL){
				player->disconnect();
				return "";
			}
		}	
		return "Can't find player " + playername;
	}
	else{
		return "Unknown command: " + command;
	}
	return "";
}