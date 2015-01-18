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

#include "Initializing.h"
#include "ChannelEventData.h"
#include "ChannelEventsData.h"
#include "ChannelEventsScriptsData.h"
#include "EventData.h"
#include "EventsData.h"
#include "EventsScriptsData.h"
#include "DropsData.h"
#include "DropData.h"
#include "MobsDropData.h"
#include "EquipData.h"
#include "EquipsData.h"
#include "ItemEffectData.h"
#include "ItemSummonData.h"
#include "ItemsData.h"
#include "ItemData.h"
#include "MapData.h"
#include "MapsData.h"
#include "MapNPCData.h"
#include "MapNPCsData.h"
#include "MapMobData.h"
#include "MapMobsData.h"
#include "MapReactorData.h"
#include "MapReactorsData.h"
#include "MapPortalData.h"
#include "MapPortalsData.h"
#include "MapFootholdData.h"
#include "MapFootholdsData.h"
#include "MapShipData.h"
#include "MobData.h"
#include "MobsData.h"
#include "PetsData.h"
#include "PetData.h"
#include "PetCommandData.h"
#include "NPCsScriptsData.h"
#include "PortalsData.h"
#include "QuestRewardData.h"
#include "QuestRewardsData.h"
#include "QuestsData.h"
#include "QuestData.h"
#include "QuestRequestData.h"
#include "QuestRequestsData.h"
#include "ReactorScriptsData.h"
#include "ReactorsData.h"
#include "ReactorData.h"
#include "ReactorStateData.h"
#include "ReactorsDropData.h"
#include "ShopData.h"
#include "ShopItemData.h"
#include "ShopsData.h"
#include "SkillData.h"
#include "SkillsData.h"
#include "SkillLevelData.h"
#include "windows.h"  
#include <string>
#include <stdlib.h>
#include "tchar.h" 
#include "Markup.h"
#include "Tools.h"
#include "Worlds.h"
#include "MySQLM.h"
using namespace Tools;
using namespace std;   
MobsDropData* TopData<MobsDropData,DropsData>::instance;
ReactorsDropData* TopData<ReactorsDropData,DropsData>::instance;
PortalsData* TopData<PortalsData,ByteCodeMemory<string>,string>::instance;
ChannelEventsScriptsData* TopData<ChannelEventsScriptsData,ByteCodeMemory<string>,string>::instance;
ChannelEventsData* TopData<ChannelEventsData,ChannelEventData, string>::instance;
EventsScriptsData* TopData<EventsScriptsData,ByteCodeMemory<string>,string>::instance;
EventsData* TopData<EventsData,EventData, string>::instance;
EquipsData* TopData<EquipsData,EquipData>::instance;
ItemsData* TopData<ItemsData,ItemData>::instance;
PetsData* TopData<PetsData,PetData>::instance;
ReactorsData* TopData<ReactorsData,ReactorData>::instance;
MapsData* TopData<MapsData,MapData>::instance;
MobsData* TopData<MobsData,MobData>::instance;
NPCsScriptsData* TopData<NPCsScriptsData,ByteCodeMemory<int>>::instance;
ReactorScriptsData* TopData<ReactorScriptsData,ByteCodeMemory<int>>::instance;
QuestsData* TopData<QuestsData,QuestData>::instance;
ShopsData* TopData<ShopsData,ShopData>::instance;
SkillsData* TopData<SkillsData,SkillData>::instance;
int strval(string& str);
	 
void wtoc(const WCHAR* Source, char* to)
{
int i = 0;

while(Source[i] != '\0')
{
to[i] = (CHAR)Source[i];
++i;
}
to[i] = '\0';
}

