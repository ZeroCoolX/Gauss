#pragma once

#include "GameObject.h"
#include "Gauss.h"
#include "EnemyLifeform.h"

class EnemySpawner
{
private:
	Vector2i gridPos;
	bool randomSpawnPos;
	int forcedVelocity; // -1 random
	int type;// -1 is random
	int levelInterval;
	int numOfEnemies; // -1 is random (1-10) // TODO: need to space them out

	bool used;

public:
	EnemySpawner(Vector2i gridPos,
		bool randomSpawnPos,
		int forcedVelocity,
		int type,
		int levelInterval,
		int numOfEnemies);
	virtual ~EnemySpawner();

	inline Vector2f getPosition() const { return Vector2f(static_cast<float>(this->gridPos.x * Gauss::GRID_SIZE), static_cast<float>(this->gridPos.y * Gauss::GRID_SIZE)); }
	inline Vector2i getGridPosition() const { return this->gridPos; }
	inline bool isRandomSpawnPos() const { return this->randomSpawnPos; }
	inline int getForcedVelocity() const { return this->forcedVelocity; }
	inline int getType() const { return this->type; }
	inline int getLevelInterval() const { return this->levelInterval; }
	inline int getNumOfEnemies() const { return this->numOfEnemies; }
	inline void setUsed() { this->used = true; }
	inline void setUnused() { this->used = false; }
	inline bool isUsed() const { return this->used; }

	void UpdateTimer();
	void Update(View &view);
	void Draw(RenderTarget &renderTarget, Font &font);

	void IsInScreen(View &view);
	void Spawn();
	std::string GetAsString() const;
};

