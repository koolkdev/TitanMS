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

#include "PacketCreator.h"
#include "Player.h"
#include "Party.h"
#include "PartyMember.h"
#include "PartyMembers.h"
#include "PacketWriter.h"

PacketWriter* PacketCreator::createParty(int partyid){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x08);
	pw.writeInt(partyid);
	pw.writeInt(999999999);
	pw.writeInt(999999999);
	pw.writeInt(0);

	return &pw;
}
PacketWriter* PacketCreator::removeParty(int partyid, int leader){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x0C);
	pw.writeInt(partyid);
	pw.writeInt(leader);
	pw.write(0);
	pw.writeInt(leader);

	return &pw;
}
PacketWriter* PacketCreator::inviteParty(int party, string& invite){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x04);
	pw.writeInt(party);
	pw.writeString(invite);
	pw.write(0);
	
	return &pw;
}
PacketWriter* PacketCreator::partyError(char error, string& err){
	pw.writeShort(PARTY_OPERATION);

	pw.write(error);
	if(err != ""){
		pw.writeString(err);
	}

	return &pw;
}

PacketWriter* PacketCreator::changeLeader(int newleader){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x1A);
	pw.writeInt(newleader);
	pw.write(0);

	return &pw;
}
PacketWriter* PacketCreator::updateParty(Party* party){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x07);
	pw.writeInt(party->getID());
	partyInfo(party);

	return &pw;
}

PacketWriter* PacketCreator::leaveParty(Party* party, PartyMember* member, bool expel){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x0C);
	pw.writeInt(party->getID());
	pw.writeInt(member->getID());
	pw.write(1);
	pw.write(expel);
	pw.writeString(member->getName());
	partyInfo(party, member->getID());

	return &pw;
}

PacketWriter* PacketCreator::joinParty(Party* party, PartyMember* member){
	pw.writeShort(PARTY_OPERATION);

	pw.write(0x0F);
	pw.writeInt(party->getID());
	pw.writeString(member->getName());
	partyInfo(party);

	return &pw;
}

void PacketCreator::partyInfo(Party* party, int leave){
	hash_map <int, PartyMember*> members = *party->getMembers()->getMembers();
	if(leave != -1){
		members = hash_map <int, PartyMember*> (members);
		members.erase(leave);
	}

	int count = 6 - members.size();

	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeInt(iter->second->getID());
	for(int i=0; i<count; i++)
		pw.writeInt(0);
	
	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeString(iter->second->getName(), 13);
	for(int i=0; i<count; i++)
		pw.writeString("", 13);
	
	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeInt(iter->second->getJob());
	for(int i=0; i<count; i++)
		pw.writeInt(0);
	
	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeInt(iter->second->getLevel());
	for(int i=0; i<count; i++)
		pw.writeInt(0);
	
	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeInt(iter->second->getChannel());
	for(int i=0; i<count; i++)
		pw.writeInt(-2);
	
	pw.writeInt(party->getLeader());

	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++)
		pw.writeInt(iter->second->getMap()); // or only to channel?
	for(int i=0; i<count; i++)
		pw.writeInt(999999999);

	//todo door:
	for(hash_map<int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++){
		pw.writeInt(999999999);
		pw.writeInt(999999999);
		pw.writeInt(0);
		pw.writeInt(0);
	}
	for(int i=0; i<count; i++){	
		pw.writeInt(999999999);
		pw.writeInt(999999999);
		pw.writeInt(-1);
		pw.writeInt(-1);
	}

}

PacketWriter* PacketCreator::updatePartyHP(Player* player){
	pw.writeShort(PARTY_HP);

	pw.writeInt(player->getID());
	pw.writeInt(player->getHP());
	pw.writeInt(player->getMaxHP());

	return &pw;
}