MobData* Initializing::loadMob(int id){
	char di[20];
	sprintf_s(di, 20, "Mobs/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Mob") return NULL;
	xml.IntoElem();
	MobData* mob = new MobData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "HP") mob->setMaxHP(atoi((char*)xml.GetData().c_str()));
		else if(s == "MP") mob->setMaxMP(atoi((char*)xml.GetData().c_str()));
		else if(s == "exp") mob->setExp(atoi((char*)xml.GetData().c_str()));
		else if(s == "Level") mob->setLevel(atoi((char*)xml.GetData().c_str()));
		else if(s == "WAtk") mob->setWAtk(atoi((char*)xml.GetData().c_str()));
		else if(s == "WDef") mob->setWDef(atoi((char*)xml.GetData().c_str()));
		else if(s == "MAtk") mob->setMAtk(atoi((char*)xml.GetData().c_str()));
		else if(s == "MDef") mob->setMDef(atoi((char*)xml.GetData().c_str()));
		else if(s == "Boss") mob->setIsBoss(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Color") mob->setColor(atoi((char*)xml.GetData().c_str()));
		else if(s == "BgColor") mob->setBgColor(atoi((char*)xml.GetData().c_str()));
		else if(s == "RemoveAfter") mob->setRemoveAfter(atoi((char*)xml.GetData().c_str()));
		else if(s == "HPRecovery") mob->setHPRecovery(atoi((char*)xml.GetData().c_str()));
		else if(s == "MPRecovery") mob->setMPRecovery(atoi((char*)xml.GetData().c_str()));
		else if(s == "DamagedByMob") mob->setDamagedByMob(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "FirstAttack") mob->setFirstAttack(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "DropItemDelay") mob->setDropDelay(atoi((char*)xml.GetData().c_str()));
		else if(s == "Summon"){
			xml.IntoElem();
			while(xml.FindElem())
				if(xml.GetTagName() == "ID")
					mob->getSummonData()->add(atoi((char*)xml.GetData().c_str()));
			xml.OutOfElem();
		}
	
	}
	MobsData::getInstance()->add(mob);
	return mob;
}

void Initializing::initializeMobs(){
	WIN32_FIND_DATAA FindFileData;
	
	HANDLE hFind = FindFirstFileA("Mobs\\*.xml", &FindFileData);
	MobsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		MobsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}

