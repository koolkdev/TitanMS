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

#ifndef PLAYERLOGIN_H
#define PLAYERLOGIN_H

#include "../Connection/AbstractPlayer.h"
#include <string>

class PacketWriter;
class Character;
template <class T> class Handler;
class PlayerLoginHandler;

class PlayerLogin:public AbstractPlayer {
public:
	PlayerLogin ();

	~PlayerLogin();
	void handleRequest(unsigned char* buf, int len);
	void setUserid(int id){
		this->userid=id;
	}
	int getUserid(){
		return this->userid;
	}
	void setStatus(int status){
		this->status=status;
	}
	int getStatus(){
		return this->status;
	}
	void setPin(int pin){
		this->pin=pin;
	}
	int getPin(){
		return this->pin;
	}
	void setGender(char gender){
		this->gender=gender;
	}
	char getGender(){
		return this->gender;
	}
	void setServer(int server){
		this->server=server;
	}
	int getServer(){
		return this->server;
	}
	void setChannel(int channel){
		this->channel=channel;
	}
	int getChannel(){
		return this->channel;
	}
	void setIDs(vector<int> ids){
		this->ids = ids;
	}
	void addID(int id){
		ids.push_back(id);
	}
	vector<int>* getIDs(){
		return &ids;
	}
	vector <Character*>* getCharacters(){
		return &characters;
	}
	void removeCharacter(int id);
	void removeID(int id){
		for(int i=0; i<(int)ids.size(); i++)
			if(ids[0] == id){
				ids.erase(ids.begin()+i);
				break;
			}
	}
	Character* getCharacter(int charid);
	void loadCharacters();
	void send(PacketWriter* packet);
	int checkLogin(string username, string password);

private:
	int status;
	int userid;
	int pin;
	char gender;
	int server;
	int channel;
	vector <int> ids;
	vector <Character*> characters;
	Handler<PlayerLoginHandler>* handler;
	
};

class PlayerLoginFactory : public AbstractPlayerFactory {
public:
	AbstractPlayer* createPlayer(int port) {
		return new PlayerLogin();
	}
};

#endif