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

#ifndef QUEST_H
#define QUEST_H

#include <hash_map>
#include <string>
using namespace std;
using namespace stdext;

struct KilledMob {
	int id;
	int count;
};

class Quest {
private: 
	int id;
	bool complete;
	__int64 time;
	vector <KilledMob> mobs;
public:
	Quest(int id, bool complete = false, __int64 time = 0){
		this->id = id;
		this->complete = complete;
		this->time = time;
	}
	void addKilledMobsInfo(vector <int>& ids){
		for(int i=0; i<(int)ids.size(); i++){
			KilledMob mob;
			mob.id = ids[i];
			mob.count =0;
			mobs.push_back(mob);
		}
	}
	void addKilledMob(int id, int count=1){
		for(int i=0; i<(int)mobs.size(); i++){
			if(mobs[i].id == id){
				mobs[i].count += count;
				break;
			}
		}
	}
	string getKilledMobs(){
		string ret;
		for(int i=0; i<(int)mobs.size(); i++){
			char temp[4];
			temp[0] = mobs[i].count/100+'0';
			temp[1] = mobs[i].count/10%10+'0';
			temp[2] = mobs[i].count%10+'0';
			temp[3] = '\0';
			ret += string(temp);
		}
		return ret;
	}
	void setID(int id){
		this->id = id;
	}
	int getID(){
		return id;
	}
	void setIsCompleted(bool complete){
		this->complete = complete;
	}
	bool isCompleted(){
		return complete;
	}
	void setCompleteTime(__int64 time){
		this->time = time;
	}
	__int64 getCompleteTime(){
		return time;
	}	
};

#endif