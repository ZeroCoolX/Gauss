#pragma once

#include "EnemyLifeform.h"

class CosmoEnemy : public EnemyLifeform
{
public:
	CosmoEnemy(View& view,
		int cosmoType,
		int playerLvlScaleFactor,
		int playerFollowNum,
		float forcedVelocity,
		Vector2f forcedPosition = Vector2f(0.f, 0.f)) : EnemyLifeform(playerFollowNum, &EnemyLifeform::enemyTextures[EnemyLifeform::GetEnemyType(cosmoType)], Vector2f(0.175f, 0.175f), true) {

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = (forcedVelocity > 0 ? forcedVelocity : rand() % 20 + 5.f);

		// STATS
		this->damageRange = Vector2i(2, 1);
		this->hpMax = (rand() % 4 + 2) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		this->InitSpritePoisition(view, forcedPosition);
	}
	virtual ~CosmoEnemy(){}

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

