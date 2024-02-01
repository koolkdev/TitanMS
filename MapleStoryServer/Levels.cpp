 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#include "Levels.h"
#include "Player.h"
#include "Maps.h"
#include "LevelsPacket.h"
#include "SkillsPacket.h"
#include "Skills.h"

int Levels::exps[200] = {15, 34, 57, 92, 135, 372, 560, 840, 1242, 1716, 2360, 3216, 4200,
	5460, 7050, 8840, 11040, 13716, 16680, 20216, 24402, 28980, 34320, 40512, 47216, 54900,
	63666, 73080, 83720, 95700, 108480, 122760, 138666, 155540, 174216, 194832, 216600, 240500,
	266682, 294216, 324240, 356916, 391160, 428280, 468450, 510420, 555680, 604416, 655200,
	709716, 748608, 789631, 832902, 878545, 926689, 977471, 1031036, 1087536, 1147032, 1209994,
	1276301, 1346242, 1420016, 1497832, 1579913, 1666492, 1757815, 1854143, 1955750, 2062925,
	2175973, 2295216, 2420993, 2553663, 2693603, 2841212, 2996910, 3161140, 3334370, 3517093,
	3709829, 3913127, 4127566, 4353756, 4592341, 4844001, 5109452, 5389449, 5684790, 5996316,
	6324914, 6671519, 7037118, 7422752, 7829518, 8258575, 8711144, 9188514, 9692044, 10223168,
	10783397, 11374327, 11997640, 12655110, 13348610, 14080113, 14851703, 15665576, 16524049,
	17429566, 18384706, 19392187, 20454878, 21575805, 22758159, 24005306, 25320796, 26708375,
	28171993, 29715818, 31344244, 33061908, 34873700, 36784778,  8800583, 40926854, 43169645,
	45535341, 48030677, 50662758, 53439077, 56367538, 59456479, 62714694, 66151459, 69776558,
	73600313, 77633610, 81887931, 86375389, 91108760, 96101520, 101367883, 106992842, 112782213,
	118962678, 125481832, 132358236, 139611467, 147262175, 155332142, 163844343, 172823012,
	182293713, 192283408, 202820538, 213935103, 225658746, 238024845, 251068606, 264827165,
	279339639, 294647508, 310794191, 327825712, 345790561, 364739883, 384727628, 405810702,
	428049128, 451506220, 476248760, 502347192, 529875818, 558913012, 589541445, 621848316,
	655925603, 691870326, 729784819, 769777027, 811960808, 856456260, 903390063, 952895838,
	1005114529, 1060194805, 1118293480, 1244216724, 1312399800, 1384319309, 1460180007,
	1540197871, 1624600714, 1179575962, 1713628833, 1807535693, 1906558648, 2011069705, 2121276324};

void Levels::giveEXP(Player* player, int exp, char type){
	int cexp = player->getExp() + exp;
	if(exp!=0)
		LevelsPacket::showEXP(player, exp, type);
	player->setExp(cexp, 1);
	if(cexp >= exps[player->getLevel()-1]){
		cexp -= exps[player->getLevel()-1];
		if(cexp >= exps[player->getLevel()])
			cexp = exps[player->getLevel()] - 1;
		player->setExp(cexp, 0);
		player->setLevel(player->getLevel()+1);
		player->setAp(player->getAp()+5);
		int job=player->getJob()/100;
		if(job == 0){
			player->setRMHP(player->getRMHP()+15);
			player->setRMMP(player->getRMMP()+10);
		}
		else if(job == 1){
			player->setRMHP(player->getRMHP()+25);
			player->setRMMP(player->getRMMP()+15);
			if(player->skills->getSkillLevel(1000001)>0){
				player->setRMHP(player->getRMHP()+Skills::skills[1000001][player->skills->getSkillLevel(1000001)].x);
			}
		}
		else if(job == 2){
			player->setRMHP(player->getRMHP()+15);
			player->setRMMP(player->getRMMP()+35);
			if(player->skills->getSkillLevel(2000001)>0)
				player->setMMP(player->getMMP()+Skills::skills[2000001][player->skills->getSkillLevel(2000001)].x);
		} 
		else{
			player->setRMHP(player->getRMHP()+25);
			player->setRMMP(player->getRMMP()+15);
		}
		player->setMHP(player->getRMHP());
		player->setMMP(player->getRMMP());
		LevelsPacket::levelUP(player, Maps::info[player->getMap()].Players);
		player->setHP(player->getMHP());
		player->setMP(player->getMMP());
		if(player->getJob() > 0){
			player->setSp(player->getSp()+3);
		}
		//
		if(player->skills->getActiveSkillLevel(1301007) != 0){
			Skills::stopSkill(player, 1301007);
		}
		//
	}
}

void Levels::addStat(Player* player, unsigned char* packet){
	int type = getInt(packet+4);
	if(player->getAp() == 0){
		// hacking
		return;
	}
	LevelsPacket::statOK(player);
	if(type == 0x40){
		player->setStr(player->getStr()+1);
		player->setAp(player->getAp()-1);
		LevelsPacket::changeStat(player, type + 0x4000, player->getStr());
	}
	else if(type == 0x80){
		player->setDex(player->getDex()+1);
		player->setAp(player->getAp()-1);
		LevelsPacket::changeStat(player, type + 0x4000, player->getDex());
	}
	else if(type == 0x100){
		player->setInt(player->getInt()+1);
		player->setAp(player->getAp()-1);
		LevelsPacket::changeStat(player, type + 0x4000, player->getInt());
	}
	else if(type == 0x200){
		player->setLuk(player->getLuk()+1);
		player->setAp(player->getAp()-1);
		LevelsPacket::changeStat(player, type + 0x4000, player->getLuk());
	}
}

void Levels::setLevel(Player* player, int level){
	player->setLevel(level);
	LevelsPacket::levelUP(player, Maps::info[player->getMap()].Players);
	
}

void Levels::setJob(Player* player, int job){
	player->setJob(job);
	LevelsPacket::jobChange(player, Maps::info[player->getMap()].Players);
}