#pragma once

#include "Bullet.h"

class Enemy
{
private:
	dArr<Texture> *textures;
	Sprite sprite;
	Vector2f moveDirection;
	Vector2f normalizedMoveDirection;
	Vector2f lookDirection;
	Vector2f normalizedLookDirection;

	float damageTimerMax;
	float damageTimer;

	float moveSpeed = 10.f;

	int type;
	int hp;
	int hpMax;
	Vector2i damageRange;
	bool divebombPlayer = false;
	int divebombDistanceThreshold = false;
	int playerFollowNum;

	float _vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	Vector2f _normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}

public:
	Enemy(dArr<Texture> &textures, 
		int type, 
		Vector2u windowBounds,
		Vector2f scale, 
		Vector2f moveDirection,
		int scalar,
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
	inline void updatePlayerFollowNum(int num) { this->playerFollowNum = num; }
	inline void updateAttackType(int type) { this->type = type; }

	// Functions
	void Collision();
	void TakeDamage(int damage);
	void Update(const float &dt, Vector2f playerPosition = Vector2f());
	void Draw(RenderTarget &renderTarget);
};

