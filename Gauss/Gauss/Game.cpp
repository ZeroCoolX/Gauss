#include "Game.h"

Game::Game(RenderWindow *window)
{
	this->window = window;
	// Init stage
	this->stage = nullptr;
	// Init Menus
	this->mainMenu = nullptr;
	this->gameOverMenu = nullptr;

	this->audioManager = new AudioManager();
	//this->audioManager->PlaySound(AudioManager::AudioSounds::SHOT_DEFAULT);
	this->audioManager->PlayMusic(AudioManager::AudioMusic::MENU);

	this->gameMode = Mode::INFINTE;
	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->InitView();
	this->InitTextures();
	this->InitMap();
	this->InitMenus();

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
	this->players.Add(Player(this->audioManager));

	/*this->players.Add(Player(this->audioManager,
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
		Keyboard::Num0));*/

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


void Game::InitTextures() {
	// buttons
	MenuButton::InitTextures();

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

	// Game Over Text
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(30);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setString("Game Over! (X___X)");
	this->gameOverText.setPosition(50.f, (float)this->window->getSize().y / 4);

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

void Game::InitMap() {
	this->stage = new Stage(10, 10);
	this->stage->LoadStage("lel.smap", this->mainView);
}

void Game::InitMenus() {
	this->mainMenu = new MainMenu(this->font, this->window);
	this->gameOverMenu = new GameOverMenu(this->font, this->window);
}

void Game::UpdateView(const float &dt) {
	this->mainView.move(this->stage->getScrollSpeed() * dt * DeltaTime::dtMultiplier, 0.f);
}

void Game::Update(const float &dt) {

	if (this->mainMenu->isActive()) {
		this->UpdateMainMenu(dt);
		return;
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

		// Update mid game if the player wants to
		this->RestartUpdate();

	}
	else if (!this->playersExistInWorld() && this->scoreTime == 0) {
		// Show end game stats
		this->DisplayGameEnd();
		// Activate the game over screen if it's not already turned on
		if (!this->gameOverMenu->isActive()) {
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
		this->gameMode = Mode::CAMPAIGN;
		this->mainMenu->Reset();
	}
	else if (this->mainMenu->onInfinitePress()) {
		this->gameMode = Mode::INFINTE;
		this->mainMenu->Reset();
	}
	else if (this->mainMenu->onCosmosPress()) {
		this->gameMode = Mode::COSMOS;
		this->mainMenu->Reset();
	}
}

void Game::UpdateGameOverMenu(const float &dt) {
	this->gameOverMenu->Update(dt);

	if (this->gameOverMenu->onRedeployPress()) {
		this->gameOverMenu->Reset();
		this->_redeploy();
	}
	else if (this->gameOverMenu->onMenuPress()) {
		this->gameOverMenu->Reset();
		this->_redeploy();
		this->mainView.setCenter(Vector2f(
			this->window->getSize().x / 2.f,
			this->window->getSize().y / 2.f));
		this->mainMenu->activate();
	}
	else {
		this->RestartUpdate();
	}
}

void Game::RestartUpdate() {
	if (Keyboard::isKeyPressed(Keyboard::F1)) {
		this->_redeploy();
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

		for (size_t i = 0; i < this->players.Size(); ++i) {

			// Players update
			this->players[i].Update(this->mainView, dt, this->stage->getScrollSpeed());

			// TESTING FOR NOW - UPDATE WALL-PLAYER COLLISION
			//this->UpdateWallColliders(dt, i);
			this->UpdatePlayerTileCollision(dt, this->players[i]);

			// Bullets update
			this->UpdatePlayerBullets(dt, this->players[i]);

			this->totalScore += this->players[i].getScore();
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
						int consumableType = rand() % 3 + 1;
						switch (consumableType) {
						case 1:
						{
							if (dropChance > 90) { // 10% chance for an upgrade

								// Only drop an upgrade we don't have - otherwise randomly choose stat point upgrade, or health
								uType = rand() % ItemUpgrade::numberOfUpgrades;
								for (size_t u = 0; u < currentPlayer.getAcquiredUpgrades().Size(); u++)
								{
									if (uType == currentPlayer.getAcquiredUpgrades()[u]) {
										uType = rand() % 1;
									}
									// Want to make it really hard to get double and triple ray upgrades
									if (uType == ItemUpgrade::Type::TRIPLE_RAY && currentPlayer.getGunLevel() == Player::LaserLevels::LEVEL_2_LASER) {
										dropChance = rand() % 100 + 1;
										if (dropChance <= 20) {
											// only a203% chance this will happen
											uType = ItemUpgrade::Type::TRIPLE_RAY;
											break;
										}
									}
									else if (uType == ItemUpgrade::Type::DOUBLE_RAY && currentPlayer.getGunLevel() == Player::LaserLevels::DEFAULT_LASER) {
										dropChance = rand() % 100 + 1;
										if (dropChance <= 25) {
											// only a 25% chance this will happen
											uType = ItemUpgrade::Type::DOUBLE_RAY;
											break;
										}
									}
								}
								this->consumables.Add(new ItemUpgrade(
									currentEnemy->getPosition(),
									uType,
									300.f));
							}
							break;
						}
						case 2: {
							if (dropChance > 85) { // 25% chance health is dropped
								this->consumables.Add(new ItemPickup(
									currentEnemy->getPosition(),
									ItemPickup::HEALTH, // health item for now
									150.f));
							}
							break;
						}
						case 3:
						{
							uType = rand() % Powerup::numberOfPowerups;
							if (dropChance > 85) { // 25% chance powerup is dropped
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
					}

					// Destroy the bullet if not piercing shot && not gauss shot
					if (!currentPlayer.getPiercingShot() && !currentPlayer.BulletAt(j).isGaussShot()) {
						// Should add effect to indicate it is piercing shots
						currentPlayer.RemoveBullet(j);
					}
					else if (currentPlayer.getPiercingShot()) {
						// Move to the end of the sprite it hit so that there is only a single point of damage calculation
						currentPlayer.BulletAt(j).setPosition(Vector2f(currentEnemy->getPosition().x + currentEnemy->getGlobalBounds().width, currentPlayer.BulletAt(j).getPosition().y));
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
				// temporary collision
				currentPlayer.move(-currentPlayer.getNormDir().x * 100 * dt * DeltaTime::dtMultiplier, -currentPlayer.getNormDir().y * 100 * dt * DeltaTime::dtMultiplier);
				currentPlayer.resetVelocity();
			}
		}
	}
}

void Game::UpdateMap(const float &dt) {
	this->stage->Update(dt, this->mainView, false);
}

void Game::UpdateWallColliders(const float &dt, int playerIndex) {
	//for (size_t i = 0; i < this->tiles.Size(); i++)
	//{
	//	if (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
	//		while (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
	//			this->players[playerIndex].move(
	//				20.f * -1.f * this->players[playerIndex].getNormDir().x,
	//				20.f * -1.f * this->players[playerIndex].getNormDir().y
	//			);
	//		}

	//		this->players[playerIndex].resetVelocity();
	//	}
	//}
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
	if (this->gameMode == Mode::INFINTE) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->_spawnEnemy(rand() % EnemyLifeform::nrOfEnemyTypes);
			this->enemySpawnTimer = 0;
		}
	}
	else {// CAMPAIGN
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
				if (this->players[j].getGlobalBounds().intersects(currentEnemy->getGlobalBounds())
					&& !this->players[j].isDamageCooldown()) {

					// Damage player		
					int damage = currentEnemy->getDamage();
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
			if (this->players[j].isShielding() && !EnemyLifeform::bullets[i].isPlayerProjectile()) {
				if (EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getDeflectorShield().getGlobalBounds())
					 || EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getGlobalBounds())) {

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
					break;
				}
			}else if (EnemyLifeform::bullets[i].getGlobalBounds().intersects(this->players[j].getGlobalBounds())) {

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
				int damage = (rand() % 2 + 1) + (this->players[j].getLevel() / 2);// /*As level increases this got rediculous*/ EnemyLifeform::bullets[i].getDamage() / 3;

				EnemyLifeform::bullets.Remove(i);

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
	}
	else {
		// Score text
		this->window->draw(this->scoreText);
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

void Game::DrawPlayers() {
	for (size_t i = 0; i < this->players.Size(); ++i) {
		this->players[i].Draw(*this->window);
	}
}

void Game::DrawMap() {
	this->stage->Draw(*this->window, this->mainView, false, this->font);
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

	// Draw Map
	this->DrawMap();

	// Draw Players
	this->DrawPlayers();

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

	this->gameOverText.setString("Score: " +
		std::to_string(this->totalScore) +
		"\nTime: " +
		std::to_string(this->scoreTime) +
		"\n Score/Second: " +
		std::to_string(std::ceil((double)this->totalScore / (double)this->scoreTime)) +
		"\nF1 to Restart"
	);

	if ((double)this->totalScore / (double)this->scoreTime > this->bestScorePerSecond) {
		this->bestScorePerSecond = (double)this->totalScore / (double)this->scoreTime;
	}
}

void Game::_spawnEnemy(int enemyType, int forcedVelocity, Vector2f position) {
	const int pNum = rand() % this->players.Size();
	const int eLevel = this->players[pNum].getLevel();
	const float velocity = static_cast<float>(forcedVelocity);
	switch (enemyType) {
	case EnemyLifeform::MOVE_LEFT:
		this->enemyLifeforms.Add(new MoveLeftEnemy(this->mainView, eLevel, pNum, velocity, position));
		break;
	case EnemyLifeform::FOLLOW:
		this->enemyLifeforms.Add(new TrackerEnemy(this->mainView, eLevel, pNum, velocity, position));
		break;
	case EnemyLifeform::MOVE_LEFT_SHOOT:
		this->enemyLifeforms.Add(new MoveLeftShootEnemy(this->window, this->mainView, eLevel, pNum, velocity, position));
		break;
	case EnemyLifeform::MOVE_LEFT_SHOOT_LINE:
		this->enemyLifeforms.Add(new MoveLeftShootLineEnemy(this->window, this->mainView, eLevel, pNum, velocity, position));
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
	for (size_t i = 0; i < this->players.Size(); i++)
	{
		this->players[i].Reset();
	}

	this->paused = true;

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

	if (this->players.Size() == 0) {
		// Reset player
		const int nrOfPlayers = Player::playerId;
		Player::playerId = 0;
		this->players.Add(Player(this->audioManager));
		// If there was a player 2 add them back in as well
		if (nrOfPlayers > 1) {
			this->players.Add(Player(this->audioManager,
				Keyboard::I,
				Keyboard::K,
				Keyboard::J,
				Keyboard::L,
				Keyboard::RShift,
				Keyboard::U,
				Keyboard::Return,
				Keyboard::Num7,
				Keyboard::Num8,
				Keyboard::Num9,
				Keyboard::Num0));
		}
	}

	this->InitUI();
	this->InitMap();
}