ItemData* Initializing::loadItem(int id){
	char di[20];
	sprintf_s(di, 20, "Items/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Item") return NULL;
	xml.IntoElem();
	ItemData* item = new ItemData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Name") item->setName(xml.GetData());
		else if(s == "Price") item->setPrice(atoi((char*)xml.GetData().c_str()));
		else if(s == "UnitPrice") item->setUnitPrice(strtod((char*)xml.GetData().c_str(), NULL));
		else if(s == "MaxSlot") item->setMaxPerSlot(atoi((char*)xml.GetData().c_str()));
		else if(s == "Quest") item->setQuest(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "TradeBlock") item->setTradeBlock(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "NotSale") item->setNotSale(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Only") item->setOnly(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "ExpireOnLogout") item->setExpireOnLogout(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Consume") item->setConsume(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Bullet") item->setBullet(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Effect"){
			xml.IntoElem();
			while(xml.FindElem()){
				string s = xml.GetTagName();
				if(s == "HP") item->getEffectData()->setHealHP(atoi((char*)xml.GetData().c_str()));
				else if(s == "MP") item->getEffectData()->setHealMP(atoi((char*)xml.GetData().c_str()));
				else if(s == "HPR") item->getEffectData()->setHealHPPer(atoi((char*)xml.GetData().c_str()));
				else if(s == "MPR") item->getEffectData()->setHealMPPer(atoi((char*)xml.GetData().c_str()));
				else if(s == "moveTo") item->getEffectData()->setMoveTo(atoi((char*)xml.GetData().c_str()));
				else if(s == "success") item->getEffectData()->setSuccess(atoi((char*)xml.GetData().c_str()));
				else if(s == "cursed") item->getEffectData()->setCursed(atoi((char*)xml.GetData().c_str()));
				else if(s == "Time") item->getEffectData()->setTime(atoi((char*)xml.GetData().c_str()));
				else if(s == "iSTR") item->getEffectData()->setStr(atoi((char*)xml.GetData().c_str()));
				else if(s == "iDEX") item->getEffectData()->setDex(atoi((char*)xml.GetData().c_str()));
				else if(s == "iINT") item->getEffectData()->setInt(atoi((char*)xml.GetData().c_str()));
				else if(s == "iLUK") item->getEffectData()->setLuk(atoi((char*)xml.GetData().c_str()));
				else if(s == "iMP") item->getEffectData()->setMP(atoi((char*)xml.GetData().c_str()));
				else if(s == "iHP") item->getEffectData()->setHP(atoi((char*)xml.GetData().c_str()));
				else if(s == "iWAtk") item->getEffectData()->setWAtk(atoi((char*)xml.GetData().c_str()));
				else if(s == "iWDef") item->getEffectData()->setWDef(atoi((char*)xml.GetData().c_str()));
				else if(s == "iMAtk") item->getEffectData()->setMAtk(atoi((char*)xml.GetData().c_str()));
				else if(s == "iMDef") item->getEffectData()->setMDef(atoi((char*)xml.GetData().c_str()));
				else if(s == "iAcc") item->getEffectData()->setAcc(atoi((char*)xml.GetData().c_str()));
				else if(s == "iAvo") item->getEffectData()->setAvo(atoi((char*)xml.GetData().c_str()));
				else if(s == "iHand") item->getEffectData()->setHand(atoi((char*)xml.GetData().c_str()));
				else if(s == "iSpeed") item->getEffectData()->setSpeed(atoi((char*)xml.GetData().c_str()));
				else if(s == "iJump") item->getEffectData()->setJump(atoi((char*)xml.GetData().c_str()));
				else if(s == "ItemOnMap") item->getEffectData()->setItemOnMap(atoi((char*)xml.GetData().c_str()));
				else if(s == "Msg") item->getEffectData()->setMsg((char*)xml.GetData().c_str());
				else if(s == "Morph") item->getEffectData()->setMorph(atoi((char*)xml.GetData().c_str()));
				else if(s == "Fullness") item->getEffectData()->setFullness(atoi((char*)xml.GetData().c_str()));
				else if(s == "Thaw") item->getEffectData()->setThaw(atoi((char*)xml.GetData().c_str()));
				else if(s == "recover") item->getEffectData()->setRecover(atoi((char*)xml.GetData().c_str()));
				else if(s == "randstat") item->getEffectData()->setRandStat(atoi((char*)xml.GetData().c_str()));
				else if(s == "MasterLevel") item->getEffectData()->setMasterLevel(atoi((char*)xml.GetData().c_str()));
				else if(s == "ReqSkillLevel") item->getEffectData()->setReqSkillLevel(atoi((char*)xml.GetData().c_str()));
				else if(s == "Skills"){
					xml.IntoElem();
					while(xml.FindElem()){
						if(xml.GetTagName() != "Skill") continue;
						xml.IntoElem();
						while(xml.FindElem()){
							string s = xml.GetTagName();
							if(s == "ID") item->getEffectData()->addSkill(atoi((char*)xml.GetData().c_str()));
						}
						xml.OutOfElem();
						xml.OutOfElem();
					}
				}
				else if(s == "Mobs"){
					xml.IntoElem();
					while(xml.FindElem()){
						if(xml.GetTagName() != "Mob") continue;
						xml.IntoElem();
						int id, chance;
						while(xml.FindElem()){
							string s = xml.GetTagName();
							if(s == "ID") id = atoi((char*)xml.GetData().c_str());
							else if(s == "Chance") chance = atoi((char*)xml.GetData().c_str());
						}
						item->getEffectData()->getSummonData()->add(id, chance);
						xml.OutOfElem();
						xml.OutOfElem();
					}
				}
			}
			xml.OutOfElem();
		}
	}
	ItemsData::getInstance()->add(item);
	return item;
}

