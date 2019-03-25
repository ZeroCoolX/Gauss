#include "Game.h"

Game::Game(RenderWindow *window)
{
	this->window = window;

	this->InitTextures();

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

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
	this->players.Add(Player());

	/*this->players.Add(Player(Keyboard::I, 
		Keyboard::K,
		Keyboard::J, 
		Keyboard::L, 
		Keyboard::RShift, 
		Keyboard::U,
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
	this->InitMap();
}

Game::~Game()
{
}



void Game::InitPlayerTextures() {
	Texture temp;

	// Body textures
	temp.loadFromFile("Textures/ship.png");
	Player::shipBodyTextures.Add(temp);

	// Bullet textures
	temp.loadFromFile("Textures/Guns/rayTex01.png");
	Player::shipBulletTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/missileTex01.png");
	Player::shipBulletTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/rayTex03.png");
	Player::shipBulletTextures.Add(temp);

	// Main Gun textures
	temp.loadFromFile("Textures/Guns/gun01.png");
	Player::shipMainGunTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/gun02.png");
	Player::shipMainGunTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/gun03.png");
	Player::shipMainGunTextures.Add(temp);

	// Ship Parts textures
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
				temp.loadFromFile(accessoryFileName);
				switch (i) {
				case 0:
					Player::shipLWingTextures.Add(Texture(temp));
					break;
				case 1:
					Player::shipRWingTextures.Add(Texture(temp));
					break;
				case 2:
					Player::shipAuraTextures.Add(Texture(temp));
					break;
				case 3:
					Player::shipCockpitTextures.Add(Texture(temp));
					break;
				}
			}
		}
		in.close();
	}
}

void Game::InitMapTextures() {
	Tile::tileTextures.loadFromFile("Textures/Map/textureSheet.png");
}

void Game::InitTextures() {
	this->InitPlayerTextures();

	Texture temp;

	// Load Enemy Textures
	temp.loadFromFile("Textures/Ships/enemyMoveLeft.png");
	this->enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyFollow.png");
	this->enemyTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Ships/enemyMoveLeftShoot.png");
	this->enemyTextures.Add(Texture(temp));

	// Load Enemy Bullet Textures
	temp.loadFromFile("Textures/Guns/roundBulletRed.png");
	this->enemyBulletTextures.Add(Texture(temp));

	// Boss textures
	// Body
	temp.loadFromFile("Textures/Bosses/Body/boss01.png");
	this->bossBodyTextures.Add(Texture(temp));
	// Gun
	temp.loadFromFile("Textures/Bosses/Guns/bossGun01.png");
	this->bossGunTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Bosses/Guns/bossGun02.png");
	this->bossGunTextures.Add(Texture(temp));
	// Bullets
	temp.loadFromFile("Textures/Bosses/Bullets/bossBullet01.png");
	this->bossBodyTextures.Add(Texture(temp));

	// Init pickups textures
	temp.loadFromFile("Textures/Pickups/hpSupply.png");
	this->pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileSupply.png");
	this->pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileHSupply.png");
	this->pickupTextures.Add(Texture(temp));
	this->numberOfPickups = this->pickupTextures.Size();

	// Init Upgrade textures
	temp.loadFromFile("Textures/Upgrades/statpoint.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/healthtank.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/doubleray.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/tripleray.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/piercingshot.png");
	this->upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/shield.png");
	this->upgradeTextures.Add(Texture(temp));
	this->numberOfUpgrades = this->upgradeTextures.Size();

	this->InitMapTextures();
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
	this->scoreText.setPosition(10.f, 10.f);
}

void Game::InitMap() {
	Tile tile(IntRect(0, 0, 50, 50), 
		Vector2f(200.f, 300.f), 
		true, 
		false);

	this->tiles.Add(tile);
}

void Game::Update(const float &dt) {

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

		//MAKE GAME HARDER WITH TIME
		this->UpdateDifficulty();

		// Update score multipliers
		this->UpdateMultipliers();

		// UPDATE PLAYERS
		this->UpdatePlayers(dt);

		// UPDATE ENEMIES
		this->UpdateEnemies(dt);

		// UPDATE CONSUMABLES
		this->UpdateConsumables(dt);

		// Update Texttags
		this->UpdateTextTags(dt);
	}
	else if (!this->playersExistInWorld() && this->scoreTime == 0) {
		// Show end game stats
		this->DisplayGameEnd();
	}

	// Restart
	if (!this->playersExistInWorld()) {
		this->RestartUpdate();
	}
}

void Game::RestartUpdate() {
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
		this->enemyLifeforms.Clear();
		this->consumables.Clear();

		// Init boss encounter
		this->bossEncounterActivated = false;
		this->bosses.Clear();

		// Reset player
		const int nrOfPlayers = Player::playerId;
		Player::playerId = 0;
		this->players.Add(Player());
		// If there was a player 2 add them back in as well
		if (nrOfPlayers > 1) {
			this->players.Add(Player(Keyboard::I,
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

		this->InitUI();
		this->InitMap();
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
			this->players[i].Update(this->window->getSize(), dt);

			// TESTING FOR NOW - UPDATE WALL-PLAYER COLLISION
			//this->UpdateWallColliders(dt, i);

			// Bullets update
			this->UpdatePlayerBullets(dt, this->players[i]);

			this->totalScore += this->players[i].getScore();
		}

		this->UpdateScoreUI();
	}
}

void Game::UpdateMap(const float &dt, int playerIndex) {
	
}

void Game::UpdateWallColliders(const float &dt, int playerIndex) {
	for (size_t i = 0; i < this->tiles.Size(); i++)
	{
		if (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
			while (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
				this->players[playerIndex].move(
					20.f * -1.f * this->players[playerIndex].getNormDir().x,
					20.f * -1.f * this->players[playerIndex].getNormDir().y
				);
			}

			this->players[playerIndex].resetVelocity();
		}
	}
}

void Game::UpdatePlayerBullets(const float &dt, Player &currentPlayer) {
	for (size_t j = 0; j < currentPlayer.getBulletsSize(); j++)
	{
		currentPlayer.BulletAt(j).Update(dt);

		// Bullet Window bounds check
		if (currentPlayer.BulletAt(j).getPosition().x > this->window->getSize().x) {
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

						// Player leveled up!
						if (currentPlayer.gainExp(exp)) {
							this->textTags.Add(
								TextTag(
									&this->font, Vector2f(currentPlayer.getPosition().x + currentPlayer.getGlobalBounds().width / 4,
										currentPlayer.getPosition().y + currentPlayer.getGlobalBounds().height), "LEVEL UP!",
									Color::Cyan,
									Vector2f(0.f, 1.f),
									36, 40.f, true
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

						// Change to drop consumable
						int dropChance = rand() % 100 + 1;
						int uType = 0;

						if (dropChance > 90) { // 10% chance for an upgrade

							// Only drop an upgrade we don't have - otherwise randomly choose stat point upgrade, or health
							uType = rand() % this->numberOfUpgrades;
							for (size_t u = 0; u < currentPlayer.getAcquiredUpgrades().Size(); u++)
							{
								if (uType == currentPlayer.getAcquiredUpgrades()[u]) {
									uType = rand() % 1;
								}
							}

							this->consumables.Add(new ItemUpgrade(
								this->upgradeTextures,
								currentEnemy->getPosition(),
								uType,
								300.f));
						}
						else if (dropChance > 75) { // 25% chance health is dropped
							this->consumables.Add(new ItemPickup(
								this->pickupTextures,
								currentEnemy->getPosition(),
								GameEnums::ITEM_HEALTH, // health item for now
								150.f));
						}

						// Destroy the enemy
						this->enemyLifeforms.Remove(k);
					}

					// Destroy the bullet if not piercing shot
					if (!currentPlayer.getPiercingShot() && !currentPlayer.BulletAt(j).gaussShot()) {
						// Should add effect to indicate it is piercing shots
						currentPlayer.RemoveBullet(j);
					}
					else if (!currentPlayer.BulletAt(j).gaussShot()) {
						// Move to the end of the sprite it hit so that there is only a single point of damage calculation
						currentPlayer.BulletAt(j).setPosition(Vector2f(currentEnemy->getPosition().x + currentEnemy->getGlobalBounds().width, currentPlayer.BulletAt(j).getPosition().y));
					}
					break;
				}
			}
		}
	}
}

void Game::UpdateScoreUI() {
	this->scoreText.setString(
		"Score: " + std::to_string(this->totalScore)
		+ "\nPerfection Score Mult: x" + std::to_string(killPerfectionMultiplier)
		+ "\nPerfect Kills / Next: " + std::to_string(this->killPerfectionAdder) + " / " + std::to_string(this->killPerfectionAdderMax)
		+ "\nKill Mult: x" + std::to_string(killboxMultiplier)
		+ "\nKillbox Kills / Next: " + std::to_string(this->killboxAdder) + " / " + std::to_string(this->killboxAdderMax)
		+ "\nKillbox Seconds Remaining: " + std::to_string((int)this->killboxTimer) + "s"
		+ "\nGame time: " + std::to_string((int)this->scoreTimer.getElapsedTime().asSeconds())
		+ "\nDifficulty: " + std::to_string(this->difficulty)
		+ "\nBest Score/Second: " + std::to_string(this->bestScorePerSecond));
}

void Game::UpdateEnemies(const float &dt) {
	// Spawn enemies
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
		this->_spawnEnemy();
		this->enemySpawnTimer = 0;
	}

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
		if (currentEnemy->getPosition().x < 0 - currentEnemy->getGlobalBounds().width) {
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

void Game::DrawEnemyUI() {
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
}

void Game::DrawPlayers() {
	for (size_t i = 0; i < this->players.Size(); ++i) {
		this->players[i].Draw(*this->window);
	}
}

void Game::DrawMap() {
	for (size_t i = 0; i < this->tiles.Size(); i++)
	{
		this->tiles[i].Draw(*this->window);
	}
}

void Game::DrawConsumables() {
	for (size_t i = 0; i < this->consumables.Size(); ++i) {
		Consumable *currentItem = this->consumables[i];
		currentItem->Draw(*this->window);
	}
}

void Game::Draw() {
	this->window->clear();

	// Draw Map
	this->DrawMap();

	// Draw Players
	this->DrawPlayers();

	// Draw Enemy Lifeform
	this->DrawEnemyUI();

	// Draw Consumables
	this->DrawConsumables();

	// Draw UI
	this->DrawUI();

	// Draw everything to the window
	this->window->display();
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

	this->gameOverText.setString("Game Over (X___X)\nScore: " +
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
		this->enemyLifeforms.Add(new MoveLeftShootEnemy(this->window, this->enemyTextures, this->enemyBulletTextures, this->window->getSize(), this->players[pNum].getLevel(), pNum));
		break;
	}

}