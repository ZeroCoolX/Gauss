#pragma once

#include "EnemyLifeform.h"
#include "Bullet.h"
#include "Enums.h"

class MoveLeftShootEnemy : public EnemyLifeform
{
private:
	float shootTimer;
	float shootTimerMax;

	dArr<Texture> *bulletTextures;
	dArr<Bullet> bullets;

public:
	MoveLeftShootEnemy(RenderWindow *window, 
		dArr<Texture> &textures,
		dArr<Texture> &bulletTextures,
		Vector2u windowBounds,
		int playerLvlScaleFactor,
		int playerFollowNum) : EnemyLifeform(playerFollowNum, &textures[GameEnums::E_MOVE_LEFT_SHOOT], Vector2f(0.15f, 0.15f)) {

		// WINDOW
		this->window = window;

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = rand() % 10 + 3.f;

		// STATS
		this->damageRange = Vector2i((rand() % 2 + 1)*playerLvlScaleFactor, (rand() % 1 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 3 + 1) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		// ATTACK
		this->bulletTextures = &bulletTextures;
		this->shootTimerMax = 50.f;

		this->InitSpritePoisition(windowBounds);
	}
	virtual ~MoveLeftShootEnemy();

	inline dArr<Bullet>& getBullets() { return this->bullets; }

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

