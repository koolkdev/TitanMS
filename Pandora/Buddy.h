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

#ifndef BUDDY_H
#define BUDDY_H

#include <string>

class Buddy {
private:
	int channel, id;
	std::string name;
public:
	Buddy(int id, std::string name, int channel) : id(id), name(name), channel(channel) {}
	int getID(){
		return id;
	}
	int getChannel(){
		return channel;
	}
	std::string& getName(){
		return name;
	}
	void setID(int id){
		this->id = id;
	}
	void setChannel(int channel){
		this->channel = channel;
	}
	void setName(std::string name){
		this->name = name;
	}
};
#endif