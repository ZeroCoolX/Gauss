#include "EnemySpawner.h"
#include "Gauss.h"


EnemySpawner::EnemySpawner(Vector2i gridPos,
	int type,
	int levelInterval,
	int numOfEnemies,
	float spawnTimerMax)
{
	this->gridPos = gridPos;
	this->type = type;
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

void EnemySpawner::Draw(RenderTarget &renderTarget, Font &font) {
	RectangleShape shape;
	shape.setFillColor(Color::White);
	shape.setSize(Vector2f(static_cast<float>(Gauss::GRID_SIZE), static_cast<float>(Gauss::GRID_SIZE)));
	shape.setPosition(static_cast<float>(this->gridPos.x * Gauss::GRID_SIZE), static_cast<float>(this->gridPos.y * Gauss::GRID_SIZE));

	Text text;
	text.setFont(font);
	text.setCharacterSize(12);
	text.setFillColor(Color::Red);
	text.setPosition(shape.getPosition());
	text.setString("Type: " + std::to_string(this->type) +
		"\nInterval: " + std::to_string(this->levelInterval) +
		"\nQuantity: " + std::to_string(this->numOfEnemies) +
		"\nTimer: " + std::to_string(this->spawnTimerMax));

	renderTarget.draw(shape);
	renderTarget.draw(text);
}
