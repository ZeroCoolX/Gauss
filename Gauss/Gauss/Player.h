#pragma once

#include "Bullet.h"
#include "AudioManager.h"
//#include "Powerup.h"

class Player
{
public:
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

	enum Auras {
		ABSORB = 0,
		GRIND
	};

	enum CosmoEffects {
		INVERT_CONTROLS_EFFECT = 0,
		VERTICAL_WARP_EFFECT,
		SPEED_INCREASE_EFFECT,
		SPEED_DECREASE_EFFECT,
		SWAP_BADDIE_TYPES_EFFECT,
		VISUAL_DISTORTION_EFFECT
	};

private:
	// Cosmo specific properties
	bool effectedByCosmo;
	bool invertControlsEffectActive;
	CosmoEffects currentCosmoEffect;
	float cosmoEffectTimer;
	float cosmoEffectTimerMax;

	AudioManager* audioManager;

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
	bool gaussReloaded;
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
	float shieldRechargeRate = 0.5f;

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
	
	// Switches needed for tutorial purposes
	// There's gotta be a better away to do this : TODO: Fix
	bool disableHorizontal;
	bool disableVertical;
	bool disableLaser;
	bool disableGaussCannon;
	bool disableSheild;

	bool gameOverOverride;
	float gameOverTimer;
	float gameOverTimerMax;

	// Movement
	Vector2f velocity;
	float maxVelocity;
	bool warpVerticalBounds;
	float acceleration; // increase or decrease acceleration i think
	Vector2f direction;
	Vector2f normalizedDir;
	float stabalizingForce;

	// STATS
	int level;
	int exp;
	int expNext;
	int statPoints;
	int cooling;				// shield recharge rate
	int plating;				// armour
	int power;					// strength
	int maneuverability;		// agility


	int hp;//--//
	int hpMax;//--//
	int hpAdded;

	int lives;

	int damage;
	int damageMax;

	// SCORE STATS
	int score;
	int enemiesKilled;
	int highestLevelAchieved;
	float totalSecondsAlive;


	int currentWeapon;

	int gaussCharge;
	int gaussChargeMax;

	// Upgrades
	dArr<int> upgradesAcquired;
	int mainGunLevel;
	bool sheild;
	bool dualMissiles01;
	bool dualMissiles02;
	
