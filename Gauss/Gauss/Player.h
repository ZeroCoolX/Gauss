#pragma once

#include "Bullet.h"

class Player
{
private:
	// INPUT
	float keyTimeMax;
	float keyTime;

	unsigned int playerNumber;

	// POSITIONING
	Vector2f playerCenter;

	// UI/HUD
	Text statsText;
	// EXP
	RectangleShape playerExpBar;
	RectangleShape playerExpBarBox;
	// HEALTH
	RectangleShape playerHealthBar;
	RectangleShape playerHealthBarBox;
	// STATS HUD
	Text playerStatsHudText;
	RectangleShape playerStatsTextBox;
	// GAUSS CHARGE
	RectangleShape playerGaussBar;
	Color gaussChargingColor; //Color(145, 49, 125, 200)
	Color gaussReadyColor; //Color(250, 17, 200, 200)
	// SHIELD CHARGE
	CircleShape playerShieldChargeCircle;
	CircleShape playerShieldChargeCircleBorder;
	Color shieldChargingColor;
	Color shieldReadyColor;
	// POWERUPS
	Sprite powerupSprite;


	// TIMERS
	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;
	float gaussChargeTimer;
	float gaussChargeTimerMax;
	float shieldChargeTimerMax;
	float shieldChargeTimer;
	float powerupTimerMax;
	float powerupTimer;

	Sprite sprite;
	RectangleShape hitBox;

	// Accessories
	Sprite mainGunSprite;
	float mainGunKickback = 40.f;
	float mainGunReturnSpeed = 5.f;

	dArr<Bullet> bullets;

	Vector2f missileScale = Vector2f(0.05f, 0.05f);
	Vector2f laserBulletScale = Vector2f(0.2f, 0.2f);

	//Texture *gaussCannonProjectileTexture;
	Vector2f gaussCannonProjectileScale = Vector2f(0.5f, 0.5f);

	float bulletSpeed;
	float bulletMaxSpeed;
	float bulletAcceleration;

	Sprite deflectorShield;
	bool shieldActive;
	float shieldAdded;

	// Accessories
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

	// Upgrades
	dArr<int> upgradesAcquired;
	int mainGunLevel;
	bool piercingShot;
	bool sheild;
	bool dualMissiles01;
	bool dualMissiles02;
	
	// Powerups
	bool powerupRF;
	bool powerupXP;

	// Utility Functions
	void _processPlayerInput(const float &dt);
	void _initTextures();
	void _initPlayerSettings();
	void _recalculatePlayerCenter();
	void _fireLaser(const Vector2f direction);
	void _fireGaussCannon(const Vector2f direction);
	void _fireMissileLight(const Vector2f direction);
	void _fireMissileHeavy(const Vector2f direction);
	void _checkBounds(View &view, bool warpVertical = false);


public:
	// Allows for the player to be created with a custom keybinding to represent WASD/Fire
	Player(
		int UP = Keyboard::W, 
		int DOWN = Keyboard::S,
		int LEFT = Keyboard::A, 
		int RIGHT = Keyboard::D,
		int FIRE = Keyboard::Space,
		int GAUSSCANNON = Keyboard::Q,
		int SHIELD = Keyboard::RAlt,
		int TOGGLESTATS = Keyboard::Tab,
		int CHANGE_LWING = Keyboard::Num1,
		int CHANGE_CPIT = Keyboard::Num2,
		int CHANGE_RWING = Keyboard::Num3,
		int CHANGE_AURA = Keyboard::Num4);
	virtual ~Player();

	// Accessories
	inline const unsigned getBulletsSize() const { return this->bullets.Size(); }
	inline dArr<Bullet>* getBullets() { return &this->bullets; }
	inline const bool shieldCharged() const { return this->shieldChargeTimer >= this->shieldChargeTimerMax; }
	inline const bool isShielding() const { return this->shieldActive; }
	inline Sprite getDeflectorShield() { return this->deflectorShield; }
	inline void upgradeShield() {
		this->shieldAdded += 33.f;
		this->UpdateStats();
		this->shieldChargeTimer = this->shieldChargeTimerMax;
	}

