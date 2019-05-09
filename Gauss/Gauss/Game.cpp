#include "Game.h"
#include <sstream>

Game::Game(RenderWindow *window) : leaderboards(2)
{
	this->window = window;
	// Init stage
	this->stage = nullptr;
	// Init Menus
	this->mainMenu = nullptr;
	this->gameOverMenu = nullptr;

	// Init tutorial
	this->tutorial = nullptr;

	this->audioManager = new AudioManager();
	//this->audioManager->PlayMusic(AudioManager::AudioMusic::MENU);

	this->gameMode = Mode::INFINTE;
	this->gameMusicSelection = AudioManager::AudioMusic::INF_COS_01;
	// Play the menu music immediately
	this->audioManager->PlayMusic(AudioManager::AudioMusic::MENU);
	this->campaignOver = false;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->InitView();
	this->InitTextures();
	this->InitMap();
	this->InitLeaderboards();

	// Init scoring multipliers
	this->killPerfectionMultiplier = 1;
	this->killPerfectionAdder = 0;
	this->killPerfectionAdderMax = 15;
	this->killboxMultiplier = 1;
	this->killboxTimerMax = 400.f;
	this->killboxTimer = this->killboxTimerMax;
	this->killboxAdder = 0;
	this->killboxAdderMax = 10;

	// Init player
	this->InitPlayersInWorld(1);

	this->InitMenus();

	// Init timers
	this->enemySpawnTimerMax = 35.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->fullscreen = false;
	this->totalScore = 0;
	this->scoreTimer.restart();
	this->scoreTime = 0;
	this->bestScorePerSecond = 0.0;
	this->difficulty = 0;
	this->difficultyTimer = 0;

	// Init Game controls
	this->paused = true;
	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	// Init boss encounter
	this->bossEncounterActivated = false;

	this->InitUI();
}

Game::~Game()
{
	delete this->stage;
	delete this->mainMenu;
	delete this->audioManager;
	delete this->gameOverMenu;
	delete this->tutorial;
	this->playerScoreMap.clear();
}

void Game::InitRenderTexture() {
	this->mainRenderTexture.create(
		this->window->getSize().x, 
		this->window->getSize().y
	);

	this->mainRenderSprite.setTexture(this->mainRenderTexture.getTexture());
}

void Game::InitView() {
	this->mainView.setSize(Vector2f(this->window->getSize()));
	this->mainView.setCenter(Vector2f(
		this->window->getSize().x / 2.f,
		this->window->getSize().y / 2.f));
}

//void Game::InitBossTextures() {
//	// Body
//	//Texture temp;
//	//temp.loadFromFile("Textures/Bosses/Body/boss01.png");
//	//this->bossBodyTextures.Add(Texture(temp));
//	//// Gun
//	//temp.loadFromFile("Textures/Bosses/Guns/bossGun01.png");
//	//this->bossGunTextures.Add(Texture(temp));
//	//temp.loadFromFile("Textures/Bosses/Guns/bossGun02.png");
//	//this->bossGunTextures.Add(Texture(temp));
//	//// Bullets
//	//temp.loadFromFile("Textures/Bosses/Bullets/bossBullet01.png");
//	//this->bossBodyTextures.Add(Texture(temp));
//}

void Game::InitPlayersInWorld(int quantity) {
	// Always add at least 1 player
	this->players.Add(Player(this->audioManager));

	// Optionally add another
	if (quantity > 1) {
		this->players.Add(Player(this->audioManager,
			Keyboard::I,
			Keyboard::K,
			Keyboard::J,
			Keyboard::L,
			Keyboard::RShift,
			Keyboard::U,
			Keyboard::RControl,
			Keyboard::Return,
			Keyboard::Num7,
			Keyboard::Num8,
			Keyboard::Num9,
			Keyboard::Num0));
	}

	this->InitPlayerScoreStats();
}

void Game::InitTextures() {
	// buttons
	MenuButton::InitTextures();

	// Game Over Menu
	GameOverMenu::InitTextures();

	// Player Textures
	Player::InitTextures();

	// Bullet Textures
	Bullet::InitTextures();

	// Load Enemy Textures
	EnemyLifeform::InitializeTextures();

	// Load Boss textures
	//this->InitBossTextures();

	// Init pickups textures
	ItemPickup::InitTextures();

	// Init Upgrade textures
	ItemUpgrade::InitTextures();

	// Init powerups
	Powerup::InitTextures();

	// Load Particle Textures
	Particle::InitTextures();

	// Load Tiles
	Tile::InitTextures();

	// Load Stage Textures
	Stage::InitTextures();
}

void Game::InitUI() {
	Text tempText;
	tempText.setFont(this->font);
	tempText.setCharacterSize(14);
	tempText.setFillColor(Color::White);

	for (size_t i = 0; i < this->players.Size(); i++)
	{
		// Follow Text init - belongs to the player
		this->players[i].InitUI(tempText);

		// Static Text init - is "about" the player but belongs to the game
		this->staticPlayerText.setFont(this->font);
		this->staticPlayerText.setCharacterSize(14);
		this->staticPlayerText.setFillColor(Color::White);
		this->staticPlayerText.setString("");
	}

	// Enemy Text
	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::White);

	// Cosmos Text
	this->cosmoEffectText.setFont(this->font);
	this->cosmoEffectText.setCharacterSize(40);
	this->cosmoEffectText.setFillColor(Color::Magenta);
	this->cosmoEffectText.setPosition(10.f, 10.f);

	// Game Over Text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(20);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setString("");
	this->gameOverText.setPosition(50.f, (float)this->window->getSize().y / 4);

	// Leaderboard Text
	this->leaderboardText.setFont(this->font);
	this->leaderboardText.setCharacterSize(30);
	this->leaderboardText.setFillColor(Color::White);
	this->leaderboardText.setString("");
	this->leaderboardText.setPosition((float)this->window->getSize().x - 600.f, 300.f);

	// Controls / Pause text
	this->controlsText.setFont(this->font);
	this->controlsText.setCharacterSize(30);
	this->controlsText.setFillColor(Color::White);
	this->controlsText.setPosition(20.f, ((float)this->window->getSize().y / 3) + 50.f);
	this->controlsText.setString("W: UP\nA: LEFT\nS: DOWN\nD: RIGHT\nSPACE: SHOOT\nP: PAUSE/CONTROLS (START GAME)\nESC: Quit\n1, 2, 3 & 4: CUSTOMIZE SHIP (ONLY WHILE PAUSED!)\nF11: FULLSCREEN\n\n\nTOP-LEFT SHIP: Player Number\nTOP-RIGHT SHIP: Hp/HpMax\nBOTTOM-LEFT SHIP: Level\nBOTTOM-RIGHT SHIP: Exp-bar");

	// Score text
	this->scoreText.setFont(this->font);
	this->scoreText.setCharacterSize(32);
	this->scoreText.setFillColor(Color(200, 200, 200, 150));
	this->scoreText.setString("Score: 0");
	this->scoreText.setPosition(10.f, 10.f);
}