void Initializing::initializeItems(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Items\\*.xml", &FindFileData);
	ItemsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ItemsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
void Initializing::initializeNPCsScripts(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Scripts\\NPCs\\*.as", &FindFileData);
	NPCsScriptsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		NPCsScriptsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
void Initializing::initializePortals(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Scripts\\Portals\\*.as", &FindFileData);
	PortalsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		PortalsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	}
}
void Initializing::initializeEventsScripts(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Scripts\\Events\\*.as", &FindFileData);
	EventsScriptsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		EventsScriptsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	}
}
void Initializing::initializeChannelEventsScripts(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Scripts\\ChannelEvents\\*.as", &FindFileData);
	ChannelEventsScriptsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ChannelEventsScriptsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	}
}
void Initializing::initializeReactorScripts(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Scripts\\Reactors\\*.as", &FindFileData);
	ReactorScriptsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ReactorScriptsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
DropsData* Initializing::loadMobDrops(int id){
	char di[50];
	sprintf_s(di, 50, "Drops/Mobs/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Mob") return NULL;
	xml.IntoElem();
	DropsData* mdrop = new DropsData(id);
	xml.FindElem("DropsLimit");
	mdrop->setDropsLimit(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Drops");	
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Drop") continue;
		DropData* drop = new DropData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "ID") drop->setID(atoi((char*)xml.GetData().c_str()));
			else if(s == "Chance") drop->setChance(atoi((char*)xml.GetData().c_str()));
			else if(s == "Quest") drop->setQuest(atoi((char*)xml.GetData().c_str()));
		}
		mdrop->add(drop);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.FindElem("Mesos");
	xml.IntoElem();
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Min") mdrop->setMinMesos(atoi((char*)xml.GetData().c_str()));
		else if(s == "Max") mdrop->setMaxMesos(atoi((char*)xml.GetData().c_str()));
		else if(s == "MinBags") mdrop->setMinBags(atoi((char*)xml.GetData().c_str()));
		else if(s == "MaxBags") mdrop->setMaxBags(atoi((char*)xml.GetData().c_str()));
	}
	MobsDropData::getInstance()->add(mdrop);
	return mdrop;
}
void Initializing::initializeMobsDrops(){
  WIN32_FIND_DATAA FindFileData;
   
   HANDLE hFind = FindFirstFileA("Drops\\Mobs\\*.xml", &FindFileData);
	MobsDropData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		MobsDropData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
DropsData* Initializing::loadReactorDrops(int id){
	char di[50];
	sprintf_s(di, 50, "Drops/Reactors/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Reactor") return NULL;
	xml.IntoElem();
	DropsData* mdrop = new DropsData(id);
	xml.FindElem("DropsLimit");
	mdrop->setDropsLimit(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Drops");	
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Drop") continue;
		DropData* drop = new DropData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "ID") drop->setID(atoi((char*)xml.GetData().c_str()));
			else if(s == "Chance") drop->setChance(atoi((char*)xml.GetData().c_str()));
			else if(s == "Quest") drop->setQuest(atoi((char*)xml.GetData().c_str()));
		}
		mdrop->add(drop);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.FindElem("Mesos");
	xml.IntoElem();
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Min") mdrop->setMinMesos(atoi((char*)xml.GetData().c_str()));
		else if(s == "Max") mdrop->setMaxMesos(atoi((char*)xml.GetData().c_str()));
		else if(s == "MinBags") mdrop->setMinBags(atoi((char*)xml.GetData().c_str()));
		else if(s == "MaxBags") mdrop->setMaxBags(atoi((char*)xml.GetData().c_str()));
	}
	ReactorsDropData::getInstance()->add(mdrop);
	return mdrop;
}
void Initializing::initializeReactorsDrops(){
  WIN32_FIND_DATAA FindFileData;
   
   HANDLE hFind = FindFirstFileA("Drops\\Reactors\\*.xml", &FindFileData);
	ReactorsDropData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ReactorsDropData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
EquipData* Initializing::loadEquip(int id){
	char di[20];
	sprintf_s(di, 20, "Equips/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Equip") return NULL;
	xml.IntoElem();
	EquipData* equip = new EquipData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Name") equip->setName(xml.GetData());
		else if(s == "Price") equip->setPrice(atoi((char*)xml.GetData().c_str()));
		else if(s == "Slots") equip->setSlots(atoi((char*)xml.GetData().c_str()));
		else if(s == "STR") equip->setStr(atoi((char*)xml.GetData().c_str()));
		else if(s == "DEX") equip->setDex(atoi((char*)xml.GetData().c_str()));
		else if(s == "INT") equip->setInt(atoi((char*)xml.GetData().c_str()));
		else if(s == "LUK") equip->setLuk(atoi((char*)xml.GetData().c_str()));
		else if(s == "MP") equip->setMP(atoi((char*)xml.GetData().c_str()));
		else if(s == "HP") equip->setHP(atoi((char*)xml.GetData().c_str()));
		else if(s == "WAtk") equip->setWAtk(atoi((char*)xml.GetData().c_str()));
		else if(s == "WDef") equip->setWDef(atoi((char*)xml.GetData().c_str()));
		else if(s == "MAtk") equip->setMAtk(atoi((char*)xml.GetData().c_str()));
		else if(s == "MDef") equip->setMDef(atoi((char*)xml.GetData().c_str()));
		else if(s == "Acc") equip->setAcc(atoi((char*)xml.GetData().c_str()));
		else if(s == "Avo") equip->setAvo(atoi((char*)xml.GetData().c_str()));
		else if(s == "Hand") equip->setHand(atoi((char*)xml.GetData().c_str()));
		else if(s == "Speed") equip->setSpeed(atoi((char*)xml.GetData().c_str()));
		else if(s == "Jump") equip->setJump(atoi((char*)xml.GetData().c_str()));
		else if(s == "Cash") equip->setCash(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "Quest") equip->setQuest(atoi((char*)xml.GetData().c_str()) == 1);
		else if(s == "TradeBlock") equip->setTradeBlock(atoi((char*)xml.GetData().c_str()) == 1);
	}
	EquipsData::getInstance()->add(equip);
	return equip;
}

