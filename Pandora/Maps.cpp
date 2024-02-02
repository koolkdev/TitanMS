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

#include "Maps.h"
#include "Map.h"
#include "MapsData.h"
#include "MapData.h"

Maps::Maps(Channel* channel){
	this->channel = channel;
}
Map* Maps::getMap(int map){
	if(maps.find(map) != maps.end())
		return maps[map];
	MapData* data = MapsData::getInstance()->getDataByID(map);
	if(data == NULL)
		return NULL;
	maps[map] = new Map(channel, map);
	return maps[map];
}