void Game::InitLeaderboards() {

	// Add infinite leaderboard
	this->leaderboards.Add(dArr<Leaderboard>(this->LEADERBOARD_MAX));
	// Add cosmos leaderboard
	this->leaderboards.Add(dArr<Leaderboard>(this->LEADERBOARD_MAX));

	// LOAD INFINITE LEADERBOARDS
	std::ifstream fin;
	std::string line;
	fin.open("Leaderboards/Infinite/leaderboard.txt");
	if (fin.is_open()) {
		// Lead line by line
		Leaderboard leader;
		while (getline(fin, line)) {
			std::stringstream ss;
			ss.str(line);
			ss >> leader.id;
			ss >> leader.score;
			this->leaderboards[LeaderboardIndex::INF].Add(leader);
		}
		this->_sortLeaderboard(LeaderboardIndex::INF);
	}
	else {
		std::cerr << "Failure to open Inifinite leaderboards file" << std::endl;
	}
	fin.close();

	// LOAD COSMOS LEADERBOARDS
	fin.open("Leaderboards/Cosmos/leaderboard.txt");
	if (fin.is_open()) {
		// Lead line by line
		Leaderboard leader;
		while (getline(fin, line)) {
			std::stringstream ss;
			ss.str(line);
			ss >> leader.id;
			ss >> leader.score;
			this->leaderboards[LeaderboardIndex::COS].Add(leader);
		}
		this->_sortLeaderboard(LeaderboardIndex::COS);
	}
	else {
		std::cerr << "Failure to open Inifinite leaderboards file" << std::endl;
	}
	fin.close();
}

void Game::InitMap() {
	this->stage = new Stage(10, 10);
	std::string mapName = "lel.smap";
	switch (this->gameMode) {
	case Game::Mode::INFINTE:
		mapName = "infinite_invasion_01.smap";
		break;
	case Game::Mode::COSMOS:
		mapName = "cosmos_01.smap";
		break;
	case Game::Mode::CAMPAIGN:
		mapName = "campaign_01.smap";
		break;
	case Game::Mode::TUTORIAL:
		mapName = "tutorial_01.smap";
		break;
	}
	this->stage->LoadStage(mapName, this->mainView);
}

void Game::InitMenus() {
	this->mainMenu = new MainMenu(this->font, this->window);
	this->gameOverMenu = new GameOverMenu(this->font, this->window);
	this->tutorial = new Tutorial(this->font, this->window, &this->players);
	Tutorial::InitTexts();
}

void Game::UpdateView(const float &dt) {
	this->mainView.move(this->stage->getScrollSpeed() * dt * DeltaTime::dtMultiplier, 0.f);
}

void Game::Update(const float &dt) {
	// Check for main menu actions
	if (this->mainMenu->isActive()) {
		this->UpdateMainMenu(dt);
		return;
	}

	if (this->gameMode == Mode::TUTORIAL) {
		this->UpdateTutorial(dt);
	}

	// Keytime update
	this->UpdateTimers(dt);

	// Fullscreen check
	this->ToggleFullscreen();

	// Pause check
	this->PauseGame();

	// Only allow changing accessories while paused
	this->UpdateWhilePaused(dt);


	// Update the world
	if (!this->paused && this->playersExistInWorld()) {
		// Update timers
		this->UpdateTimersUnpaused(dt);

		// view update
		this->UpdateView(dt);

		//MAKE GAME HARDER WITH TIME
		this->UpdateDifficulty();

		// Update score multipliers
		this->UpdateMultipliers();

		// Update Map
		this->UpdateMap(dt);

		// UPDATE PLAYERS
		this->UpdatePlayers(dt);

		// UPDATE ENEMIES
		this->UpdateEnemies(dt);

		// UPDATE CONSUMABLES
		this->UpdateConsumables(dt);

		// Update Texttags
		this->UpdateTextTags(dt);

		// Update Particles 
		this->UpdateParticles(dt);
	}
	else if (!this->playersExistInWorld() && this->scoreTime == 0) {
		if (this->gameMode != Mode::CAMPAIGN) {
			// Show end game stats
			this->DisplayGameEnd();
		}

		// Activate the game over screen if it's not already turned on
		if (!this->gameOverMenu->isActive()) {
			this->audioManager->PlaySound(AudioManager::AudioSounds::GAME_OVER);
			// This is just in case you die with the grinder on...otherwise it plays into the menu
			this->audioManager->StopSound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);
			this->audioManager->StopMusic(this->gameMusicSelection);
			this->gameOverMenu->activate();
		}
	}

	// Restart
	if (!this->playersExistInWorld()) {
		this->UpdateGameOverMenu(dt);
	}
}

void Game::UpdateMainMenu(const float &dt) {
	this->mainMenu->Update(dt);

	if (this->mainMenu->onCampaignPress()) {
		this->audioManager->StopMusic(AudioManager::AudioMusic::MENU);
		this->gameMusicSelection = AudioManager::AudioMusic::CAMPAIGN;
		this->audioManager->PlayMusic(this->gameMusicSelection);
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->gameMode = Mode::CAMPAIGN;
		this->mainMenu->Reset();
		this->InitMap();
		this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::CAMPAIGN);
		this->_setPlayerLives(); // lives = 3
		this->paused = false;
	}
	else if (this->mainMenu->onInfinitePress()) {
		// Start each mode with the same music everytime, but swap on redeploys
		this->gameMusicSelection = AudioManager::AudioMusic::INF_COS_01;
		this->audioManager->StopMusic(AudioManager::AudioMusic::MENU);
		this->audioManager->PlayMusic(this->gameMusicSelection);
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->gameMode = Mode::INFINTE;
		this->mainMenu->Reset();
		this->InitMap();
		this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::INFINTE);
		this->_setPlayerLives(); // lives = 1
	}
	else if (this->mainMenu->onCosmosPress()) {
		// Start each mode with the same music everytime, but swap on redeploys
		this->gameMusicSelection = AudioManager::AudioMusic::INF_COS_02;
		this->audioManager->StopMusic(AudioManager::AudioMusic::MENU);
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->audioManager->PlayMusic(this->gameMusicSelection);
		this->gameMode = Mode::COSMOS;
		this->mainMenu->Reset();
		this->InitMap();
		this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::COSMOS);
		this->_setPlayerLives(); // lives = 1
	}
	else if (this->mainMenu->onTutorialPress()) {
		this->audioManager->StopMusic(AudioManager::AudioMusic::MENU);
		this->gameMusicSelection = AudioManager::AudioMusic::INF_COS_03;
		this->audioManager->PlayMusic(this->gameMusicSelection);
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->gameMode = Mode::TUTORIAL;
		this->mainMenu->Reset();
		this->InitMap();
		this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::CAMPAIGN);// Needs to be TUTORIAL
		this->_setPlayerLives(); // lives = 3 // Irrelevent for tutorial
		this->paused = false;
		this->tutorial->Activate();
	}
}

void Game::UpdateGameOverMenu(const float &dt) {
	this->gameOverMenu->Update(dt);

	if (this->gameOverMenu->onRedeployPress()) {
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->gameOverMenu->Reset();
		this->_redeploy();
		if (this->gameMode != Mode::CAMPAIGN) {
			this->gameMusicSelection = rand() % 3 + 2;
		}
		this->audioManager->PlayMusic(this->gameMusicSelection);
	}
	else if (this->gameOverMenu->onMenuPress()) {
		this->audioManager->PlaySound(AudioManager::AudioSounds::BUTTON_CLICK);
		this->gameOverMenu->Reset();
		this->_redeploy();
		this->audioManager->PlayMusic(AudioManager::AudioMusic::MENU);
		this->mainView.setCenter(Vector2f(
			this->window->getSize().x / 2.f,
			this->window->getSize().y / 2.f));
		this->mainMenu->Reset();
		this->mainMenu->activate();
	}
}

