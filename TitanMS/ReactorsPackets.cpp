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

	return &pw;
}
PacketWriter* PacketCreator::destroyReactor(Reactor* reactor){
	pw.writeShort(DESTROY_REACTOR);

	pw.writeInt(reactor->getID());
	pw.write(reactor->getState());
	pw.writeShort(reactor->getPosition().x);
	pw.writeShort(reactor->getPosition().y);

	return &pw;
}
