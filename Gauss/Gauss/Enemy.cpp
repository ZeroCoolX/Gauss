#include "Enemy.h"
#include "Enums.h"

Enemy::Enemy(Texture* texture,
	int type,
	Vector2u windowBounds,
	Vector2f scale,
	Vector2f direction,
	int hpMax,
	Vector2i damageRange)
{
	this->type = type;

	this->texture = texture;
	this->sprite.setTexture(*texture);
	this->sprite.setScale(scale);

	const float xPos((float)windowBounds.x);
	const float yPos((float)(rand() % windowBounds.y + this->sprite.getGlobalBounds().height));
	this->sprite.setPosition(xPos, yPos);

	this->damageTimerMax = 3.f;
	this->damageTimer = 0;

	this->direction = direction;

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->damageRange = damageRange;
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage(int damage){
	this->hp = std::max(0, (this->hp - damage));

	// Damage effect
	this->damageTimer = this->damageTimerMax;
}

void Enemy::Update(const float &dt){
	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->sprite.move(this->direction.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->direction.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
			break;
	}

	if (this->damageTimer > 0.f) {
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		this->sprite.setColor(Color::Red);
		this->sprite.move(this->moveSpeed/2 * this->damageTimer * dt * DeltaTime::dtMultiplier, 0.f);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

void Enemy::Draw(RenderTarget &renderTarget){
	renderTarget.draw(this->sprite);
}
