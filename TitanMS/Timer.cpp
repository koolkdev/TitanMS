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
#include "Run.h"
#include "Timer.h"
#include <Winbase.h>
#include <hash_map>
using namespace stdext;

bool TimersQueue::remove(Timer* timer){
	vector<Timer*>::iterator iter = find(c.begin(), c.end(), timer);
	if(iter != c.end()){
		vector <Timer*> timers;
		while(top() != timer){ timers.push_back(top()); pop(); }
		pop();
		while(!timers.empty()){ push(timers.back()); timers.pop_back(); }
		return true;
	}
	return false;
}
void _timerThread (Timers* timerObject) {
	timerObject->timerThread();
}
Timers* Timers::instance;

Timer::~Timer(){
	delete run;
}

Timers::Timers () {
	id = 0;
	terminate = false;

	minTimer = NULL;

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

	InitializeCriticalSection(&critic);
}

Timers::~Timers() {
	terminate = true;
	SetEvent (timerEvent);
	DeleteCriticalSection(&critic);
}

Timer* Timers::startTimer (int msec, Run* run) {
	Timer* timer = new Timer(msec+GetTickCount(), run);
	timer->otime = msec;
	EnterCriticalSection(&critic);
	timers.push(timer);
	LeaveCriticalSection(&critic);
	SetEvent (timerEvent);
	return timer;
}

void Timers::reset(Timer* timer){
	// not working.. todo: fix
	EnterCriticalSection(&critic);
	bool find = timers.remove(timer); 
	LeaveCriticalSection(&critic);
	if(!find) return;
	timer->t = timer->otime+GetTickCount();
	EnterCriticalSection(&critic);
	timers.push(timer);
	LeaveCriticalSection(&critic);
	SetEvent (timerEvent);
}

void Timers::remove(Timer* timer){
	if(minTimer == timer){
		// TODO: thread-safe
		EnterCriticalSection(&critic);
		minTimer = NULL;
		LeaveCriticalSection(&critic);
		SetEvent (timerEvent);
		//until it will be fixed: delete timer;
	}
	else{
		EnterCriticalSection(&critic);
		bool find = timers.remove(timer); 
		LeaveCriticalSection(&critic);
		SetEvent (timerEvent);
		if(find) delete timer;
	}
}

void Timers::cancelTimer (Timer* timer) {
	remove(timer);
}

Timer* Timers::findMin(){
	if(timers.empty())
		return NULL;
	return timers.top();
}

/*
Timers::Timer* Timers::getTimer(int id){
	for(unsigned int i=0; i<timers.size(); i++){
		if(timers[i]->id == id){
			return timers[i];
		}
	}
	return NULL;
}*/

void Timers::timerThread () {
	while (!terminate) {
		try {
			// find minimum wakeup time
			EnterCriticalSection(&critic);
			minTimer = findMin();
			if(minTimer != NULL)
				timers.pop();
			LeaveCriticalSection(&critic);
			long msec = minTimer==NULL || minTimer->t < 0 ? msec = 1000000000 : minTimer->t - GetTickCount();
			if (msec <= 0) {
				minTimer->run->run();
				delete minTimer;
				continue;
			}
			DWORD r = WaitForSingleObject(timerEvent, msec);
			if (r == WAIT_OBJECT_0){
				EnterCriticalSection(&critic);
				if(minTimer != NULL){
					timers.push(minTimer);
				}
				LeaveCriticalSection(&critic);
				continue;
			}
			if (r == WAIT_FAILED) {
				// TODO: write message
				return;
			}
			if (minTimer != NULL) {
				minTimer->run->run();
				delete minTimer;
				continue;
			}
		}
		catch (...) {
			// TODO error
		}
	}
}
