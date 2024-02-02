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

#include "DataProvider.h"
#include "PacketCreator.h"
#include "PacketWriter.h"
#include "Reactor.h"


PacketWriter* PacketCreator::showReactor(Reactor* reactor){
	pw.writeShort(SHOW_REACTOR);

	pw.writeInt(reactor->getID());
	pw.writeInt(reactor->getReactorID());
	pw.write(reactor->getState());
	pw.writeShort(reactor->getPosition().x);
	pw.writeShort(reactor->getPosition().y);
	pw.write(0);
	pw.show();

	return &pw;
}
PacketWriter* PacketCreator::updateReactor(Reactor* reactor, short stance){
	pw.writeShort(UPDATE_REACTOR);

	pw.writeInt(reactor->getID());
	pw.write(reactor->getState());
	pw.writeShort(reactor->getPosition().x);
	pw.writeShort(reactor->getPosition().y);
	pw.writeShort(stance);
	pw.write(0);

	pw.write(5); //?
	pw.show();
	return &pw;
}
PacketWriter* PacketCreator::destroyReactor(Reactor* reactor){
	pw.writeShort(DESTROY_REACTOR);

	pw.writeInt(reactor->getID());
	pw.write(reactor->getState());
	pw.writeShort(reactor->getPosition().x);
	pw.writeShort(reactor->getPosition().y);
	pw.show();

	return &pw;
}
