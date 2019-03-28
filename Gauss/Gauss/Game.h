#pragma once

#include "Player.h"
#include "MoveLeftEnemy.h"
#include "TrackerEnemy.h"
#include "MoveLeftShootEnemy.h"
#include "ItemPickup.h"
#include "ItemUpgrade.h"
#include "Boss.h"
#include "WorldMap.h"
#include "Particle.h"

class Game
{
private:
	// Game
	RenderWindow *window;
	View mainView;
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

	// MAP
	WorldMap worldMap;
	Stage stage;

	// Players
	dArr<Player> players;

	// Enemies
	dArr<EnemyLifeform*> enemyLifeforms;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	// Bosses
	bool bossEncounterActivated;
	dArr<Boss> bosses;

	// Consumables
	dArr<Consumable*> consumables;

	// Particles
	dArr<Particle> particles;

	// Player Textures
	dArr<Texture> playerBulletTextures;
	dArr<Texture> playerBodyTextures;
	dArr<Texture> playerMainGunTextures;
	dArr<Texture> lWingTextures;
	dArr<Texture> rWingTextures;
	dArr<Texture> cockpitTextures;
	dArr<Texture> auraTextures;

	// Enemy Textures
	dArr<Texture> enemyTextures;
	dArr<Texture> enemyBulletTextures;

	// Consumable Textures
	unsigned numberOfPickups;
	dArr<Texture> pickupTextures;
	unsigned numberOfUpgrades;
	dArr<Texture> upgradeTextures;

	// Boss textures
	dArr<Texture> bossBodyTextures;
	dArr<Texture> bossGunTextures;
	dArr<Texture> bossBulletTextures;


	// Utility Functions
	void _spawnEnemy();
	void _generateParticles(Vector2f pos, Vector2f velocity, bool dead);

public:
	Game(RenderWindow *window);
	virtual ~Game();

	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }
	inline const bool playersExistInWorld() const { return this->players.Size() > 0; }

	// Functions
	void InitView();
	void InitMapTextures();
	void InitTextures();
	void InitPlayerTextures();
	void InitUI();
	void InitMap();

	void UpdateView();
	void Update(const float &dt);
	void RestartUpdate();
	void UpdateTimers(const float &dt);
	void UpdateTimersUnpaused(const float &dt);
	void UpdateDifficulty();
	void UpdateMultipliers();
	void UpdateWhilePaused(const float &dt);
	void UpdatePlayers(const float &dt);
	void UpdateMap(const float &dt, int playerIndex);
	void UpdateWallColliders(const float &dt, int playerIndex);
	void UpdatePlayerBullets(const float &dt, Player &currentPlayer);
	void UpdateScoreUI();
	void UpdateEnemies(const float &dt);
	void UpdateEnemyBullets(const float &dt);
	void UpdateTextTags(const float &dt);
	void UpdateConsumables(const float &dt);
	void UpdateParticles(const float &dt);

	void DrawUI();
	void Draw();
	void DrawEnemy();
	void DrawPlayers();
	void DrawMap();
	void DrawConsumables();
	void DrawParticles();

	void ToggleFullscreen();
	void PauseGame();
	void DisplayGameEnd();

};

