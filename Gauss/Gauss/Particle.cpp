#include "Particle.h"

dArr<Texture> Particle::particleTextures;

Particle::Particle(float posX,
	float posY,
	int textureIndex,
	float dirX,
	float dirY,
	float maxVelocity,
	float maxRotation,
	float lifetime)
{
	this->maxVelocity = maxVelocity;
	this->maxRotation = maxRotation;
	this->lifetime = lifetime;
	this->dir.x = dirX;
	this->dir.y = dirY;

	this->sprite.setTexture(Particle::particleTextures[textureIndex]);
	this->sprite.setPosition(Vector2f(posX, posY));
	this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
}


Particle::~Particle()
{
}

void Particle::Update(const float &dt) {
	// Lifetime
	if (this->lifetime > 0) {
		this->lifetime -= 1.f * dt * DeltaTime::dtMultiplier;

		// Reduce the alpha over time
		if (this->sprite.getColor().a > 0) {
			this->sprite.setColor(Color(255, 255, 255, this->sprite.getColor().a - 1 * dt * DeltaTime::dtMultiplier));
			// Once it drops below 0 it goes above 255 because unsigned int
			if (this->sprite.getColor().a > 255) {
				this->sprite.setColor(Color(255, 255, 255, 0));
			}
		}
		//Movement - TODO: Can multiply dt by whole vector
		this->sprite.move(this->maxVelocity * this->dir.x * dt * DeltaTime::dtMultiplier,
			this->maxVelocity * this->dir.y * dt * DeltaTime::dtMultiplier);
		this->sprite.rotate(this->maxRotation * dt * DeltaTime::dtMultiplier);
	}
}

void Particle::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
