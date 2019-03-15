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
	this->players.push_back(Player(this->textureMap));
	//this->players.push_back(Player(this->textureMap, Keyboard::Numpad8, Keyboard::Numpad5, Keyboard::Numpad4, Keyboard::Numpad6, Keyboard::RShift));

	// Init enemy
	this->enemies.push_back(Enemy(&this->textureMap[GameEnums::T_ENEMY01],
		GameEnums::E_MOVE_LEFT,
		this->window->getSize(),
		this->enemyScale,
		this->enemyDirection,
		this->enemyHp,
		this->enemyDamageRange));

	this->enemySpawnTimerMax = 100;
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

	for (size_t i = 0; i < this->players.size(); i++)
	{
		// Follow Text init
		//tempText.setString(std::to_string(i));
		this->followPlayerTexts.push_back(tempText);

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
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		Vector2f fixedPos = Vector2f(this->players[i].getPosition().x, (this->players[i].getPosition().y + this->players[i].getGlobalBounds().height));
		this->followPlayerTexts[i].setPosition(fixedPos);
		this->followPlayerTexts[i].setString("[" + std::to_string(i+1) + "]					" + this->players[i].getHpAsString());
	}

	for (size_t i = 0; i < this->staticPlayerTexts.size(); i++)
	{
		//offset * player num
	}
}


void Game::Update(float dt) {
	// Update timers
	if (this->enemySpawnTimer < this->enemySpawnTimerMax) { ++this->enemySpawnTimer; }

	// Spawn enemies
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
		this->enemies.push_back(Enemy(&this->textureMap[GameEnums::T_ENEMY01],
			GameEnums::E_MOVE_LEFT,
			this->window->getSize(),
			this->enemyScale,
			this->enemyDirection,
			this->enemyHp,
			this->enemyDamageRange));

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
					this->players[j].TakeDamage(this->enemies[i].getHp());
					this->enemies.erase(this->enemies.begin() + i);

					break;
				}
			}
		}
	}

	//UI update
	this->UpdateUI();
}

void Game::DrawUI() {
	for (size_t i = 0; i < this->followPlayerTexts.size(); i++)
	{
		this->window->draw(this->followPlayerTexts[i]);
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
