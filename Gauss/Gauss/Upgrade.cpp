#include "Upgrade.h"



Upgrade::Upgrade(dArr<Texture> &textures,
	 Vector2f position, int type, float lifetime)
{
	this->textures = &textures;
	this->type = type;
	if (this->type >= (int)(*this->textures).Size()) {
		std::cout << "Error! Upgrade.cpp - no type found for " << this->type << std::endl;
	}
	else {
		this->sprite.setTexture((*this->textures)[this->type]);
	}
	// Center the origin
	this->sprite.setOrigin(
		this->sprite.getGlobalBounds().width / 2,
		this->sprite.getGlobalBounds().height / 2
	);
	this->sprite.setPosition(position);
	this->sprite.setScale(0.75f, 0.75f);
	this->aliveTimerMax = lifetime;
	this->aliveTimer = 0.f;
}


Upgrade::~Upgrade()
{
}

bool Upgrade::CheckCollision(const FloatRect rect) {
	return this->sprite.getGlobalBounds().intersects(rect);
}


void Upgrade::Update(const float &dt) {
	if (this->aliveTimer <= this->aliveTimerMax) {
		this->aliveTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}

	this->sprite.rotate(-5.f * dt * DeltaTime::dtMultiplier);
}

void Upgrade::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
