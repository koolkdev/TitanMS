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

#ifndef PARTYMEMBER_H
#define PARTYMEMBER_H

#include <string>

class PartyMember {
private:
	int id, job, level, channel, map;
	std::string name;
public:
	void setID(int id){
		this->id = id;
	}
	void setJob(int job){
		this->job = job;
	}
	void setLevel(int level){
		this->level = level;
	}
	void setChannel(int channel){
		this->channel = channel;
	}
	void setMap(int map){
		this->map = map;
	}
	void setName(std::string name){
		this->name = name;
	}
	int getID(){
		return id;
	}
	int getJob(){
		return job;
	}
	int getLevel(){
		return level;
	}
	int getChannel(){
		return channel;
	}
	int getMap(){
		return map;		
	}
	std::string& getName(){
		return name;
	}
};

#endif