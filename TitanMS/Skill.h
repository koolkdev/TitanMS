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

#ifndef SKILL_H
#define SKILL_H

class Skill {
private:
	int id;
	int level;
	int masterlevel;
public:
	Skill(int id, int level = 1, int masterlevel = 10){
		this->id = id;
		this->level = level;
		this->masterlevel = masterlevel;
	}
	void setID(int id){
		this->id = id;
	}
	void setLevel(int level){
		this->level = level;
	}
	void setMasterLevel(int ml){
		masterlevel = ml;
	}
	int getID(){
		return id;
	}
	int getLevel(){
		return level;
	}
	int getMasterLevel(){
		return masterlevel;
	}
};

#endif