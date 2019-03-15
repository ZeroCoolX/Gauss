#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
	RenderWindow *window;

	// Text
	Font font;
	std::vector<Text> followPlayerTexts;
	std::vector<Text> staticPlayerTexts;
	Text enemyText;

	// Players
	std::vector<Player> players;

	// Enemies
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	int enemySpawnTimer;
	int enemySpawnTimerMax;

	// Enemy Spawn Data
	// TODO: Definetly make a struct of like EnemySpawnData or something in the future
	const Vector2f enemyScale = Vector2f(0.1f, 0.1f);
	const Vector2f enemyDirection = Vector2f(-1.0f, 0.f);
	const int enemyHp = rand() % 3 + 1;
	const Vector2i enemyDamageRange = Vector2i(1, 3);

	// Textures
	std::vector<Texture> textureMap;

public:
	Game(RenderWindow *window);
	virtual ~Game();
	
	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	// Setters

	// Functions
	void InitUI();
	void UpdateUI();
	void Update(float dt);
	void DrawUI();
	void Draw();
};

