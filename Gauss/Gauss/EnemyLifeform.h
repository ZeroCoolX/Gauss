#pragma once

#include "GameObject.h"
#include "Bullet.h"

class EnemyLifeform : public GameObject
{
protected:
	const float damageTimerMax = 3.f;

	// Game window
	RenderWindow *window;

	// STATS
	int hp;
	int hpMax;
	float moveSpeed = 10.f;

	// TRACKING
	int playerFollowNum;

	// MOVEMENT
	Vector2f moveDirection;
	Vector2f normalizedMoveDirection;
	Vector2f lookDirection;
	Vector2f normalizedLookDirection;

	// TIMERS
	float damageTimer;
	Vector2i damageRange;

	// MODIFIER?
	bool universeModifier;

protected:
	float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	Vector2f normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}

public:
	EnemyLifeform(int playerFollowNum, Texture *texture, Vector2f spriteScale, bool universeModifier = false) : GameObject(texture, spriteScale) {
		this->universeModifier = universeModifier;
		// TRACKING
		this->playerFollowNum = playerFollowNum;
	}
	virtual ~EnemyLifeform();

	inline const int getHp() const { return hp; }
	inline const int getHpMax() const { return this->hpMax; }
	inline const int getDamage() const { return rand() % this->damageRange.y + this->damageRange.x; }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	inline const int& getPlayerFollowNum() const { return this->playerFollowNum; }
	inline void updatePlayerFollowNum(int num) { this->playerFollowNum = num; }
	inline Vector2f getVelocity() const { return Vector2f(this->moveSpeed * this->normalizedMoveDirection.x, this->moveSpeed * this->normalizedMoveDirection.y); }
	inline const bool isUniverseModifier() const { return this->universeModifier; }

	virtual void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	virtual void Draw(RenderTarget &renderTarget);
	void InitSpritePoisition(const View& view, Vector2f forcedPosition);
	void Collision();
	void TakeDamage(int damage);

	// statics
	static int nrOfEnemyTypes;
	static dArr<Bullet> bullets;
	static dArr<Texture> enemyTextures;
	enum EnemyType { 
		MOVE_LEFT = 0, 
		FOLLOW, 
		MOVE_LEFT_SHOOT, 
		MOVE_LEFT_SHOOT_LINE, 
		COSMOSOS_01,
		COSMOSOS_02,
		COSMOSOS_03,
		COSMOSOS_04,
		FOLLOW_SHOOT,
		FOLLOW_FAST_SHOOT 
	};

	static bool hotSwap;
	static int GetEnemyType(int eType);
	static void InitializeTextures();
};

