#include "PlayerLoginHandler.h"
#include "PlayerLogin.h"
#include "PacketReader.h"
#include "PacketCreator.h"
#include "MySQLM.h"
#include "Character.h"
#include "Worlds.h"
#include "World.h"
#include "Channels.h"
#include "Channel.h"
#include "Tools.h"
using namespace Tools;

void PlayerLoginHandler::handle(PacketReader* packet){
	switch(packet->getHeader()){
		case PING: pingHandle(packet); break;
		case LOGIN_REQUEST: loginRequestHandle(packet); break;
		case CHANNEL_SELECT: channelSelectHandle(packet); break;
		case WORLD_SELECT: worldSelectHandle(packet); break;
		case HANDLE_LOGIN: loginHandle(packet); break;
		case SHOW_WORLD: showWorldRequestHandle(packet); break;
		case WORLD_BACK: showWorldRequestHandle(packet); break;
		case CHARACTER_SELECT: characterSelectHandle(packet); break;
		case NAME_CHECK: nameSelectHandle(packet); break;
		case CREATE_CHARACTER: createCharacterHandle(packet); break;
		case DELETE_CHARACTER: deleteCharacterHandle(packet); break;
		case LOGIN_BACK: loginBackHandle(packet); break;
	}
}

void PlayerLoginHandler::pingHandle(PacketReader* packet){
}
void PlayerLoginHandler::loginRequestHandle(PacketReader* packet){
	string username, password;
	username = packet->readString();
	password = packet->readString();
	int s = player->checkLogin(username, password);
	if(s == 1){
		player->setUserid(MySQL::getInstance()->getUserID((char*)username.c_str()));
		player->setIDs(MySQL::getInstance()->getCharactersIDs(player->getUserid()));
		for(int i=0; i<(int)player->getIDs()->size(); i++){
			if(Worlds::getInstance()->isPlayerConnected((*player->getIDs())[i])){
				s=-2;
				break;
			}
		}
	}
	if(s == 1){
		printf("%s logged in.\n", (char*)username.c_str());
		player->setPin(MySQL::getInstance()->getInt("users", player->getUserid(), "pin"));
		int pin = player->getPin();
		if(pin == -1)
			player->setStatus(1); // New PIN
		else
			player->setStatus(2); // Ask for PIN
		player->setGender(MySQL::getInstance()->getInt("users", player->getUserid(), "gender"));
		player->send(PacketCreator().loginConnect(player->getGender(), username, (player->getStatus() == 1)));
		player->loadCharacters();
	}
	else{
		short error = 0;
		switch(s){
			case -2: error=7;break;
			case -1: error=5;break;
			case 0: error=4;break;
		}
		player->send(PacketCreator().loginError(error));
	}
	
}
void PlayerLoginHandler::channelSelectHandle(PacketReader* packet){
	packet->read();
	player->setChannel(packet->read());
	vector <Character* > toshow;
	for(int i=0; i<(int)player->getCharacters()->size(); i++)
		if((*player->getCharacters())[i]->getWorld() == player->getServer())
			toshow.push_back((*player->getCharacters())[i]);
	player->send(PacketCreator().showCharacters(&toshow));
}
void PlayerLoginHandler::worldSelectHandle(PacketReader* packet){
	player->setServer(packet->read());
	player->send(PacketCreator().showChannels());
}
void PlayerLoginHandler::loginHandle(PacketReader* packet){
	int status = player->getStatus();
	if(status == 1)
		player->send(PacketCreator().loginProcess(0x01));
	else if(status == 2){
		//player->send(PacketCreator().loginProcess(0x04));
		//player->setStatus(3);
		player->send(PacketCreator().loginProcess(0x00));
		player->setStatus(4);
	}
	//else if(status == 3)
		//checkPin(player, packet);
	else if(status == 4)
		player->send(PacketCreator().loginProcess(0x00));
}
void PlayerLoginHandler::showWorldRequestHandle(PacketReader* packet){
	for(int i=0; i<Worlds::getInstance()->getWorldsCount(); i++){
		player->send(PacketCreator().showWorld(Worlds::getInstance()->getWorld(i)));
	}
	player->send(PacketCreator().endWorlds());

}
void PlayerLoginHandler::characterSelectHandle(PacketReader* packet){
	int id = packet->readInt();
	if(player->getCharacter(id) == NULL) return;
	MySQL::getInstance()->setString("characters", "ip", id, (char*)player->getIP().c_str());
	Channel* channel = Worlds::getInstance()->getWorld(player->getServer())->getChannels()->getChannel(player->getChannel());
	if(channel == NULL) return;
	player->send(PacketCreator().connectChannel(id, channel->getPort()));
}
void PlayerLoginHandler::nameSelectHandle(PacketReader* packet){
	string name = packet->readString();
	player->send(PacketCreator().checkName(name, MySQL::getInstance()->isString("characters", "name", name)));
}
void PlayerLoginHandler::createCharacterHandle(PacketReader* packet){
	string name = packet->readString();
	if(MySQL::getInstance()->isString("characters", "name", name))
		return;
	int face = packet->readInt();
	int hair = packet->readInt();
	int haircolor = packet->readInt();
	int skin = packet->readInt();
	int e1 = packet->readInt();
	int e2 = packet->readInt();
	int e3 = packet->readInt();
	int e4 = packet->readInt();
	int gender = packet->read();
	int str=packet->read(),dex=packet->read(),intt=packet->read(), luk=packet->read();
	if(str + dex + intt + luk != 25 || str < 4 || dex < 4 || intt < 4 || luk < 4)
		return;

	hair += haircolor;
	int id = MySQL::getInstance()->setChar(player->getUserid(), name, face, hair, skin, gender, str, dex, intt, luk, player->getServer());
	MySQL::getInstance()->createEquip(e1, 0x05, id);
	MySQL::getInstance()->createEquip(e2, 0x06, id);
	MySQL::getInstance()->createEquip(e3, 0x07, id);
	MySQL::getInstance()->createEquip(e4, 0x0b, id);
	player->addID(id);
	player->loadCharacters();
	Character* character = player->getCharacter(id);
	if(character == NULL) return;
	player->send(PacketCreator().addCharacter(character));
}
void PlayerLoginHandler::deleteCharacterHandle(PacketReader* packet){
	int date = packet->readInt();
	int id = packet->readInt();
	Character* character =  player->getCharacter(id);
	if(character == NULL)
		return;
	MySQL::getInstance()->deletePlayer(id);
	player->removeID(id);
	player->removeCharacter(id);
	player->send(PacketCreator().removeCharacter(id));
}
void PlayerLoginHandler::loginBackHandle(PacketReader* packet){
	player->send(PacketCreator().logBack());
}