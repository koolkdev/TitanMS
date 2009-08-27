#include "Parties.h"
#include "Party.h"
#include "Player.h"
#include "PartyMembers.h"
#include "PartyMember.h"
#include "PacketCreator.h"
#include "World.h"
#include "Channel.h"
#include "Event.h"
#include "Events.h"
#include "Map.h"
#include <Winsock2.h>
#include "MySQLM.h"
#include "..\mysql++\mysql++.h"

using namespace mysqlpp;

string valstr(int id);

void Parties::create(Player* player){
	if(player->getParty() != NULL) return;
	if(player->getJob() == 0) player->send(PacketCreator().partyError(10));
	Party* party = new Party(world, player);
	player->setParty(party);
	parties[party->getID()] = party;
	party->getMembers()->send(PacketCreator().createParty(party->getID()));
}

void Parties::leave(Player* player){
	Party* party = player->getParty();
	if(party == NULL){
		return;
	}
	if(player->getMap()->getEvent() != NULL && player->getMap()->getEvent()->getParty())
		player->getChannel()->getEvents()->deleteEvent(player->getMap()->getEvent()->getName());
	if(party->getLeader() == player->getID()){
		party->getMembers()->send(PacketCreator().removeParty(party->getID(), party->getLeader()));
		parties.erase(party->getID());
		delete party;
	}
	else{
		PartyMember* mem = party->getMembers()->getMember(player->getID());
		if(mem == NULL) return;
		party->getMembers()->send(PacketCreator().leaveParty(party, mem));
		party->removeMember(player->getID());
		player->setParty(NULL);
	}
}

void Parties::join(Player* player, int partyid){
	if(player->getParty() != NULL || player->getPartyInvite() != partyid){
		player->setPartyInvite(0);
		return;
	}
	Party* party = getParty(partyid);
	if(party != NULL){
		if(party->getMembers()->getMembers()->size() >= 6){
			player->send(PacketCreator().partyError(17));
		}
		else{
			party->getMembers()->addMember(player);
			player->setParty(party);
			MySQL::getInstance()->getDataBase()->query("update characters set party=" + valstr(partyid) + " where id=" + valstr(player->getID())).execute();
			PartyMember* member = party->getMembers()->getMember(player->getID());
			if(member != NULL){
				party->getMembers()->send(PacketCreator().joinParty(party, member));
				party->getMembers()->send(PacketCreator().updatePartyHP(player), player->getMap(), player);
				hash_map <int, Player*>* players = party->getMembers()->getPlayers();
				for(hash_map<int, Player*>::iterator iter = players->begin(); iter != players->end(); iter++)
					if(iter->second->getMap() == player->getMap() && iter->second != player)
						player->send(PacketCreator().updatePartyHP(iter->second));
			}
		}
	}
	player->setPartyInvite(0);
}

void Parties::invite(Player* player, std::string& name){
	Party* party = player->getParty();
	if(party == NULL || party->getLeader() != player->getID() || player->getName() == name)
		return;
	Player* invited = player->getChannel()->getPlayers()->getPlayerByName(name);
	if(invited == NULL){
		player->send(PacketCreator().partyError(19));
	}
	else{
		if(invited->getPartyInvite() != 0){
			player->send(PacketCreator().partyError(22, name));
		}
		else if(invited->getParty() != NULL){
			player->send(PacketCreator().partyError(16));
		}
		else{
			if(party->getMembers()->getMembers()->size() >= 6){
				player->send(PacketCreator().partyError(16));
			}
			else{
				invited->setPartyInvite(party->getID());
				invited->send(PacketCreator().inviteParty(party->getID(), player->getName()));
			}
		}
	}
}

void Parties::expel(Player* player, int playerid){
	Party* party = player->getParty();
	if(party == NULL || party->getLeader() != player->getID() || player->getID() == playerid){
		return;
	}
	PartyMember* mem = party->getMembers()->getMember(playerid);
	if(mem == NULL) return;
	party->getMembers()->send(PacketCreator().leaveParty(party, mem, true));
	Player* expelled = party->getMembers()->getPlayerByID(playerid);
	party->removeMember(playerid);
	if(expelled != NULL){
		expelled->setParty(NULL);
		if(player->getMap()->getEvent() != NULL && player->getMap()->getEvent()->getParty() && expelled->getMap()->getEvent() == player->getMap()->getEvent())
			player->getChannel()->getEvents()->deleteEvent(player->getMap()->getEvent()->getName());
	}
}

void Parties::changeLeader(Player* player, int playerid){
	//TODO: check that player is in area and in the same channel
	Party* party = player->getParty();
	if(party == NULL || party->getLeader() != player->getID() || player->getID() == playerid){
		return;
	}
	PartyMember* mem = party->getMembers()->getMember(playerid);
	if(mem == NULL) return;
	Player* lead = player->getChannel()->getPlayers()->getPlayerByID(playerid);
	if(lead == NULL){
		player->send(PacketCreator().partyError(29));
	}
	else if(lead->getMap() != player->getMap()){
		player->send(PacketCreator().partyError(27));
	}
	else{
		party->setLeader(playerid);
		party->getMembers()->send(PacketCreator().changeLeader(playerid));
	}
}

void Parties::denied(Player* player, string& to, string& from, bool block){
	Player* fromp = world->getPlayerByName(from);
	if(fromp == NULL || fromp->getParty() == NULL || fromp->getParty()->getID() != player->getPartyInvite() || fromp->getParty()->getLeader() != fromp->getID() || player->getParty() != NULL || player->getName() != to){
		player->setPartyInvite(0);
		return;
	}
	fromp->send(PacketCreator().partyError((block) ? 21 : 23, to));
	player->setPartyInvite(0);
}

Party* Parties::getParty(int id){
	if(parties.find(id) == parties.end()){
		StoreQueryResult res = MySQL::getInstance()->getDataBase()->query("select leader from party where id = " + valstr(id) + " and world = " + valstr(world->getID())).store();
		if(res.empty())
			return NULL;
		Party* party = new Party(id, atoi(res[0][0].c_str()));
		res = MySQL::getInstance()->getDataBase()->query("select id, name, job, level from characters where party = " + valstr(id)).store();
		for(int i=0; i<(int)res.num_rows(); i++){
			PartyMember* member = new PartyMember();
			member->setID(atoi(res[i][0].c_str()));
			member->setName(string(res[i][1].c_str()));
			member->setJob(atoi(res[i][2].c_str()));
			member->setLevel(atoi(res[i][3].c_str()));
			member->setChannel(-2);
			member->setMap(999999999);
			party->getMembers()->addMember(member);
		}
		parties[id] = party;
	}
	return parties[id];
}