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

#include "PacketCreator.h"
#include "Worlds.h"
#include "World.h"
#include "Channels.h"
#include "Character.h"
#include "CharacterEquip.h"
#include "PacketWriter.h"
#include <sstream>
#include <hash_map>
using namespace stdext;

PacketWriter* PacketCreator::loginError(short errorid){
	pw.writeShort(LOGIN_PROCESS);

	pw.writeShort(errorid);
	pw.writeInt(0);
	return &pw;
}
PacketWriter* PacketCreator::loginProcess(char type){
	pw.writeShort(PIN);

	pw.write(type);
	return &pw;
}
PacketWriter* PacketCreator::loginConnect(char gender, string username, bool firstlogin){
	pw.writeShort(LOGIN_PROCESS);

	pw.writeInt(0);
	pw.writeShort(0);
	pw.writeBytes("2A7949");
	pw.write(0);
	if(firstlogin)
		pw.write(0x0A);
	else
		pw.write(gender);
	pw.writeBytes("0465");
	pw.writeString(username);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeBytes("000000A6B89C2B4CC701");
	pw.writeInt(8);
	pw.writeShort(0);
	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::processOk(){
	pw.writeShort(CHECK_NAME);

	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::showWorld(World* world){
	pw.writeShort(SHOW_WORLDS);

	pw.write(world->getID());
	pw.writeString(Worlds::getName(world->getID()));
	pw.write(0); //Type 2-new
	pw.writeShort(0);
	pw.write(100);
	pw.write(0);
	pw.write(100);
	pw.writeShort(0);
	pw.write(world->getChannels()->getChannelsCount());
	for(int i=0; i<world->getChannels()->getChannelsCount(); i++){
		stringstream out;
		out << i+1;
		string name = Worlds::getName(world->getID()) + "-" + out.str();
		pw.writeString(name);
		pw.writeInt(0x0); // Pop - TODO
		pw.write(world->getID());
		pw.writeShort(i);
	}
	pw.writeShort(0);
	return &pw;
}
PacketWriter* PacketCreator::endWorlds(){
	pw.writeShort(SHOW_WORLDS);

	pw.write(-1);
	return &pw;
}
PacketWriter* PacketCreator::showChannels(){
	pw.writeShort(WORLD_STATUS);

	pw.writeShort(0);
	return &pw;
}
PacketWriter* PacketCreator::showCharacters(vector <Character*>& chars){
	pw.writeShort(SHOW_CHARACTERS);

	pw.write(0);
	pw.write(chars.size());
	for(int i=0; i<(int)chars.size(); i++){
		showCharacter(chars[i]);
	}
	pw.writeInt(3); //
	return &pw;
}
void PacketCreator::showCharacter(Character* character){

	pw.writeInt(character->getID());
	pw.writeString(character->getName(), 12);
	pw.write(0);
	pw.write(character->getGender());
	pw.write(character->getSkin());
	pw.writeInt(character->getFace());
	pw.writeInt(character->getHair());
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.write(character->getLevel());
	pw.writeShort(character->getJob());
	pw.writeShort(character->getStr());
	pw.writeShort(character->getDex());
	pw.writeShort(character->getInt());
	pw.writeShort(character->getLuk());
	pw.writeShort(character->getHP());
	pw.writeShort(character->getMaxHP());
	pw.writeShort(character->getMP());
	pw.writeShort(character->getMaxMP());
	pw.writeShort(character->getAP());
	pw.writeShort(character->getSP());
	pw.writeInt(character->getExp());
	pw.writeShort(character->getFame());
	pw.writeInt(character->getMap());
	pw.write(character->getMappos());
	pw.write(character->getGender());
	pw.write(character->getSkin());
	pw.writeInt(character->getFace());
	pw.write(1);
	pw.writeInt(character->getHair());
	hash_map<char, int> visible;
	hash_map<char, int> invisible;
	int cWeapon = 0;
	vector <CharacterEquip*>* equips = character->getEquips();
	for(int j=0; j<(int)equips->size(); j++){
		if((*equips)[j]->getPos() != -111){ // For cash weapons
			char epos = (*equips)[j]->getPos()*-1;
			if(epos < 100){
				if(visible.find(epos) == visible.end())
					visible[epos] = (*equips)[j]->getID();
				else
					invisible[epos] = (*equips)[j]->getID();
			}
			else { // cash
				epos-=100;
				if(visible.find(epos) != visible.end())
					invisible[epos] = visible[epos];
				visible[epos] = (*equips)[j]->getID();
			}
		}
		else{
			cWeapon = (*equips)[j]->getID();
		}
	}
	// Show visible items
	for (hash_map<char,int>::iterator iter = visible.begin(); iter != visible.end(); iter++){
		pw.write(iter->first);
		pw.writeInt(iter->second);
	}
	// Show unvisible items
	pw.write(-1);
	for (hash_map<char,int>::iterator iter = invisible.begin(); iter != invisible.end(); iter++){
		pw.write(iter->first);
		pw.writeInt(iter->second);
	}
	pw.write(-1);
	pw.writeInt(cWeapon);
	pw.writeInt(0); // Pet
	pw.writeInt(0);
	pw.writeInt(0);
	pw.write(1);
	// Ranks:
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
	pw.writeInt(0);
}
PacketWriter* PacketCreator::addCharacter(Character* character){
	pw.writeShort(ADD_CHARACTER);

	pw.write(0);
	showCharacter(character);
	pw.writeLong(0);
	return &pw;
}
PacketWriter* PacketCreator::checkName(string name, bool ok){
	pw.writeShort(CHECK_NAME);

	pw.writeString(name);
	pw.write(ok);
	return &pw;
}
PacketWriter* PacketCreator::removeCharacter(int characterid){
	pw.writeShort(REMOVE_CHARACTER);

	pw.writeInt(characterid);
	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::connectChannel(int charid, short port){
	pw.writeShort(CHANNEL_INFO);

	pw.writeShort(0);

	IP* ip = Worlds::getInstance()->getIP();

	pw.write(ip->p1); // IP
	pw.write(ip->p2);
	pw.write(ip->p3);
	pw.write(ip->p4); 

	pw.writeShort(port);
	pw.writeInt(charid);
	pw.writeInt(0);
	pw.write(0);
	return &pw;
}
PacketWriter* PacketCreator::logBack(){
	pw.writeShort(LOG_BACK);

	pw.write(1);
	return &pw;
}