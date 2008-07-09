#ifndef PHANDLEERROR_H
#define PHANDLEERROR_H

#include <string>
using namespace std;

class PacketHandlingError { 
protected:
	string error;
public:
	virtual string getError(){
		return error;
	}
};

#endif