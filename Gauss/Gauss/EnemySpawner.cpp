#include "EnemySpawner.h"



EnemySpawner::EnemySpawner(Vector2f position,
	int type,
	int level,
	int levelInterval,
	int numOfEnemies,
	float spawnTimerMax)
{
	this->sprite.setPosition(position);
	this->type = type;
	this->level = level;
	this->levelInterval = levelInterval;
	this->numOfEnemies = numOfEnemies;
	this->spawnTimerMax = spawnTimerMax;
	this->spawnTimer = this->spawnTimerMax;
}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::UpdateTimer() {

}

void EnemySpawner::IsInScreen(View &view) {

}

void EnemySpawner::Spawn() {

}

void EnemySpawner::Update(View &view) {

}

void EnemySpawner::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
