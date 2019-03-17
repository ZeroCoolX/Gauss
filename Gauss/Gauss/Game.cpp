#include "Game.h"
#include "Enums.h"

Game::Game(RenderWindow *window)
{
	this->window = window;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	// Init textures
	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_SHIP].loadFromFile("Textures/ship.png");

	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_LASER01].loadFromFile("Textures/Guns/rayTex01.png");

	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_MISSILE01].loadFromFile("Textures/Guns/missileTex01.png");

	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_MAIN_GUN].loadFromFile("Textures/Guns/gun01.png");

	this->textureMap.push_back(Texture());
	this->textureMap[GameEnums::T_ENEMY01].loadFromFile("Textures/enemy.png");

	// Init player
	this->players.Add(Player(this->textureMap));
	//this->players.Add(Player(this->textureMap, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift));

	this->_spawnEnemy();
	this->enemySpawnTimerMax = 25;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->InitUI();
}

Game::~Game()
{
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
}

void Game::Update(const float &dt) {
	
	if (this->playersExistInWorld()) {
		// Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax) { this->enemySpawnTimer += 1.f * dt * DeltaTime::dtMultiplier; } // 1.f is not needed here

		// Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->_spawnEnemy();
			this->enemySpawnTimer = 0;
		}

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

							this->_generateTextTag("-" + std::to_string(damage), Color::Yellow, Vector2f(
								this->enemies[k].getPosition().x + this->enemies[k].getGlobalBounds().width / 4,
								this->enemies[k].getPosition().y - this->enemies[k].getGlobalBounds().height / 2),
								24, 15.f);

							this->enemies[k].TakeDamage(damage);

							if (this->enemies[k].getHp() <= 0) {

								// Player earned some EXP!
								int exp = this->enemies[k].getHpMax()
									+ (rand() % this->enemies[k].getHpMax() + 1);

								// Player leveled up!
								if (this->players[i].gainExp(exp)) {
									this->_generateTextTag("LEVEL UP!", Color::White, Vector2f(
										this->players[i].getPosition().x + this->players[i].getGlobalBounds().width / 4,
										this->players[i].getPosition().y - this->players[i].getGlobalBounds().height / 2),
										30, 25.f);
								}else{
									this->_generateTextTag("+" + std::to_string(exp) + " EXP", Color::Cyan, Vector2f(
										this->players[i].getPosition().x,
										this->players[i].getPosition().y - this->players[i].getGlobalBounds().height / 2),
										20, 20.f);
								}
								this->enemies.Remove(k);
							}

							// Destroy the bullet regardless
							this->players[i].RemoveBullet(j);
							break;
						}
					}
				}
			}
		}
		// Update Enemy Movement
		for (size_t i = 0; i < this->enemies.Size(); i++)
		{
			this->enemies[i].Update(dt);

			// Enemy Window Bounds check
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width) {
				this->enemies.Remove(i);
			}
			else {
				// Check Player - Enemy collision
				for (size_t j = 0; j < this->players.Size(); j++)
				{
					if (this->players[j].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())) {
						// Damage player		
						int damage = this->enemies[i].getDamage();
						this->players[j].takeDamage(damage);
						
						// Create Texttag effect
						this->_generateTextTag("-" + std::to_string(damage), Color::Red,Vector2f(
							this->players[j].getPosition().x + this->players[j].getGlobalBounds().width / 4,
							this->players[j].getPosition().y - this->players[j].getGlobalBounds().height / 2),
							28, 20.f);

						// Destroy enemy
						this->enemies.Remove(i);

						// Check for player death
						if (players[j].isDead()) {
							this->players.Remove(j);
						}
						break;
					}
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
	}
}

void Game::DrawUI() {
	// Draw enemies
	for (size_t i = 0; i < this->enemies.Size(); i++)
	{
		this->enemyText.setPosition(this->enemies[i].getPosition());
		this->enemyText.setString(std::to_string(this->enemies[i].getHp()) + "/" + std::to_string(this->enemies[i].getHpMax()));

		// Draw Enemy
		this->enemies[i].Draw(*this->window);
		// Draw Enemy UI
		this->window->draw(this->enemyText);
	}

	// Draw Texttags
	for (size_t i = 0; i < this->textTags.Size(); i++)
	{
		this->textTags[i].Draw(*this->window);
	}

	// Draw Game Over Text - if needed
	if (!this->playersExistInWorld()) {
		this->window->draw(this->gameOverText);
	}
}

void Game::Draw(){
	this->window->clear();

	// Draw Players
	for (size_t i = 0; i < this->players.Size(); ++i) {
		this->players[i].Draw(*this->window);
	}

	this->DrawUI();

	this->window->display();
}

void Game::_spawnEnemy() {
	this->enemies.Add(Enemy(&this->textureMap[GameEnums::T_ENEMY01],
		GameEnums::E_MOVE_LEFT,
		this->window->getSize(),
		this->enemyScale,
		this->enemyDirection,
		this->enemyHp,
		this->enemyDamageRange));
}

void Game::_generateTextTag(std::string text, Color color, Vector2f position, unsigned int charSize, float showTime) {
	// Create Texttag effect
	this->textTags.Add(
		TextTag(
			&this->font,
			text,
			color,
			position,
			charSize, showTime)
	);
}