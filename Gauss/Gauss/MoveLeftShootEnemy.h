#pragma once

#include "EnemyLifeform.h"
#include "Enums.h"

class MoveLeftShootEnemy : public EnemyLifeform
{
private:
	float shootTimer;
	float shootTimerMax;

	dArr<Texture> *bulletTextures;

public:
	MoveLeftShootEnemy(RenderWindow *window, 
		dArr<Texture> &textures,
		dArr<Texture> &bulletTextures,
		View& view,
		int playerLvlScaleFactor,
		int playerFollowNum) : EnemyLifeform(playerFollowNum, &textures[GameEnums::E_MOVE_LEFT_SHOOT], Vector2f(0.15f, 0.15f)) {

		// WINDOW
		this->window = window;

		// MOVEMENT
		this->moveDirection = Vector2f(-1.f, 0.f);
		this->moveSpeed = rand() % 10 + 3.f;

		// STATS
		this->damageRange = Vector2i((rand() % 2 + 1)*playerLvlScaleFactor, (rand() % 1 + 1)*playerLvlScaleFactor);
		this->hpMax = (rand() % 3 + 2) * playerLvlScaleFactor;
		this->hp = this->hpMax;

		// ATTACK
		this->bulletTextures = &bulletTextures;
		this->shootTimerMax = rand() % 200 + 100.f;

		this->InitSpritePoisition(view);
	}
	virtual ~MoveLeftShootEnemy();

	void Draw(RenderTarget &renderTarget);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
};

