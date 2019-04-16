#include "EnemySpawner.h"
#include "Gauss.h"


EnemySpawner::EnemySpawner(Vector2i gridPos,
	int type,
	int level,
	int levelInterval,
	int numOfEnemies,
	float spawnTimerMax)
{
	this->gridPos = gridPos;
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
	RectangleShape shape;
	shape.setFillColor(Color::White);
	shape.setSize(Vector2f(static_cast<float>(Gauss::GRID_SIZE), static_cast<float>(Gauss::GRID_SIZE)));
	shape.setPosition(static_cast<float>(this->gridPos.x * Gauss::GRID_SIZE), static_cast<float>(this->gridPos.y * Gauss::GRID_SIZE));
	renderTarget.draw(shape);
}
