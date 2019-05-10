#pragma once

#include "GameObject.h"

class KeyManager
{
private:
	dArr<std::map<std::string, int>> keys;
	/*
		int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE,
	int GAUSSCANNON,
	int SHIELD,
	int TOGGLESTATS,
	int CHANGE_LWING,
	int CHANGE_CPIT,
	int CHANGE_RWING,
	int CHANGE_AURA
	*/

public:
	KeyManager();
	~KeyManager();

	inline int Up(int scheme) { return this->keys[scheme]["UP"]; }
	inline int Down(int scheme) { return this->keys[scheme]["DOWN"]; }
	inline int Left(int scheme) { return this->keys[scheme]["LEFT"]; }
	inline int Right(int scheme) { return this->keys[scheme]["RIGHT"]; }
	inline int Fire(int scheme) { return this->keys[scheme]["FIRE"]; }
	inline int GaussCannon(int scheme) { return this->keys[scheme]["GAUSSCANNON"]; }
	inline int Shield(int scheme) { return this->keys[scheme]["SHIELD"]; }
	inline int ToggleStats(int scheme) { return this->keys[scheme]["TOGGLESTATS"]; }

};

