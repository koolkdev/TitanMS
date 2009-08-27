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

#ifndef MAPDATA_H
#define MAPDATA_H

#include "TopDataStruct.h"
#include "MapMobsData.h"
#include "MapNPCsData.h"
#include "MapReactorsData.h"
#include "MapPortalsData.h"
#include "MapFootholdsData.h"
#include "MapShipData.h"

class MapData : public DataStruct {
private:
	MapMobsData* mobs;
	MapNPCsData* NPCs;
	MapReactorsData* reactors;
	MapPortalsData* portals;
	MapFootholdsData* footholds;
	MapShipData* ship;
	int returnMap;
	int fieldType;
	string music;
	int timeLimit;
	int forcedReturn;
	double mobRate;
	int HPDecrease;
	int protectedItem;	
	bool personalShop;
	bool clock;
public:
	MapData(int id): returnMap(0), fieldType(0), music(""), timeLimit(0), mobRate(0), HPDecrease(0), protectedItem(0), clock(0) {
		this->id = id;
		mobs = new MapMobsData();
		NPCs = new MapNPCsData();
		reactors = new MapReactorsData();
		portals = new MapPortalsData();
		footholds = new MapFootholdsData();
		ship = NULL;
	}

	MapMobsData* getMobsData(){
		return mobs;
	}
	MapNPCsData* getNPCsData(){
		return NPCs;
	}
	MapReactorsData* getReactorsData(){
		return reactors;
	}
	MapPortalsData* getPortalsData(){
		return portals;
	}
	MapFootholdsData* getFootholdsData(){
		return footholds;
	}
	MapShipData* getShip(){
		return ship;
	}
	void setShip(MapShipData* ship){
		this->ship = ship;
	}
	void setReturnMap(int returnMap){
		this->returnMap = returnMap;
	}
	int getReturnMap(){
		return returnMap;
	}
	void setFieldType(int t){
		fieldType = t;
	}
	int getFieldType(){
		return fieldType;
	}
	void setMusic(string m){
		music = m;
	}
	string& getMusic(){
		return music;
	}
	void setTimeLimit(int t){
		timeLimit = t;
	}
	int getTimeLimit(){
		return timeLimit;
	}
	void setForcedReturn(int f){
		forcedReturn = f;
	}
	int getForcedReturn(){
		return forcedReturn;
	}
	void setHPDecrease(int d){
		HPDecrease = d;
	}
	int getHPDecrease(){
		return HPDecrease;
	}
	void setProtectedItem(int p){
		protectedItem = p;
	}
	int getProtectedItem(){
		return protectedItem;
	}
	void setClock(bool c){
		clock = c;
	}
	bool getClock(){
		return clock;
	}
	void setPersonalShop(bool p){
		personalShop = p;
	}
	bool getPersonalShop(){
		return personalShop;
	}
	void setMobRate(double rate){
		mobRate = rate;
	}
	double getMobRate(){
		return mobRate;
	}
};


#endif