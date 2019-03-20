#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
	// Game
	RenderWindow *window;
	bool paused;
	float keyTimeMax;
	float keyTime;
	bool fullscreen;

	// Score
	Clock scoreTimer;
	int scoreTime;
	unsigned totalScore;
		// Increasingly massive multiplier for killing enemies without ever taking damage
		// Effects the score AFTER the killbox multiplier has been applied
		// It resets on any damage taken
	unsigned killPerfectionMultiplier;
	int killPerfectionAdder;
	int killPerfectionAdderMax;
		// Slight multiplier added to each kill for continuously killing enemies 
		// Multiplier increases as long as you kill within the alotted time
			// Each multiplier increase you get longer to kill enemies, but you need more and they are more difficult to kill
		// Everytime the timer runs out the it resets
	unsigned killboxMultiplier;
	float killboxTimer;
	float killboxTimerMax;
	int killboxAdder;
	int killboxAdderMax;
	float difficultyTimer;
	int difficulty;
	double bestScorePerSecond;

	// UI
	Font font;
	Text staticPlayerText;
	Text enemyText;
	Text gameOverText;
	Text scoreText;
	Text controlsText;

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
	const Vector2f enemyScale = Vector2f(0.1f, 0.1f);
	const Vector2f enemyDirection = Vector2f(-1.0f, 0.f);
	const int enemyHp = rand() % 2 + 1;
	const Vector2i enemyDamageRange = Vector2i(1, 3);
	
	// Pickups
	dArr<Pickup> pickups;

	// Upgrades
	dArr<Upgrade> upgrades;

	// Textures
	std::vector<Texture> textureMap;
	dArr<Texture> playerMainGunTextures;
	dArr<Texture> enemyTextures;
	dArr<Texture> enemyBulletTextures;
	dArr<Texture> lWingTextures;
	dArr<Texture> rWingTextures;
	dArr<Texture> cockpitTextures;
	dArr<Texture> auraTextures;
	dArr<Texture> pickupTextures;
	dArr<Texture> upgradeTextures;

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
	void InitTextures();
	void Update(const float &dt);
	void DrawUI();
	void Draw();
	void ToggleFullscreen();

};

