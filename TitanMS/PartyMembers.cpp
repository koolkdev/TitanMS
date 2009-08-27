#include "PartyMembers.h"
#include "PartyMember.h"
#include "Player.h"
#include "Channel.h"
#include "Map.h"


PartyMembers::~PartyMembers(){
	for(hash_map <int, PartyMember*>::iterator iter = members.begin(); iter != members.end(); iter++){
		delete iter->second;
	}
}
void PartyMembers::addMember(Player* player){
	add(player);
	PartyMember* member = new PartyMember();
	member->setID(player->getID());
	member->setJob(player->getJob());
	member->setLevel(player->getLevel());
	member->setChannel(player->getChannel()->getID());
	member->setMap(player->getMap()->getID());
	member->setName(player->getName());
	addMember(member);
}
void PartyMembers::addMember(PartyMember* member){
	if(members.find(member->getID()) != members.end()) delete members[member->getID()];
	members[member->getID()] = member;
}
void PartyMembers::removeMember(int id){
	if(members.find(id) != members.end()){
		members[id]->setChannel(-2);
		members[id]->setMap(999999999);
	}
	if(players.find(id) != players.end()){
		players.erase(id);
	}
}
void PartyMembers::deleteMember(int id){
	if(members.find(id) != members.end()){
		delete members[id];
		members.erase(id);
	}
	if(players.find(id) != players.end()){
		players.erase(id);
	}

}