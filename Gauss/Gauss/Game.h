#pragma once

#include "Player.h"
#include "MoveLeftEnemy.h"
#include "TrackerEnemy.h"
#include "MoveLeftShootEnemy.h"
#include "MoveLeftShootLineEnemy.h"
#include "CosmoEnemy.h"
#include "ItemPickup.h"
#include "ItemUpgrade.h"
#include "Boss.h"
#include "WorldMap.h"
#include "Particle.h"
#include "Powerup.h"
#include "MainMenu.h"
#include "GameOverMenu.h"
#include "PlayerScore.h"
#include "Tutorial.h"
#include "KeyBindingMenu.h"
#include "ShipBayMenu.h"
#include "CreditsMenu.h"

class Game
{
public:
	enum Mode {
		CAMPAIGN = 0,
		INFINTE,
		COSMOS,
		TUTORIAL
	};

	enum DataTextures {
		PLAYER_LIFE = 0,
		PAUSE_OVERLAY
	};

	enum LeaderboardIndex {
		INF = 0,
		COS
	};

private:
	const int LEADERBOARD_MAX = 8;
	struct Leaderboard {
	public:
		int id;
		unsigned long int score;
		inline bool operator== (const Leaderboard &i)
		{
			if (this == nullptr) return false;
			return id == i.id && score == i.score;
		}
		inline bool operator!= (nullptr_t n)
		{
			if (this != n) return true;
		}
	};
	dArr<dArr<Leaderboard>> leaderboards; // 0 = infinite, 1 = cosmos

	KeyManager keyManager;
	int keyScheme;

	// Only for test
	AudioManager* audioManager;
	int gameMusicSelection;

	// Game
	MainMenu *mainMenu;
	GameOverMenu *gameOverMenu;
	KeyBindingMenu *keybindMenu;
	ShipBayMenu *shipBayMenu;
	CreditsMenu *creditsMenu;

	Tutorial* tutorial;

	bool campaignOver;

	// Necessary to support multiple resolutions
	Vector2f center;
	View originalView;
	Vector2u screenResolutionSize;

	RenderWindow *window;
	View mainView;
	float keyTimeMax;
	float keyTime;
	bool fullscreen;
	int gameMode;

	// Pause 
	bool paused;
	Sprite pauseBackground;

	// Score
	Clock scoreTimer;
	int scoreTime;
	unsigned long int totalScore;
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
	Text cosmoEffectText;
	Text leaderboardText;
	Text scoreText;
	Text pauseKeyHelperText;
	Text controlsText;

	// Texttags
	dArr<TextTag> textTags;

	// MAP
	WorldMap worldMap;
	Stage *stage;
	// Optimization - this can be local variable in UpdateEnemySpawns
	int fromCol, toCol;
	int fromRow, toRow;

	// Players
	dArr<Player> players;
	std::map<int,PlayerScore> playerScoreMap;
	int playerShipType;
	int playerShipSkin;
	dArr<Sprite> playerLives;
	float playerRespawnTimer;
	float playerRespawnTimerMax;
	bool pausedForRespawn;


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

	// Game Textures
	dArr<Texture> gameDataTextures;

	// Utility Functions
	void _spawnEnemy(int enemyType, int velocity = -1, Vector2f position = Vector2f(0.f, 0.f));
	std::string _getPlayerLivesText();
	void _redeploy();
	unsigned long int _calculateScore(PlayerScore &playerScore);
	void _sortLeaderboard(LeaderboardIndex leadIndex);
	void _insertLeaderboardEntry(LeaderboardIndex leadIndex, int id, unsigned long int score);
	void _storeLeaderboard(LeaderboardIndex leadIndex, std::string filename);
	void _setPlayerLives();
	void _loadPlayerShipPreferences();
	void _savePlayerShipPreferences();
	void _refreshPauseControlText();

public:
	Game(RenderWindow *window, Vector2u screenResolutionSize);
	virtual ~Game();

	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }
	inline const bool playersExistInWorld() const { return this->players.Size() > 0; }

	// Helpers
	float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	Vector2f normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}

	// Functions
	void InitPlayersInWorld(int quantity);
	void InitView();
	void InitTextures();
	void InitUI();
	void InitLeaderboards();
	void InitMap();
	void InitMenus();
	void InitGameTextures();
	void InitPlayerLivesUI();

	void UpdateView(const float &dt);
	void Update(const float &dt, const Event *event);
	void UpdateTimers(const float &dt);
	void UpdateTimersUnpaused(const float &dt);
	void UpdateDifficulty();
	void UpdateMultipliers();
	void UpdatePlayers(const float &dt);
	void UpdatePlayerBullets(const float &dt, Player &currentPlayer);
	void UpdatePlayerTileCollision(const float &dt, Player &currentPlayer);
	void UpdateMap(const float &dt);
	void UpdateScoreUI();
	void UpdateEnemySpawns(const float &dt);
	void UpdateEnemies(const float &dt);
	void UpdateEnemyBullets(const float &dt);
	void UpdateTextTags(const float &dt);
	void UpdateConsumables(const float &dt);
	void UpdateParticles(const float &dt);
	void UpdateMainMenu(const float &dt);
	void UpdateShipBayMenu(const float &dt);
	void UpdateCreditsMenu(const float &dt);
	void UpdateKeybindingMenu(const float &dt, const Event *event);
	void UpdateGameOverMenu(const float &dt);
	void UpdateTutorial(const float &dt);

	void DrawUI();
	void DrawPlayerLivesUI();
	void DrawTextTags();
	void Draw();
	void DrawEnemy();
	void DrawPlayersAndMap();
	void DrawConsumables();
	void DrawParticles();

	void InitPlayerScoreStats();
	void ToggleFullscreen();
	void PauseGame();
	void PausedForRespawn(const float &dt);
	void DisplayGameEnd();
	void RedeployUI();
};

