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

#include "Run.h"
#include "Timer.h"
#include <Winbase.h>

void _timerThread (Timer* timerObject) {
	timerObject->timerThread();
}
Timer* Timer::instance;

Timer::Timer () {
	id = 0;
	terminate = false;

	HANDLE thread = CreateThread(NULL, 20000,
		(LPTHREAD_START_ROUTINE)_timerThread,
		(LPVOID)this,
		NULL,
		NULL);

	timerEvent = CreateEvent( 
            NULL,     // no security attributes
            FALSE,    // auto-reset event
            FALSE,    // initial state is not signaled
            NULL);    // object not named
}

Timer::~Timer() {
	terminate = true;
	SetEvent (timerEvent);
}

int Timer::setTimer (int msec, Run* run) {
	int nid = id++;
	timers.push_back(new OneTimer(msec+GetTickCount(), nid, run));
	SetEvent (timerEvent);
	return nid++;
}

void Timer::remove(int id){
	for(unsigned int i=0; i<timers.size(); i++){
		if(timers[i]->id == id){
			timers.erase(timers.begin()+i);
			break;
		}
	}
}

void Timer::cancelTimer (int id) {
	remove(id);
	SetEvent (timerEvent);
}

Timer::OneTimer* Timer::findMin(){
	if(timers.size() == 0)
		return NULL;
	OneTimer* min=timers[0];
	for(unsigned int i=1; i<timers.size(); i++){
		if(timers[i]->t < min->t)
			min = timers[i];
	}
	return min;
}


Timer::OneTimer* Timer::getTimer(int id){
	for(unsigned int i=0; i<timers.size(); i++){
		if(timers[i]->id == id){
			return timers[i];
		}
	}
	return NULL;
}

void Timer::timerThread () {
	while (!terminate) {
		try {
			// find minimum wakeup time
			OneTimer* minTimer = findMin();
			long msec = minTimer==NULL ? msec = 1000000000 : minTimer->t - GetTickCount();
			if (msec <= 0) {
				minTimer->run->run();
				remove (minTimer->id);
				delete minTimer->run;
				delete minTimer;
				continue;
			}
			DWORD r = WaitForSingleObject(timerEvent, msec);
			// TODO: temp
			continue;
			if (r == WAIT_OBJECT_0) continue;
			if (r == WAIT_FAILED) {
				// TODO: write message
				return;
			}
			if (minTimer != NULL) {
				minTimer->run->run();
				remove (minTimer->id);
				delete minTimer->run;
				delete minTimer;
			}
		}
		catch (...) {
			// TODO error
		}
	}
}
