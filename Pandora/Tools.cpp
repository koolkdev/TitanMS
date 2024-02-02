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

#include "DataProvider.h"
#include "Tools.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <sstream>
#include <algorithm>
using namespace std;

#define GMT 3

int strval(string& str);

bool operator<(const Value& x, const Value& y)
{
	return ((Value&)x).getType() < ((Value&)y).getType();
}

void Values::sort(){
	std::sort(values.begin(), values.end());
}

int Tools::getDistance(Position& pos1, Position& pos2){ // Not using sqrt
	return (int)((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-pos2.y)*(pos1.y-pos2.y));
}

void Tools::randomize(){
	srand((unsigned int)time(NULL));
}

int Tools::randomInt(){
	char num[4];
	(*(short*)num) = rand();
	(*(short*)(num+2)) = rand();
	return num[0] + num[1]*0x100 + num[2]*0x100*0x100 + num[3]*0x100*0x100*0x100;
}
int Tools::random(int min, int max){
	return rand()%(max-min+1)+min;
}

int Tools::random(int n){
	return rand()%n;
}

__int64 Tools::doubleAsLong(double num){
	return *(__int64*)(&num); // or maybe just pw.writeDouble?
}
__int64 Tools::getStatsType(Values* values){
	__int64 type = 0;
	vector <Value>* v = values->getValues();
	for(int i=0; i<(int)v->size(); i++){
		type |= (*v)[i].getType();	// For not adding the same stat twice	
	}
	return type;
}

int Tools::getServerTime(){
	return GetTickCount();
}

__int64 Tools::getLongTime(){
	//SYSTEMTIME systemTime;
	//GetSystemTime( &systemTime );
	FILETIME fileTime; 
	//SystemTimeToFileTime( &systemTime, &fileTime );
	GetSystemTimeAsFileTime(&fileTime);
	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime; 
	uli.HighPart = fileTime.dwHighDateTime;
	return uli.QuadPart;

}
int Tools::getTimeUntilNextMinute(){
	SYSTEMTIME systemTime;
	GetSystemTime( &systemTime );
	return 60000*(1+(systemTime.wMilliseconds > 58)) - systemTime.wSecond*1000 - systemTime.wMilliseconds;
}
Time Tools::getTime(){
	SYSTEMTIME systemTime;
	GetSystemTime( &systemTime );
	Time time;
	time.hour = (unsigned char)systemTime.wHour + GMT;
	time.minute = (unsigned char)systemTime.wMinute;
	time.second = (unsigned char)systemTime.wSecond;
	return time;
}
int Tools::getMinute(){
	SYSTEMTIME systemTime;
	GetSystemTime( &systemTime );
	return systemTime.wMinute + (systemTime.wSecond > 58);
}
__int64 Tools::getTimeByDays(int days){
	return getLongTime() + (__int64)days*24*60*60*10000000;
}

IP Tools::stringToIP(string& str){
	IP ip;
	ip.p1 = 0;
	ip.p2 = 0;
	ip.p3 = 0;
	ip.p4 = 0;
	int f = 0;
	int l = str.find(".");
	if(l == -1)
		return ip;
	ip.p1 = strval(str.substr(f, l));
	f = l;
	l = str.find(".", l+1);
	if(l == -1)
		return ip;
	ip.p2 = strval(str.substr(f+1, l));
	f = l;
	l = str.find(".", l+1);
	if(l == -1)
		return ip;
	ip.p3 = strval(str.substr(f+1, l));
	ip.p4 = strval(str.substr(l+1));
	return ip;
}