#pragma once
#include "EnemyLifeform.h"

class MoveLeftShootLineEnemy : public EnemyLifeform
{
private:
	float shootTimer;
	float shootTimerMax;
	int nrOfBullets;

public:
	MoveLeftShootLineEnemy(RenderWindow *window,
		View& view,
		int playerLvlScaleFactor,
		int playerFollowNum,
		float forcedVelocity,
		Vector2f forcedPosition = Vector2f(0.f, 0.f)) : EnemyLifeform(playerFollowNum, &EnemyLifeform::enemyTextures[EnemyLifeform::MOVE_LEFT_SHOOT_LINE], Vector2f(0.15f, 0.15f)) {

		// WINDOW
		this->window = window; // TODO: remove

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = (forcedVelocity > 0 ? forcedVelocity : rand() % 10 + 3.f);

		// STATS
		this->damageRange = Vector2i((rand() % 3 + 1)*playerLvlScaleFactor, (rand() % 1 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 5 + 1) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		// ATTACK
		this->shootTimerMax = 7.f;
		this->shootTimer = 0.f;
		this->nrOfBullets = 3;

		this->InitSpritePoisition(view, forcedPosition);
	}
	virtual ~MoveLeftShootLineEnemy();

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