	// Powerups
	bool powerupRF;
	bool powerupXP;	
	bool powerupAbsorb;	
	bool powerupGrind;
	bool powerupPiercingShot;

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
		AudioManager* audioManager,
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
	inline void addShieldCharge(float charge) { this->shieldChargeTimer = std::min(this->shieldChargeTimerMax, this->shieldChargeTimer + charge); }
	inline const bool isShielding() const { return this->shieldActive; }
	inline Sprite getDeflectorShield() { return this->deflectorShield; }
	inline void upgradeShield() {
		this->audioManager->PlaySound(AudioManager::AudioSounds::UPGRADE_COLLECT);
		this->shieldAdded += 33.f;
		this->UpdateStats();
		this->shieldChargeTimer = this->shieldChargeTimerMax;
	}
	inline void impactSheild() { this->shieldChargeTimer = std::max(0.f, this->shieldChargeTimer - (this->shieldChargeTimerMax / 3)); }

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
	inline void enablePowerupAbsorb() {
		this->powerupAbsorb = true;
		this->powerupSprite.setTexture(Player::powerupIndicatorTextures[2]);
		this->aura.setTexture(Player::shipAuraTextures[Auras::ABSORB]);
		this->setupPowerupSprite();
	}
	inline void enablePowerupGrind() {
		// Start looping the idle sound for duration of powerup
		this->audioManager->PlaySound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);
		this->powerupGrind = true;
		this->powerupSprite.setTexture(Player::powerupIndicatorTextures[3]);
		this->aura.setTexture(Player::shipAuraTextures[Auras::GRIND]);
		this->setupPowerupSprite();
	}
	inline void enablePowerupPiercingShot() {
		this->powerupPiercingShot = true;
		this->powerupSprite.setTexture(Player::powerupIndicatorTextures[4]);
		this->setupPowerupSprite();
	}
	inline void setupPowerupSprite() {
		this->audioManager->PlaySound(AudioManager::AudioSounds::POWERUP_COLLECT);
		this->powerupTimer = this->powerupTimerMax;
		this->powerupSprite.setScale(0.3f, 0.3f);
		this->powerupSprite.setOrigin(this->powerupSprite.getGlobalBounds().width / 2, this->powerupSprite.getGlobalBounds().height / 2);
	}
	inline const bool isAnyPowerupActive() const { return (this->powerupRF || this->powerupXP || this->powerupAbsorb || this->powerupGrind || this->powerupPiercingShot); }
	inline bool getPowerupXP() const { return this->powerupXP; }
	inline bool getPowerupPiercingShot() const { return this->powerupPiercingShot; }
	inline bool getPowerupAbsorb() const { return this->powerupAbsorb; }
	inline bool getPowerupGrind() const { return this->powerupGrind; }
	inline float getCalculatedShootTimer() const { return this->powerupRF ? 10.f : 20.f; }

	// Positional
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline void resetVelocity() { this->velocity = Vector2f(0.f, 0.f); }
	inline void resetVelocityY() { this->velocity.y = 0.f; }
	inline void move(float x, float y) { this->sprite.move(Vector2f(x, y)); this->mainGunSprite.move(Vector2f(x, y)); }
	inline const Vector2f& getNormDir() const { return this->normalizedDir; }
	inline const bool collidesWith(FloatRect other) const { return this->sprite.getGlobalBounds().intersects(other); }

	// Life
	inline const int getPlayerNumber() const { return this->playerNumber; }
	inline const bool isDead() const { return this->lives <= 0 && this->hp <= 0; }
	inline const bool shouldLoseLife() { return this->hp <= 0 && this->lives > 0; }
	inline const int getLives() const { return this->lives; }
	inline const void setLives(int lives) { this->lives = lives; }
	inline const bool loseLife() { --this->lives; return this->isDead(); }
	inline bool gainHp(int hp) { 
		this->audioManager->PlaySound(AudioManager::AudioSounds::HEALTH_PICKUP);
		this->hp = std::min(this->hp + hp, this->hpMax); 
		return this->hp < this->hpMax; 
	}
	inline const int& getHp() const { return hp; }
	inline const int& getHpMax() const { return hpMax; }
	inline const String getHpAsString() const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline void upgradeHP() {
		this->audioManager->PlaySound(AudioManager::AudioSounds::UPGRADE_COLLECT);
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
	inline void incrementEnemiesKilled() { ++this->enemiesKilled; }
	inline const int getEnemiesKilled() const { return this->enemiesKilled; }
	inline const float getTimeAlive() const { return this->totalSecondsAlive; }
	inline const int getHighestLevelAchieved() const { return this->highestLevelAchieved; }
	inline void setTimeAlive(float seconds) { this->totalSecondsAlive = seconds; }

	// Damage/Attack
	int getDamage() const;
	inline bool isDamageCooldown() { return this->damageTimer < this->damageTimerMax; }
	inline void enableSheild() { this->sheild = true; }
	inline void enableDualMissile01() { this->dualMissiles01 = true; }
	inline void enableDualMissile02() { this->dualMissiles02 = true; }

	// Vector Utility
	inline float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	inline Vector2f normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}

	// Tutorial modification
	inline dArr<int> *getControls() { return &this->controls; }
	inline void enableAllControls() {
		this->disableGaussCannon = false;
		this->disableHorizontal = false;
		this->disableLaser = false;
		this->disableSheild = false;
		this->disableVertical = false;
	}
	inline void disableAllControls() {
		this->disableGaussCannon = true;
		this->disableHorizontal = true;
		this->disableLaser = true;
		this->disableSheild = true;
		this->disableVertical = true;
	}
	inline void setDisableLaser(bool flag) { this->disableLaser = flag; }
	inline void setDisableHorizontal(bool flag) { this->disableHorizontal = flag; }
	inline void setDisableVertical(bool flag) { this->disableVertical = flag; }
	inline void setDisableGaussCannon(bool flag) { this->disableGaussCannon = flag; }
	inline void setDisableShield(bool flag) { this->disableSheild = flag; }

	// Functions
	void UpdateAccessories(const float &dt, const float scollSpeed);
	void UpdatePowerups(const float &dt);
	bool UpdateLeveling();
	void UpdateStatsUI();
	void Update(View &view, const float &dt, const float scrollSpeed);
	void UpdateStats();

	void Draw(RenderTarget &renderTarget);
	void DrawUI(RenderTarget &renderTarget);

	void Reset();
	bool ChangeAccessories(const float &dt);
	void Combat(const float &dt);
	void Movement(const float &dt, View &view, const float scrollSpeed);
	void InitUI(Text t);
	Bullet& BulletAt(unsigned index);
	void RemoveBullet(unsigned index);
	void TakeDamage(int damage);
	void UpgradeGunLevel(int gunLevel);
	void SetGunLevel(int gunLevel);
	void AddStatPointRandom();
	bool PlayerShowStatsIsPressed();
	std::string GetStatsAsString();
	void ResetOnLifeLost(View &view);
	void ActivateGameEnd(Vector2f pos);
	inline const bool isGameOverActivated() const { return this->gameOverOverride; }
	inline const bool campaignLevelBeat() const { return this->gameOverTimer <= 0.f; }

	// Cosmo Effects
	inline const bool isEffectedByCosmo() const { return this->effectedByCosmo; }
	std::string ApplyCosmoEffect();
	void ResetAllCosmoEffects();
	void ToggleInvertControlsEffect();
	void ToggleWarpBoundsEffect(bool warp);
	void ToggleSpeedIncreaseEffect(bool on);
	void ToggleSpeedDecreaseEffect(bool on);
	void ToggleBaddieSwap(bool swap);

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
};

