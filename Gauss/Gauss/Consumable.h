#pragma once

#include "GameObject.h"
#include "TextTag.h"
#include "Player.h"

class Consumable : public GameObject
{
protected:
	int type;

	float lifetimeTimerMax;
	float lifetimeTimer;

	// TextTag properties
	Color color;
	Vector2f moveDir;
	int charSize;
	float lifetime;
	bool accelerate;

public:
	Consumable(Texture *texture,
		Vector2f scale) : GameObject(texture, scale) {
		this->sprite.setOrigin(
			this->sprite.getGlobalBounds().width / 2,
			this->sprite.getGlobalBounds().height / 2
		);
	}
	virtual ~Consumable();

	inline const int& getType() const { return this->type; }
	inline bool canDelete() { return this->lifetimeTimer >= this->lifetimeTimerMax; }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }

	virtual void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	virtual void Draw(RenderTarget &renderTarget);
	virtual void Consume(dArr<TextTag> &tags, Font *font, Player *player);
	bool CollidesWith(const FloatRect rect);
};

