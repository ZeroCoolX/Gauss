#include "Game.h"



Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	// Init textures
	this->playerTexture.loadFromFile("Textures/ship.png");
	this->bulletTexture.loadFromFile("Textures/missileTex01.png");

	// Init player
	this->players.push_back(Player(&this->playerTexture, &this->bulletTexture));
	this->players.push_back(Player(&this->playerTexture, &this->bulletTexture, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift));

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


void Game::Update() {
	for (size_t i = 0; i < this->players.size(); ++i) {
		
		// Players update
		this->players[i].Update(this->window->getSize());

		// Bullets update
		for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
		{
			this->players[i].getBullets()[k].Update();

			// Window bounds check
			if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x) {
				this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
				break;
			}

			// Enemy collision check
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

	this->DrawUI();
	this->window->display();
}
