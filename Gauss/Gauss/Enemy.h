#pragma once

#include "Bullet.h"

class Enemy
{
private:
	Texture *texture;
	Sprite sprite;
	Vector2f direction;

	float damageTimerMax;
	float damageTimer;

	float moveSpeed = 10.f;

	int type;
	int hp;
	int hpMax;
	Vector2i damageRange;

public:
	Enemy(Texture* texture, 
		int type, 
		Vector2u windowBounds,
		Vector2f scale, 
		Vector2f direction, 
		int hpMax, 
		Vector2i damageRange);

	virtual ~Enemy();

	// Accessors
	inline const int getDamage() const { return rand() % this->damageRange.y + this->damageRange.x; }
	inline const int getHp() const { return hp; }
	inline const int getHpMax() const { return this->hpMax; }
	inline const bool isDead() const { return this->hp <= 0; }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }

	// Functions
	void TakeDamage(int damage);
	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