	// Powerups
	inline void enablePowerupRF() { 
		this->powerupRF = true; 
		this->powerupSprite.setTexture(Player::powerupIndicatorTextures[0]);
		this->setupPowerupSprite();
	}
	inline void enablePowerupXP() { 
		this->powerupXP = true;
		this->powerupSprite.setTexture(Player::powerupIndicatorTextures[1]);
		this->setupPowerupSprite();
	}
	inline void setupPowerupSprite() {
		this->powerupTimer = this->powerupTimerMax;
		this->powerupSprite.setScale(0.3f, 0.3f);
		this->powerupSprite.setOrigin(this->powerupSprite.getGlobalBounds().width / 2, this->powerupSprite.getGlobalBounds().height / 2);
	}
	inline bool getPowerupRF() const { return this->powerupRF; }
	inline bool getPowerupXP() const { return this->powerupXP; }
	inline float getCalculatedShootTimer() const { return this->powerupRF ? 10.f : 20.f; }

	// Positional
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline void resetVelocity() { this->velocity = Vector2f(0.f, 0.f); }
	inline void move(float x, float y) { this->sprite.move(Vector2f(x, y)); this->mainGunSprite.move(Vector2f(x, y)); }
	inline const Vector2f& getNormDir() const { return this->normalizedDir; }
	inline const bool collidesWith(FloatRect other) const { return this->sprite.getGlobalBounds().intersects(other); }

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
	inline bool hasUpgrade(int type) {
		for (size_t i = 0; i < this->upgradesAcquired.Size(); i++)
		{
			if (type == this->upgradesAcquired[i]) {
				return true;
			}
		}
		return false;
	}

	// Damage/Attack
	int getDamage() const;
	inline bool isDamageCooldown() { return this->damageTimer < this->damageTimerMax; }
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
	void UpdateAccessories(const float &dt, const float scollSpeed);
	void UpdatePowerups(const float &dt);
	void Combat(const float &dt);
	void Movement(const float &dt, View &view, const float scrollSpeed);
	void Draw(RenderTarget &renderTarget);
	void DrawUI(RenderTarget &renderTarget);
	void UpdateStatsUI();
	void Update(View &view, const float &dt, const float scrollSpeed);
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
	// Textures
	static dArr<Texture> shipBodyTextures;
	static dArr<Texture> shipMainGunTextures;
	static dArr<Texture> shipLWingTextures;
	static dArr<Texture> shipRWingTextures;
	static dArr<Texture> shipCockpitTextures;
	static dArr<Texture> shipAuraTextures;
	static dArr<Texture> shipShieldTextures;
	static dArr<Texture> powerupIndicatorTextures;

	static void InitTextures();

	enum Weapon { 
		LASER_GUN = 0, 
		MISSILE_LIGHT_GUN, 
		MISSILE_HEAVY_GUN 
	};
	enum Controls { 
		CONTROL_UP = 0, 
		CONTROL_DOWN, 
		CONTROL_LEFT, 
		CONTROL_RIGHT, 
		CONTROL_FIRE, 
		CONTROL_GAUSSCANNON, 
		CONTROL_SHIELD, 
		CONTROL_TOGGLE_STATS, 
		CONTROL_CHANGE_LWING, 
		CONTROL_CHANGE_CPIT, 
		CONTROL_CHANGE_RWING, 
		CONTROL_CHANGE_AURA 
	};
	enum ShipBody { 
		DEFAULT_SHIP_BODY = 0 
	};

	enum LaserLevels {
		DEFAULT_LASER = 0, 
		LEVEL_2_LASER, 
		LEVEL_3_LASER
	};
	enum Sheild {
		DEFAULT_SHIELD = 0
	};
};

