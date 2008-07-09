#include "PlayerHandler.h"
#include "PacketReader.h"
#include "PacketWriter.h"
#include "PacketCreator.h"
#include "DataProvider.h"
#include "Player.h"
#include "Channel.h"
#include "Maps.h"
#include "Skill.h"
#include "PlayerSkills.h"
#include "Map.h"
#include "MapMobs.h"
#include "Pet.h"
#include "PlayerInventories.h"
int strval(string& str);

void PlayerHandler::chatCommandHandle(PacketReader* packet){
}
void PlayerHandler::useChatHandle(PacketReader* packet){
	string msg = packet->readString();
	if(msg.substr(0, 1) == "!" && player->isGM()){
		string command = msg.substr(1, msg.find(" ")-1);
		if(command == "map"){
			int mapid = strval(msg.substr(msg.find(" ")));
			Map* map = player->getChannel()->getMaps()->getMap(mapid);
			if(map != NULL)
				player->changeMap(map);
		}
		else if(command == "shop"){
			ShopData* shop = DataProvider::getInstance()->getShopData(9999999);
			if(shop != NULL){
				player->setShop(shop);	
				player->send(PacketCreator().showShop(shop));	
				return;	
			}
		}
		else if(command == "item"){
			int p = msg.find(" ");
			int ap = msg.substr(p+1).find(" ");
			int itemid = 0;
			int amount = 1;
			if(ap == -1)
				itemid = strval(msg.substr(p));
			else{
				itemid = strval(msg.substr(p+1, ap));
				amount = strval(msg.substr(ap+p+2));
			}
			player->getInventories()->giveItem(itemid, amount); 
		}
		else if(command == "mesos"){
			int mesos = strval(msg.substr(msg.find(" ")));
			player->addMesos(mesos);
		}
		else if(command == "level"){
			int level = strval(msg.substr(msg.find(" ")));
			player->setLevel(level);
			player->setExp(0);
		}
		else if(command == "job"){
			int job = strval(msg.substr(msg.find(" ")));
			player->setJob(job);
		}
		else if(command == "sp"){
			int sp = strval(msg.substr(msg.find(" ")));
			player->addSP(sp);
		}
		else if(command == "ap"){
			int ap = strval(msg.substr(msg.find(" ")));
			player->addAP(ap);
		}
		else if(command == "str"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addSTR(s);
		}
		else if(command == "dex"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addDEX(s);
		}
		else if(command == "int"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addINT(s);
		}
		else if(command == "luk"){
			int s = strval(msg.substr(msg.find(" ")));
			player->addLUK(s);
		}
		else if(command == "hp"){
			int hp = strval(msg.substr(msg.find(" ")));
			player->setHP(hp);
		}
		else if(command == "mp"){
			int mp = strval(msg.substr(msg.find(" ")));
			player->setMP(mp);
		}
		else if(command == "heal"){
			player->setHP(player->getMaxHP());
			player->setMP(player->getMaxMP());
		}
		else if(command == "maxhp"){
			int maxhp = strval(msg.substr(msg.find(" ")));
			player->setMaxHP(maxhp);
		}
		else if(command == "maxmp"){
			int maxmp = strval(msg.substr(msg.find(" ")));
			player->setMaxMP(maxmp);
		}
		else if(command == "mount"){
			Skill* skill = player->getSkills()->getSkill(1004);
			skill->setMasterLevel(1);
			skill->setLevel(1);
			player->send(PacketCreator().updateSkill(skill));
		}
		else if(command == "summon"){
			int p = msg.find(" ");
			int ap = msg.substr(p+1).find(" ");
			int mob = 0;
			int amount = 1;
			if(ap == -1)
				mob = strval(msg.substr(p));
			else{
				mob = strval(msg.substr(p+1, ap));
				amount = strval(msg.substr(ap+p+2));
			}
			for(int i=0; i<amount; i++)
				player->getMap()->getMobs()->spawnMob(mob, player->getPosition().x, player->getPosition().y, -1, false);
		}
		else if(command == "test"){
			Pet* pet = player->getPet();
			int a = strval(msg.substr(msg.find(" ")));
			pet->setFullness(a);
			player->send(PacketCreator().updatePet(pet, true));
			//player->send(PacketCreator().showPlayerEffect(4));
			//player->getMap()->send(PacketCreator().showEffect(player->getID(), 4));
		}
	}
	else{
		player->getMap()->send(PacketCreator().showChatMassage(player->getID(), msg, player->isGM() == 1));
	}

}