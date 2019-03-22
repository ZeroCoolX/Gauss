#pragma once

#include "EnemyLifeform.h"
#include "Enums.h"

class TrackerEnemy : public EnemyLifeform
{
private:
	int attackDistance;
	bool divebombPlayer = false;

public:
	TrackerEnemy(dArr<Texture> &textures,
		Vector2u windowBounds,
		int playerLvlScaleFactor,
		int playerFollowNum): EnemyLifeform(playerFollowNum, &textures[GameEnums::E_FOLLOW], Vector2f(0.12f, 0.12f)) {
		
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

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

