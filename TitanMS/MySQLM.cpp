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
#include <Winsock2.h>
#include "MySQLM.h"
#include <sstream>
#include <string>
#include "Tools.h"
using namespace Tools;
using namespace std;
using namespace mysqlpp;

Connection MySQL::maple_db(false);
MySQL* MySQL::instance;
int strval(string& str);
int MySQL::connectToMySQL(){
	maple_db = mysql_init(NULL);
	if(!maple_db.connect((char*)database.c_str(), (char*)host.c_str(), (char*)username.c_str(), (char*)password.c_str(), port)){
		printf(maple_db.error());
		return 0;
	}
	return 1;
}

int MySQL::getInt(char* table, int id, char* value){
	stringstream query;
	query << "SELECT " << value << " FROM " << table << " WHERE ID=" << id;
	StoreQueryResult res = maple_db.query(query.str()).store();
	if(!res.empty()){
		return strval(string((res[0][0].c_str())));
	}
	return 0;
}
int MySQL::getInt(char* table, char* whr, char* wht, char* value){
	Query query = maple_db.query();
	query << "SELECT " << value << " FROM " << table << " WHERE " << whr << "=" << wht;
	StoreQueryResult res = query.store();
	if(!res.empty()){
		return strval(string((res[0][0].c_str())));
	}
	return 0;
}
vector<int> MySQL::getCharactersIDs(int id){
	vector <int> ret;
	Query query = maple_db.query();
	query << "SELECT ID FROM characters WHERE userid=" << id;
	StoreQueryResult res = query.store();
	for(int i=0; i<(int)res.num_rows() && i<3; i++){
		ret.push_back(strval(string(res[i][0].c_str())));
	}
	return ret;
}
int MySQL::isString(char* table, char* whr, string wht){
	Query query = maple_db.query();
	query << "SELECT * FROM " << table << " WHERE " << whr << "=" << "'" << wht << "'";
	StoreQueryResult res = query.store();
	return !res.empty();
}

string MySQL::getString(char* table, char* whr, char* wht, char* value){
	Query query = maple_db.query();
	query << "SELECT " << value << " FROM " << table << " WHERE " << whr << "=" << "'" << wht << "'";
	StoreQueryResult res = query.store();
	if(!res.empty())
		return string(res[0][0].c_str());
	return string("");
}
string MySQL::getString(char* table, char* whr, int wht, char* value){
	Query query = maple_db.query();
	query << "SELECT " << value << " FROM " << table << " WHERE " << whr << "=" << "'" << wht << "'";
	StoreQueryResult res = query.store();
	if(!res.empty())
		return string(res[0][0].c_str());
	return string("");
}

void MySQL::setInt(char* table, char* wht, int id, int value){
	Query query = maple_db.query();
	query << "UPDATE " << table << " SET " << wht << "=" << "'" << value << "'" << " WHERE ID=" << "'" << id << "'";
	query.execute();
}

void MySQL::setString(char* table, char* wht, int id, char* value){
	Query query = maple_db.query();
	query << "UPDATE " << table << " SET " << wht << "=" << "'" << value << "'" << " WHERE ID=" << "'" << id << "'";
	query.execute();
}

