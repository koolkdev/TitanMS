#ifndef TIMER_H
#define TIMER_H

#include <queue>
#include <hash_map>
#include "windows.h"
using namespace std;

class Run;

class Timer {
public:

	Timer ();
	~Timer();

	int setTimer 
		(int msec, Run* run);

	void cancelTimer (int id);

	void timerThread();

	static Timer* getInstance(){
		if(instance == NULL)
			instance = new Timer();
		return instance;
	}

private:
	static Timer* instance;
	bool terminate;
	class OneTimer {
	public:
		long t;
		int id;
		Run* run;
		OneTimer (long t, int id, Run* run):t(t),id(id),run(run){}
	};
	vector<OneTimer*> timers;
	HANDLE timerEvent;
	OneTimer* findMin();
	OneTimer* getTimer(int id);
	void remove(int id);
	int id;
};

#endif