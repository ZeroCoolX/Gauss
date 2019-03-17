#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include "DeltaTime.h"

using namespace sf;

class TextTag
{
private:
	Font *font;
	Text text;

	float speed;
	Vector2f direction;
	bool follow;

	// Timers
	float timer;
	float timerMax;

public:
	TextTag(Font *font, std::string text, const Color color, Vector2f position, unsigned int characterSize, float timerMax);
	virtual ~TextTag();

	// Accessors
	inline const float& getTimer() const { return this->timer; }

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

