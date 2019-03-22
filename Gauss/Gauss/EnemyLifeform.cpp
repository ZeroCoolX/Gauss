#include "EnemyLifeform.h"



EnemyLifeform::~EnemyLifeform()
{
}

void EnemyLifeform::InitSpritePoisition(const Vector2u windowBounds) {
	const float xPos((float)windowBounds.x);
	const float yPos((float)(rand() % windowBounds.y - this->sprite.getGlobalBounds().height));
	this->sprite.setPosition(xPos, yPos);
}

void EnemyLifeform::Collision() {
	this->damageTimer = this->damageTimerMax;
}

void EnemyLifeform::TakeDamage(int damage) {
	this->hp = std::max(0, (this->hp - damage));

	// Damage effect
	this->damageTimer = this->damageTimerMax;
}

void EnemyLifeform::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}

void EnemyLifeform::Update(const float &dt, Vector2f target) {
	if (this->damageTimer > 0.f) {
		this->sprite.setColor(Color::Red);
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		// Knockback
		this->sprite.move(this->moveSpeed * -this->normalizedMoveDirection.x * this->damageTimer * dt * DeltaTime::dtMultiplier,
			this->moveSpeed * -this->normalizedMoveDirection.y * this->damageTimer * dt * DeltaTime::dtMultiplier);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

