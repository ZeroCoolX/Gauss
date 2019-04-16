#pragma once

#include "GameObject.h"

class EnemySpawner
{
private:
	Vector2i gridPos;
	int type;// -1 is random
	int level;
	int levelInterval;
	int numOfEnemies; // -1 is random
	float spawnTimerMax;
	float spawnTimer;

public:
	EnemySpawner(Vector2i gridPos,
		int type,
		int level,
		int levelInterval,
		int numOfEnemies,
		float spawnTimerMax);
	virtual ~EnemySpawner();

	void UpdateTimer();
	void IsInScreen(View &view);
	void Spawn();
	void Update(View &view);
	void Draw(RenderTarget &renderTarget);
};

