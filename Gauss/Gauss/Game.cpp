#include "Game.h"
#include "Enums.h"

Game::Game(RenderWindow *window)
{
	this->window = window;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	// Init textures
	this->InitTextures();

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
	this->players.Add(Player(this->textureMap, this->playerMainGunTextures, this->lWingTextures, this->rWingTextures, this->auraTextures, this->cockpitTextures));
	//this->players.Add(Player(this->textureMap, this->playerMainGunTextures, this->lWingTextures, this->rWingTextures, this->auraTextures, this->cockpitTextures, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift));

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

	this->InitUI();
}

Game::~Game()
{
}

void Game::InitTextures() {
	// Init textures
	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_SHIP].loadFromFile("Textures/ship.png");
	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_LASER01].loadFromFile("Textures/Guns/rayTex01.png");
	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_MISSILE01].loadFromFile("Textures/Guns/missileTex01.png");

	// Load Enemy Textures
	Texture temp;
	temp.loadFromFile("Textures/Ships/enemyMoveLeft.png");
	this->enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyFollow.png");
	this->enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyMoveLeftShoot.png");
	this->enemyTextures.Add(Texture(temp));

	// Load Enemy Bullet Textures
	temp.loadFromFile("Textures/Guns/roundBulletRed.png");
	this->enemyBulletTextures.Add(Texture(temp));

	// Init player Main gun textures
	temp.loadFromFile("Textures/Guns/gun01.png");
	this->playerMainGunTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun02.png");
	this->playerMainGunTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Guns/gun03.png");
	this->playerMainGunTextures.Add(Texture(temp));

	// Init pickups textures
	temp.loadFromFile("Textures/Pickups/hpSupply.png");
	this->pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileSupply.png");
	this->pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileHSupply.png");
	this->pickupTextures.Add(Texture(temp));

	// Init Upgrade textures
	temp.loadFromFile("Textures/Upgrades/doubleray.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/tripleray.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/piercingshot.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/shield.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/healthtank.png");
	this->upgradeTextures.Add(Texture(temp));


	// Init Accessories
	std::string accessoriesBaseDir = "Textures/Accessories/";
	std::string accessories[] = {
	"leftwings.txt","rightwings.txt","auras.txt","cockpits.txt" };

	std::ifstream in;
	std::string accessoryFileName;
	for (int i = 0; i < 4; ++i)
	{
		in.open(accessoriesBaseDir + accessories[i]);
		if (in.is_open()) {
			while (getline(in, accessoryFileName)) {
				Texture temp;
				temp.loadFromFile(accessoryFileName);
				switch (i) {
				case 0:
					this->lWingTextures.Add(Texture(temp));
					break;
				case 1:
					this->rWingTextures.Add(Texture(temp));
					break;
				case 2:
					this->auraTextures.Add(Texture(temp));
					break;
				case 3:
					this->cockpitTextures.Add(Texture(temp));
					break;
				}
			}
		}
		in.close();
	}
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
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setString("Game Over! (X___X)");
	this->gameOverText.setPosition((float)this->window->getSize().x / 4, (float)this->window->getSize().y / 2);

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
	this->scoreText.setPosition(10.f, 10.f); // top left magic numbers for now
}

