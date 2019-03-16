#include "Bullet.h"

Bullet::Bullet(Texture *texture,
	Vector2f scale,
	Vector2f position,
	Vector2f direction,
	float initialVelocity,
	float maxVelocity,
	float acceleration)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->maxVelocity = maxVelocity;
	this->acceleration = acceleration;
	this->direction = direction;
	this->velocity = Vector2f(
		initialVelocity * this->direction.x, 
		initialVelocity * this->direction.y);

	// Setup bullet
	this->sprite.setScale(scale);
	// Compensate for the top left origin of the sprite
	this->sprite.setPosition(
		Vector2f(
			position.x - this->sprite.getGlobalBounds().width / 2, 
			position.y - this->sprite.getGlobalBounds().height / 2)
	);
}

Bullet::~Bullet()
{
}

void Bullet::Movement(const float &dt) {
	// Accelerate to constant max speed
	if (this->acceleration >= 0.f) {
		if (this->velocity.x < this->maxVelocity) {
			this->velocity.x += this->acceleration * this->direction.x * dt * DeltaTime::dtMultiplier;
		}
		if (this->velocity.y < this->maxVelocity) {
			this->velocity.y += this->acceleration * this->direction.y * dt * DeltaTime::dtMultiplier;
		}
	}
	else { // Move at constant speed
		this->velocity = Vector2f(this->maxVelocity * this->direction.x, this->maxVelocity * this->direction.y);
	}

	this->sprite.move(this->velocity.x * dt * DeltaTime::dtMultiplier,this->velocity.y * dt * DeltaTime::dtMultiplier);
}

void Bullet::Update(const float &dt) {
	this->Movement(dt);

}
void Bullet::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
