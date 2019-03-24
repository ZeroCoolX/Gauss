#pragma once

#include "EnemyLifeform.h"
#include "Enums.h"

class MoveLeftEnemy : public EnemyLifeform
{

public:
	MoveLeftEnemy(dArr<Texture> &textures,
		Vector2u windowBounds,
		int playerLvlScaleFactor,
		int playerFollowNum) : EnemyLifeform(playerFollowNum, &textures[GameEnums::E_MOVE_LEFT], Vector2f(0.175f, 0.175f)) {

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = rand() % 15 + 4.f;

		// STATS
		this->damageRange = Vector2i((rand() % 3 + 1)*playerLvlScaleFactor, (rand() % 2 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 5 + 2) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		this->InitSpritePoisition(windowBounds);
	}
	virtual ~MoveLeftEnemy();

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};
