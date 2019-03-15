#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;

	Vector2f playerCenter;

	int shootTimer;
	int shootTimerMax;
	int damageTimer;
	int damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	// Accessories
	Sprite mainGunSprite;
	const float mainGunKickback = 25.f;

	Texture *bulletTexture;
	std::vector<Bullet> bullets;
	float bulletSpeed;
	float bulletMaxSpeed;
	float bulletAcceleration;

	int controls[5];

	// Movement
	Vector2f velocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	float stabalizingForce;

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
	Player(std::vector<Texture> &textureMap,
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
	void UpdateAccessories();
	void Combat();
	void Movement();
	void Draw(RenderTarget &renderTarget);
	void Update(Vector2u windowBounds);

	// Statics
	static unsigned playerId;
};

