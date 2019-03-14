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
}

Game::~Game()
{
}

void Game::Update() {
	for (size_t i = 0; i < this->players.size(); ++i) {
		this->players[i].Update();
	}
}

void Game::Draw(){
	this->window->clear();

	for (size_t i = 0; i < this->players.size(); ++i) {
		players[i].Draw(*this->window);
	}

	this->window->display();
}
