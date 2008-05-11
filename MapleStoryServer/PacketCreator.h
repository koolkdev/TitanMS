 /*This file is part of TitanMS.

    TitanMS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TitanMS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TitanMS.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef PACKETCREATOR_H
#define PACKETCREATOR_H

#define MAX_LEN 10000
#include <Winsock2.h>
#include <Vector>
using namespace std;

class Player;
class PlayerLogin;
class MasterServer;
class Channel;

class Packet {
public:
	Packet(){
		this->pos=2;
	}
	void addHeader(short headerid);
	void addInt(int intg);
	void addInt64(__int64 int64);
	void addShort(short shrt);
	void addString(char* str, int slen);
	void addByte(unsigned char byte);
	void addBytes(char* hex);
	void addBytesHex(unsigned char* bytes, int len);
	void packetSend(Player* player);	
	void packetSendLogin(PlayerLogin* player);	
	void packetSendMasterServer(MasterServer* masterServer);	
	void packetSendChannel(Channel* channel);	
	void sendToChannels(Channel* channel, Channel* channels[], bool is);	
	void sendTo(Player* player, vector <Player*> players, bool is);
private:
	int pos;
	SOCKET socketid;
	unsigned char packet[MAX_LEN];
};

#endif