void Game::Update(const float &dt) {
	
	// Keytime update
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}

	// Fullscreen check
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax) {

	}

	// Pause check
	if (Keyboard::isKeyPressed(Keyboard::P) && this->keyTime >= this->keyTimeMax) {
		this->paused = !this->paused;
		this->keyTime = 0.f;
	}

	// Only allow changing accessories while paused
	if (this->paused) {
		for (size_t i = 0; i < this->players.Size(); i++)
		{
			if (!this->players[i].isDead()) {
				this->players[i].ChangeAccessories(dt);
			}
		}
	}

	// Update the world
	if (!this->paused && this->playersExistInWorld()) {
		// Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax) { this->enemySpawnTimer += 1.f * dt * DeltaTime::dtMultiplier; } // 1.f is not needed here
		
		if (this->enemySpawnTimerMax > 10) {
			this->difficultyTimer += 1.f * dt * DeltaTime::dtMultiplier;
		}

		// Make the game harder with time by decreasing the wait between enemy spawns over time
		if ((int)this->difficultyTimer % 1000 == 0 && this->enemySpawnTimerMax > 10) {
			this->enemySpawnTimerMax--;
			this->difficulty++;
			this->difficultyTimer = 1.f;
		}

		// Update Killbox timer
		if (this->killboxTimer > 0.f) {
			this->killboxTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		}
		else {
			this->killboxTimer = 0.f;
			this->killboxAdder = 0;
			this->killboxMultiplier = 1;
		}

		// Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->_spawnEnemy();
			this->enemySpawnTimer = 0;
		}

		// Reset the total each calculation
		this->totalScore = 0;

		for (size_t i = 0; i < this->players.Size(); ++i) {

			// Players update
			this->players[i].Update(this->window->getSize(), dt);

			// Bullets update
			for (size_t j = 0; j < this->players[i].getBulletsSize(); j++)
			{
				this->players[i].BulletAt(j).Update(dt);

				// Bullet Window bounds check
				if (this->players[i].BulletAt(j).getPosition().x > this->window->getSize().x) {
					this->players[i].RemoveBullet(j);
				}
				else {
					// Enemy - Bullet Collision check since it still exists in the world
					for (size_t k = 0; k < this->enemies.Size(); k++)
					{
						if (this->players[i].BulletAt(j).getGlobalBounds().intersects(this->enemies[k].getGlobalBounds())) {
								
							// Health check for damage or destruction
							int damage = this->players[i].getDamage();

							this->textTags.Add(
								TextTag(
									&this->font, "-" + std::to_string(damage), Color::Red,
									Vector2f(this->enemies[k].getPosition().x + this->enemies[k].getGlobalBounds().width / 4,
										this->enemies[k].getPosition().y - this->enemies[k].getGlobalBounds().height / 2),
									Vector2f(1.f, 0.f),
									24, 18.f, true
								)
							);

							this->enemies[k].TakeDamage(damage);

							if (this->enemies[k].getHp() <= 0) {

								// Gain score & Reset multiplier timer
								this->killboxTimer = this->killboxTimerMax;
								this->killboxAdder++;
								this->killPerfectionAdder++;

								// Total Score = (EnemyMaxHp + (EnemyMaxHp * KillboxMultiplier)) * PerfectionMultiplier 
								int score = (this->enemies[k].getHpMax() + (this->enemies[k].getHpMax() * this->killboxMultiplier)) * this->killPerfectionMultiplier;
								this->players[i].gainScore(score);

								// Score text tag
								this->textTags.Add(
									TextTag(
										&this->font, "+" + std::to_string(score), Color::White,
										Vector2f(100.f, 10.f),
										Vector2f(1.f, 0.f),
										30, 40.f, true
									)
								);

								// Player earned some EXP!
								int exp = this->enemies[k].getHpMax()
									+ (rand() % this->enemies[k].getHpMax() + 1) * (this->killboxMultiplier + 1);

								// Player leveled up!
								if (this->players[i].gainExp(exp)) {
									this->textTags.Add(
										TextTag(
											&this->font, "LEVEL UP!", Color::Cyan,
											Vector2f(this->players[i].getPosition().x + this->players[i].getGlobalBounds().width / 4,
												this->players[i].getPosition().y + this->players[i].getGlobalBounds().height),
											Vector2f(0.f, 1.f),
											36, 40.f, true
										)
									);
								}else{
									this->textTags.Add(
										TextTag(
											&this->font, "+" + std::to_string(exp) + " EXP", Color::Cyan,
											Vector2f(this->players[i].getPosition().x + this->players[i].getGlobalBounds().width / 4,
												this->players[i].getPosition().y + this->players[i].getGlobalBounds().height),
											Vector2f(0.f, 1.f),
											24, 40.f, true
										)
									);

								}

								// Change to drop pickup
								int dropChance = rand() % 100 + 1;

								if (dropChance > 75) { // 25% chance health is dropped
									this->pickups.Add(Pickup(
										this->pickupTextures,
										this->enemies[k].getPosition(),
										0,
										150.f));
								}
								else {
									if (dropChance > 90) { // 10% chance for an upgrade
										this->upgrades.Add(Upgrade(
											this->upgradeTextures,
											this->enemies[k].getPosition(),
											rand() % 5,
											300.f));
									}
								}

								// Destroy the enemy
								this->enemies.Remove(k);
							}

							// Destroy the bullet if not piercing shot
							if (!this->players[i].getPiercingShot()) {
								// Should add effect to indicate it is piercing shots
								this->players[i].RemoveBullet(j);
							}
							break;
						}
					}
				}
			}

			this->totalScore += this->players[i].getScore();
		}

		// Update Score text
		this->scoreText.setString(
			"Score: " + std::to_string(this->totalScore)
		+	"\nPerfection Score Mult: x" + std::to_string(killPerfectionMultiplier)
		+	"\nPerfect Kills / Next: " + std::to_string(this->killPerfectionAdder) + " / " + std::to_string(this->killPerfectionAdderMax)
		+	"\nKill Mult: x" + std::to_string(killboxMultiplier)
		+	"\nKillbox Kills / Next: " + std::to_string(this->killboxAdder) + " / " + std::to_string(this->killboxAdderMax)
		+	"\nKillbox Seconds Remaining: " + std::to_string((int)this->killboxTimer) + "s"
		+	"\nGame time: " + std::to_string((int)this->scoreTimer.getElapsedTime().asSeconds())
		+	"\nDifficulty: " + std::to_string(this->difficulty)
		+	"\nBest Score/Second: " + std::to_string(this->bestScorePerSecond));

		// Update score multipliers
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

		for (size_t i = 0; i < this->enemyLifeforms.Size(); i++)
		{
			(*this->enemyLifeforms[i]).Update(dt, this->players[(*this->enemyLifeforms[i]).getPlayerFollowNum()].getPosition());
		}


		// Update Enemy Movement
		for (size_t i = 0; i < this->enemies.Size(); i++)
		{
			// Safety check in case there are no more players in the world
			if (!this->playersExistInWorld()){
				this->enemies[i].updateAttackType(GameEnums::E_MOVE_LEFT);
				this->enemies[i].Update(dt);
			}
			else {
				// Check if we need to update the player this enemy is following incase they died
				if (this->enemies[i].getPlayerFollowNum() > (int)this->players.Size() - 1) {
					this->enemies[i].updatePlayerFollowNum(rand()% this->players.Size());
				}
				this->enemies[i].Update(dt, this->players[this->enemies[i].getPlayerFollowNum()].getPosition());
			}

			// Enemy Bullet Update
			for (size_t j = 0; j < this->enemies[i].getBullets().Size(); j++)
			{
				this->enemies[i].getBullets()[j].Update(dt);
			}

			// Enemy Window Bounds check
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width) {
				this->enemies.Remove(i);
			}
			else {
				// Check Player - Enemy collision
				for (size_t j = 0; j < this->players.Size(); j++)
				{
					if (this->players[j].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()) 
						&& !this->players[j].isDamageCooldown()) {
						
						// Damage player		
						int damage = this->enemies[i].getDamage();
						this->players[j].TakeDamage(damage);
						// Collision resets the perfection streak
						this->killPerfectionAdder = 0;
						this->killPerfectionMultiplier = 1;

						this->enemies[i].Collision();

						// Player collision damage
						this->textTags.Add(
							TextTag(
								&this->font, "-" + std::to_string(damage), Color::Red,
								Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
									this->players[j].getPosition().y - this->players[j].getGlobalBounds().height / 2),
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

		// Update Upgrades
		for (size_t i = 0; i < this->upgrades.Size(); i++)
		{
			this->upgrades[i].Update(dt);

			if (this->upgrades[i].canDelete()) {
				this->upgrades.Remove(i);
				continue;
			}

			for (size_t j = 0; j < this->players.Size(); j++)
			{
				if (this->upgrades[i].CheckCollision(this->players[j].getGlobalBounds())) {
					switch (this->upgrades[i].getType()) {
					case GameEnums::UT_DOUBLE_RAY:
						if (this->players[j].getGunLevel() < GameEnums::MGT_MAIN_GUN02) {
							this->players[j].SetGunLevel(GameEnums::MGT_MAIN_GUN02);
							this->textTags.Add(
								TextTag(
									&this->font, "DOUBLE RAY UPGRADE", Color::Yellow,
									Vector2f(this->players[j].getPosition()),
									Vector2f(1.f, 0.f),
									40, 100.f, true
								));
						}
						break;
					case GameEnums::UT_TRIPLE_RAY:
						if (this->players[j].getGunLevel() < GameEnums::MGT_MAIN_GUN03) {
							this->players[j].SetGunLevel(GameEnums::MGT_MAIN_GUN03);
							this->textTags.Add(
								TextTag(
									&this->font, "TRIPLE RAY UPGRADE", Color::Yellow,
									Vector2f(this->players[j].getPosition()),
									Vector2f(1.f, 0.f),
									40, 100.f, true
								));
						}
						break;
					case GameEnums::UT_PIERCING_SHOT: // need to take away once it dies off
						this->players[j].enablePiercingShot();
						this->textTags.Add(
							TextTag(
								&this->font, "PIERCING SHOT UPGRADE", Color::Yellow,
								Vector2f(this->players[j].getPosition()),
								Vector2f(1.f, 0.f),
								40, 100.f, true
							));
						break;
					case GameEnums::UT_SHEILD: // need to take away once it dies off
						this->players[j].enableSheild();
						this->textTags.Add(
							TextTag(
								&this->font, "SHEILD UPGRADE", Color::Yellow,
								Vector2f(this->players[j].getPosition()),
								Vector2f(1.f, 0.f),
								40, 100.f, true
							));
						break;
					case GameEnums::UT_HEALTH_TANK:
						this->players[j].upgradeHP();
						this->textTags.Add(
							TextTag(
								&this->font, "PERMANANT HEALTH UPGRADE", Color::Yellow,
								Vector2f(this->players[j].getPosition()),
								Vector2f(1.f, 0.f),
								40, 100.f, true
							));
						break;
					}
					this->upgrades.Remove(i);
					break;
				}
			}
		}

		// Update pickups
		for (size_t i = 0; i < this->pickups.Size(); i++)
		{
			this->pickups[i].Update(dt);

			if (this->pickups[i].canDelete()) {
				this->pickups.Remove(i);
				continue;
			}

			// Check player collision
			for (size_t j = 0; j < this->players.Size(); j++)
			{
				if (this->pickups[i].CheckCollision(this->players[j].getGlobalBounds())) {
					switch (this->pickups[i].getType()) {
					case 0: // HP
					{
						const int gainedHp = this->players[j].getHpMax() / 5;
						if (this->players[j].gainHp(gainedHp)) {
							this->textTags.Add(
								TextTag(
									&this->font, "+" + std::to_string(gainedHp) + " HP", Color::Green,
									Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
										this->players[j].getPosition().y - this->players[j].getGlobalBounds().height),
									Vector2f(0.f, 1.f),
									24, 40.f, true
								)
							);
						}
						else {
							if (this->players[j].gainExp(10)) {
								this->textTags.Add(
									TextTag(
										&this->font, "LEVEL UP!", Color::Cyan,
										Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
											this->players[j].getPosition().y + this->players[j].getGlobalBounds().height),
										Vector2f(0.f, 1.f),
										36, 40.f, true
									)
								);
							}
							else {
								this->textTags.Add(
									TextTag(
										&this->font, "+" + std::to_string(10) + " EXP", Color::Cyan,
										Vector2f(this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
											this->players[j].getPosition().y + this->players[j].getGlobalBounds().height),
										Vector2f(0.f, 1.f),
										24, 40.f, true
									)
								);
							}
						}
						break;
					}
					case 1: // Missile
						break;

					case 2: // Missile Heavy
						break;
					}

					this->pickups.Remove(i);
					break;
				}
			}
		}

		// Update Texttags
		for (size_t i = 0; i < this->textTags.Size(); i++)
		{
			this->textTags[i].Update(dt);

			if (this->textTags[i].getTimer() <= 0.f) {
				this->textTags.Remove(i);
			}
		}
	}else if (!this->playersExistInWorld() && this->scoreTime == 0) {
		this->scoreTime = std::max(1, (int)this->scoreTimer.getElapsedTime().asSeconds());

		this->gameOverText.setString("Game Over (X___X)\nScore: " +
			std::to_string(this->totalScore) +
			"\nTime: " +
			std::to_string(this->scoreTime) +
			"\n Score/Second: " +
			std::to_string(std::ceil((double)this->totalScore / (double)this->scoreTime)) +
			"\nF11 to Restart"
		);

		if ((double)this->totalScore / (double)this->scoreTime > this->bestScorePerSecond) {
			this->bestScorePerSecond = (double)this->totalScore / (double)this->scoreTime;
		}
	}

	// Restart
	if (!this->playersExistInWorld()) {
		if (Keyboard::isKeyPressed(Keyboard::F1)) {
			for (size_t i = 0; i < this->players.Size(); i++)
			{
				this->players[i].Reset();
			}

			// Reset score and multipliers
			this->totalScore = 0;
			this->scoreTime = 0;
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
			this->enemies.Clear();
			this->enemyLifeforms.Clear(); // temporary
			this->upgrades.Clear();
			this->pickups.Clear();

			// Reset player
			this->players.Add(Player(this->textureMap, this->playerMainGunTextures, this->lWingTextures, this->rWingTextures, this->auraTextures, this->cockpitTextures));
			this->InitUI();
		}
	}
}

