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

#include "../Connection/PacketHandler.h"
#include "PlayerLogin.h"
#include "PlayerLoginHandler.h"
#include "Character.h"
#include "CharacterEquip.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "MySQLM.h"

using namespace mysqlpp;

int strval(string& str);
PlayerLogin::PlayerLogin () {
	status=0;
	handler = new PlayerLoginHandler(this);
}
PlayerLogin::~PlayerLogin() {
	delete handler;
	for(int i=0; i<(int)characters.size(); i++)
		delete characters[i];
}
void PlayerLogin::handleRequest(unsigned char* buf, int len){
	try{
		handler->handle(PacketReader(buf, len, userid));
	}
#ifdef _DEBUG
	catch(PacketHandlingError ph){
		cout << ph.getError();
	}
	catch (BadQuery er ) {
		printf("%s\n", er.what());
	}
#endif
	catch(...){
		//TODO
	}
}

int PlayerLogin::checkLogin(string username, string password){
	if(!MySQL::getInstance()->isString("users","username", username)){
		return -1;
	}
	string rpassword = MySQL::getInstance()->getString("users", "username",(char*)username.c_str(),"password");
	if(password == string(rpassword))
		return 1;
	return 0;
}

void PlayerLogin::send(PacketWriter* packet){
	unsigned char bytes [MAX_LENGTH];
	memcpy_s(bytes, packet->getLength(), packet->getBytes(), packet->getLength());
	sendPacket(bytes, packet->getLength());
}
void PlayerLogin::loadCharacters(){
	for(int i=0; i<(int)ids.size(); i++){
		int id = ids[i];
		if(getCharacter(id) != NULL)
			continue;
		Character* character = new Character();
		character->setID(id);
		string values[29];
		MySQL::getInstance()->getCharacter(id, values);
		character->setName(values[1]);
		character->setWorld(strval(values[3]));	
		character->setLevel(strval(values[4]));
		character->setJob(strval(values[5]));
		character->setStr(strval(values[6]));
		character->setDex(strval(values[7]));
		character->setInt(strval(values[8]));
		character->setLuk(strval(values[9]));
		character->setHP(strval(values[10]));
		character->setMaxHP(strval(values[11]));
		character->setMP(strval(values[12]));
		character->setMaxMP(strval(values[13]));
		character->setAP(strval(values[14]));
		character->setSP(strval(values[15]));
		character->setExp(strval(values[16]));
		character->setFame(strval(values[17]));
		character->setMap(strval(values[18]));
		character->setMappos(strval(values[19]));	
		character->setGender(strval(values[20]));
		character->setSkin(strval(values[21]));
		character->setFace(strval(values[22]));
		character->setHair(strval(values[23]));
		int equips [100][2];
		int count = MySQL::getInstance()->showEquips(id, equips);
		for(int i=0; i<count; i++){
			character->addEquip(new CharacterEquip(equips[i][0], equips[i][1]));
		}
		characters.push_back(character);
	}
}

Character* PlayerLogin::getCharacter(int charid){
	for(int i=0; i<(int)characters.size(); i++)
		if(characters[i]->getID() == charid)
			return characters[i];
	return NULL;
}
void PlayerLogin::removeCharacter(int id){
	for(int i=0; i<(int)characters.size(); i++){
		if(id == characters[i]->getID()){
			delete characters[i];
			characters.erase(characters.begin()+i);
			break;
		}
	}
}