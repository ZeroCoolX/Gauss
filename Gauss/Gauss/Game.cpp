#include "Game.h"



Game::Game(RenderWindow *window)
{
	this->window = window;
	this->window->setFramerateLimit(60);

	// Init textures
	playerTexture.loadFromFile("Textures/ship.png");

	// Init player
	player = new Player(&this->playerTexture/*, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::LShift*/);
}

Game::~Game()
{
	// Cleanup due to new()
	delete player;
}

void Game::Update() {
	player->Update();
}

void Game::Draw(){
	window->clear();

	player->Draw(*window);

	window->display();
}
