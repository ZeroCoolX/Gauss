#pragma once

#include "Consumable.h"

class ItemPickup : public Consumable
{
public:
	ItemPickup(Vector2f position, int type, float lifetime) : Consumable(&pickupTextures[0], Vector2f(1.f, 1.f)) {
		this->type = type;

		this->sprite.setPosition(position);
		this->sprite.setColor(Color(255, 255, 255, 200));

		this->lifetimeTimerMax = lifetime;
		this->lifetimeTimer = 0.f;

		// TEXT SETTINGS
		this->color = Color::Green;
		this->moveDir = Vector2f(0.f, 1.f);
		this->charSize = 24;
		this->lifetime = 40.f;
		this->accelerate = true;
	}
	virtual ~ItemPickup();

	void Consume(dArr<TextTag> &tags, Font *font, Player *player);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);

	// Statics
	static dArr<Texture> pickupTextures;
	static void InitTextures();

	static int numberOfPickups;
};

