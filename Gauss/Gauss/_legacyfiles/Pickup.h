#pragma once

#include "Upgrade.h"

class Pickup
{
private:
	int type;

	dArr<Texture> *textures;
	Sprite sprite;

	float aliveTimerMax;
	float aliveTimer;

public:
	Pickup(dArr<Texture> &textures, Vector2f position, int type, float lifetime);
	virtual ~Pickup();

	inline const int& getType() const { return this->type; }
	inline const bool canDelete() const { return this->aliveTimer >= this->aliveTimerMax; }
	inline Vector2f getPosition() { return this->sprite.getPosition(); }

	bool CheckCollision(FloatRect rect);
	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