void Game::UpdateTutorial(const float &dt) {
	this->tutorial->Update(dt);

	if (this->tutorial->IsTutorialComplete()) {
		this->tutorial->Reset();
		this->_redeploy();
		this->audioManager->StopMusic(this->gameMusicSelection);
		this->audioManager->PlayMusic(AudioManager::AudioMusic::MENU);
		this->mainView.setCenter(Vector2f(
			this->window->getSize().x / 2.f,
			this->window->getSize().y / 2.f));
		this->mainMenu->Reset();
		this->mainMenu->activate();
	}
}


void Game::UpdateTimers(const float &dt) {
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void Game::UpdateTimersUnpaused(const float &dt) {
	// Update timers
	if (this->enemySpawnTimer < this->enemySpawnTimerMax) { this->enemySpawnTimer += 1.f * dt * DeltaTime::dtMultiplier; } // 1.f is not needed here

	this->difficultyTimer += 1.f * dt * DeltaTime::dtMultiplier;

	// TODO: deleted code that is now in UpdateDifficulty

	// Update Killbox timer
	if (this->killboxTimer > 0.f) {
		this->killboxTimer -= 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		this->killboxTimer = 0.f;
		this->killboxAdder = 0;
		this->killboxMultiplier = 1;
	}
}

void Game::UpdateDifficulty() {
	// Make the game harder with time by decreasing the wait between enemy spawns over time
	if ((int)this->difficultyTimer % 1000 == 0 && this->enemySpawnTimerMax > 10) {
		this->enemySpawnTimerMax--;
		this->difficulty++;
		this->difficultyTimer = 1.f;
	}
}

void Game::UpdateMultipliers() {
	// Increase the killbox multiplier by 1 everytime the max is reached and raise the max by half the current. Reset the adder
	if (this->killboxAdder >= this->killboxAdderMax) {
		this->killboxMultiplier++;
		this->killboxAdder = 0;
		this->killboxAdderMax = (int)std::floor(this->killboxAdderMax * 1.5);
		this->killboxTimer = this->killboxTimerMax;
	}
	// Increase the perfection multiplier by factor of 2 everytime the max is reached and reset the adder
	if (this->killPerfectionAdder >= this->killPerfectionAdderMax) {
		this->killPerfectionMultiplier *= 2;
		this->killPerfectionAdder = 0;
		this->killPerfectionAdderMax = (int)std::floor(this->killPerfectionAdderMax * 1.25);
	}
}

void Game::UpdateWhilePaused(const float &dt) {
	if (this->paused) {
		for (size_t i = 0; i < this->players.Size(); i++)
		{
			if (!this->players[i].isDead() && this->keyTime >= this->keyTimeMax) {
				if (this->players[i].ChangeAccessories(dt)) {
					this->keyTime = 0.f;
				}
			}
		}
	}
}

void Game::UpdatePlayers(const float &dt) {
	if (this->playersExistInWorld()) {
		// Reset the total each calculation
		this->totalScore = 0;

		bool playersEffectedByCosmos = true;
		for (size_t i = 0; i < this->players.Size(); ++i) {

			// Players update
			this->players[i].Update(this->mainView, dt, this->stage->getScrollSpeed());

			// Check for game over state
			if (this->gameMode == Mode::CAMPAIGN && this->players[i].campaignLevelBeat()) {
				this->campaignOver = true;
				this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::CAMPAIGN_BEAT);
				this->players.Remove(i);
				return;
			}

			playersEffectedByCosmos = this->players[i].isEffectedByCosmo();

			this->UpdatePlayerTileCollision(dt, this->players[i]);

			// Bullets update
			this->UpdatePlayerBullets(dt, this->players[i]);

			this->totalScore += this->players[i].getScore();

			if (this->players[i].shouldLoseLife()) {
				this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_DEATH);
				if (this->players[i].loseLife()) {
					this->players[i].setTimeAlive(this->scoreTimer.getElapsedTime().asSeconds());
					this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setScore(this->players[i].getScore());
					this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setEnemiesKilled(this->players[i].getEnemiesKilled());
					this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setSecondsSurvive(this->players[i].getTimeAlive());
					this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setHighestLevelAchieved(this->players[i].getHighestLevelAchieved());
					this->players.Remove(i);
					return;
				}
				this->players[i].ResetOnLifeLost(this->mainView);

			}else if (this->players[i].isDead()) {
				this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_DEATH);
				this->players[i].setTimeAlive(this->scoreTimer.getElapsedTime().asSeconds());
				this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setScore(this->players[i].getScore());
				this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setEnemiesKilled(this->players[i].getEnemiesKilled());
				this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setSecondsSurvive(this->players[i].getTimeAlive());
				this->playerScoreMap.find(this->players[i].getPlayerNumber())->second.setHighestLevelAchieved(this->players[i].getHighestLevelAchieved());
				this->players.Remove(i);
				return;
			}
		}
		if (!playersEffectedByCosmos) {
			this->cosmoEffectText.setString("");
		}

		this->UpdateScoreUI();

	}
}

