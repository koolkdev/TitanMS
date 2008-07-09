#ifndef PLAYERLOGINHANDLER_H
#define PLAYERLOGINHANDLER_H

class PacketReader;
class PlayerLogin;

class PlayerLoginHandler {
private:
	static const short PING = 0xA;
	static const short LOGIN_REQUEST = 0x1b;
	static const short CHANNEL_SELECT = 0x19;
	static const short WORLD_SELECT = 0x13;
	static const short HANDLE_LOGIN = 0x03;
	static const short SHOW_WORLD = 0x18;
	static const short WORLD_BACK = 0x02;
	static const short CHARACTER_SELECT = 0x16;
	static const short NAME_CHECK = 0x09;
	static const short CREATE_CHARACTER = 0x0E;
	static const short DELETE_CHARACTER = 0x0F;
	static const short LOGIN_BACK = 0x1A;
	/*static const short PING = 0x18;
	static const short LOGIN_REQUEST = 0x1;
	static const short CHANNEL_SELECT = 0x5;
	static const short WORLD_SELECT = 0x6;
	static const short HANDLE_LOGIN = 0x09;
	static const short SHOW_WORLD = 0xB;
	static const short WORLD_BACK = 0x11;
	static const short CHARACTER_SELECT = 0x13;
	static const short NAME_CHECK = 0x15;
	static const short CREATE_CHARACTER = 0x16;
	static const short DELETE_CHARACTER = 0x17;
	static const short LOGIN_BACK = 0x1C;
	//static const short SET_GENDER = 0x07;
	//static const short REGISTER_PIN = 0x09;*/


	PlayerLogin* player;
public:
	PlayerLoginHandler(PlayerLogin* player){
		this->player = player;
	}
	void handle(PacketReader* packet);

	void pingHandle(PacketReader* packet);
	void loginRequestHandle(PacketReader* packet);
	void channelSelectHandle(PacketReader* packet);
	void worldSelectHandle(PacketReader* packet);
	void loginHandle(PacketReader* packet);
	void showWorldRequestHandle(PacketReader* packet);
	void characterSelectHandle(PacketReader* packet);
	void nameSelectHandle(PacketReader* packet);
	void createCharacterHandle(PacketReader* packet);
	void deleteCharacterHandle(PacketReader* packet);
	void loginBackHandle(PacketReader* packet);
	//void setGenderHandle(PacketReader* packet);
	//void registerPINHandle(PacketReader* packet);
};

#endif