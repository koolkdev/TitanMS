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

#ifndef ANGELSCRIPTENGINE_H
#define ANGELSCRIPTENGINE_H

#include "../angelscript/angelscript.h"
#include "ByteCodeMemory.h"
#include <string>

using namespace std;

class PlayerNPC;
class Player;
class Reactor;
class MapPortalData;
class Channel;

class AngelScriptEngine {
private:
	static asIScriptEngine* pScriptEngine;
public:
	static int Initialize();
	static int handleNPC(PlayerNPC* npc);
	static int handleReactor(Player* player, Reactor* reactor);
	static ByteCodeMemory<int>* loadNPC(int id);
	static ByteCodeMemory<string>* loadPortal(string id);
	static ByteCodeMemory<string>* loadEvent(string id);
	static ByteCodeMemory<string>* loadChannelEvent(string id);
	static ByteCodeMemory<int>* loadReactor(int id);
	static int handlePortal(Player* player, MapPortalData* portal);
	static int handleEvent(Channel* channel, string& eventn);
	static int handleChannelEvent(Channel* channel, string& eventn);
	static int LoadScript(asIScriptEngine * pScriptEngine, const char *filename, const char * module = 0);
};

#endif