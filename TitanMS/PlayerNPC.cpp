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

#include "PlayerNPC.h"
#include "AngelScriptEngine.h"
#include <string>
#include "Player.h"
#include "PacketCreator.h"
#include "Channel.h"
#include "Maps.h"
#include "Map.h"
#include "Inventory.h"
#include "PlayerInventories.h"
#include "PlayerQuests.h"

/*
void NPCs::showNPCs(Player* player){
	for(unsigned int i=0; i<info[player->getMap()].size(); i++)
		NPCPacket::showNPC(player, info[player->getMap()][i], i);
}

void NPCs::handleNPC(Player* player, unsigned char* packet){
	if(player->getNPC() != NULL)
		return;
	int npcid = info[player->getMap()][getInt(packet)-100].id;
	NPC* npc = new NPC(npcid, player);
	if(!AngelScriptEngine::handleNPC(npc)) npc->end();

	if(npc->isEnd())
		delete npc;
}

void NPCs::handleQuestNPC(Player* player, int npcid, string name){
	if(player->getNPC() != NULL)
		return;
	NPC* npc = new NPC(npcid, player, 1, name);
	if(!AngelScriptEngine::handleNPC(npc)) npc->end();
	
	if(npc->isEnd())
		delete npc;
}
*/
PlayerNPC::PlayerNPC(int npcid, Player* player, bool isquest, string sname){
	this->isquest = isquest;
	getnum=0;
	gettext = "";
	state=0;
	selected=-1;
	cend = false;
	this->npcid = npcid;
	this->player = player;
	text = "";
	this->sname = sname;
	player->setNPC(this);
}

PlayerNPC::~PlayerNPC(){
	player->setNPC(NULL);
}

void PlayerNPC::sendSimple(){
	player->send(PacketCreator().sendSimple(npcid, text));
	text = "";
}
void PlayerNPC::sendYesNo(){
	player->send(PacketCreator().sendYesNo(npcid, text));
	text = "";
}
void PlayerNPC::sendNext(){
	player->send(PacketCreator().sendBackNext(npcid, text, false, true));
	text = "";
}
void PlayerNPC::sendBackNext(){
	player->send(PacketCreator().sendBackNext(npcid, text, true, true));
	text = "";
}
void PlayerNPC::sendBackOK(){
	player->send(PacketCreator().sendBackNext(npcid, text, true, false));
	text = "";
}
void PlayerNPC::sendOK(){
	player->send(PacketCreator().sendBackNext(npcid, text, false, false));
	text = "";
}

void PlayerNPC::sendAcceptDecline(){
	player->send(PacketCreator().sendAcceptDecline(npcid, text));
	text = "";
}

void PlayerNPC::sendGetText(){
	player->send(PacketCreator().sendGetText(npcid, text));
	text = "";
}

void PlayerNPC::sendGetNumber(int def, int min, int max){
	player->send(PacketCreator().sendGetNumber(npcid, text, def, min, max));
	text = "";
}

void PlayerNPC::sendStyle(asIScriptArray* styles){
	int size = styles->GetElementCount();
	int style[100];
	for(int i = 0; i < size; i++ )
	{
		int* a = (int*)styles->GetElementPointer(i);
		style[i] = *a;
	}
	player->send(PacketCreator().sendStyle(npcid, text, style, size));
	text = "";
}

void PlayerNPC::teleport(int mapid){
	Map* to = player->getChannel()->getMaps()->getMap(mapid);
	if(to != NULL)
		player->changeMap(to);
}
int PlayerNPC::getItemAmount(int itemid){
	return player->getInventories()->getItemAmount(itemid);
}
int PlayerNPC::getMesos(){
	return player->getMesos();
}

void PlayerNPC::giveItem(int itemid, short amount){
	player->getInventories()->giveItem(itemid, amount);
	player->send(PacketCreator().itemGainChat(itemid, amount));
}

void PlayerNPC::giveMesos(int amount){
	player->addMesos(amount);
	player->send(PacketCreator().mesosGainChat(amount));
}
int PlayerNPC::getPlayerMap(){
	return player->getMap()->getID();
}
void PlayerNPC::setPlayerHP(short hp){
	player->setHP(hp);
}
short PlayerNPC::getPlayerHP(){
	return player->getHP();
}
int PlayerNPC::getPlayerHair(){
	return player->getHair();
}
int PlayerNPC::getPlayerFace(){
	return player->getFace();
}

void PlayerNPC::addQuest(short questid){
	player->getQuests()->giveQuest(questid, npcid);
}

void PlayerNPC::endQuest(short questid){
	player->getQuests()->completeQuest(questid);
}

void PlayerNPC::giveEXP(int exp){
	player->addExp(exp);
}

int PlayerNPC::getPlayerLevel(){
	return player->getLevel();	
}
void PlayerNPC::setPlayerLevel(int level){
	player->setLevel(level);
}
int PlayerNPC::getPlayerJob(){
	return player->getJob();	
}
void PlayerNPC::setPlayerJob(int job){
	player->setJob(job);
}

void PlayerNPC::setStyle(int id){
	if(id/10000 == 0){
		player->setSkin((char)id);
	}
	else if(id/10000 == 2){
		player->setFace(id);
	}
	else if(id/10000 == 3){
		player->setHair(id);
	}
	player->send(PacketCreator().updatePlayer(player));
}

int PlayerNPC::getPlayerStr(){
	return player->getStr();	
}
int PlayerNPC::getPlayerDex(){
	return player->getDex();	
}
int PlayerNPC::getPlayerInt(){
	return player->getInt();	
}
int PlayerNPC::getPlayerLuk(){
	return player->getLuk();	
}
void PlayerNPC::setPlayerStr(int a){
	player->setStr(a);	
}
void PlayerNPC::setPlayerDex(int a){
	player->setDex(a);	
}
void PlayerNPC::setPlayerInt(int a){
	player->setInt(a);	
}
void PlayerNPC::setPlayerLuk(int a){
	player->setLuk(a);
}
int PlayerNPC::getPlayerSp(){
	return player->getSP();	
}
int PlayerNPC::getPlayerAp(){
	return player->getAP();	
}
void PlayerNPC::setPlayerSp(int a){
	player->setSP(a);
}
void PlayerNPC::setPlayerAp(int a){
	player->setAP(a);
}