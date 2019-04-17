#include "TrackerEnemy.h"



TrackerEnemy::~TrackerEnemy()
{
}

void TrackerEnemy::Update(const float &dt, Vector2f target) {
	// Get distance to see if we need to divebomb
	float adjustedMoveSpeed;

	const float distance = sqrt(pow(this->sprite.getPosition().x - target.x, 2) + pow(this->sprite.getPosition().y - target.y, 2));

	if (distance < this->attackDistance || this->divebombPlayer) {
		this->divebombPlayer = true;
		this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));
		adjustedMoveSpeed = this->moveSpeed;
	}
	else {
		// Get the direction from us to the player 
		this->moveDirection.x = target.x - this->sprite.getPosition().x;
		this->moveDirection.y = target.y - this->sprite.getPosition().y;
		// Normalize direction
		this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));
		// Angle needed to go from current facing dir to face player
		float angle = atan2(this->normalizedMoveDirection.y, this->normalizedMoveDirection.x) * 180.f / 3.14159265359f + 180.f; // TODO: DEFINE PI
		// Rotate
		this->sprite.setRotation(angle);
		adjustedMoveSpeed = this->moveSpeed * 0.75f;
	}

	this->sprite.move(this->normalizedMoveDirection.x * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier, this->normalizedMoveDirection.y * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier);

	EnemyLifeform::Update(dt);
}

void TrackerEnemy::Draw(RenderTarget &renderTarget) {
	EnemyLifeform::Draw(renderTarget);
}