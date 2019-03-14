#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;

	Vector2f playerCenter;

	int shootTimer;
	int shootTimerMax = 25;
	int damageTimer;
	int damageTimerMax = 5;

	Texture *texture;
	Sprite sprite;
	RectangleShape hitBox;

	Texture *bulletTexture;
	std::vector<Bullet> bullets;
	float bulletSpeed = 5.f;
	float bulletMaxSpeed = 25.f;
	float bulletAcceleration = 0.5f;

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
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline const String getHpAsString() const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline const FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

	// Functions
	void Combat();
	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update(Vector2u windowBounds);

	// Statics
	static unsigned playerId;
};

