#include "Channels.h"
#include "../Connection/Acceptor.h"
#include "../Connection/Selector.h"
#include "Player.h"
#include "Worlds.h"
#include "Channel.h"

void Channels::newChannel(){
	int port = Worlds::getOpenPort();
	channelsports.push_back(port);
	//new Acceptor(channelsports[channels.size()], Worlds::getInstance()->getSelector(), new PlayerFactory());
	new Acceptor(channelsports[channels.size()], new Selector(), new PlayerFactory());
	channels.push_back(new Channel(world, channels.size(), port));

		
}