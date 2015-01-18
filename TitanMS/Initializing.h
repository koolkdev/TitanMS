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

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <string>
class DropsData;
class MobData;
class MapData;
class EquipData;
class PetData;
class ShopData;
class ItemData;
class QuestData;
class SkillData;
class ReactorData;
class EventData;

class Initializing {
public:
	static void initializing();
	static void initializeSettings();
	static void initializeMobs();
	static void initializeNPCsScripts();
	static void initializeMobsDrops();
	static void initializeReactorsDrops();
	static void initializeMaps();
	static void initializeEquips();
	static void initializeShops();
	static void initializeItems();
	static void initializeQuests();
	static void initializeSkills();
	static void initializeReactors();
	static void initializePortals();
	static void initializeReactorScripts();
	static void initializeEvents();
	static void initializeChannelEvents();
	static void initializeEventsScripts();
	static void initializeChannelEventsScripts();
	static void initializePets();
	static DropsData* loadMobDrops(int id);
	static DropsData* loadReactorDrops(int id);
	static MapData* loadMap(int id);
	static MobData* loadMob(int id);
	static EquipData* loadEquip(int id);
	static ShopData* loadShop(int id);
	static ItemData* loadItem(int id);
	static QuestData* loadQuest(int id);
	static SkillData* loadSkill(int id);
	static PetData* loadPet(int id);
	static ReactorData* loadReactor(int id);
	static EventData* loadEvent(std::string& id);
};

#endif