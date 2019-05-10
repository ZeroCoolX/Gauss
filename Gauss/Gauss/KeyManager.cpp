#include "KeyManager.h"

KeyManager::KeyManager() 
{
	// Populate defaults
	std::map<std::string, int> defaultKeys;

	defaultKeys["UP"] = Keyboard::W;
	defaultKeys["DOWN"] = Keyboard::S;
	defaultKeys["LEFT"] = Keyboard::A;
	defaultKeys["RIGHT"] = Keyboard::D;
	defaultKeys["FIRE"] = Keyboard::Space;
	defaultKeys["GAUSSCANNON"] = Keyboard::Q;
	defaultKeys["SHIELD"] = Keyboard::RAlt;
	defaultKeys["TOGGLESTATS"] = Keyboard::Tab;

	this->keys.Add(defaultKeys);
}


KeyManager::~KeyManager()
{
}

