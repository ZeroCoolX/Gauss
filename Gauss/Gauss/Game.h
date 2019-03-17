#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
	RenderWindow *window;

	// UI
	Font font;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;

	// Texttags
	dArr<TextTag> textTags;

	// Players
	dArr<Player> players;

	// Enemies
	dArr<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	// Enemy Spawn Data
	// TODO: Definetly make a struct of like EnemySpawnData or something in the future
	const Vector2f enemyScale = Vector2f(0.1f, 0.1f);
	const Vector2f enemyDirection = Vector2f(-1.0f, 0.f);
	const int enemyHp = rand() % 2 + 1;
	const Vector2i enemyDamageRange = Vector2i(1, 3);

	// Textures
	std::vector<Texture> textureMap;

	// Utility Functions
	void _spawnEnemy();

public:
	Game(RenderWindow *window);
	virtual ~Game();
	
	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }
	inline const bool playersExistInWorld() const { return this->players.Size() > 0; }

	// Setters

	// Functions
	void InitUI();
	void Update(const float &dt);
	void DrawUI();
	void Draw();
};

