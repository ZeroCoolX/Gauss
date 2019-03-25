#pragma once

#include "Bullet.h"

class Player
{
private:
	// Stops keyspamming
	float keyTimeMax;
	float keyTime;

	unsigned int playerNumber;
	Text statsText;
	RectangleShape playerExpBar;
	RectangleShape playerHealthBar;
	RectangleShape playerExpBarBox;
	RectangleShape playerHealthBarBox;

	Text playerStatsHudText;
	RectangleShape playerStatsTextBox;

	Color gaussChargingColor;
	Color gaussReadyColor;
	CircleShape playerGaussChargeCircle;
	CircleShape playerGaussChargeCircleBorder;

	Vector2f playerCenter;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;

	Sprite sprite;
	RectangleShape hitBox;

	// Accessories
	Sprite mainGunSprite;
	float mainGunKickback = 30.f; // TODO: checkout what 40 looks like
	float mainGunReturnSpeed = 2.f; // TODO: maybe change to 5 - see what that looks like

	dArr<Bullet> bullets;
	dArr<Texture> *mainGunTextureMap;

	Texture *missile01ProjectileTexture;
	Texture *missile02ProjectileTexture;
	Vector2f missileScale = Vector2f(0.05f, 0.05f);

	Texture *laserProjectileTexture;
	Vector2f laserBulletScale = Vector2f(0.2f, 0.2f);

	Texture *gaussCannonProjectileTexture;
	Vector2f gaussCannonProjectileScale = Vector2f(0.5f, 0.5f);

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
	int rWingSelect;
	int cPitSelect;
	int auraSelect;

	// Player input controls
	dArr<int> controls;

	// Movement
	Vector2f velocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	Vector2f normalizedDir;
	float stabalizingForce;

	int level;
	int exp;
	int expNext;
	int statPoints;

	int cooling;				// shot rate
	int plating;				// armour
	int power;					// strength
	int maneuverability;		// agility


	int hp;//--//
	int hpMax;//--//
	int hpAdded;

	int damage;
	int damageMax;

	int score;

	int currentWeapon;

	int gaussCharge;
	int gaussChargeMax;
	float gaussChargeTimer;
	float gaussChargeTimerMax;

	// Upgrades
	dArr<int> upgradesAcquired;
	int mainGunLevel;
	bool piercingShot;
	bool sheild;
	bool dualMissiles01;
	bool dualMissiles02;

	// Utility Functions
	void _processPlayerInput(const float &dt);
	void _initTextures(std::vector <Texture> &textureMap);
	void _initPlayerSettings();
	void _recalculatePlayerCenter();
	void _fireLaser(const Vector2f direction);
	void _fireGaussCannon(const Vector2f direction);
	void _fireMissileLight(const Vector2f direction);
	void _fireMissileHeavy(const Vector2f direction);
	void _checkBounds(Vector2u windowBounds, bool warpVertical);


public:
	// Allows for the player to be created with a custom keybinding to represent WASD/Fire
	Player(std::vector<Texture> &textureMap,
		dArr<Texture> &mainGunTextureMap,
		dArr<Texture> &lWingTextureMap,
		dArr<Texture> &rWingTextureMap,
		dArr<Texture> &auraTextureMap,
		dArr<Texture> &cPitTextureMap,
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space,
		int GAUSSCANNON = Keyboard::Q,
		int TOGGLESTATS = Keyboard::Tab,
		int CHANGE_LWING = Keyboard::Num1,
		int CHANGE_CPIT = Keyboard::Num2,
		int CHANGE_RWING = Keyboard::Num3,
		int CHANGE_AURA = Keyboard::Num4);
	virtual ~Player();

	// Accessors
	// Weapons
	inline const unsigned getBulletsSize() const { return this->bullets.Size(); }
	inline const int& getGaussCharge() const { return this->gaussCharge; }
	inline const int& getGaussChargeMax() const { return this->gaussChargeMax; }
	inline void gainGaussCharge(int charge) { this->gaussCharge = std::min(this->gaussCharge + charge, this->gaussChargeMax); }

	// Positional
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline void resetVelocity() { this->velocity = Vector2f(0.f, 0.f); }
	inline void move(float x, float y) { this->sprite.move(Vector2f(x, y)); this->mainGunSprite.move(Vector2f(x, y)); }
	inline const Vector2f& getNormDir() const { return this->normalizedDir; }

	// Life
	inline const bool isDead() const { return this->hp <= 0; }
	inline bool gainHp(int hp) { this->hp = std::min(this->hp + hp, this->hpMax); return this->hp < this->hpMax; }
	inline const int& getHp() const { return hp; }
	inline const int& getHpMax() const { return hpMax; }
	inline const String getHpAsString() const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline void upgradeHP() {
		this->hpAdded += 10;
		this->UpdateStats();
		this->hp = this->hpMax;
	}

	// Experience
	inline const int& getLevel() const { return this->level; }
	inline const int& getExp() const { return this->exp; }
	inline const int& getExpNext() const { return this->expNext; }
	inline bool gainExp(int exp) { this->exp += exp; return this->UpdateLeveling(); }
	inline void gainScore(int score) { this->score += score; }
	inline const int getScore() const { return this->score; }
	inline const int& getGunLevel() const { return this->mainGunLevel; }
	inline const void addStatPoint() { this->statPoints++; }
	inline dArr<int>& getAcquiredUpgrades() { return this->upgradesAcquired; }

	// Damage/Attack
	int getDamage() const;
	bool isDamageCooldown() { return this->damageTimer < this->damageTimerMax; }
	inline bool getPiercingShot() const { return this->piercingShot; }
	inline void enablePiercingShot() { this->piercingShot = true; }
	inline void enableSheild() { this->sheild = true; }
	inline void enableDualMissile01() { this->dualMissiles01 = true; }
	inline void enableDualMissile02() { this->dualMissiles02 = true; }

	// Vector Utility
	inline float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	inline Vector2f normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}



	// Functions
	void Reset();
	bool UpdateLeveling();
	bool ChangeAccessories(const float &dt);
	void UpdateAccessories(const float &dt);
	void Combat(const float &dt);
	void Movement(const float &dt, Vector2u windowBounds);
	void Draw(RenderTarget &renderTarget);
	void DrawUI(RenderTarget &renderTarget);
	void UpdateStatsUI();
	void Update(Vector2u windowBounds, const float &dt);
	void UpdateStats();
	void InitUI(Text t);
	Bullet& BulletAt(unsigned index);
	void RemoveBullet(unsigned index);
	void TakeDamage(int damage);
	void SetGunLevel(int gunLevel);
	void AddStatPointRandom();
	bool PlayerShowStatsIsPressed();
	std::string GetStatsAsString();

	// Statics
	static unsigned playerId;
};