void Initializing::initializeEquips(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Equips\\*.xml", &FindFileData);
	EquipsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		EquipsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}

ShopData* Initializing::loadShop(int id){
	char di[20];
	sprintf_s(di, 20, "Shops/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Shop") return NULL;
	xml.IntoElem();
	ShopData* shop = new ShopData(id);
	xml.FindChildElem("NPC");
	shop->setNPC(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Items");
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Item") continue;
		ShopItemData* item = new ShopItemData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "ID") item->setID(atoi((char*)xml.GetData().c_str()));
			else if(s == "Price") item->setPrice(atoi((char*)xml.GetData().c_str()));
		}
		shop->add(item);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	ShopsData::getInstance()->add(shop);
	return shop;
}
void Initializing::initializeShops(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Shops\\*.xml", &FindFileData);
	ShopsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ShopsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
QuestData* Initializing::loadQuest(int id){
	char di[20];
	sprintf_s(di, 20, "Quests/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Quest") return NULL;
	xml.IntoElem();
	QuestData* quest = new QuestData(id);
	xml.FindChildElem("NextQuest");
	quest->setNextQuest(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Requests");
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Request") continue;
		QuestRequestData* req = new QuestRequestData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "Mob") req->setID(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "Item") req->setItem(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "Quest") req->setQuest(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "ID") req->setID(atoi((char*)xml.GetData().c_str()));
			else if(s == "Count") req->setCount(atoi((char*)xml.GetData().c_str()));
		}
		quest->getRequestsData()->add(req);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.FindElem("Rewards");
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Reward") continue;
		QuestRewardData* rew = new QuestRewardData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "Start") rew->setStart(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "Item") rew->setItem(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "EXP") rew->setExp(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "Mesos") rew->setMesos(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "Fame") rew->setFame(atoi((char*)xml.GetData().c_str()) == 1);
			else if(s == "ID") rew->setID(atoi((char*)xml.GetData().c_str()));
			else if(s == "Count") rew->setCount(atoi((char*)xml.GetData().c_str()));
			else if(s == "Gender") rew->setGender(atoi((char*)xml.GetData().c_str()));
			else if(s == "Job") rew->setJob(atoi((char*)xml.GetData().c_str()));
			else if(s == "Prop") rew->setProp(atoi((char*)xml.GetData().c_str()));
		}
		quest->getRewardsData()->add(rew);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	QuestsData::getInstance()->add(quest);
	return quest;
}
void Initializing::initializeQuests(){
  WIN32_FIND_DATAA FindFileData;
   
   HANDLE hFind = FindFirstFileA("Quests\\*.xml", &FindFileData);
   
	QuestsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		QuestsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
SkillData* Initializing::loadSkill(int id){
	char di[20];
	sprintf_s(di, 20, "Skills/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Skill") return NULL;
	xml.IntoElem();
	SkillData* skill = new SkillData(id);
	int count=0;
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "Level") continue;
		SkillLevelData* level = new SkillLevelData(++count);
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "Time") level->setTime(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "MP") level->setMP(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "HP") level->setHP(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Item") level->setItem(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "ItemCount") level->setItemCount(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "X") level->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Y") level->setY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Speed") level->setSpeed(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Jump") level->setJump(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "WAtk") level->setWAtk(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "WDef") level->setWDef(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "MAtk") level->setMAtk(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "MDef") level->setMDef(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Acc") level->setAcc(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Avo") level->setAvo(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "HPP") level->setHPPer(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "Cooltime") level->setCooltime(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "LTX") level->setLTX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "LTY") level->setLTY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "RBX") level->setRBX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "RBY") level->setRBY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "prop") level->setProp(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "AttackCount") level->setAttackCount(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "MobCount") level->setMobCount(atoi((char*)xml.GetChildData().c_str()));
		}
		skill->add(level);
		xml.OutOfElem();
	}
	SkillsData::getInstance()->add(skill);
	return skill;
}
void Initializing::initializeSkills(){
  WIN32_FIND_DATAA FindFileData;
   
   HANDLE hFind = FindFirstFileA("Skills\\*.xml", &FindFileData);
   
	SkillsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		SkillsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}


