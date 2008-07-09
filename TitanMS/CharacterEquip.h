#ifndef CHARACTER_EQUIP
#define CHARACTER_EQUIP

class CharacterEquip{
private:
	int id, pos;
public:
	CharacterEquip(int id, int pos){
		this->id = id;
		this->pos = pos;
	}
	void setID(int id){
		this->id = id;
	}
	int getID(){
		return id;
	}
	void setPos(int pos){
		this->pos = pos;
	}
	int getPos(){
		return pos;
	}
};

#endif