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
	this->sprite.setPosition(windowBounds.x, rand() % windowBounds.y + this->sprite.getGlobalBounds().height);

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->damageRange = damageRange;
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage(int damage){
	this->hp = std::max(0, (this->hp - damage));
}

void Enemy::Update(){
	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->sprite.move(-10.f, 0.f); // TODO: Enemy speed should be a const
			break;
	}
}

void Enemy::Draw(RenderTarget &renderTarget){
	renderTarget.draw(this->sprite);
}
