#include "Game.h"
#include "Enums.h"

Game::Game(RenderWindow *window)
{
	this->window = window;
	this->dtMultiplier = 62.5f;

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
	this->players.push_back(Player(this->textureMap));
	//this->players.push_back(Player(this->textureMap, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift));

	this->_spawnEnemy();
	this->enemySpawnTimerMax = 25;
	this->enemySpawnTimer = this->enemySpawnTimerMax;

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setFillColor(Color::Red);
	this->gameOverText.setString("Game Over! (X___X)");
	this->gameOverText.setPosition((float)this->window->getSize().x / 4, (float)this->window->getSize().y / 2);

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

	for (size_t i = 0; i < this->players.size(); i++)
	{
		// Follow Text init
		this->players[i].InitStatsText(tempText);

		// Static Text init
		tempText.setString("");
		this->staticPlayerTexts.push_back(tempText);
	}

	// Enemy Text
	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::White);
}

void Game::UpdateUI() {
	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		//offset * player num
	}
}


void Game::Update(const float &dt) {
	
	if (this->playersExistInWorld()) {
		// Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax) { this->enemySpawnTimer += 1.f * dt * this->dtMultiplier; } // 1.f is not needed here

		// Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->_spawnEnemy();
			this->enemySpawnTimer = 0;
		}

		for (size_t i = 0; i < this->players.size(); ++i) {

			// Players update
			this->players[i].Update(this->window->getSize(), dt);

			// Bullets update
			for (size_t j = 0; j < this->players[i].getBullets().size(); j++)
			{
				this->players[i].getBullets()[j].Update(dt);

				// Bullet Window bounds check
				if (this->players[i].getBullets()[j].getPosition().x > this->window->getSize().x) {
					this->players[i].getBullets().erase(this->players[i].getBullets().begin() + j);
				}
				else {
					// Enemy - Bullet Collision check since it still exists in the world
					for (size_t k = 0; k < this->enemies.size(); k++)
					{
						if (this->players[i].getBullets()[j].getGlobalBounds().intersects(this->enemies[k].getGlobalBounds())) {
								
							// Health check for damage or destruction
							this->enemies[k].TakeDamage(this->players[i].getDamage());

							if (this->enemies[k].getHp() <= 0) {
								this->enemies.erase(this->enemies.begin() + k);
							}

							// Destroy the bullet regardless
							this->players[i].getBullets().erase(this->players[i].getBullets().begin() + j);
							break;
						}
					}
				}
			}
		}
		// Update Enemy Movement
		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt);

			// Enemy Window Bounds check
			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width) {
				this->enemies.erase(this->enemies.begin() + i);
			}
			else {
				// Check Player - Enemy collision
				for (size_t j = 0; j < this->players.size(); j++)
				{
					if (this->players[j].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds())) {
						// The amount of damage the player takes is relative to how much health the enemy has.
						// A fully healed enemy will produce maximum amount of damage - whereas an enemy that is barely clinging to life will only damage the player a little		
						this->players[j].takeDamage(this->enemies[i].getHp());
						this->enemies.erase(this->enemies.begin() + i);

						// Check for death
						if (players[j].isDead()) {
							this->players.erase(this->players.begin() + j);
						}
						break;
					}
				}
			}
		}

		//UI update
		this->UpdateUI();
	}
}

void Game::DrawUI() {
	if (!this->playersExistInWorld()) {
		this->window->draw(this->gameOverText);
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		//offset * player num
		this->window->draw(this->staticPlayerTexts[i]);
	}
}

void Game::Draw(){
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); ++i) {
		this->players[i].Draw(*this->window);
	}

	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemyText.setPosition(this->enemies[i].getPosition());
		this->enemyText.setString(std::to_string(this->enemies[i].getHp()) + "/" + std::to_string(this->enemies[i].getHpMax()));

		this->enemies[i].Draw(*this->window);
		this->window->draw(this->enemyText);
	}

	this->DrawUI();
	this->window->display();
}

void Game::_spawnEnemy() {
	this->enemies.push_back(Enemy(&this->textureMap[GameEnums::T_ENEMY01],
		GameEnums::E_MOVE_LEFT,
		this->window->getSize(),
		this->enemyScale,
		this->enemyDirection,
		this->enemyHp,
		this->enemyDamageRange));
}
