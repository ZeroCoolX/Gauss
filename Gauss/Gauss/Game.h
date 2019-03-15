#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
	RenderWindow *window;

	// Text
	Font font;
	std::vector<Text> followPlayerTexts;
	std::vector<Text> staticPlayerTexts;

	// Players
	std::vector<Player> players;

	// Enemies
	std::vector<Enemy> enemies;

	// Textures
	std::vector<Texture> textureMap;

public:
	Game(RenderWindow *window);
	virtual ~Game();
	
	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	// Setters

	// Functions
	void InitUI();
	void UpdateUI();
	void Update();
	void DrawUI();
	void Draw();
};

