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
#include "NPCsScriptsData.h"
#include "PortalsData.h"
#include "ReactorScriptsData.h"
#include "DataProvider.h"
#include "Reactor.h"
#include "Event.h"
#include "Player.h"
#include "Party.h"
#include "Players.h"
#include "PlayerInventories.h"
#include "Map.h"
#include "Mob.h"
#include "MapPortalData.h"
#include "Channel.h"
#include "EventsScriptsData.h"
#include "ChannelEventsScriptsData.h"
using namespace Tools;

asIScriptEngine* AngelScriptEngine::pScriptEngine;


const int tr=1;
const int fa=0;
const int asEQUIP = EQUIP;
const int asUSE = USE;
const int asSETUP= SETUP;
const int asETC = ETC;
const int asCASH = CASH;

void f(){}

int strval(string& num){
	return atoi((char*)num.c_str());
}
__int64 strval64(string& num){
	__int64 ret;
	stringstream s(num);
	s >> ret;
	return ret;
}
string& valstr(int num){
	stringstream buff;
	buff << num;
	string r;
	buff >> r;
	return *(new string(r));
}
int randomminmax(int min, int max){
	return Tools::random(min, max);
}
int randomnumber(int n){
	return Tools::random(n);
}

unsigned int sizeofF(asIScriptArray* arr){
	return arr->GetElementCount();
}

#ifdef _DEBUG
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "Error";
	if( msg->type == asMSGTYPE_WARNING )
		type = "Warning";
	else if( msg->type == asMSGTYPE_INFORMATION ) 
		type = "Info";
	printf("%s(%d) : %s: %s\n", msg->section, msg->row, type, msg->message);
}
#endif

