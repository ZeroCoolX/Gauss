#pragma once

#include "Player.h"

class Game
{
private:
	RenderWindow *window;

	std::vector<Player> players;
	Texture playerTexture;
	Texture bulletTexture;

public:
	Game(RenderWindow *window);
	virtual ~Game();
	
	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	// Setters

	// Functions
	void Update();
	void Draw();
};

