#pragma once

#include "Consumable.h"

enum powerup_types {RF = 0, XP2};

class Powerup : public Consumable
{
public:
	Powerup(float lifeTimeMax , int type, Vector2f pos) : Consumable(&powerupTextures[type], Vector2f(1.f, 1.f)) {
		this->sprite.setPosition(pos);

		this->lifetimeTimerMax = lifeTimeMax;
		this->lifetimeTimer = 0.f;
	}
	virtual ~Powerup();

	void Consume(dArr<TextTag> &tags, Font *font, Player *player);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);

	// Statics
	static dArr<Texture> powerupTextures;
};