int AngelScriptEngine::Initialize(){
	pScriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if( pScriptEngine == NULL )	return 0; 

	int r;

	RegisterScriptString(pScriptEngine);
	// Defenitions
	
	r = pScriptEngine->RegisterGlobalProperty("const int YES", (void*)&tr); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int NO", (void*)&fa); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int ACCEPT", (void*)&tr); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int DECLINE", (void*)&fa); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int EQUIP", (void*)&asEQUIP); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int USE", (void*)&asUSE); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int SETUP", (void*)&asSETUP); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int ETC", (void*)&asETC); assert( r >= 0 );
	r = pScriptEngine->RegisterGlobalProperty("const int CASH", (void*)&asCASH); assert( r >= 0 );

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
	// Players
	r = pScriptEngine->RegisterObjectType("Players", sizeof(Players), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Players", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Players", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
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
	// Portal
	r = pScriptEngine->RegisterObjectType("Portal", sizeof(MapPortalData), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Portal", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Portal", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Party
	r = pScriptEngine->RegisterObjectType("Party", sizeof(Party), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Party", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Party", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	// Event
	r = pScriptEngine->RegisterObjectType("Event", sizeof(Event), asOBJ_REF ); assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Event", asBEHAVE_ADDREF, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectBehaviour("Event", asBEHAVE_RELEASE, "void f()", asFUNCTION(f), asCALL_CDECL_OBJLAST); assert( r >= 0) ;

	r = pScriptEngine->RegisterGlobalFunction("int strval(string&)",
		asFUNCTION(strval), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("int random(int, int)",
		asFUNCTION(randomminmax), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("int random(int)",
		asFUNCTION(randomnumber), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("int getMinute()",
		asFUNCTION(Tools::getMinute), asCALL_CDECL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterGlobalFunction("uint sizeof(int[]&)",
		asFUNCTION(sizeofF), asCALL_CDECL);	 assert( r >= 0) ;

	// Map functions	

	r = pScriptEngine->RegisterObjectMethod("Map","Mob@ spawnMob(int, int, int)",
		asMETHOD(Map, spawnMob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Reactor@ spawnReactor(int, int, int)",
		asMETHOD(Map, spawnReactor), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void killMob(Mob@)",
		asMETHOD(Map, killMob), asCALL_THISCALL);	 assert( r >= 0) ;
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
	r = pScriptEngine->RegisterObjectMethod("Map","void clearMobs()",
		asMETHOD(Map, clearMobs), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void clearReactors()",
		asMETHOD(Map, clearReactors), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void clearDrops()",
		asMETHOD(Map, clearDrops), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void clear()",
		asMETHOD(Map, clear), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void setShip(bool)",
		asMETHOD(Map, setShip), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","bool getShip()",
		asMETHOD(Map, getShip), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Position getShipPosition()",
		asMETHOD(Map, getShipPosition), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Players@ getPlayers()",
		asMETHOD(Map, getPlayers), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void changeMusic(string&)",
		asMETHOD(Map, setMusic), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","string& getMusic()",
		asMETHOD(Map, getMusic), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Channel@ getChannel()",
		asMETHOD(Map, getChannel), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void showEffect(string&)",
		asMETHOD(Map, showEffect), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void playSound(string&)",
		asMETHOD(Map, playSound), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void showObject(string&)",
		asMETHOD(Map, showObject), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","void enablePortal(string&)",
		asMETHOD(Map, enablePortal), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Map","Event@ getEvent()",
		asMETHOD(Map, getEvent), asCALL_THISCALL);	 assert( r >= 0) ;

	// Event functions

	r = pScriptEngine->RegisterObjectMethod("Event","Players@ getPlayers()",
		asMETHOD(Event, getPlayers), asCALL_THISCALL);	 assert( r >= 0) ;

	// Player functions

	r = pScriptEngine->RegisterObjectMethod("Player","Position getPosition()",
		asMETHOD(Player, getPosition), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getFoothold()",
		asMETHOD(Player, getFoothold), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getID()",
		asMETHOD(Player, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void changeMap(int)",
		asMETHOD(Player, setMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void changeMap(int, int)",
		asMETHOD(Player, setMapPos), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void changeMap(int, string&)",
		asMETHOD(Player, setMapPortal), asCALL_THISCALL);	 assert( r >= 0) ;
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
		asMETHOD(Player, giveExp), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","uint8 getLevel()",
		asMETHOD(Player, getLevel), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getJob()",
		asMETHOD(Player, getJob), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setJob(int16)",
		asMETHOD(Player, setJobS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setLevel(uint8)",
		asMETHOD(Player, setLevelS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getHair()",
		asMETHOD(Player, getHair), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getFace()",
		asMETHOD(Player, getFace), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setHP(int16)",
		asMETHOD(Player, setHPS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","uint16 getHP()",
		asMETHOD(Player, getHP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setAP(int16)",
		asMETHOD(Player, setAPS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setSP(int16)",
		asMETHOD(Player, setSPS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setStr(int16)",
		asMETHOD(Player, setStrS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setDex(int16)",
		asMETHOD(Player, setDexS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setInt(int16)",
		asMETHOD(Player, setIntS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setLuk(int16)",
		asMETHOD(Player, setLukS), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getAP()",
		asMETHOD(Player, getAP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getSP()",
		asMETHOD(Player, getSP), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getStr()",
		asMETHOD(Player, getStr), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getDex()",
		asMETHOD(Player, getDex), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getInt()",
		asMETHOD(Player, getInt), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int16 getLuk()",
		asMETHOD(Player, getLuk), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","Channel@ getChannel()",
		asMETHOD(Player, getChannel), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setVariable(string&, int)",
		asMETHOD(Player, setVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getVariable(string&)",
		asMETHOD(Player, getVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void deleteVariable(string&)",
		asMETHOD(Player, deleteVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setGlobalVariable(string&, int)",
		asMETHOD(Player, setGlobalVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","int getGlobalVariable(string&)",
		asMETHOD(Player, getGlobalVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void deleteGlobalVariable(string&)",
		asMETHOD(Player, deleteGlobalVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","void setEvent(string&)",
		asMETHOD(Player, setEvent), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","Party@ getParty()",
		asMETHOD(Player, getParty), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Player","bool checkSlot(int)",
		asMETHOD(Player, checkSlot), asCALL_THISCALL);	 assert( r >= 0) ;

	// Players functions

	r = pScriptEngine->RegisterObjectMethod("Players","void changeMap(int)",
		asMETHOD(Players, changeMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Players","void changeMap(int, int)",
		asMETHOD(Players, changeMapPos), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Players","void changeMap(int, string&)",
		asMETHOD(Players, changeMapPortal), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Players","int getAmount()",
		asMETHOD(Players, getPlayersCount), asCALL_THISCALL);	 assert( r >= 0) ;

	r = pScriptEngine->RegisterObjectBehaviour("Players", asBEHAVE_INDEX, "Player@ f(int)", asMETHOD(Players,operator[]), asCALL_THISCALL); assert( r >= 0 );


	// Mob object

	r = pScriptEngine->RegisterObjectMethod("Mob","int getID()",
		asMETHOD(Mob, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Mob","int getMobID()",
		asMETHOD(Mob, getMobID), asCALL_THISCALL);	 assert( r >= 0) ;

	// Channel object

	r = pScriptEngine->RegisterObjectMethod("Channel","Map@ getMap(int)",
		asMETHOD(Channel, getMap), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","int getID()",
		asMETHOD(Channel, getID), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool createEvent(string&, Player@)",
		asMETHOD(Channel, createEventForPlayer), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool createEvent(string&, Players@)",
		asMETHOD(Channel, createEventForPlayers), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool createEvent(string&, Party@)",
		asMETHOD(Channel, createEventForParty), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool createEvent(string&)",
		asMETHOD(Channel, createEvent), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","void deleteEvent(string&)",
		asMETHOD(Channel, deleteEvent), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool deleteEvent(string&, Player@)",
		asMETHOD(Channel, deleteEventForPlayer), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Channel","bool isEventActive(string&)",
		asMETHOD(Channel, isEventActive), asCALL_THISCALL);	 assert( r >= 0) ;

	// Portal object

	r = pScriptEngine->RegisterObjectMethod("Portal","int getID()",
		asMETHOD(MapPortalData, getID), asCALL_THISCALL);	 assert( r >= 0) ;

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
	r = pScriptEngine->RegisterObjectMethod("NPC","void sendStyle(int[]&)",
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

	// Party object
	r = pScriptEngine->RegisterObjectMethod("Party","int getLeader()",
		asMETHOD(Party, getLeader), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Party","Players@ getMembers()",
		asMETHOD(Party, getMembers), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Party","void setVariable(string&, int)",
		asMETHOD(Party, setVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Party","int getVariable(string&)",
		asMETHOD(Party, getVariable), asCALL_THISCALL);	 assert( r >= 0) ;
	r = pScriptEngine->RegisterObjectMethod("Party","void deleteVariable(string&)",
		asMETHOD(Party, deleteVariable), asCALL_THISCALL);	 assert( r >= 0) ;


#ifdef _DEBUG
	pScriptEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
#endif

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

ByteCodeMemory<string>* AngelScriptEngine::loadPortal(string id){
	char name[100];
	sprintf_s(name, 100, "Scripts/Portals/%s.as", (char*)id.c_str());
	if (LoadScript( pScriptEngine, name, name) < 0){
		return 0; 
	}
	int d = pScriptEngine->Build( name );
	if( d < 0 )
	{
		printf("Failed to compile script: %s \n", name);
		return 0;
	}
	
	ByteCodeMemory<string>* st = new ByteCodeMemory<string>(id, name);
	//pScriptEngine->SaveByteCode(name, st);
	PortalsData::getInstance()->add(st);
	return st;
}
ByteCodeMemory<string>* AngelScriptEngine::loadEvent(string id){
	char name[100];
	sprintf_s(name, 100, "Scripts/Events/%s.as", (char*)id.c_str());
	if (LoadScript( pScriptEngine, name, name) < 0){
		return 0; 
	}
	int d = pScriptEngine->Build( name );
	if( d < 0 )
	{
		printf("Failed to compile script: %s \n", name);
		return 0;
	}
	
	ByteCodeMemory<string>* st = new ByteCodeMemory<string>(id, name);
	//pScriptEngine->SaveByteCode(name, st);
	EventsScriptsData::getInstance()->add(st);
	return st;
}

ByteCodeMemory<string>* AngelScriptEngine::loadChannelEvent(string id){
	char name[100];
	sprintf_s(name, 100, "Scripts/ChannelEvents/%s.as", (char*)id.c_str());
	if (LoadScript( pScriptEngine, name, name) < 0){
		return 0; 
	}
	int d = pScriptEngine->Build( name );
	if( d < 0 )
	{
		printf("Failed to compile script: %s \n", name);
		return 0;
	}
	
	ByteCodeMemory<string>* st = new ByteCodeMemory<string>(id, name);
	//pScriptEngine->SaveByteCode(name, st);
	ChannelEventsScriptsData::getInstance()->add(st);
	return st;
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
	//pScriptEngine->SaveByteCode(name, st);
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
	//pScriptEngine->SaveByteCode(name, st);
	NPCsScriptsData::getInstance()->add(st);
	return st;
}

int AngelScriptEngine::handleReactor(Player* player, Reactor* reactor){
	reactor->drop(player);
	ByteCodeMemory<int>* st = ReactorScriptsData::getInstance()->getDataByID(reactor->getReactorID());
	if(st == NULL)
		return 0;
	//st->resetReadPos();
	//pScriptEngine->LoadByteCode(st->getName(), st);

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

	pContext->Execute( );

	return 1;

}
int AngelScriptEngine::handleEvent(Channel* channel, string& eventn){
	ByteCodeMemory<string>* st = EventsScriptsData::getInstance()->getDataByID(eventn);
	if(st == NULL)
		return 0;
	//st->resetReadPos();
	//pScriptEngine->LoadByteCode(st->getName(), st);

	asIScriptContext * pContext = pScriptEngine->CreateContext();
	if( pContext == 0 ) 
	{
		printf("Failed to create the context.");
		pScriptEngine->Release();
		return 0;
	}
	int nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "event_main");
	if(nFunctionID < 0){
		return 0;
	}
	pContext->Prepare( nFunctionID );

	pContext->SetArgObject(0, channel);

	pContext->Execute( );

	return 1;

}
int AngelScriptEngine::handleChannelEvent(Channel* channel, string& eventn){
	ByteCodeMemory<string>* st = ChannelEventsScriptsData::getInstance()->getDataByID(eventn);
	if(st == NULL)
		return 0;
	//st->resetReadPos();
	//pScriptEngine->LoadByteCode(st->getName(), st);

	asIScriptContext * pContext = pScriptEngine->CreateContext();
	if( pContext == 0 ) 
	{
		printf("Failed to create the context.");
		pScriptEngine->Release();
		return 0;
	}
	int nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "event_main");
	if(nFunctionID < 0){
		return 0;
	}
	pContext->Prepare( nFunctionID );

	pContext->SetArgObject(0, channel);

	pContext->Execute( );

	return 1;

}
int AngelScriptEngine::handlePortal(Player* player, MapPortalData* portal){
	ByteCodeMemory<string>* st = PortalsData::getInstance()->getDataByID(portal->getScript());
	if(st == NULL)
		return 0;
	//st->resetReadPos();
	//pScriptEngine->LoadByteCode(st->getName(), st);

	asIScriptContext * pContext = pScriptEngine->CreateContext();
	if( pContext == 0 ) 
	{
		printf("Failed to create the context.");
		pScriptEngine->Release();
		return 0;
	}
	int nFunctionID = pScriptEngine->GetFunctionIDByName(st->getName(), "portal_main");
	if(nFunctionID < 0){
		return 0;
	}
	pContext->Prepare( nFunctionID );

	pContext->SetArgObject(0, player);

	pContext->SetArgObject(1, portal);

	pContext->Execute( );

	return 1;

}
int AngelScriptEngine::handleNPC(PlayerNPC* npc){
	ByteCodeMemory<int>* st = NPCsScriptsData::getInstance()->getDataByID(npc->getNpcID());
	if(st == NULL)
		return 0;
	//st->resetReadPos();
	//pScriptEngine->LoadByteCode(st->getName(), st);

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

	if(pContext->Execute() !=  asEXECUTION_FINISHED){
		return 0;
	}

	return 1;

}