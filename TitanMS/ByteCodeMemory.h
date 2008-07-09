#ifndef BYTECODE_H
#define BYTECODE_H

#include <string>
#include "AngelScriptEngine.h"
using namespace std;

template <typename T>
class ByteCodeMemory : public asIBinaryStream {
private:
	int readPos, writePos;
	unsigned char* bytes;
	string name;
	int len;
	T id;
public:
	ByteCodeMemory(T id, char* name): bytes(new unsigned char[256]), len(256), readPos(0), writePos(0), name(string(name)), id(id){}
	void adjust(){
		unsigned char* newbytes = new unsigned char[len*2];
		memcpy (newbytes, bytes, len);
		len *= 2;
		delete [] bytes;
		bytes = newbytes;
	}
	void resetReadPos(){
		readPos = 0;
	}
	void resetWritePos(){
		readPos = 0;
	}
	void Write(const void *ptr, asUINT size){
		while (writePos + (int)size > len) {
			adjust();
		}
		memcpy (bytes+writePos, ptr, size);
		writePos+=size;
	}
	void Read(void *ptr, asUINT size){
		memcpy (ptr, bytes+readPos, size);
		readPos+=size;		
	}
	T getID(){
		return id;
	}
	char* getName(){
		return (char*)name.c_str();
	}

};

#endif
