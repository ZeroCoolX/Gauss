#pragma once

#include "Player.h"

class Game
{
private:
	RenderWindow *window;

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