PetData* Initializing::loadPet(int id){
	char di[20];
	sprintf_s(di, 20, "Pets/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Pet") return NULL;
	xml.IntoElem();
	PetData* pet = new PetData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Hungry") pet->setHungry(atoi((char*)xml.GetData().c_str()));
		else if(s == "NameTag") pet->setNameTag(atoi((char*)xml.GetData().c_str()));
		else if(s == "ChatBalloon") pet->setChatBalloon(atoi((char*)xml.GetData().c_str()));
		else if(s == "Name") pet->setName(xml.GetData());
		else if(s == "Life") pet->setLife(atoi((char*)xml.GetData().c_str()));
		else if(s == "Commands"){
			xml.IntoElem();
			int counter = 0;
			while(xml.FindElem()){
				if(xml.GetTagName() != "Command") continue;
				PetCommandData* c = new PetCommandData(counter++);
				xml.IntoElem();
				while(xml.FindElem()){
					string s = xml.GetTagName();
					if(s == "Closeness") c->setCloseness(atoi((char*)xml.GetData().c_str()));
					else if(s == "Prob") c->setProb(atoi((char*)xml.GetData().c_str()));
					else if(s == "MinLevel") c->setMinLevel(atoi((char*)xml.GetData().c_str()));
					else if(s == "MaxLevel") c->setMaxLevel(atoi((char*)xml.GetData().c_str()));
				}
				pet->add(c);
				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
	}
	PetsData::getInstance()->add(pet);
	return pet;
}

void Initializing::initializePets(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Pets\\*.xml", &FindFileData);
	PetsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		PetsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}

ReactorData* Initializing::loadReactor(int id){
	char di[50];
	sprintf_s(di, 50, "Reactors/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Reactor") return NULL;
	xml.IntoElem();
	ReactorData* reactor = new ReactorData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Action") reactor->setAction(xml.GetData());
		else if(s == "States"){
			xml.IntoElem();
			while(xml.FindElem()){
				if(xml.GetTagName() != "State") continue;
				ReactorStateData* r = new ReactorStateData();
				xml.IntoElem();
				while(xml.FindElem()){
					string s = xml.GetTagName();
					if(s == "ID") r->setID(atoi((char*)xml.GetData().c_str()));
					else if(s == "Type") r->setType(atoi((char*)xml.GetData().c_str()));
					else if(s == "State") r->setNextState(atoi((char*)xml.GetData().c_str()));
					else if(s == "Item") r->setItemID(atoi((char*)xml.GetData().c_str()));
					else if(s == "Amount") r->setAmount(atoi((char*)xml.GetData().c_str()));
					else if(s == "LTX") r->setLTX(atoi((char*)xml.GetData().c_str()));
					else if(s == "LTY") r->setLTY(atoi((char*)xml.GetData().c_str()));
					else if(s == "RBX") r->setRBX(atoi((char*)xml.GetData().c_str()));
					else if(s == "RBY") r->setRBY(atoi((char*)xml.GetData().c_str()));
				}
				reactor->add(r);
				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
	}
	ReactorsData::getInstance()->add(reactor);
	return reactor;
}

void Initializing::initializeReactors(){
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = FindFirstFileA("Reactors\\*.xml", &FindFileData);
	ReactorsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		ReactorsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
MapData* Initializing::loadMap(int id){
	char di[20];
	sprintf_s(di, 20, "Maps/%d.xml", id);
	CMarkup xml;
	xml.Load(string(di));
	xml.FindElem();
	if(xml.GetTagName() != "Map") return NULL;
	xml.IntoElem();
	MapData* map = new MapData(id);
	xml.FindElem("returnMap");
	map->setReturnMap(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("fieldType");
	map->setFieldType(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Music");
	map->setMusic(xml.GetData());
	xml.FindElem("timeLimit");
	map->setTimeLimit(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("forcedReturn");
	map->setForcedReturn(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("mobRate");
	map->setMobRate(strtod((char*)xml.GetData().c_str(), NULL));
	xml.FindElem("DecHP");
	map->setHPDecrease(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("protectedItem");
	map->setProtectedItem(atoi((char*)xml.GetData().c_str()));
	xml.FindElem("Clock");
	map->setClock(atoi((char*)xml.GetData().c_str()) == 1);
	xml.FindElem("personalShop");
	map->setPersonalShop(atoi((char*)xml.GetData().c_str()) == 1);
	xml.FindElem("NPCs");
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "NPC") continue;
		MapNPCData* npc = new MapNPCData();
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "id") npc->setID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x") npc->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "cy") npc->setCY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "fh") npc->setFH(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "rx0") npc->setRX0(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "rx1") npc->setRX1(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "flip") npc->setFlip(atoi((char*)xml.GetChildData().c_str()) == 1);
		}
		map->getNPCsData()->add(npc);
		xml.OutOfElem();
	}
	xml.FindElem("Mobs");
	int count = 0;
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "Mob") continue;
		MapMobData* mob = new MapMobData();
		mob->setInMapID(count++);
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "id") mob->setID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x") mob->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "cy") mob->setCY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "fh") mob->setFH(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "time") mob->setDelay(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "flip") mob->setFlip(atoi((char*)xml.GetChildData().c_str()) == 1);
		}
		map->getMobsData()->add(mob);
		xml.OutOfElem();
	}
	xml.FindElem("Reactors");
	count = 0;
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "Reactor") continue;
		MapReactorData* reactor = new MapReactorData();
		reactor->setInMapID(count++);
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "id") reactor->setID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x") reactor->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "y") reactor->setY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "time") reactor->setDelay(atoi((char*)xml.GetChildData().c_str()));
		}
		map->getReactorsData()->add(reactor);
		xml.OutOfElem();
	}
	xml.FindElem("Portals");
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "Portal") continue;
		MapPortalData* portal = new MapPortalData();
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "id") portal->setID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "from") portal->setFromPortal(xml.GetChildData().c_str());
			else if(s == "to") portal->setToPortal(xml.GetChildData().c_str());
			else if(s == "toid") portal->setToMapID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "type") portal->setType(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x") portal->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "y") portal->setY(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "script") portal->setScript(xml.GetChildData());
		}
		map->getPortalsData()->add(portal);
		xml.OutOfElem();
	}

	xml.FindElem("Footholds");
	while(xml.FindChildElem()){
		if(xml.GetChildTagName() != "Foothold") continue;
		MapFootholdData* foot = new MapFootholdData();
		xml.IntoElem();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "id") foot->setID(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x1") foot->setX1(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "y1") foot->setY1(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x2") foot->setX2(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "y2") foot->setY2(atoi((char*)xml.GetChildData().c_str()));
		}
		map->getFootholdsData()->add(foot);
		xml.OutOfElem();
	}

	if(xml.FindElem("Ship")){
		MapShipData* ship = new MapShipData();
		while(xml.FindChildElem()){
			string s = xml.GetChildTagName();
			if(s == "Type") ship->setType(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "x") ship->setX(atoi((char*)xml.GetChildData().c_str()));
			else if(s == "y") ship->setY(atoi((char*)xml.GetChildData().c_str()));
		}
		map->setShip(ship);
	}
	MapsData::getInstance()->add(map);
	return map;
}
void Initializing::initializeMaps(){
	  WIN32_FIND_DATAA FindFileData;
   HANDLE hFind = FindFirstFileA(("Maps\\*.xml"), &FindFileData);
	MapsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		MapsData::getInstance()->registerData(atoi((char*)string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')).c_str()));
	}
}
void Initializing::initializeChannelEvents(){
	CMarkup xml;
	xml.Load("Events.xml");
	xml.FindElem();
	if(xml.GetTagName() != "Events") return;
	xml.IntoElem();
	while(xml.FindElem()){
		if(xml.GetTagName() != "Event") continue;
		ChannelEventData* eventd = new ChannelEventData();
		xml.IntoElem();
		while(xml.FindElem()){
			string s = xml.GetTagName();
			if(s == "Name") eventd->setName(xml.GetData().c_str());
			else if(s == "First") eventd->setFirst(atoi((char*)xml.GetData().c_str()));
			else if(s == "Time") eventd->setTime(atoi((char*)xml.GetData().c_str()));
		}
		ChannelEventsData::getInstance()->add(eventd);
		xml.OutOfElem();
	}
}
EventData* Initializing::loadEvent(string& id){
	CMarkup xml;
	xml.Load("Events/" + id + ".xml");
	xml.FindElem();
	if(xml.GetTagName() != "Event") return NULL;
	xml.IntoElem();
	EventData* eventd = new EventData(id);
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "Time") eventd->setTime(atoi((char*)xml.GetData().c_str()));
		else if(s == "Start") eventd->setStartMap(atoi((char*)xml.GetData().c_str()));
		else if(s == "End") eventd->setEndMap(atoi((char*)xml.GetData().c_str()));
		else if(s == "Maps"){
			xml.IntoElem();
			while(xml.FindElem())
				if(xml.GetTagName() == "Map")
					eventd->getMapsData()->add(atoi((char*)xml.GetData().c_str()));
			xml.OutOfElem();
		}
	
	}
	EventsData::getInstance()->add(eventd);
	return eventd;
}
void Initializing::initializeEvents(){
	  WIN32_FIND_DATAA FindFileData;
   HANDLE hFind = FindFirstFileA(("Events\\*.xml"), &FindFileData);
	EventsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	while (FindNextFileA(hFind, &FindFileData)) 
	{
		EventsData::getInstance()->registerData(string((char*)FindFileData.cFileName).substr(0, string((char*)FindFileData.cFileName).find('.')));
	}
}
void Initializing::initializeSettings(){
	CMarkup xml;
	xml.Load("Server.xml");
	xml.FindElem();
	if(xml.GetTagName() != "Server") return;
	xml.IntoElem();
	while(xml.FindElem()){
		string s = xml.GetTagName();
		if(s == "IP") Worlds::getInstance()->setIP(stringToIP(xml.GetData()));
		else if(s == "Password") Worlds::getInstance()->setPassword(xml.GetData().c_str());
		else if(s == "Worlds") Worlds::getInstance()->setWorldsCount(atoi((char*)xml.GetData().c_str()));
		else if(s == "Channels") Worlds::getInstance()->setChannelsCount(atoi((char*)xml.GetData().c_str()));
		else if(s == "DefaultNotice") Worlds::getInstance()->setDefaultNotice(xml.GetData());
		else if(s == "EXP") Worlds::getInstance()->setEXP(atoi((char*)xml.GetData().c_str()));
		else if(s == "MySQL"){
			xml.IntoElem();
			while(xml.FindElem()){
				string s = xml.GetTagName();
				if(s == "Host") MySQL::getInstance()->setHost(xml.GetData());
				else if(s == "Port") MySQL::getInstance()->setPort(atoi((char*)xml.GetData().c_str()));
				else if(s == "Database") MySQL::getInstance()->setDatabase(xml.GetData());
				else if(s == "Username") MySQL::getInstance()->setUsername(xml.GetData());
				else if(s == "Password") MySQL::getInstance()->setPassword(xml.GetData());
			}
			xml.OutOfElem();
		}
	}
}
void Initializing::initializing(){
	printf("Initializing Settings... ");
	initializeSettings();
	printf("DONE\n");
	printf("Initializing Mobs... ");
	initializeMobs();
	printf("DONE\n");
	printf("Initializing Items... ");
	initializeItems();
	printf("DONE\n");
	printf("Initializing NPCs Scripts... ");
	initializeNPCsScripts();
	printf("DONE\n");
	printf("Initializing Portals Scripts... ");
	initializePortals();
	printf("DONE\n");
	printf("Initializing Mobs Drops... ");
	initializeMobsDrops();
	printf("DONE\n");
	printf("Initializing Equips... ");
	initializeEquips();
	printf("DONE\n");
	printf("Initializing Shops... ");
	initializeShops();
	printf("DONE\n");
	printf("Initializing Quests... ");
	initializeQuests();
	printf("DONE\n");	
	printf("Initializing Skills... ");
	initializeSkills();
	printf("DONE\n");
	printf("Initializing Pets... ");
	initializePets();
	printf("DONE\n");
	printf("Initializing Reactors... ");
	initializeReactors();
	printf("DONE\n");
	printf("Initializing Reactors Scripts... ");
	initializeReactorScripts();
	printf("DONE\n");
	printf("Initializing Reactors Drops... ");
	initializeReactorsDrops();
	printf("DONE\n");
	printf("Initializing Events... ");
	initializeEvents();
	printf("DONE\n");
	printf("Initializing Channel Events... ");
	initializeChannelEvents();
	printf("DONE\n");
	printf("Initializing Events Scripts... ");
	initializeEventsScripts();
	printf("DONE\n");
	printf("Initializing Channel Events... ");
	initializeChannelEventsScripts();
	printf("DONE\n");
	printf("Initializing Maps... ");
	initializeMaps();
	printf("DONE\n");
}