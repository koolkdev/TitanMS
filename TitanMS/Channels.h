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

#ifndef CHANNELS_H
#define CHANNELS_H

#include <vector>

using namespace std;

class Channel;
class Selector;
class World;

class Channels {
private:
	vector <Channel*> channels;
	vector <int> channelsports;
	World* world;
public:
	Channels(World* world, int n){
		this->world = world;
		for(int i=0; i<n; i++) {
			newChannel();
		}
	}

	void newChannel();

	int getChannelsCount(){
		return channels.size();
	}
	Channel* getChannel(unsigned int id){
		if(id > channels.size())
			return NULL;
		return channels[id];
	}
	int getChannelPort(unsigned int id){
		if(id > channels.size())
			return -1;
		return channelsports[id];
		
	}

};

#endif
