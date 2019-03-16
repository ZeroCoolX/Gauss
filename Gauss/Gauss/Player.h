#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;
	Text statsText;

	Vector2f playerCenter;

	int shootTimer;
	int shootTimerMax;
	int damageTimer;
	int damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	// Accessories
	Sprite mainGunSprite;
	float mainGunKickback = 30.f;
	float mainGunReturnSpeed = 2.f;

	std::vector<Bullet> bullets;

	Texture *missile01ProjectileTexture;
	Texture *missile02ProjectileTexture;
	Vector2f missileScale = Vector2f(0.05f, 0.05f);

	Texture *laserProjectileTexture;
	Vector2f laserBulletScale = Vector2f(0.2f, 0.2f);

	float bulletSpeed;
	float bulletMaxSpeed;
	float bulletAcceleration;

	// Player input controls
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

	int currentWeapon;

	// Upgrades
	int mainGunLevel;
	bool dualMissiles01;
	bool dualMissiles02;

	// Utility Functions
	void _processPlayerInput();
	void _initTextures(std::vector <Texture> &textureMap);
	void _initPlayerSettings();
	void _recalculatePlayerCenter();
	void _fireLaser(const Vector2f direction);
	void _fireMissileLight(const Vector2f direction);
	void _fireMissileHeavy(const Vector2f direction);

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
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline const int getDamage() const { return rand() % this->damageMax + this->damage; }
	inline const int getHp() const { return hp; }
	inline void takeDamage(int damage) { this->hp = std::max(0, (this->hp - damage)); }
	inline const bool isDead() const { return this->hp <= 0; }

	// Functions
	void UpdateAccessories(float dt);
	void Combat(float dt);
	void Movement(float dt);
	void Draw(RenderTarget &renderTarget);
	void DrawStatsUI(RenderTarget &renderTarget);
	void UpdateStatsUI();
	void Update(Vector2u windowBounds, float dt);
	void InitStatsText(Text t);


	// Statics
	static unsigned playerId;
};

