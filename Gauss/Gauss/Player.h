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
	Sprite sprite;
	RectangleShape hitBox;

	int controls[5];

	int level;
	int exp;
	int expNext;

	int hp;
	int hpMax;

	int damage;
	int damageMax;

	int score;

public:
	Player(Texture *texture,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space);
	virtual ~Player();

	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update();
};

