#pragma once

#include "Bullet.h"

class Player
{
private:
	unsigned int playerNumber;
	Text statsText;
	RectangleShape playerExpBar;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	// Accessories
	Sprite mainGunSprite;
	float mainGunKickback = 30.f;
	float mainGunReturnSpeed = 2.f;

	dArr<Bullet> bullets;

	Texture *missile01ProjectileTexture;
	Texture *missile02ProjectileTexture;
	Vector2f missileScale = Vector2f(0.05f, 0.05f);

	Texture *laserProjectileTexture;
	Vector2f laserBulletScale = Vector2f(0.2f, 0.2f);

	float bulletSpeed;
	float bulletMaxSpeed;
	float bulletAcceleration;

	// Accessories
	dArr<Texture>* lWingTextureMap;
	dArr<Texture>* rWingTextureMap;
	dArr<Texture>* auraTextureMap;
	dArr<Texture>* cPitTextureMap;
	Sprite lWing;
	Sprite rWing;
	Sprite aura;
	Sprite cPit;

	int lWingSelect;
	int rwingSelect;
	int cPitSelect;
	int auraSelect;

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
	int statPoints;

	int cooling;				// shot rate
	int plating;				// armour
	int power;					// strength
	int maneuverability;		// agility


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
	void _processPlayerInput(const float &dt);
	void _initTextures(std::vector <Texture> &textureMap);
	void _initPlayerSettings();
	void _recalculatePlayerCenter();
	void _fireLaser(const Vector2f direction);
	void _fireMissileLight(const Vector2f direction);
	void _fireMissileHeavy(const Vector2f direction);

public:
	// Allows for the player to be created with a custom keybinding to represent WASD/Fire
	Player(std::vector<Texture> &textureMap,
		dArr<Texture> &lWingTextureMap,
		dArr<Texture> &rWingTextureMap,
		dArr<Texture> &auraTextureMap,
		dArr<Texture> &cPitTextureMap,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space);
	virtual ~Player();

	// Accessors
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline const unsigned getBulletsSize() const { return this->bullets.Size(); }
	inline const String getHpAsString() const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	int getDamage() const;
	inline const int& getHp() const { return hp; }
	inline void takeDamage(int damage) { this->hp = std::max(0, (this->hp - damage)); }
	inline const bool isDead() const { return this->hp <= 0; }
	inline const int& getLevel() const { return this->level; }
	inline const int& getExp() const { return this->exp; }
	inline const int& getExpNext() const { return this->expNext; }
	inline bool gainExp(int exp) { this->exp += exp; return this->UpdateLeveling(); }

	// Functions
	bool UpdateLeveling();
	void UpdateAccessories(const float &dt);
	void Combat(const float &dt);
	void Movement(const float &dt);
	void Draw(RenderTarget &renderTarget);
	void DrawUI(RenderTarget &renderTarget);
	void UpdateStatsUI();
	void Update(Vector2u windowBounds, const float &dt);
	void InitUI(Text t);
	Bullet& BulletAt(unsigned index);
	void RemoveBullet(unsigned index);


	// Statics
	static unsigned playerId;
};

