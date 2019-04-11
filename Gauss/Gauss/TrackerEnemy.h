#pragma once

#include "EnemyLifeform.h"

class TrackerEnemy : public EnemyLifeform
{
private:
	int attackDistance;
	bool divebombPlayer = false;

public:
	TrackerEnemy(View& view,
		int playerLvlScaleFactor,
		int playerFollowNum): EnemyLifeform(playerFollowNum, &EnemyLifeform::enemyTextures[EnemyLifeform::FOLLOW], Vector2f(0.12f, 0.12f)) {
		
		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = rand() % 12 + 5.f;

		// STATS
		this->damageRange = Vector2i((rand() % 2 + 1)*playerLvlScaleFactor, (rand() % 1 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 4 + 1) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		// ATTACK
		this->attackDistance = rand() % 700 + 300;

		this->InitSpritePoisition(view);
	}
	virtual ~TrackerEnemy();

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

