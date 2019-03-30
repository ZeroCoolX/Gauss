#pragma once

#include "Consumable.h"

enum powerup_types {RF = 0, XP2};

class Powerup : public Consumable
{
public:
	Powerup(Vector2f position, int type, float lifetime) : Consumable(&powerupTextures[type], Vector2f(0.75f, 0.75f)) {
		this->sprite.setPosition(position);

		this->lifetimeTimerMax = lifetime;
		this->lifetimeTimer = 0.f;
	}
	virtual ~Powerup();

	void Consume(dArr<TextTag> &tags, Font *font, Player *player);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);

	// Statics
	static dArr<Texture> powerupTextures;
	static int numberOfPowerups;
};
