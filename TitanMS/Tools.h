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

#ifndef TOOLS_H
#define TOOLS_H

struct Position {
	short x;
	short y;
	bool operator ==(const int& n) const{
		return x == n && y == n;
	}
	bool operator !=(const int& n) const{
		return !(*this==n);
	}
};

#include <vector>
#include <string>

class Value {
	__int64 type;
	int value;
public:
	Value(__int64 type, int value){
		this->type = type;
		this->value = value;
	}
	__int64 getType(){
		return type;
	}
	int getValue(){
		return value;
	}
};

class Values {
protected:
	int pos;
	std::vector <Value> values;
public:
	Values(){
		pos = -1;
	}
	void add(Value value){
		values.push_back(value);
	}/*
	void beforeFirst(){
		pos=-1;
	}
	bool next(){
		if(pos + 1 < (int)values.size()){
			pos++;
			return true;
		}
		return false;
	}
	Value* getValues(){
		if(pos == -1 || pos >= (int)values.size()){
			if(values.size() == 0)
				return NULL;
			return &values[0];
		}
		return &values[pos];
	}
	void sort(){
		sort(
		int n = values.size();
		for(int i=0; i<n; i++){
			for(int j=0; j<n-1-i; j++){
				if(values[j].getType() > values[j+1].getType()){
					Value temp = values[j];
					values[j] = values[j+1];
					values[j+1] = temp;
				}
			}
		}
	}*/
	void sort();
	std::vector <Value>* getValues(){
		return &values;
	}
	int getSize(){
		return values.size();
	}
	int getValueByType(__int64 type){
		for(int i=0; i<(int)values.size(); i++){
			if(values[i].getType() == type)
				return values[i].getValue();
		}
		return 0;

	}
	bool contain(__int64 type){
		for(int i=0; i<(int)values.size(); i++){
			if(values[i].getType() == type)
				return true;
		}
		return false;
	}
};

struct IP {
	unsigned char p1;
	unsigned char p2;
	unsigned char p3;
	unsigned char p4;
	
	void operator=(const IP& ip)
	{
		p1 = ip.p1;
		p2 = ip.p2;
		p3 = ip.p3;
		p4 = ip.p4;
	}
	bool operator==(const IP& ip)
	{
		return p1 == ip.p1 && p2 == ip.p2 && p3 == ip.p3 && p4 == ip.p4;
	}
	bool operator!=(const IP& ip)
	{
		return !(*this == ip);
	}
};
struct Time {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
};

namespace Tools {
	int getDistance(Position& pos1, Position& pos2);
	void randomize();
	int randomInt();
	int random(int min, int max);
	int random(int n);
	Time getTime();
	int getServerTime();
	int getTimeUntilNextMinute();
	int getMinute();
	__int64 getLongTime();
	__int64 doubleAsLong(double num);
	__int64 getStatsType(Values* values);
	__int64 getTimeByDays(int days);
	IP stringToIP(std::string& ip);
};
#endif