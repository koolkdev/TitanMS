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

#ifndef KEY_H
#define KEY_H

class Key {
private:
	char type;
	int action;
public:
	Key(char type, int action){
		this->type = type;
		this->action = action;
	}
	char getType(){
		return type;
	}
	int getAction(){
		return action;
	}
	void setAction(int action){
		this->action = action;
	}
	void setType(char type){
		this->type = type;
	}
};

#endif