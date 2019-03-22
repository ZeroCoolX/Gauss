#include "Bullet.h"

Bullet::~Bullet()
{
}

void Bullet::Move(const float &dt) {
	if (this->acceleration >= 0.f) {
		_accelerationMovement(dt);
	}
	else {
		_constantMovement(dt);
	}
	// Final Move
	this->sprite.move(this->velocity.x * dt * DeltaTime::dtMultiplier,this->velocity.y * dt * DeltaTime::dtMultiplier);
}

void Bullet::Update(const float &dt, Vector2f target) {
	this->Move(dt);
}
void Bullet::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}

void Bullet::_constantMovement(const float &dt) {
	this->velocity = Vector2f(this->maxVelocity * this->direction.x, this->maxVelocity * this->direction.y);
}

void Bullet::_accelerationMovement(const float &dt) {
	if (this->velocity.x < this->maxVelocity) {
		this->velocity.x += this->acceleration * this->direction.x * dt * DeltaTime::dtMultiplier;
	}
	if (this->velocity.y < this->maxVelocity) {
		this->velocity.y += this->acceleration * this->direction.y * dt * DeltaTime::dtMultiplier;
	}
}
