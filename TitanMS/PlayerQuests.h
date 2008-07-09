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

#ifndef PLAYERQUESTS_H
#define PLAYERQUESTS_H

#include <vector>
#include <hash_map>

using namespace std;
using namespace stdext;

class Player;
class Quest;

class PlayerQuests {
public:
	PlayerQuests(Player* player){
		this->player = player;
	}
	void deleteAll();
	/*////////////////
	void addQuest(int questid, int npcid);
	void updateQuestMob(int mobid);
	void checkDone(Quest &quest);
	void finishQuest(short questid, int npcid);
	void setPlayer(Player* player){
		this->player=player;
	}
	bool isQuestActive(short questid){
		for(unsigned int i=0; i<quests.size(); i++){
			if(quests[i].id == questid){
				return 1;
			}
		}
		return 0;
	}
	*//////////////////
	vector <Quest*> getQuestsInProgress(){
		vector<Quest*> ret;
		for (hash_map<int,Quest*>::iterator iter = pquests.begin(); iter != pquests.end(); iter++){
			ret.push_back(iter->second);
		}
		return ret;
	}
	vector <Quest*> getCompletedQuests(){
		vector<Quest*> ret;
		for (hash_map<int,Quest*>::iterator iter = cquests.begin(); iter != cquests.end(); iter++){
			ret.push_back(iter->second);
		}
		return ret;
	}
	void giveQuest(int questid, int npcid);
	Quest* getQuestByID(int questid);
	void completeQuest(int questid);
	void completeQuest(Quest* quest);
private:
	Player* player;
	hash_map <int, Quest*> pquests;
	hash_map <int, Quest*> cquests;
};

#endif