#include "EnemySpawner.h"
#include "Gauss.h"


EnemySpawner::EnemySpawner(Vector2i gridPos,
	bool randomSpawnPos,
	int type,
	int levelInterval,
	int numOfEnemies)
{
	this->gridPos = gridPos;
	this->type = type;
	this->levelInterval = levelInterval;
	this->numOfEnemies = numOfEnemies;
	this->randomSpawnPos = randomSpawnPos;
}


EnemySpawner::~EnemySpawner()
{
}

void EnemySpawner::UpdateTimer() {

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
	text.setString("Rand Pos: " + std::to_string(this->randomSpawnPos) +
		"\nType: " + std::to_string(this->type) +
		"\nInterval: " + std::to_string(this->levelInterval) +
		"\nQuantity: " + std::to_string(this->numOfEnemies));

	renderTarget.draw(shape);
	renderTarget.draw(text);
}

void EnemySpawner::IsInScreen(View &view) {

}

void EnemySpawner::Spawn() {

}

std::string EnemySpawner::GetAsString() const {
	return std::to_string(this->gridPos.x) + " " +
		std::to_string(this->gridPos.y) + " " +
		std::to_string(this->randomSpawnPos) + " " +
		std::to_string(this->type) + " " +
		std::to_string(this->levelInterval) + " " +
		std::to_string(this->numOfEnemies);
}