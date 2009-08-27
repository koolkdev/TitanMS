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
#include "PacketWriter.h"
#include "Tools.h"
#include "PlayerSkills.h"
#include "Skill.h"

using namespace Tools;

PacketWriter* PacketCreator::gainEXP(int exp, bool chat, bool yellow, int partybonus){
	pw.writeShort(SHOW_GAIN);

	pw.write(3);
	pw.write(!yellow);
	pw.writeInt(exp);
	pw.write(chat);
	pw.writeInt(0); // bonus event-exp
	pw.write(0); // % EXP awarded for every 3rd monster
	pw.write(partybonus);
	pw.writeInt(0); // bonuse wedding exp
	pw.writeShort(0); // some wierd things

	return &pw;
}

PacketWriter* PacketCreator::enableAction(){
	return updateStats(Values(), true);
}
PacketWriter* PacketCreator::updateStats(Values& stats, bool item, char pets){
	pw.writeShort(STATS_UPDATE);

	pw.write(item);
	pw.writeInt((int)getStatsType(&stats));
	stats.sort();
	vector <Value>* v = stats.getValues();
	for(int i=0; i<(int)v->size(); i++){
		if((*v)[i].getType() > 0){
			if((*v)[i].getType() < 0x2)
				pw.writeShort((*v)[i].getValue());
			else if((*v)[i].getType() < 0x8)
				pw.writeInt((*v)[i].getValue());
			else if((*v)[i].getType() < 0x10){
				pw.writeLong((*v)[i].getValue());
				pw.write(pets);
			}
			else if((*v)[i].getType() < 0x20)
				pw.write((*v)[i].getValue());
			else if((*v)[i].getType() < 0x10000)
				pw.writeShort((*v)[i].getValue());
			else
				pw.writeInt((*v)[i].getValue());
		}
	}
	return &pw;
}

PacketWriter* PacketCreator::updateSkill(Skill* skill){
	vector <Skill*> skills;
	skills.push_back(skill);
	return updateSkill(skills);
}

PacketWriter* PacketCreator::updateSkill(vector <Skill*>& skills){
	pw.writeShort(ADD_SKILL);

	pw.write(1);
	pw.writeShort(skills.size());
	for(int i=0; i<(int)skills.size(); i++){
		pw.writeInt(skills[i]->getID());
		pw.writeInt(skills[i]->getLevel()); 
		pw.writeInt(skills[i]->getMasterLevel());
	}
	pw.write(1);

	return &pw;
}
