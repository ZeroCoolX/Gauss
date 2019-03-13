#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>

using namespace sf;

class Player
{
private:
	Texture *texture;
	Sprite shape;
	RectangleShape hitBox;

	int level;
	int exp;
	int expNext;

	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

public:
	Player();
	virtual ~Player();

	void Draw();
	void Update();
};

