#include "Tools.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <sstream>
using namespace std;

int Tools::getDistance(Position pos1, Position pos2){ // Not using sqrt
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
	char temp[25];
	sprintf_s(temp, 25, "%I64d", num);
	__int64 ret;
	stringstream ss(temp);
	ss >> ret;
	return ret;
}
__int64 Tools::getStatsType(Values* values){
	__int64 type = 0;
	vector <Value>* v = values->getValues();
	for(int i=0; i<(int)v->size(); i++){
		type |= (*v)[i].getType();	// For not adding the same stat twice	
	}
	return type;
}

int Tools::getTime(){
	return (int)(getLongTime()/0x100000000);
}

__int64 Tools::getLongTime(){
	SYSTEMTIME systemTime;
	GetSystemTime( &systemTime );
	FILETIME fileTime; 
	SystemTimeToFileTime( &systemTime, &fileTime );
	ULARGE_INTEGER uli;
	uli.LowPart = fileTime.dwLowDateTime; 
	uli.HighPart = fileTime.dwHighDateTime;
	return uli.QuadPart;

}
__int64 Tools::getTimeByDays(int days){
	return getLongTime() + (__int64)days*24*60*60*10000000;
}