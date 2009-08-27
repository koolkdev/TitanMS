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

#ifndef MYSQL_H
#define MYSQL_H

#pragma warning (disable:4275)

#include "..\mysql++\mysql.h"
#include "..\mysql++\mysql++.h"
#include <string>

class MySQL {
private:
	static mysqlpp::Connection MySQL::maple_db;
	static MySQL* instance;
	std::string host;
	std::string username;
	std::string password;
	std::string database;
	int port;
public:
	MySQL(){
		instance = this;
	}
	static MySQL* getInstance(){
		if(instance == NULL)
			return new MySQL();
		return instance;
	}
	void setHost(string h){
		host = h;
	}
	void setUsername(string u){
		username = u;
	}
	void setPassword(string p){
		password = p;
	}
	void setDatabase(string b){
		database = b;
	}
	void setPort(int p){
		port = p;
	}

	int connectToMySQL();
	int getUserID(char* username);
	int getInt(char* table, int id, char* value);
	void setInt(char* table, char* wht, int id, int value);
	void setString(char* table, char* wht, int id, char* value);
	int isString(char* table, char* whr, std::string wht);
	std::string getString(char* table, char* whr, char* wht, char* value);
	std::string getString(char* table, char* whr, int wht, char* value);
	std::vector<int> getCharactersIDs(int id);
	int setChar(int userid, std::string name, int face, int hair, int skin, int gender, int str, int dex, int intt, int lukl, int world);
	void charInfo(char* wht, int id);
	void deleteRow(char* table, int id);
	void insert(char* query);
	int showEquips(int id, int equips[30][2]);
	int getInt(char* table, char* whr, char* wht, char* value);
	void createEquip(int equipid, int type, int charid);
	void getCharacter(int id, std::string info[29]);
	void deletePlayer(int id);
	mysqlpp::Connection* getDataBase(){
		return &maple_db;
	}
};

#endif