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
	int playerFollowNum;

	float _vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	Vector2f _normalize(Vector2f v, float length) {
		if (length == 0) { return v; }
		return (v / length);
	}

public:
	Enemy(Texture* texture, 
		int type, 
		Vector2u windowBounds,
		Vector2f scale, 
		Vector2f direction, 
		int hpMax, 
		Vector2i damageRange,
		int playerFollowNum);

	virtual ~Enemy();

	// Accessors
	inline const int getDamage() const { return rand() % this->damageRange.y + this->damageRange.x; }
	inline const int getHp() const { return hp; }
	inline const int getHpMax() const { return this->hpMax; }
	inline const bool isDead() const { return this->hp <= 0; }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	inline const int& getPlayerFollowNum() const { return this->playerFollowNum; }

	// Functions
	void TakeDamage(int damage);
	void Update(const float &dt, Vector2f playerPosition);
	void Draw(RenderTarget &renderTarget);
};

