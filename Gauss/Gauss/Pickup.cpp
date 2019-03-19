#include "Pickup.h"



Pickup::Pickup(dArr<Texture> *textures, Vector2f position, int type, float lifetime)
{
	this->textures = textures;
	this->type = type;
	if (this->type >= (int)(*this->textures).Size()) {
		throw "ERROR! No texture for this type";
	}
	this->sprite.setTexture((*this->textures)[this->type]);

	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2);

	this->sprite.setPosition(position);

	this->sprite.setColor(Color(255, 255, 255, 200));

	this->aliveTimerMax = lifetime;
	this->aliveTimer = 0;
}


Pickup::~Pickup()
{
}

bool Pickup::CheckCollision(FloatRect rect) {
	return this->sprite.getGlobalBounds().intersects(rect);
}

void Pickup::Update(const float &dt) {
	if (this->aliveTimer < this->aliveTimerMax) {
		this->aliveTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}

	// Constantly slowly spinning
	this->sprite.rotate(5.f * dt * DeltaTime::dtMultiplier);
}

void Pickup::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
