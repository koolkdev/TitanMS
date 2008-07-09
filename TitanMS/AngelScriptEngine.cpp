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

#include "AngelScriptEngine.h"
#include "PlayerNPC.h"
#include <assert.h>
#include "scriptstring.h"
#include "Tools.h"
#include <sstream>
#include "ByteCodeMemory.h"
#include "NPCsData.h"
#include "ReactorScriptsData.h"
#include "DataProvider.h"
#include "Reactor.h"
#include "Player.h"
#include "Map.h"
#include "Mob.h"
#include "Channel.h"
using namespace Tools;

asIScriptEngine* AngelScriptEngine::pScriptEngine;


const int tr=1;
const int fa=0;

void f(){}

int strval(string& num){
	return atoi((char*)num.c_str());
}
string& valstr(int num){
	stringstream buff;
	buff << num;
	string r;
	buff >> r;
	return *(new string(r));
}
bool isNULL(void* ptr){
	return ptr == NULL;
}
int AngelScriptEngine::Initialize(){
	pScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if( pScriptEngine == NULL )	return 0; 

	int r;

	RegisterScriptString(pScriptEngine);
	// Defenitions
	
	pScriptEngine->RegisterGlobalProperty("const int YES", (void*)&tr);
	pScriptEngine->RegisterGlobalProperty("const int NO", (void*)&fa);
	pScriptEngine->RegisterGlobalProperty("const int ACCEPT", (void*)&tr);
	pScriptEngine->RegisterGlobalProperty("const int DECLINE", (void*)&fa);
	pScriptEngine->RegisterGlobalProperty("const int NULL", (void*)&fa);

	r = pScriptEngine->RegisterObjectType("Position", sizeof(Position), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS); assert( r >= 0 );
	r = pScriptEngine->RegisterObjectProperty("Position", "int16 x", offsetof(Position,x)); assert( r >= 0 );
	r = pScriptEngine->RegisterObjectProperty("Position", "int16 y", offsetof(Position,y)); assert( r >= 0 );

	// Register Objects

	// Map
	r = pScriptEngine->RegisterObjectType("Map", sizeof(Map), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Map", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Map", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Player
	r = pScriptEngine->RegisterObjectType("Player", sizeof(Player), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Player", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Player", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// NPC
	r = pScriptEngine->RegisterObjectType("NPC", sizeof(PlayerNPC), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("NPC", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("NPC", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Mob
	r = pScriptEngine->RegisterObjectType("Mob", sizeof(Mob), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Mob", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Mob", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Reactor
	r = pScriptEngine->RegisterObjectType("Reactor", sizeof(Reactor), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Reactor", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Reactor", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Channel
	r = pScriptEngine->RegisterObjectType("Channel", sizeof(Channel), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Channel", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Channel", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Map functions	

	r = pScriptEngine->RegisterGlobalFunction("bool isNULL(Reactor@)",
		asFUNCTION(isNULL), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("bool isNULL(Player@)",
		asFUNCTION(isNULL), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("bool isNULL(Map@)",
		asFUNCTION(isNULL), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("bool isNULL(Mob@)",
		asFUNCTION(isNULL), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("bool isNULL(NPC@)",
		asFUNCTION(isNULL), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("int strval(string&)",
		asFUNCTION(strval), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("string& valstr(int)",
		asFUNCTION(valstr), asCALL_CDECL);	 assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectMethod("Map","Mob@ spawnMob(int, int, int)",
		asMETHOD(Map, spawnMob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Reactor@ spawnReactor(int, int, int)",
		asMETHOD(Map, spawnReactor), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void killMob(Mob@)",
		asMETHOD(Map, killMob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void killMobs()",
		asMETHOD(Map, killMobs), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","int getMobsCount()",
		asMETHOD(Map, getMobsCount), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","int getID()",
		asMETHOD(Map, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Reactor@ getReactor(int)",
		asMETHOD(Map, getReactor), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Mob@ getMob(int)",
		asMETHOD(Map, getMob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void destroyReactor(Reactor@)",
		asMETHOD(Map, destroyReactor), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Reactor@ getReactorByReactorID(int)",
		asMETHOD(Map, getReactorByReactorID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void setSpawn(bool)",
		asMETHOD(Map, setSpawn), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void respawnReactors()",
		asMETHOD(Map, respawnReactors), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void clearDrops()",
		asMETHOD(Map, clearDrops), asCALL_THISCALL);	 assert( r >= 0) ;

	// Player functions

	r = pScriptEngine->RegisterObjectMethod("Player","Position getPosition()",
		asMETHOD(MapObject, getPosition), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void changeMap(int)",
		asMETHOD(Player, setMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getItemAmount(int)",
		asMETHOD(Player, getItemAmount), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getMesos()",
		asMETHOD(Player, getMesos), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","bool giveItem(int, int16)",
		asMETHOD(Player, giveItem), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void giveMesos(int)",
		asMETHOD(Player, giveMesos), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","Map@ getMap()",
		asMETHOD(Player, getMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setStyle(int)",
		asMETHOD(Player, setStyle), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void giveExp(int)",
		asMETHOD(Player, addExp), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getLevel()",
		asMETHOD(Player, getLevel), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getJob()",
		asMETHOD(Player, getJob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setJob(int)",
		asMETHOD(Player, setJob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setLevel(int)",
		asMETHOD(Player, setLevel), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getHair()",
		asMETHOD(Player, getHair), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getFace()",
		asMETHOD(Player, getFace), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setHP(int16)",
		asMETHOD(Player, setHP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getHP()",
		asMETHOD(Player, getHP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setAP(int)",
		asMETHOD(Player, setAP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setSP(int)",
		asMETHOD(Player, setSP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setStr(int)",
		asMETHOD(Player, setStr), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setDex(int)",
		asMETHOD(Player, setDex), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setInt(int)",
		asMETHOD(Player, setInt), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setLuk(int)",
		asMETHOD(Player, setLuk), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getAP()",
		asMETHOD(Player, getAP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getSP()",
		asMETHOD(Player, getSP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getStr()",
		asMETHOD(Player, getStr), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getDex()",
		asMETHOD(Player, getDex), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getInt()",
		asMETHOD(Player, getInt), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getLuk()",
		asMETHOD(Player, getLuk), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","Channel@ getChannel()",
		asMETHOD(Player, getChannel), asCALL_THISCALL);	 assert( r >= 0) ;

	// Mob object

	r = pScriptEngine->RegisterObjectMethod("Mob","int getID()",
		asMETHOD(Mob, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Mob","int getMobID()",
		asMETHOD(Mob, getMobID), asCALL_THISCALL);	 assert( r >= 0) ;

	// Channel object

	r = pScriptEngine->RegisterObjectMethod("Channel","Map@ getMap(int)",
		asMETHOD(Channel, getMap), asCALL_THISCALL);	 assert( r >= 0) ;

	// NPC object

	r = pScriptEngine->RegisterObjectMethod("NPC","void addText(string&)",
		asMETHOD(PlayerNPC, addText), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void addChar(int8)",
		asMETHOD(PlayerNPC, addChar), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendSimple()",
		asMETHOD(PlayerNPC, sendSimple), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendYesNo()",
		asMETHOD(PlayerNPC, sendYesNo), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendNext()",
		asMETHOD(PlayerNPC, sendNext), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendBackNext()",
		asMETHOD(PlayerNPC, sendBackNext), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendBackOK()",
		asMETHOD(PlayerNPC, sendBackOK), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendOK()",
		asMETHOD(PlayerNPC, sendOK), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendAcceptDecline()",
		asMETHOD(PlayerNPC, sendAcceptDecline), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendGetText()",
		asMETHOD(PlayerNPC, sendGetText), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendGetNumber(int, int, int)",
		asMETHOD(PlayerNPC, sendGetNumber), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendStyle(int[]& , int8)",
		asMETHOD(PlayerNPC, sendStyle), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","bool isQuest()",
		asMETHOD(PlayerNPC, isQuest), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","int getState()",
		asMETHOD(PlayerNPC, getState), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","int getSelected()",
		asMETHOD(PlayerNPC, getSelected), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void setState(int)",
		asMETHOD(PlayerNPC, setState), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void setSelected(int)",
		asMETHOD(PlayerNPC, setSelected), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void setGetNumber(int)",
		asMETHOD(PlayerNPC, setGetNumber), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","int getNumber()",
		asMETHOD(PlayerNPC, getNumber), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void setGetText(string&)",
		asMETHOD(PlayerNPC, setGetText), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","string& getText()",
		asMETHOD(PlayerNPC, getText), asCALL_THISCALL); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void end()",
		asMETHOD(PlayerNPC, end), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","bool isEnd()",
		asMETHOD(PlayerNPC, isEnd), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","int getNpcID()",
		asMETHOD(PlayerNPC, getNpcID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void setVariable(string&, int)",
		asMETHOD(PlayerNPC, setVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","int getVariable(string&)",
		asMETHOD(PlayerNPC, getVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","Player@ getPlayer()",
		asMETHOD(PlayerNPC, getPlayer), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void addQuest(int16)",
		asMETHOD(PlayerNPC, addQuest), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("NPC","void endQuest(int16)",
		asMETHOD(PlayerNPC, endQuest), asCALL_THISCALL);	 assert( r >= 0) ;

	// Reactor object

	r = pScriptEngine->RegisterObjectMethod("Reactor","Map@ getMap()",
		asMETHOD(Reactor, getMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","void drop()",
		asMETHOD(Reactor, drop), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","int getState()",
		asMETHOD(Reactor, getState), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","void setState(int)",
		asMETHOD(Reactor, setState), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","Position getPosition()",
		asMETHOD(Reactor, getPosition), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","int getID()",
		asMETHOD(Reactor, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Reactor","int getReactorID()",
		asMETHOD(Reactor, getReactorID), asCALL_THISCALL);	 assert( r >= 0) ;

	return 1;	

}

int AngelScriptEngine::LoadScript(asIScriptEngine * pScriptEngine, const char *filename, const char * module){


	FILE * pFile;
	fopen_s(&pFile, filename, "rb");
	if( pFile == 0 )
	{
		//printf("Failed to open the script file.\n");
		return -1;
	}

	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);


	std::string code;
	code.resize(len);

	size_t c = fread(&code[0], len, 1, pFile);
	fclose(pFile);

	if( c == 0 ) 
	{
		//printf("Failed to load script file.\n");
		return -1;
	}

	int nRet = pScriptEngine->AddScriptSection(module, filename, code.c_str(), len, 0);
	if( nRet < 0 ) 
	{
		printf("An error occured while adding the script section.\n");
	} 

	return nRet;
}

ByteCodeMemory<int>* AngelScriptEngine::loadReactor(int id){
	char name[100];
	sprintf_s(name, 100, "Scripts/Reactors/%d.as", id);
	if (LoadScript( pScriptEngine, name, name) < 0){
		return 0; 
	}
	int d = pScriptEngine->Build( name );
	if( d < 0 )
	{
		printf("Failed to compile script: %s \n", name);
		return 0;
	}
	
	ByteCodeMemory<int>* st = new ByteCodeMemory<int>(id, name);
	pScriptEngine->SaveByteCode(name, st);
	ReactorScriptsData::getInstance()->add(st);
	return st;
}
ByteCodeMemory<int>* AngelScriptEngine::loadNPC(int id){
	char name[100];
	sprintf_s(name, 100, "Scripts/NPCs/%d.as", id);
	if (LoadScript( pScriptEngine, name, name) < 0){
		return 0; 
	}
	int d = pScriptEngine->Build( name );
	if( d < 0 )
	{
		printf("Failed to compile script: %s \n", name);
		return 0;
	}
	
	ByteCodeMemory<int>* st = new ByteCodeMemory<int>(id, name);
	pScriptEngine->SaveByteCode(name, st);
	NPCsData::getInstance()->add(st);
	return st;
}

int AngelScriptEngine::handleReactor(Player* player, Reactor* reactor){
	ByteCodeMemory<int>* st = ReactorScriptsData::getInstance()->getDataByID(reactor->getReactorID());
	if(st == NULL)
		return 0;
	st->resetReadPos();
	pScriptEngine->LoadByteCode(st->getName(), st);

	asIScriptContext * pContext = pScriptEngine->CreateContext();
	if( pContext == 0 ) 
	{
		printf("Failed to create the context.");
		pScriptEngine->Release();
		return 0;
	}
	int nFunctionID = -1;
	nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "reactor_main");
	if(nFunctionID < 0){
		return 0;
	}
	pContext->Prepare( nFunctionID );

	pContext->SetArgObject(0, reactor);

	pContext->SetArgObject(1, player);


	pContext->Execute( );

	return 1;

}
int AngelScriptEngine::handleNPC(PlayerNPC* npc){
	ByteCodeMemory<int>* st = NPCsData::getInstance()->getDataByID(npc->getNpcID());
	if(st == NULL)
		return 0;
	st->resetReadPos();
	pScriptEngine->LoadByteCode(st->getName(), st);

	asIScriptContext * pContext = pScriptEngine->CreateContext();
	if( pContext == 0 ) 
	{
		printf("Failed to create the context.");
		pScriptEngine->Release();
		return 0;
	}
	int nFunctionID = -1;
	if(npc->isQuest()){ 
		// TODO:
		//
		//	nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "end_quest_main");
		//else
			nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "start_quest_main");
	}
	else
		nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "npc_main");
	if(nFunctionID < 0){
		return 0;
	}
	pContext->Prepare( nFunctionID );

	pContext->SetArgObject(0, npc);

	pContext->Execute( );

	return 1;

}