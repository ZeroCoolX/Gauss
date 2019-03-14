#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;

	int shootTimer;
	int shootTimerMax;
	int damageTimer;
	int damageTimerMax;

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

	void processPlayerInput();

public:
	// Allows for the player to be created with a custom keybinding to represent WASD/Fire
	Player(Texture *texture, Texture *bulletTexture,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space);
	virtual ~Player();

	// Accessors
	inline std::vector<Bullet>& getBullets() { return this->bullets; }

	// Functions
	void Combat();
	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update(Vector2u windowBounds);

	// Statics
	static unsigned playerId;
};

