#pragma once

#include "EnemyLifeform.h"
#include "Enums.h"

class TrackerEnemy : EnemyLifeform
{
private:
	const int type = GameEnums::E_FOLLOW;
	const Vector2f scale = Vector2f(0.12f, 0.12f);

	int attackDistance;

public:
	TrackerEnemy(dArr<Texture> &textures,
		dArr<Texture> &bulletTextures,
		Vector2u windowBounds,
		int playerLvlScaleFactor,
		int playerFollowNum): EnemyLifeform(playerFollowNum, &textures[this->type], this->scale) {
		
		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = rand() % 12 + 5.f;

		// STATS
		this->damageRange = Vector2i((rand() % 2 + 1)*playerLvlScaleFactor, (rand() % 1 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 3 + 1) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		// ATTACK
		this->attackDistance = rand() % 700 + 300;

		this->InitSpritePoisition(windowBounds);
	}
	virtual ~TrackerEnemy();
};

