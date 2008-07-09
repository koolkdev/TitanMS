#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Selector.h"
#include "AbstractPlayer.h"
#include <hash_map>
using namespace std;
using namespace stdext;

class Acceptor: public Selector::SelectHandler {
public:

	Acceptor (int port, Selector* selector, AbstractPlayerFactory* apf, bool master = false);
	virtual int handle (Selector* selector, int socket);
	bool master;

protected:
	AbstractPlayerFactory* abstractPlayerFactory;
private:
	static hash_map <int, int> ports;
};

#endif