void Game::DrawUI() {
	// Draw Texttags
	for (size_t i = 0; i < this->textTags.Size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}

	// Draw Game Over Text - if needed
	if (!this->playersExistInWorld()) {
		this->window->draw(this->gameOverText);
	}

	// Score text
	this->window->draw(this->scoreText);

	if (this->paused) {
		this->window->draw(this->controlsText);
	}
}

void Game::Draw(){
	this->window->clear();

	// Draw Players
	for (size_t i = 0; i < this->players.Size(); ++i) {
		this->players[i].Draw(*this->window);
	}

	// Draw Enemy Lifeform
	for (size_t i = 0; i < this->enemyLifeforms.Size(); i++)
	{
		this->enemyText.setPosition((*this->enemyLifeforms[i]).getPosition().x, (*this->enemyLifeforms[i]).getPosition().y - 10.f);
		this->enemyText.setString(std::to_string((*this->enemyLifeforms[i]).getHp()) + "/" + std::to_string((*this->enemyLifeforms[i]).getHpMax()));

		// Draw Enemy Lifeform
		(*this->enemyLifeforms[i]).Draw(*this->window);
		// Draw Enemy Lifeform UI
		this->window->draw(this->enemyText);
	}

	// Draw enemies
	for (size_t i = 0; i < this->enemies.Size(); i++)
	{
		this->enemyText.setPosition(this->enemies[i].getPosition().x, this->enemies[i].getPosition().y - 10.f);
		this->enemyText.setString(std::to_string(this->enemies[i].getHp()) + "/" + std::to_string(this->enemies[i].getHpMax()));

		// Draw Enemy
		this->enemies[i].Draw(*this->window);
		// Draw Enemy UI
		this->window->draw(this->enemyText);
	}

	// Draw Pickups
	for (size_t i = 0; i < this->pickups.Size(); ++i) {
		this->pickups[i].Draw(*this->window);
	}

	// Draw Upgrades
	for (size_t i = 0; i < this->upgrades.Size(); ++i) {
		this->upgrades[i].Draw(*this->window);
	}

	this->DrawUI();

	this->window->display();
}

void Game::_spawnEnemy() {
	const int pNum = rand() % this->players.Size();
	const int randType = rand() % 3;

	switch (randType) {
	case GameEnums::E_MOVE_LEFT:
		this->enemyLifeforms.Add(new MoveLeftEnemy(this->enemyTextures, this->window->getSize(), this->players[pNum].getLevel(), pNum));
		break;
	case GameEnums::E_FOLLOW:
		this->enemyLifeforms.Add(new TrackerEnemy(this->enemyTextures, this->window->getSize(), this->players[pNum].getLevel(), pNum));
		break;
	case GameEnums::E_MOVE_LEFT_SHOOT:
		// COMMENTED OUT UNTIL CLEANUP BULLETS HAPPEN
		//this->enemyLifeforms.Add(new MoveLeftShootEnemy(this->enemyTextures, this->enemyBulletTextures, this->window->getSize(), this->players[pNum].getLevel(), pNum));
		break;
	}

}