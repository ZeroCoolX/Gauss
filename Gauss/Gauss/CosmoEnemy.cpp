#include "CosmoEnemy.h"

void CosmoEnemy::Update(const float &dt, Vector2f target) {
	// Get distance to see if we need to divebomb
	float adjustedMoveSpeed;

	// Get the direction from us to the player 
	this->moveDirection.x = target.x - this->sprite.getPosition().x;
	this->moveDirection.y = target.y - this->sprite.getPosition().y;
	// Normalize direction
	this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));
	// Angle needed to go from current facing dir to face player
	float angle = atan2(this->normalizedMoveDirection.y, this->normalizedMoveDirection.x) * 180.f / 3.14159265359f + 180.f;
	// Rotate
	this->sprite.setRotation(angle);
	adjustedMoveSpeed = this->moveSpeed * 0.75f;

	this->sprite.move(this->normalizedMoveDirection.x * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier, this->normalizedMoveDirection.y * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier);

	EnemyLifeform::Update(dt);
}

void CosmoEnemy::Draw(RenderTarget &renderTarget) {
	EnemyLifeform::Draw(renderTarget);
}
