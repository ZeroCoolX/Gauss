#pragma once

#include "Pickup.h"

class TextTag
{
private:
	Font *font;
	Text text;

	float speed;
	Vector2f direction;
	bool accelerate;

	// Timers
	float timer;
	float timerMax;

public:
	TextTag(Font *font, std::string text, const Color color, Vector2f position, Vector2f direction, unsigned int characterSize, float timerMax, bool accelerate = false);
	virtual ~TextTag();

	// Accessors
	inline const float& getTimer() const { return this->timer; }

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

