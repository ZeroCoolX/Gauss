#pragma once

#include "GameObject.h"

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
	TextTag(Font *font, Vector2f position, std::string text = "", const Color color = Color::White, Vector2f direction = Vector2f(0.f, 1.f), unsigned int characterSize = 20, float timerMax = 40.f, bool accelerate = false);
	virtual ~TextTag();

	// Accessors
	inline const float& getTimer() const { return this->timer; }

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
	void SetTextProperties(std::string text, const Color color, Vector2f direction, unsigned int characterSize, float timerMax, bool accelerate = false);
};

