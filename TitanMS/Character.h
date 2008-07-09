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

#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

using namespace std;

class CharacterEquip;
class Character {
public:
	void setID(int id){
		this->id = id;
	}
	int getID(){
		return id;
	}
	void setName(string name){
		this->name = name;
	}
	string getName(){
		return this->name;
	}
	void setGender(char g){
		this->gender = g;
	}
	char getGender(){
		return this->gender;
	}
	void setSkin(char id){
		this->skin = id;
	}
	char getSkin(){
		return this->skin;
	}
	void setFace(int id){
		this->face = id;
	}
	int getFace(){
		return this->face;
	}
	void setHair(int id){
		this->hair = id;
	}
	int getHair(){
		return this->hair;
	}
	void setMap(int map){
		this->map=map;
	}
	int getMap(){
		return this->map;
	}
	void setMappos(char pos){
		this->mappos = pos;
	}
	char getMappos(){
		return this->mappos;
	}
	void setLevel(int level){
		this->level = level;
	}
	unsigned char getLevel(){
		return this->level;
	}
	void setJob(short job){
		this->job = job;
	}
	short getJob(){
		return this->job;
	}
	void setStr(short str){
		this->str=str;
	}
	short getStr(){
		return this->str;
	}
	void setDex(short dex){
		this->dex=dex;
	}
	short getDex(){
		return this->dex;
	}
	void setInt(short intt){
		this->intt=intt;
	}
	short getInt(){
		return this->intt;
	}
	void setLuk(short luk){
		this->luk=luk;
	}
	short getLuk(){
		return this->luk;
	}
	void setHP(int hp){
		this->hp = hp;
	}
	short getHP(){
		return this->hp;
	}
	void setMP(int mp){
		this->mp = mp;
	}
	short getMP(){
		return this->mp;
	}
	void setMaxHP(int mhp){
		this->mhp=mhp;
	}
	short getMaxHP(){
		return this->mhp;
	}
	void setMaxMP(int mmp){
		this->mmp=mmp;
	}
	short getMaxMP(){
		return this->mmp;
	}
	void setAP(short ap){
		this->ap=ap;
	}
	short getAP(){
		return this->ap;
	}
	void setSP(short sp){
		this->sp = sp;
	}
	short getSP(){
		return this->sp;
	}
	void setFame(short fame){
		this->fame=fame;
	}
	short getFame(){
		return this->fame;
	}
	void setExp(int exp){
		this->exp = exp;
	}
	int getExp(){
		return this->exp;
	}
	vector <CharacterEquip*>* getEquips(){
		return &equips;
	}
	void setWorld(int world){
		this->world = world;
	}
	int getWorld(){
		return world;
	}
	~Character();
	void addEquip(CharacterEquip* equip){
		equips.push_back(equip);
	}
private:
	int id;
	string name;
	char gender;
	char skin;
	int face;
	int hair;	
	unsigned char level;
	short job;
	short str;
	short dex;
	short intt;
	short luk;
	short hp;
	short mhp;
	short mp;
	short mmp;
	short ap;
	short sp;
	int exp;
	short fame;
	int map;
	char mappos;
	int world;
	vector <CharacterEquip*> equips;
};

#endif