void Game::UpdatePlayerBullets(const float &dt, Player &currentPlayer) {
	for (size_t j = 0; j < currentPlayer.getBulletsSize(); j++)
	{
		currentPlayer.BulletAt(j).Update(dt);

		// Bullet Window bounds check
		if (currentPlayer.BulletAt(j).getPosition().x > this->mainView.getCenter().x + (this->mainView.getSize().x / 2)) {
			currentPlayer.RemoveBullet(j);
		}
		else {
			// Enemy - Bullet Collision check since it still exists in the world
			for (size_t k = 0; k < this->enemyLifeforms.Size(); k++)
			{
				EnemyLifeform *currentEnemy = this->enemyLifeforms[k];
				if (currentPlayer.BulletAt(j).getGlobalBounds().intersects(currentEnemy->getGlobalBounds())) {

					// Health check for damage or destruction
					int damage = currentPlayer.BulletAt(j).getDamage();

					this->textTags.Add(
						TextTag(
							&this->font,
							Vector2f(currentEnemy->getPosition().x + currentEnemy->getGlobalBounds().width / 4,
								currentEnemy->getPosition().y - currentEnemy->getGlobalBounds().height / 2),
							"-" + std::to_string(damage), Color::Red,
							Vector2f(1.f, 0.f),
							24, 18.f, true
						)
					);

					currentEnemy->TakeDamage(damage);

					// Generate particles based on how alive enemy is
					if (currentEnemy->getHp() <= 0) {
						const int nrOfPatricles = rand() % 30 + 10;
						for (int m = 0; m < nrOfPatricles; m++)
						{
							this->particles.Add(Particle(currentEnemy->getPosition(),
								0,
								currentPlayer.BulletAt(j).getVelocity(),
								rand() % 40 + 10.f,
								rand() % 30 + 1.f,
								50.f));
						}
					}
					else {
						const int nrOfPatricles = rand() % 10 + 3;
						for (int m = 0; m < nrOfPatricles; m++)
						{
							this->particles.Add(Particle(currentEnemy->getPosition(),
								0,
								currentPlayer.BulletAt(j).getVelocity(),
								rand() % 30 + 10.f,
								rand() % 20 + 1.f,
								40.f));
						}
					}

					if (currentEnemy->getHp() <= 0) {

						// Gain score & Reset multiplier timer
						this->killboxTimer = this->killboxTimerMax;
						this->killboxAdder++;
						this->killPerfectionAdder++;

						// Total Score = (EnemyMaxHp + (EnemyMaxHp * KillboxMultiplier)) * PerfectionMultiplier 
						int score = (currentEnemy->getHpMax() + (currentEnemy->getHpMax() * this->killboxMultiplier)) * this->killPerfectionMultiplier;
						currentPlayer.gainScore(score);

						// Score text tag
						this->textTags.Add(
							TextTag(
								&this->font, Vector2f(100.f, 10.f), "+" + std::to_string(score), Color::White,
								Vector2f(1.f, 0.f),
								30, 40.f, true
							)
						);

						// Player earned some EXP!
						int exp = currentEnemy->getHpMax()
							+ (rand() % currentEnemy->getHpMax() + 1) * (this->killboxMultiplier + 1);
						if (currentPlayer.getPowerupXP()) {
							exp *= 2;
						}

						// Player leveled up!
						if (currentPlayer.gainExp(exp)) {
							this->textTags.Add(
								TextTag(
									&this->font, Vector2f(currentPlayer.getPosition().x + currentPlayer.getGlobalBounds().width / 4,
										currentPlayer.getPosition().y + currentPlayer.getGlobalBounds().height), "LEVEL UP!",
									Color::Cyan,
									Vector2f(0.f, 1.f),
									42, 40.f, true
								)
							);
						}
						else {
							this->textTags.Add(
								TextTag(
									&this->font, Vector2f(currentPlayer.getPosition().x + currentPlayer.getGlobalBounds().width / 4,
										currentPlayer.getPosition().y + currentPlayer.getGlobalBounds().height),
									"+" + std::to_string(exp) + " EXP", Color::Cyan,
									Vector2f(0.f, 1.f),
									24, 40.f, true
								)
							);

						}

						// Change to drop consumable - perhaps revisit this
						int dropChance = rand() % 100 + 1;
						int uType = 0;
						int consumableType = rand() % 9 + 1;
						switch (consumableType) {
						case 1: case 2: case 3:
						{
							if (dropChance > 85) { // 10% chance for an upgrade

								// Only drop an upgrade we don't have - otherwise randomly choose stat point upgrade, or health
								uType = rand() % ItemUpgrade::numberOfUpgrades;
								for (size_t u = 0; u < currentPlayer.getAcquiredUpgrades().Size(); u++)
								{
									if (uType == currentPlayer.getAcquiredUpgrades()[u]) {
										uType = rand() % 3;
										break;
									}
								}

								// Want to make it really hard to get double and triple ray upgrades
								if (uType == ItemUpgrade::Type::DOUBLE_RAY) {
									if (currentPlayer.getLevel() >= 6 && currentPlayer.getGunLevel() == Player::LaserLevels::DEFAULT_LASER) {
										dropChance = rand() % 100 + 1;
										if (dropChance < 30) {
											// Spawn random powerup instead
											uType = rand() % Powerup::numberOfPowerups;
											this->consumables.Add(new Powerup(
												currentEnemy->getPosition(),
												uType,
												300.f));
											break;
										}
									}
									else {
										uType = rand() % 3;
									}
								}
								if (uType == ItemUpgrade::Type::TRIPLE_RAY) {
									if (currentPlayer.getLevel() >= 10 && currentPlayer.getGunLevel() == Player::LaserLevels::LEVEL_2_LASER) {
										dropChance = rand() % 100 + 1;
										if (dropChance < 30) {
											// Spawn random powerup instead
											uType = rand() % Powerup::numberOfPowerups;
											this->consumables.Add(new Powerup(
												currentEnemy->getPosition(),
												uType,
												300.f));
											break;
										}
									}
									else {
										uType = rand() % 3;
									}
								}

								this->consumables.Add(new ItemUpgrade(
									currentEnemy->getPosition(),
									uType,
									300.f));
							}
							break;
						}
						case 4: case 5:
						{
							if (dropChance > 50) { // 50% chance health is dropped
								this->consumables.Add(new ItemPickup(
									currentEnemy->getPosition(),
									ItemPickup::HEALTH, // health item for now
									150.f));
							}
							break;
						}
						case 6: case 7: case 8: case 9:
						{
							uType = rand() % Powerup::numberOfPowerups;
							if (dropChance > 78) { // 20% chance powerup is dropped
								this->consumables.Add(new Powerup(
									currentEnemy->getPosition(),
									uType,
									300.f));
							}
							break;
						}
						}

						// Destroy the enemy
						this->enemyLifeforms.Remove(k);
						this->audioManager->PlaySound(AudioManager::AudioSounds::BADDIE_DEATH_01);
						currentPlayer.incrementEnemiesKilled();
					}

					// Destroy the bullet if not piercing shot && not gauss shot
					if (!currentPlayer.getPowerupPiercingShot() && !currentPlayer.BulletAt(j).isGaussShot()) {
						// Should add effect to indicate it is piercing shots
						currentPlayer.RemoveBullet(j);
					}
					else if (currentPlayer.getPowerupPiercingShot()) {
						// Move to the end of the sprite it hit so that there is only a single point of damage calculation
						currentPlayer.BulletAt(j).setPosition(Vector2f(currentEnemy->getPosition().x + currentEnemy->getGlobalBounds().width + 1.f, currentPlayer.BulletAt(j).getPosition().y));
						std::cout << "Current enemy position far right x = " << std::to_string(currentEnemy->getPosition().x + currentEnemy->getGlobalBounds().width) << std::endl;
						std::cout << "Bullet position.x = " << std::to_string(currentPlayer.BulletAt(j).getPosition().x) << std::endl;
					}
					break;
				}
			}
		}
	}
}

void Game::UpdatePlayerTileCollision(const float &dt, Player &currentPlayer) {
	if (currentPlayer.isDead()) {
		return;
	}
	this->fromCol = static_cast<int>((currentPlayer.getPosition().x - Gauss::GRID_SIZE * 2) / Gauss::GRID_SIZE);
	if (this->fromCol <= 0) {
		this->fromCol = 0;
	}
	if (this->fromCol >= this->stage->getSizeX()) {
		this->fromCol = this->stage->getSizeX();
	}

	this->toCol = static_cast<int>((currentPlayer.getPosition().x + Gauss::GRID_SIZE * 2) / Gauss::GRID_SIZE + 1);
	if (this->toCol <= 0) {
		this->toCol = 0;
	}
	if (this->toCol >= this->stage->getSizeX()) {
		this->toCol = this->stage->getSizeX();
	}


	this->fromRow = static_cast<int>((currentPlayer.getPosition().y - Gauss::GRID_SIZE * 2) / Gauss::GRID_SIZE);
	if (this->fromRow <= 0) {
		this->fromRow = 0;
	}
	if (this->fromRow >= this->stage->getSizeY()) {
		this->fromRow = this->stage->getSizeY();
	}

	this->toRow = static_cast<int>((currentPlayer.getPosition().y + Gauss::GRID_SIZE * 2) / Gauss::GRID_SIZE + 1);
	if (this->toRow <= 0) {
		this->toRow = 0;
	}
	if (this->toRow >= this->stage->getSizeY()) {
		this->toRow = this->stage->getSizeY();
	}

	for (int i = fromCol; i < toCol; i++)
	{
		for (int j = fromRow; j < toRow; j++)
		{
			// Collision!
			if (!this->stage->getTiles()[i].IsNull(j)
				&& this->stage->getTiles()[i][j].isColliderType()
				&& currentPlayer.collidesWith(this->stage->getTiles()[i][j].getBounds())) {
				if (this->stage->getTiles()[i][j].isDamageType()) {
					// Once touch and only death awaits
					currentPlayer.TakeDamage(999);
					// Explode into particles
					const int nrOfPatricles = rand() % 20 + 5;
					for (int m = 0; m < nrOfPatricles; m++)
					{
						this->particles.Add(Particle(currentPlayer.getPosition(),
							0,
							Vector2f(-currentPlayer.getNormDir().x, -currentPlayer.getNormDir().y),
							rand() % 40 + 10.f,
							rand() % 30 + 1.f,
							50.f));
					}
					return;
				}
				// Not great but not all together aweful
				currentPlayer.move(0.f, -currentPlayer.getNormDir().y * 50 * dt * DeltaTime::dtMultiplier);
				currentPlayer.resetVelocityY();
				return;
			}
		}
	}
}

