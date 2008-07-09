#ifndef BUFF_H
#define BUFF_H

class Buff {
private:
	Effect* effect;
	int timer;
	int stimer;
public:
	Buff(Effect* effect, int timer, int stimer=0){
		this->effect = effect;
		this->timer = timer;
		this->stimer = stimer;
	}
	int getTimer(){
		return timer;
	}
	int* getTimerPtr(){
		return &stimer;
	}
	Effect* getEffect(){
		return effect;
	}
};

#endif