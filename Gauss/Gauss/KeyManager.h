#pragma once

#include "GameObject.h"

class KeyManager
{
private:
	dArr<std::map<std::string, int>> keys;

	static std::map<int, std::string> KeyNames;

public:
	KeyManager();
	~KeyManager();

	inline void SetKey(int scheme, std::string name, int key) { this->keys[scheme][name] = key; }

	inline int Up(int scheme) { return this->keys[scheme]["UP"]; }

	inline int Down(int scheme) { return this->keys[scheme]["DOWN"]; }
	inline int Left(int scheme) { return this->keys[scheme]["LEFT"]; }
	inline int Right(int scheme) { return this->keys[scheme]["RIGHT"]; }
	inline int Fire(int scheme) { return this->keys[scheme]["FIRE"]; }
	inline int GaussCannon(int scheme) { return this->keys[scheme]["MAC"]; }
	inline int Shield(int scheme) { return this->keys[scheme]["SHIELD"]; }
	inline int ToggleStats(int scheme) { return this->keys[scheme]["STATS"]; }

	static void InitKeyNames();
	static std::string KeyName(int code);

	enum DefaultControls {
		DEF_UP = Keyboard::W,
		DEF_DOWN = Keyboard::S,
		DEF_LEFT = Keyboard::A,
		DEF_RIGHT = Keyboard::D,
		DEF_FIRE = Keyboard::Space,
		DEF_MAC = Keyboard::Q,
		DEF_SHIELD = Keyboard::RAlt,
		DEF_STATS = Keyboard::Tab
	};
};

