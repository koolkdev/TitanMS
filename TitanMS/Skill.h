#ifndef SKILL_H
#define SKILL_H

class Skill {
private:
	int id;
	int level;
	int masterlevel;
public:
	Skill(int id, int level = 1, int masterlevel = 10){
		this->id = id;
		this->level = level;
		this->masterlevel = masterlevel;
	}
	void setID(int id){
		this->id = id;
	}
	void setLevel(int level){
		this->level = level;
	}
	void setMasterLevel(int ml){
		masterlevel = ml;
	}
	int getID(){
		return id;
	}
	int getLevel(){
		return level;
	}
	int getMasterLevel(){
		return masterlevel;
	}
};

#endif