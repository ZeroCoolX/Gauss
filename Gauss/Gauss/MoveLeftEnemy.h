#pragma once

#include "EnemyLifeform.h"

class MoveLeftEnemy : public EnemyLifeform
{

public:
	MoveLeftEnemy(View& view,
		int playerLvlScaleFactor,
		int playerFollowNum, 
		float forcedVelocity,
		Vector2f forcedPosition = Vector2f(0.f, 0.f)) : EnemyLifeform(playerFollowNum, &EnemyLifeform::enemyTextures[EnemyLifeform::GetEnemyType(EnemyLifeform::MOVE_LEFT)], Vector2f(0.175f, 0.175f)) {

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = (forcedVelocity > 0 ? forcedVelocity : rand() % 15 + 4.f);

		// STATS
		this->damageRange = Vector2i((rand() % 3 + 1)*playerLvlScaleFactor, (rand() % 2 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 5 + 2) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		this->InitSpritePoisition(view, forcedPosition);
	}
	virtual ~MoveLeftEnemy();

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

