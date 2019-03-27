#include "MoveLeftShootEnemy.h"


MoveLeftShootEnemy::~MoveLeftShootEnemy()
{
}

void MoveLeftShootEnemy::Draw(RenderTarget &renderTarget) {
	EnemyLifeform::Draw(renderTarget);
}

void MoveLeftShootEnemy::Update(const float &dt, Vector2f target){
	if (this->shootTimer < this->shootTimerMax) {
		this->shootTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}

	this->lookDirection.x = target.x - this->sprite.getPosition().x;
	this->lookDirection.y = target.y - this->sprite.getPosition().y;
	this->normalizedLookDirection = this->normalize(this->lookDirection, this->vectorLength(this->lookDirection));

	// Rotate
	this->sprite.setRotation(atan2(this->normalizedLookDirection.y, this->normalizedLookDirection.x) * 180.f / 3.14159265359f + 180.f);

	this->sprite.move(this->moveDirection.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->moveDirection.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
	this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));

	// Shoot bullet
	if (this->shootTimer >= this->shootTimerMax) {
		this->shootTimer = 0.f;
		MoveLeftShootEnemy::bullets.Add(
			Bullet(&(*this->bulletTextures)[GameEnums::EB_DEFAULT],
				Vector2f(0.2f, 0.2f),
				this->sprite.getPosition(),
				this->normalizedLookDirection,
				15.f, 15, this->getDamage(), false, 0.f) // No acceleration - only constant velocity
		);
	}

	EnemyLifeform::Update(dt);
}