void Game::UpdateMap(const float &dt) {
	this->stage->Update(dt, this->mainView, false);
}

void Game::UpdateScoreUI() {
	this->scoreText.setString(
		this->_getPlayerLivesText() +
		+ "\nScore: " + std::to_string(this->totalScore)
		+ "\nPerfection Score Mult: x" + std::to_string(killPerfectionMultiplier)
		+ "\nPerfect Kills / Next: " + std::to_string(this->killPerfectionAdder) + " / " + std::to_string(this->killPerfectionAdderMax)
		+ "\nKill Mult: x" + std::to_string(killboxMultiplier)
		+ "\nKillbox Kills / Next: " + std::to_string(this->killboxAdder) + " / " + std::to_string(this->killboxAdderMax)
		+ "\nKillbox Seconds Remaining: " + std::to_string((int)this->killboxTimer) + "s"
		+ "\nGame time: " + std::to_string((int)this->scoreTimer.getElapsedTime().asSeconds())
		+ "\nDifficulty: " + std::to_string(this->difficulty)
		+ "\nBest Score/Second: " + std::to_string(this->bestScorePerSecond));
}

void Game::UpdateEnemySpawns(const float &dt) {
	// Safety check in case there are no more players in the world
	if (!this->playersExistInWorld()) {
		return;
	}

	if (this->gameMode == Mode::INFINTE) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->_spawnEnemy(rand() % EnemyLifeform::nrOfEnemyTypes);
			this->enemySpawnTimer = 0;
		}
	}else if (this->gameMode == Mode::COSMOS) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			const int cosmoChance = rand() % 100;
			int numOfEnemies = EnemyLifeform::nrOfEnemyTypes;
			if (cosmoChance > 90) {
				numOfEnemies += 4;
				// if even 1 player is currently effected by a cosmo don't spawn anymore!
				for (size_t i = 0; i < this->players.Size(); i++)
				{
					if (this->players[i].isEffectedByCosmo()) {
						numOfEnemies -= 4;
						break;
					}
				}
				int cosmosToSpawn = 1;
				// There is a chance we can spawn 1-3 cosmos
				int randType = rand() % numOfEnemies;
				if (numOfEnemies > EnemyLifeform::nrOfEnemyTypes && randType >= 4) {
					cosmosToSpawn = rand() % 3 + 1;
				}
				// Either spawn 1 normal, or multiple cosmos
				for (int i = 0; i < cosmosToSpawn; i++)
				{
					this->_spawnEnemy(rand() % numOfEnemies);
				}
			}
			else {
				// No chance for cosmo spawns so spawn 1 normal enemy
				this->_spawnEnemy(rand() % numOfEnemies);
			}
			this->enemySpawnTimer = 0;
		}
	}
	else if(this->gameMode == Mode::CAMPAIGN){
		// Get those spawners that are in the screen view only
		// Functionize
		this->fromCol = static_cast<int>((this->mainView.getCenter().x - this->mainView.getSize().x / 2) / Gauss::GRID_SIZE);
		if (this->fromCol <= 0) {
			this->fromCol = 0;
		}
		if (this->fromCol >= this->stage->getSizeX()) {
			this->fromCol = this->stage->getSizeX();
		}

		this->toCol = static_cast<int>((this->mainView.getCenter().x + this->mainView.getSize().x / 2) / Gauss::GRID_SIZE + 1);
		if (this->toCol <= 0) {
			this->toCol = 0;
		}
		if (this->toCol >= this->stage->getSizeX()) {
			this->toCol = this->stage->getSizeX();
		}


		this->fromRow = static_cast<int>((this->mainView.getCenter().y - this->mainView.getSize().y / 2) / Gauss::GRID_SIZE);
		if (this->fromRow <= 0) {
			this->fromRow = 0;
		}
		if (this->fromRow >= this->stage->getSizeY()) {
			this->fromRow = this->stage->getSizeY();
		}

		this->toRow = static_cast<int>((this->mainView.getCenter().y + this->mainView.getSize().y / 2) / Gauss::GRID_SIZE + 1);
		if (this->toRow <= 0) {
			this->toRow = 0;
		}
		if (this->toRow >= this->stage->getSizeY()) {
			this->toRow = this->stage->getSizeY();
		}

		const float viewRightSide = this->mainView.getCenter().x + this->mainView.getSize().x / 2;
		for (int i = this->fromCol; i < this->toCol; i++)
		{
			for (int j = this->fromRow; j < this->toRow; j++)
			{
				// Piggyback check for end game tile
				if (!this->stage->getTiles()[i].IsNull(j) && this->stage->getTiles()[i][j].isGameEndType()) {
					for (size_t i = 0; i < this->players.Size(); i++)
					{
						if (!this->players[i].isGameOverActivated()) {
							this->players[i].ActivateGameEnd(this->mainView.getCenter());
						}
					}
				}

				if (this->stage->getEnemySpawners()[i].IsNull(j) || this->stage->getEnemySpawners()[i][j].isUsed()) {
					continue;
				}
				if (this->stage->getEnemySpawners()[i][j].getPosition().x < viewRightSide) {
					this->stage->getEnemySpawners()[i][j].setUsed();
					int eType = 0;
					int nrOfE = 1;
					if (this->stage->getEnemySpawners()[i][j].getType() < 0) {
						// randomize
						eType = rand() % EnemyLifeform::nrOfEnemyTypes;
					}
					else {
						eType = this->stage->getEnemySpawners()[i][j].getType();
					}
					if (this->stage->getEnemySpawners()[i][j].getNumOfEnemies() < 0) {
						// randomize
						nrOfE = rand() % 10 + 1;
					}
					while (nrOfE > 0) {
						--nrOfE;
						if (this->stage->getEnemySpawners()[i][j].isRandomSpawnPos()) {
							// random spawn point
							this->_spawnEnemy(eType, this->stage->getEnemySpawners()[i][j].getForcedVelocity());
						}
						else {
							this->_spawnEnemy(eType, this->stage->getEnemySpawners()[i][j].getForcedVelocity(), this->stage->getEnemySpawners()[i][j].getPosition());
						}
					}
				}
			}
		}
	}
}

