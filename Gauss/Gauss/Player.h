#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;

	Texture *texture;
	Sprite sprite;
	RectangleShape hitBox;

	Texture *bulletTexture;
	std::vector<Bullet> bullets;

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
	// Allows for the player to be created with a custom keybinding to represent WASD/Fire
	Player(Texture *texture, Texture *bulletTexture,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space);
	virtual ~Player();

	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update();

	// Statics
	static unsigned playerId;
};

