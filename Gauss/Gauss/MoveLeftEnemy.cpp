#include "MoveLeftEnemy.h"


MoveLeftEnemy::~MoveLeftEnemy()
{
}

void MoveLeftEnemy::Update(const float &dt, Vector2f target) {
	this->sprite.move(this->moveDirection.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->moveDirection.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
	this->normalizedMoveDirection = this->normalize(this->moveDirection, this->vectorLength(this->moveDirection));
	EnemyLifeform::Update(dt);
}

void MoveLeftEnemy::Draw(RenderTarget &renderTarget) {
	EnemyLifeform::Draw(renderTarget);
}
