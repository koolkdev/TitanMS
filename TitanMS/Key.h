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