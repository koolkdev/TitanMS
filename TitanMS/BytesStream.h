#ifndef BYTESSTREAM_H
#define BYTESSTREAM_H

#include <string>

#define MAX_LENGTH 10000

class BytesStream {
private:
	unsigned char bytes [MAX_LENGTH];
	int insert;
public:
	BytesStream (): insert(0) {}
	void add (unsigned char* v, int len) {
		if(insert+len < MAX_LENGTH){
			memcpy (bytes+insert, v, len);
			insert+=len;
		}
	}
	void add (char v) {
		add((unsigned char*)&v, sizeof(char));
	}
	void add (short v) {
		add((unsigned char*)&v, sizeof(short));
	}
	void add (int v) {
		add((unsigned char *)&v, sizeof(int));
	}
	void add (__int64 v) {
		add((unsigned char *)&v, sizeof(__int64));
	}
	void add (std::string v) {
		add((unsigned char *)v.data(), v.length());
	}
	unsigned char* getBytes(){
		return bytes;
	}
	int getLength(){
		return insert;
	}
	void show(){
		for(int i=0; i<insert; i++){
			printf("%x ", bytes[i]);
		}
		printf("\n");
	}
};

#endif