#pragma once

#include "GameObject.h"

class EnemySpawner
{
private:
	Vector2i gridPos;
	bool randomSpawnPos;
	int type;// -1 is random
	int levelInterval;
	int numOfEnemies; // -1 is random

public:
	EnemySpawner(Vector2i gridPos,
		bool randomSpawnPos,
		int type,
		int levelInterval,
		int numOfEnemies);
	virtual ~EnemySpawner();

	inline Vector2f getPosition() const { return Vector2f(this->gridPos.x * Gauss::GRID_SIZE, this->gridPos.y * Gauss::GRID_SIZE); }
	inline Vector2i getGridPosition() const { return this->gridPos; }
	inline bool isRandomSpawnPos() const { return this->randomSpawnPos; }
	inline int getType() const { return this->type; }
	inline int getLevelInterval() const { return this->levelInterval; }
	inline int getNumOfEnemies() const { return this->numOfEnemies; }

	void UpdateTimer();
	void Update(View &view);
	void Draw(RenderTarget &renderTarget, Font &font);

	void IsInScreen(View &view);
	void Spawn();
	std::string GetAsString() const;
};