void Game::UpdateEnemies(const float &dt) {
	// Spawn enemies
	this->UpdateEnemySpawns(dt);
		
	for (size_t i = 0; i < this->enemyLifeforms.Size(); i++)
	{
		EnemyLifeform *currentEnemy = this->enemyLifeforms[i];

		// Safety check in case there are no more players in the world
		if (!this->playersExistInWorld()) {
			return;
		}
		else {
			// Check if we need to update the player this enemy is following incase they died
			if (currentEnemy->getPlayerFollowNum() > (int)this->players.Size() - 1) {
				currentEnemy->updatePlayerFollowNum(rand() % this->players.Size());
			}
			currentEnemy->Update(dt, this->players[currentEnemy->getPlayerFollowNum()].getPosition());
		}

		// Enemy Window Bounds check
		if (currentEnemy->getPosition().x < (this->mainView.getCenter().x - (this->mainView.getSize().x / 2)) - currentEnemy->getGlobalBounds().width) {
			this->enemyLifeforms.Remove(i);
		}
		else {
			// Check Player - Enemy collision
			for (size_t j = 0; j < this->players.Size(); j++)
			{
				// Check if this is a Cosmosos - collide regardless of damage cooldown because Cosmos don't cause damage - they cause CHAOS ^_^
				if (this->players[j].getGlobalBounds().intersects(currentEnemy->getGlobalBounds()) 
					&& currentEnemy->isUniverseModifier()) 
				{
					// do stuff
					const int nrOfPatricles = rand() % 20 + 5;
					for (int m = 0; m < nrOfPatricles; m++)
					{
						this->particles.Add(Particle(currentEnemy->getPosition(),
							0,
							currentEnemy->getVelocity(),
							rand() % 40 + 10.f,
							rand() % 30 + 1.f,
							50.f));
					}
					this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_ENEMY_IMPACT);
					this->enemyLifeforms.Remove(i);
					// Player is going to not be a happy camper for a bit
					std::string effectText = this->players[j].ApplyCosmoEffect();
					if (effectText != "") {
						this->cosmoEffectText.setString("Cosmo Effect: " + effectText);

						this->textTags.Add(
							TextTag(
								&this->font, Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
									this->players[j].getPosition().y - this->players[j].getGlobalBounds().height / 2),
								effectText,
								Color::Magenta,
								Vector2f(-1.f, 0.f),
								50, 120.f, true
							)
						);
						break;
					}
				}

				if (this->players[j].getGlobalBounds().intersects(currentEnemy->getGlobalBounds())
					&& !this->players[j].isDamageCooldown()) {

					if (this->players[j].isShielding() || this->players[j].getPowerupGrind()) {
						Color particleColor = Color::Magenta;

						// Damage the sheild from impact
						if (this->players[j].isShielding()) {
							this->players[j].impactSheild();
						}

						// Grind powerup is active
						if (this->players[j].getPowerupGrind()) {
							this->audioManager->PlaySound(AudioManager::AudioSounds::POWERUP_GRIND);
							particleColor = Color::Green;
						}

						const int nrOfPatricles = rand() % 20 + 5;
						for (int m = 0; m < nrOfPatricles; m++)
						{
							this->particles.Add(Particle(currentEnemy->getPosition(),
								0,
								currentEnemy->getVelocity(),
								rand() % 40 + 10.f,
								rand() % 30 + 1.f,
								50.f, particleColor));
						}
						this->players[j].incrementEnemiesKilled();
						this->enemyLifeforms.Remove(i);
						break;
					}

					// Damage player		
					int damage = currentEnemy->getDamage();
					// Half damage player takes for Cosmos mode since its way harder
					if (this->gameMode == Game::Mode::COSMOS) {
						damage /= 2;
					}
					this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_ENEMY_IMPACT);
					this->players[j].TakeDamage(damage);
					// Collision resets the perfection streak
					this->killPerfectionAdder = 0;
					this->killPerfectionMultiplier = 1;

					currentEnemy->Collision();

					// Player collision damage
					this->textTags.Add(
						TextTag(
							&this->font, Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
								this->players[j].getPosition().y - this->players[j].getGlobalBounds().height / 2),
							"-" + std::to_string(damage), Color::Red,
							Vector2f(-1.f, 0.f),
							28, 30.f, true
						)
					);

					const int nrOfPatricles = rand() % 20 + 5;
					for (int m = 0; m < nrOfPatricles; m++)
					{
						this->particles.Add(Particle(currentEnemy->getPosition(),
							0,
							currentEnemy->getVelocity(),
							rand() % 40 + 10.f,
							rand() % 30 + 1.f,
							50.f));
					}

					// Check for player death
					if (players[j].isDead()) {
						this->players[j].setTimeAlive(this->scoreTimer.getElapsedTime().asSeconds());
						this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setScore(this->players[j].getScore());
						this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setEnemiesKilled(this->players[j].getEnemiesKilled());
						this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setSecondsSurvive(this->players[j].getTimeAlive());
						this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setHighestLevelAchieved(this->players[j].getHighestLevelAchieved());
						this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_DEATH);
						this->players.Remove(j);
						return;
					}
					break;
				}
			}
		}
	}

	// Update Enemy Bullets
	this->UpdateEnemyBullets(dt);
}

void Game::UpdateEnemyBullets(const float &dt) {
	for (size_t i = 0; i < EnemyLifeform::bullets.Size(); i++)
	{
		EnemyLifeform::bullets[i].Update(dt);

		// Bullet Window bounds check
		if (EnemyLifeform::bullets[i].getPosition().x > (this->mainView.getCenter().x + (this->mainView.getSize().x / 2))
			|| EnemyLifeform::bullets[i].getPosition().y > (this->mainView.getCenter().y + (this->mainView.getSize().y / 2))
			|| EnemyLifeform::bullets[i].getPosition().x <= this->mainView.getCenter().x - (this->mainView.getSize().x / 2)
			|| EnemyLifeform::bullets[i].getPosition().y <= this->mainView.getCenter().y - (this->mainView.getSize().y / 2)) {
			EnemyLifeform::bullets.Remove(i);
			break;
		}

		// Bullet Player collision check
		for (size_t j = 0; j < this->players.Size(); j++)
		{
			if ((this->players[j].isShielding() || this->players[j].getPowerupAbsorb()) && !EnemyLifeform::bullets[i].isPlayerProjectile()) {
				if (EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getDeflectorShield().getGlobalBounds())
					 || EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getGlobalBounds())) {

					// Absorb, add charge to the shield
					if (!this->players[j].isShielding() && this->players[j].getPowerupAbsorb()) {
						// Add a small charge to the shield because of absorbtion
						this->players[j].addShieldCharge(10);
						// Play sound effect
						this->audioManager->PlaySound(AudioManager::AudioSounds::POWERUP_ABSORB);
						// Remove from enemy bullet
						EnemyLifeform::bullets.Remove(i);
						break;
					}
					else {
						// Reflect bullet
						this->players[j].getDeflectorShield().setColor(Color::Red);

						// Reverse direction
						EnemyLifeform::bullets[i].reverseDirection();
						// Add bullet to player
						this->players[j].getBullets()->Add(Bullet(Bullet::ORB_BLUE,
							Vector2f(0.2f, 0.2f),
							EnemyLifeform::bullets[i].getPosition(),
							this->normalize(EnemyLifeform::bullets[i].getVelocity(), this->vectorLength(EnemyLifeform::bullets[i].getVelocity())),
							EnemyLifeform::bullets[i].getMaxVelocity(),
							EnemyLifeform::bullets[i].getMaxVelocity(),
							EnemyLifeform::bullets[i].getDamage()));
						// Remove from enemy bullet
						EnemyLifeform::bullets.Remove(i);
						this->audioManager->PlaySound(AudioManager::AudioSounds::REFLECT_BULLET);
						break;
					}
				}
			}
			else if (EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getGlobalBounds())) {

				// Generate a few collision particles
				const int nrOfPatricles = rand() % 5 + 3;
				for (int m = 0; m < nrOfPatricles; m++)
				{
					this->particles.Add(Particle(EnemyLifeform::bullets[i].getPosition(),
						0,
						EnemyLifeform::bullets[i].getVelocity(),
						rand() % 20 + 5.f,
						rand() % 20 + 1.f,
						30.f));
				}

				// Damage player		
				int damage = (rand() % 2 + 1) + (this->players[j].getLevel() / 2);

				EnemyLifeform::bullets.Remove(i);

				// Half damage player takes for Cosmos mode since its way harder
				if (this->gameMode == Game::Mode::COSMOS) {
					damage /= 2;
				}
				this->audioManager->PlaySound(AudioManager::AudioSounds::ENEMY_BULLET_IMPACT);
				this->players[j].TakeDamage(damage);

				// Player collision damage
				this->textTags.Add(
					TextTag(
						&this->font, Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
							this->players[j].getPosition().y - this->players[j].getGlobalBounds().height / 2),
						"-" + std::to_string(damage), Color::Red,
						Vector2f(-1.f, 0.f),
						28, 30.f, true
					)
				);

				// Check for player death
				if (players[j].isDead()) {
					this->players[j].setTimeAlive(this->scoreTimer.getElapsedTime().asSeconds());
					this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setScore(this->players[j].getScore());
					this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setEnemiesKilled(this->players[j].getEnemiesKilled());
					this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setSecondsSurvive(this->players[j].getTimeAlive());
					this->playerScoreMap.find(this->players[j].getPlayerNumber())->second.setHighestLevelAchieved(this->players[j].getHighestLevelAchieved());
					this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_DEATH);
					this->players.Remove(j);
					return;
				}
				break;
			}
		}
	}
}

