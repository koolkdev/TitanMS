#include "PlayerLogin.h"
#include "Characters.h"
#include "Login.h"
#include "Worlds.h"

void PlayerLogin::handleRequest(unsigned char* buf, int len){
	short header = buf[0] + buf[1]*0x100;
	switch(header){
		case 0x01: Login::loginUser(this, buf+2); break;
		case 0x04: Worlds::channelSelect(this, buf+2); break;
		case 0x05: Worlds::selectWorld(this, buf+2); break;
		case 0x07: Login::setGender(this, buf+2); break;
		case 0x08: Login::handleLogin(this, buf+2); break;
		case 0x09: Login::registerPIN(this, buf+2); break;
		case 0x0a: Worlds::showWorld(this); break;
		case 0x0b: Worlds::showWorld(this); break;
		case 0x0f: Characters::connectGame(this, buf+2); break;
		case 0x11: Characters::checkCharacterName(this, buf+2); break;
		case 0x15: Characters::createCharacter(this, buf+2); break;
		case 0x16: Characters::deleteCharacter(this, buf+2); break;
		case 0x1b: Login::loginBack(this); break;
	}
}