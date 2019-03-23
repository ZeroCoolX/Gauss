#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include "DeltaTime.h"
#include "dArr.h"

using namespace sf;

class Upgrade
{
private:
	int type;
	dArr<Texture> *textures;
	Sprite sprite;

	float aliveTimerMax;
	float aliveTimer;

public:
	Upgrade(dArr<Texture> &textures,
		Vector2f position, int type, float lifetime);
	virtual ~Upgrade();

	inline const int& getType() const { return this->type; }
	inline bool canDelete() { return this->aliveTimer >= this->aliveTimerMax; }

	bool CheckCollision(const FloatRect rect);
	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