void Game::UpdateTextTags(const float &dt) {
	for (size_t i = 0; i < this->textTags.Size(); i++)
	{
		this->textTags[i].Update(dt);

		if (this->textTags[i].getTimer() <= 0.f) {
			this->textTags.Remove(i);
		}
	}
}

void Game::UpdateConsumables(const float &dt) {
	for (size_t i = 0; i < this->consumables.Size(); i++)
	{
		Consumable *currentItem = this->consumables[i];

		currentItem->Update(dt);

		if (currentItem->canDelete()) {
			this->consumables.Remove(i);
			continue;
		}

		for (size_t j = 0; j < this->players.Size(); j++)
		{
			if (currentItem->CollidesWith(this->players[j].getGlobalBounds())) {

				currentItem->Consume(this->textTags, &this->font, &this->players[j]);
				this->consumables.Remove(i);

				break;
			}
		}
	}
}

void Game::UpdateParticles(const float &dt) {
	for (size_t i = 0; i < this->particles.Size(); i++)
	{
		this->particles[i].Update(dt);

		if (this->particles[i].canDelete()) {
			this->particles.Remove(i);
			continue;
		}
	}
}

void Game::DrawUI() {
	// Draw Game Over Text - if needed
	if (!this->playersExistInWorld()) {
		this->gameOverMenu->Draw(*this->window);
		this->window->draw(this->gameOverText);
		this->window->draw(this->leaderboardText);
	}
	else {
		if (this->gameMode == Mode::COSMOS) {
			this->window->draw(this->cosmoEffectText);
		}
		else if (this->gameMode == Mode::INFINTE || this->gameMode == Mode::CAMPAIGN) {
			this->window->draw(this->scoreText);
		}
	}

	if (this->paused) {
		this->window->draw(this->controlsText);
	}
}

void Game::DrawTextTags() {
	for (size_t i = 0; i < this->textTags.Size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}
}

void Game::DrawEnemy() {
	for (size_t i = 0; i < this->enemyLifeforms.Size(); i++)
	{
		EnemyLifeform *currentEnemy = this->enemyLifeforms[i];
		this->enemyText.setPosition(currentEnemy->getPosition().x, currentEnemy->getPosition().y - 10.f);
		this->enemyText.setString(std::to_string(currentEnemy->getHp()) + "/" + std::to_string(currentEnemy->getHpMax()));

		// Draw Enemy Lifeform
		currentEnemy->Draw(*this->window);
		// Draw Enemy Lifeform UI
		this->window->draw(this->enemyText);
	}

	// Draw Enemy bullets
	for (size_t i = 0; i < EnemyLifeform::bullets.Size(); i++)
	{
		EnemyLifeform::bullets[i].Draw(*this->window);
	}
}

void Game::DrawPlayersAndMap() {
	this->stage->Draw(*this->window, this->mainView, false, this->font, &this->players);
}

void Game::DrawConsumables() {
	for (size_t i = 0; i < this->consumables.Size(); ++i) {
		Consumable *currentItem = this->consumables[i];
		currentItem->Draw(*this->window);
	}
}

void Game::DrawParticles() {
	for (size_t i = 0; i < this->particles.Size(); i++)
	{
		this->particles[i].Draw(*this->window);
	}
}

void Game::Draw() {
	this->window->clear();

	// Set View
	this->window->setView(this->mainView);

	if (this->mainMenu->isActive()) {
		this->mainMenu->Draw(*this->window);
		return;
	}

	// Draw Map and Players
	this->DrawPlayersAndMap();

	// Draw Enemy Lifeform
	this->DrawEnemy();

	// Draw Consumables
	this->DrawConsumables();

	// Draw Particles
	this->DrawParticles();

	// Draw Text tags
	this->DrawTextTags();

	// Draw UI - update view
	this->window->setView(this->window->getDefaultView());
	this->DrawUI();

	if (this->gameMode == Mode::TUTORIAL) {
		this->tutorial->Draw(*this->window);
	}
}

void Game::InitPlayerScoreStats() {
	for (size_t i = 0; i < this->players.Size(); i++)
	{
		this->playerScoreMap[this->players[i].getPlayerNumber()] = PlayerScore();
	}
}

void Game::ToggleFullscreen() {
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;

		this->window->close();
		this->fullscreen = !this->fullscreen;
		this->window->create(sf::VideoMode(1920, 1080), "Gauss", this->fullscreen ? Style::Fullscreen : Style::Default);
	}
}

void Game::PauseGame() {
	if (Keyboard::isKeyPressed(Keyboard::P) && this->keyTime >= this->keyTimeMax) {
		this->paused = !this->paused;
		this->keyTime = 0.f;
	}
}

void Game::DisplayGameEnd() {
	this->scoreTime = std::max(1, (int)this->scoreTimer.getElapsedTime().asSeconds());
	std::string scoreText = "";
	std::map<int, PlayerScore>::iterator it = this->playerScoreMap.begin();
	// Store whichever leaderboard we need based off game mode
	LeaderboardIndex leadIndex = this->gameMode == Mode::COSMOS
		? LeaderboardIndex::COS
		: LeaderboardIndex::INF;

	while (it != this->playerScoreMap.end()) {
		// calculate the final player score
		const int finalPlayerScore = this->_calculateScore(it->second);
		
		// insert it into the leaderboard if possible
	   this->_insertLeaderboardEntry(leadIndex, it->first,finalPlayerScore);
		
		// display stats on screen
		scoreText += "Player " + std::to_string(it->first) + " Score: " + std::to_string(finalPlayerScore) + "\n";
		scoreText +=	"\tEnemies Killed: " + std::to_string(it->second.getEnemiesKilled()) +
						"\n\tHighest Level Achieved: Level " + std::to_string(it->second.getHighestLevelAchieved()) +
						"\n\tTime Survived: " + std::to_string(static_cast<int>(it->second.getSecondsSurvived())) + " seconds" +
					 "\n\n";
		++it;
	}
	this->gameOverText.setString(scoreText);

	// Write leaderboard text
	std::string leaderboardText = "Gaussian\t\t\tScore\n";
	for (size_t i = 0; i < this->leaderboards[leadIndex].Size(); i++)
	{
		leaderboardText += "G." + std::to_string(this->leaderboards[leadIndex][i].id) + "\t\t\t\t\t\t" + std::to_string(this->leaderboards[leadIndex][i].score);
		leaderboardText += "\n\n";
	}
	for (size_t i = 0; i < (this->LEADERBOARD_MAX - this->leaderboards[leadIndex].Size()); i++)
	{
		leaderboardText += "G...\t\t\t\t\t\t....";;
		leaderboardText += "\n\n";
	}
	this->leaderboardText.setString(leaderboardText);
	
	// Write to file
	this->_storeLeaderboard(leadIndex, (leadIndex == LeaderboardIndex::COS ? "Cosmos" : "Infinite"));
}


