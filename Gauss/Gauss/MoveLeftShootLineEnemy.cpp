#include "MoveLeftShootLineEnemy.h"

MoveLeftShootLineEnemy::~MoveLeftShootLineEnemy()
{
}

void MoveLeftShootLineEnemy::Draw(RenderTarget &renderTarget) {
	EnemyLifeform::Draw(renderTarget);
}

void MoveLeftShootLineEnemy::Update(const float &dt, Vector2f target) {
	this->sprite.move(this->moveDirection.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->moveDirection.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
	this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));

	// Shoot bullet
	if (this->shootTimer < this->shootTimerMax) {
		this->shootTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->shootTimer >= this->shootTimerMax) {
		this->shootTimer = 0.f;
		const float bulletSpeed = rand() % 15 + 10.f;
		MoveLeftShootLineEnemy::bullets.Add(
			Bullet(Bullet::ORB_RED,
				Vector2f(0.2f, 0.2f),
				this->sprite.getPosition(),
				this->normalizedMoveDirection,
				bulletSpeed,
				bulletSpeed,
				this->getDamage(), false, 0.f) // No acceleration - only constant velocity
		);
	}

	EnemyLifeform::Update(dt);
}