int MySQL::getUserID(char *username){
	Query query = maple_db.query();
	query << "SELECT id FROM users WHERE username=" << "'" << username << "'";
	StoreQueryResult res = query.store();
	if(!res.empty()){
		return strval(string((res[0][0].c_str())));
	}
	return 0;
}
int MySQL::setChar(int userid, string name, int face, int hair, int skin, int gender, int str, int dex, int intt, int luk, int world){
	Query query = maple_db.query();
	query << "INSERT INTO characters(userid, world, name, face, hair, skin, gender, str, dex, intt, luk) VALUES(" << userid << "," << world << ",'" << name << "'," << face << "," << hair << "," << skin << "," << gender << "," << str << "," << dex << "," << intt << "," << luk << ")";
	SimpleResult res = query.execute();
	int id =  (int)res.insert_id();
	query << "insert into items values(4161001, " << id << ", 4, 1, 1)";
	query.execute();
	query <<"insert into keymap values(" << id << ", 2,4,10)"<<
		",(" << id << ", 3,4,12)"<<
		",(" << id << ", 4,4,13)"<<
		",(" << id << ", 5,4,18)"<<
		",(" << id << ", 6,4,24)"<<
		",(" << id << ", 7,4,21)"<<
		",(" << id << ", 16,4,8)"<<
		",(" << id << ", 17,4,5)"<<
		",(" << id << ", 18,4,0)"<<
		",(" << id << ", 19,4,4)"<<
		",(" << id << ", 23,4,1)"<<
		",(" << id << ", 25,4,19)"<<
		",(" << id << ", 26,4,14)"<<
		",(" << id << ", 27,4,15)"<<
		",(" << id << ", 29,5,52)"<<
		",(" << id << ", 31,4,2)"<<
		",(" << id << ", 34,4,17)"<<
		",(" << id << ", 35,4,11)"<<
		",(" << id << ", 37,4,3)"<<
		",(" << id << ", 38,4,20)"<<
		",(" << id << ", 40,4,16)"<<
		",(" << id << ", 41,4,23)"<<
		",(" << id << ", 43,4,9)"<<
		",(" << id << ", 44,5,50)"<<
		",(" << id << ", 45,5,51)"<<
		",(" << id << ", 46,4,6)"<<
		",(" << id << ", 48,4,22)"<<
		",(" << id << ", 50,4,7)"<<
		",(" << id << ", 56,5,53)"<<
		",(" << id << ", 47,5,54)"<<
		",(" << id << ", 59,6,100)"<<
		",(" << id << ", 60,6,101)"<<
		",(" << id << ", 61,6,102)"<<
		",(" << id << ", 62,6,103)"<<
		",(" << id << ", 63,6,104)"<<
		",(" << id << ", 64,6,105)"<<
		",(" << id << ", 65,6,106)";
	query.execute();


	return id;
}
void MySQL::createEquip(int equipid, int type, int charid){
	Query query = maple_db.query();
	query << "insert into equips(equipid,charid,iwatk,iwdef,slots,pos) values(" << equipid << "," << charid << ",";
	switch(type){
		case 0x05: query << "0, 3, 7,"; break;
		case 0x06: query << "0, 2, 7,"; break;
		case 0x07: query << "0, 2, 5,"; break;
		case 0x0b: query << "17, 0, 7,"; break;
	}
	query << -type << ")";
	query.execute();
}
void MySQL::charInfo(char* wht, int id){
	Query query = maple_db.query();
	query << "UPDATE characters SET " << wht << " WHERE ID="<< "'" << id << "'";
	query.execute();
}

void MySQL::deleteRow(char* table, int id){
	Query query = maple_db.query();
	query << "DELETE FROM " << table << " WHERE ID="<< "'" << id << "'";
	query.execute();
}

void MySQL::insert(char* query){
	maple_db.query(query).execute();
}

int MySQL::showEquips(int id, int equips[30][2]){
	Query query = maple_db.query();
	query << "SELECT equipid, pos FROM equips WHERE charid=" << id << " AND pos<0";
	StoreQueryResult res = query.store();
	for(int i=0; i<(int)res.num_rows(); i++){
		equips[i][0] = strval(string(res[i][0]));
		equips[i][1] = strval(string(res[i][1]));
	}
	return (int)res.num_rows();
}
void MySQL::getCharacter(int id, string info[29]){
	Query query = maple_db.query();
	query << "SELECT * FROM characters WHERE ID=" << id;
	StoreQueryResult res = query.store();
	if(res.empty())
		return;
	for(int i=0; i<29; i++){
		info[i] = string(res[0][i]);
	}
}


void MySQL::deletePlayer(int id){
	Query query = maple_db.query();
	query << "DELETE FROM characters WHERE ID=" << id;
	query.execute();
	query << "DELETE FROM equips WHERE charid=" << id;
	query.execute();
	query << "DELETE FROM items WHERE charid=" << id;
	query.execute();
	query << "DELETE FROM skills WHERE charid=" << id;
	query.execute();
	query << "DELETE FROM keymap WHERE charid=" << id;
	query.execute();
	query << "DELETE FROM pets WHERE charid=" << id;
	query.execute();
	query << "DELETE FROM vars WHERE charid=" << id;
	query.execute();
}