void Game::_insertLeaderboardEntry(LeaderboardIndex leadIndex, int id, int score) {
	if (this->leaderboards[leadIndex].Size() == this->LEADERBOARD_MAX && score < this->leaderboards[leadIndex][this->leaderboards[leadIndex].Size() - 1].score) {
		return; 
	}

	if (this->leaderboards[leadIndex].Size() == this->LEADERBOARD_MAX && score >= this->leaderboards[leadIndex][this->leaderboards[leadIndex].Size() - 1].score) {
		// Remove lowest
		this->leaderboards[leadIndex].Remove(this->leaderboards[leadIndex].Size() - 1);
	}

	Leaderboard lead;
	lead.id = id;
	lead.score = score;
	// Add new one
	this->leaderboards[leadIndex].Add(lead);
	// Sort
	this->_sortLeaderboard(leadIndex);
}

void Game::_storeLeaderboard(LeaderboardIndex leadIndex, std::string leaderFile) {
	std::ofstream fout;
	std::string filename = "Leaderboards/" + leaderFile +"/leaderboard.txt";
	fout.open(filename, std::ofstream::out | std::ofstream::trunc);
	if (fout.is_open()) {
		for (size_t i = 0; i < this->leaderboards[leadIndex].Size(); i++)
		{
			fout << std::to_string(this->leaderboards[leadIndex][i].id) << " " << std::to_string(this->leaderboards[leadIndex][i].score);
			if (i + 1 < this->leaderboards[leadIndex].Size()) {
				fout << "\n";
			}
		}
	}
	else {
		std::cerr << "Failure to open Leaderboards file to write " << std::endl;
	}
	fout.close();
}

void Game::_sortLeaderboard(LeaderboardIndex leadIndex) {
		for (size_t i = 0; i < this->leaderboards[leadIndex].Size() - 1; i = i + 1) {
			int min = i;
			for (size_t j = i + 1; j < this->leaderboards[leadIndex].Size(); j = j + 1) {
				if (this->leaderboards[leadIndex][j].score > this->leaderboards[leadIndex][min].score) { min = j; }
			}
			std::swap(this->leaderboards[leadIndex][i], this->leaderboards[leadIndex][min]);
		}
}


int Game::_calculateScore(PlayerScore &playerScore) {
	// total score + enemies killed per second over lifetime + score per second over lifetime
	return static_cast<int>(playerScore.getScore() + (static_cast<float>(playerScore.getEnemiesKilled()) / playerScore.getSecondsSurvived()) + (static_cast<float>(totalScore) / playerScore.getSecondsSurvived()) + (playerScore.getHighestLevelAchieved() * 10));
}

void Game::_spawnEnemy(int enemyType, int forcedVelocity, Vector2f position) {
	const int pNum = rand() % this->players.Size();
	const int eLevel = this->players[pNum].getLevel();
	const float velocity = static_cast<float>(forcedVelocity);

	Vector2f compensatedPos = position;
	// Compensation over time for increasing hardness
	if (enemyType == EnemyLifeform::MOVE_LEFT || enemyType == EnemyLifeform::MOVE_LEFT_SHOOT_LINE) {
		if (position == Vector2f(0.f, 0.f)) {
			const int inLineChance = rand() % 100;
			if (inLineChance > 50) {
				compensatedPos = Vector2f((this->mainView.getCenter().x + (this->mainView.getSize().x / 2.f)), this->players[pNum].getPosition().y);
			}
		}
	}

	switch (enemyType) {
	case EnemyLifeform::MOVE_LEFT:
		this->enemyLifeforms.Add(new MoveLeftEnemy(this->mainView, eLevel, pNum, velocity, compensatedPos));
		break;
	case EnemyLifeform::FOLLOW:
		this->enemyLifeforms.Add(new TrackerEnemy(this->mainView, eLevel, pNum, velocity, position));
		break;
	case EnemyLifeform::MOVE_LEFT_SHOOT:
		this->enemyLifeforms.Add(new MoveLeftShootEnemy(this->window, this->mainView, eLevel, pNum, velocity, position));
		break;
	case EnemyLifeform::MOVE_LEFT_SHOOT_LINE:
		this->enemyLifeforms.Add(new MoveLeftShootLineEnemy(this->window, this->mainView, eLevel, pNum, velocity, compensatedPos));
		break;
	case EnemyLifeform::COSMOSOS_01:
	case EnemyLifeform::COSMOSOS_02:
	case EnemyLifeform::COSMOSOS_03:
	case EnemyLifeform::COSMOSOS_04:
		this->enemyLifeforms.Add(new CosmoEnemy(this->mainView, enemyType, eLevel, pNum, velocity, position));
		break;
	}

}

std::string Game::_getPlayerLivesText() {
	if (!this->playersExistInWorld()) {
		return "All Gaussians Dead";
	}
	std::string text = "";
	text += "Player Lives: ";
	int lives = 0;
	for (size_t i = 0; i < this->players.Size(); i++)
	{
		lives += this->players[i].getLives();
	}
	text += std::to_string(lives);
	return text;
}

void Game::_redeploy() {
	if (this->gameMode == Mode::CAMPAIGN) {
		this->gameOverMenu->LoadGameOverBackground(GameOverMenu::Backgrounds::CAMPAIGN);
		this->paused = false;
	}
	else {
		this->paused = true;
	}

	// Reset score and multipliers
	this->totalScore = 0;
	this->scoreTime = 0;
	this->scoreTimer.restart();
	this->killPerfectionMultiplier = 1;
	this->killPerfectionAdder = 0;
	this->killPerfectionAdderMax = 15;
	this->killboxMultiplier = 1;
	this->killboxTimerMax = 400.f;
	this->killboxTimer = this->killboxTimerMax;
	this->killboxAdder = 0;
	this->killboxAdderMax = 10;

	// Reset difficulty
	this->difficulty = 0;
	this->enemySpawnTimerMax = 35.f; // Also in constructor

	// Reset collections
	this->enemyLifeforms.Clear();
	this->consumables.Clear();
	this->particles.Clear();
	this->textTags.Clear();

	// Init boss encounter
	this->bossEncounterActivated = false;
	this->bosses.Clear();

	// Reset Stage
	this->mainView.setCenter(Vector2f(
		this->window->getSize().x / 2.f,
		this->window->getSize().y / 2.f));
	this->stage->Reset(this->mainView);

	// Reset all players
	this->players.Clear();
	// Clear out players score
	this->playerScoreMap.clear();

	// Reset player
	const int nrOfPlayers = Player::playerId;
	Player::playerId = 0;
	this->InitPlayersInWorld(nrOfPlayers);

	// Set player lives 
	this->_setPlayerLives();

	this->InitUI();
	this->InitMap();
}

void Game::_setPlayerLives() {
	int pLives = 1;
	if (this->gameMode == Mode::CAMPAIGN) {
		pLives = 3;
	}

	for (size_t i = 0; i < this->players.Size(); i++)
	{
		this->players[i].setLives(pLives);
	}
}

