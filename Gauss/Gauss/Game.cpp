#include "Game.h"



Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	// Init textures
	this->playerTexture.loadFromFile("Textures/ship.png");
	this->bulletTexture.loadFromFile("Textures/missileTex01.png");

	// Init player
	this->players.push_back(Player(&this->playerTexture, &this->bulletTexture));
	//this->players.push_back(Player(&this->playerTexture, &this->bulletTexture, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::RShift));
}

Game::~Game()
{
}

void Game::CombatUpdate() {
}

void Game::Update() {
	for (size_t i = 0; i < this->players.size(); ++i) {
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
}

void Game::Draw(){
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); ++i) {
		players[i].Draw(*this->window);
	}

	this->window->display();
}
