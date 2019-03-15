#include <algorithm>
#include "Player.h"
#include "Enums.h"

unsigned Player::playerId = 0;

Player::Player(std::vector<Texture> &textureMap,
	int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE
) :level(1), exp(0), expNext(100), hp(10), hpMax(10), damage(1), damageMax(2), score(0)
{
	// Assign ship
	this->sprite.setTexture(textureMap[GameEnums::T_SHIP]);
	this->sprite.setScale(0.13f, 0.13f);

	// Assign accessories
	this->mainGunSprite.setTexture(textureMap[GameEnums::T_MAIN_GUN]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2);
	this->mainGunSprite.rotate(90);
	
	// Assign bullet properties
	this->laserProjectileTexture = &textureMap[GameEnums::T_LASER01];
	this->missile01ProjectileTexture = &textureMap[GameEnums::T_MISSILE01];

	this->bulletSpeed = 2.f;
	this->bulletMaxSpeed = 75;
	this->bulletAcceleration = 1.f;

	// Setup timers
	this->shootTimerMax = 15;
	this->shootTimer = this->shootTimerMax;
	this->damageMax = 5;
	this->damageTimer = this->damageMax;

	// Set player controls
	this->controls[GameEnums::C_UP] = UP;
	this->controls[GameEnums::C_DOWN] = DOWN;
	this->controls[GameEnums::C_LEFT] = LEFT;
	this->controls[GameEnums::C_RIGHT] = RIGHT;
	this->controls[GameEnums::C_FIRE] = FIRE;

	// Movement setup
	this->maxVelocity = 15.f;
	this->acceleration = 1.f;
	this->stabalizingForce = 0.3f;

	// Weapons
	this->currentWeapon = GameEnums::G_LASER;

	// UPGRADES
	this->mainGunLevel = GameEnums::DEFAULT_LASER;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;

	// Number of players for co-op
	this->playerNumber = Player::playerId;
	Player::playerId++;
}

Player::~Player()
{
}

void Player::UpdateAccessories() {
	// Update the position of the gun to track the player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x,
		this->playerCenter.y);

	// Compensate after fire kickback
	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 4;
	if (this->mainGunSprite.getPosition().x < origin) {
		this->mainGunSprite.move(this->mainGunReturnSpeed + this->velocity.x, 0.f);

	}
	if (this->mainGunSprite.getPosition().x > origin) {
		this->mainGunSprite.setPosition(
			origin,
			this->playerCenter.y);
	}
}

void Player::Movement() {
	this->processPlayerInput();

	// Update sprite center
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2;
}

void Player::Combat() {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_FIRE])) && this->shootTimer >= this->shootTimerMax)
	{
		const Vector2f direction = Vector2f(1.f, 0.f);
		switch (this->currentWeapon) {
			case GameEnums::G_LASER:
				// Create Laser
				switch (this->mainGunLevel) {
					case GameEnums::DEFAULT_LASER:
						this->bullets.push_back(
							Bullet(laserProjectileTexture,
								laserBulletScale,
								Vector2f(this->playerCenter.x + (this->mainGunSprite.getGlobalBounds().width / 2), this->playerCenter.y),
								direction,
								this->bulletMaxSpeed, this->bulletMaxSpeed, 0.f) // No acceleration - only constant velocity
						);
						break;
					case GameEnums::LEVEL_2_LASER:
						break;
					case GameEnums::LEVEL_3_LASER:
						break;
				}
				// Animate gun
				this->mainGunSprite.move(-mainGunKickback, 0.f);
				break;
			case GameEnums::G_MISSILE01:
				// Create Missile
				this->bullets.push_back(
					Bullet(missile01ProjectileTexture,
						missileScale,
						Vector2f(this->playerCenter.x, this->playerCenter.y - (this->sprite.getGlobalBounds().height / 2)),
						direction,
						this->bulletSpeed, this->bulletMaxSpeed, this->bulletAcceleration)
				);
				if (dualMissiles01) {
					this->bullets.push_back(
						Bullet(missile01ProjectileTexture,
							missileScale,
							Vector2f(this->playerCenter.x, this->playerCenter.y + (this->sprite.getGlobalBounds().height / 2)),
							direction,
							this->bulletSpeed, this->bulletMaxSpeed, this->bulletAcceleration)
					);
				}
				break;
			case GameEnums::G_MISSILE02:
				if (dualMissiles02) {
					// setup
				}
				break;
		}

		this->shootTimer = 0; // RESET TIMER
	}
}

void Player::Update(Vector2u windowBounds) {
	// Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;
	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer++;

	this->Movement();
	this->UpdateAccessories();
	this->Combat();
}

void Player::Draw(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->bullets.size(); ++i) {
		this->bullets[i].Draw(renderTarget);
	}
	renderTarget.draw(this->mainGunSprite);

	renderTarget.draw(this->sprite);
}

void Player::processPlayerInput() {
	// Collect player input
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_UP]))) {
		this->direction.y = -1;
		if (this->velocity.y > -this->maxVelocity && this->direction.y < 0) {
			this->velocity.y += direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_DOWN]))) {
		this->direction.y = 1;
		if (this->velocity.y < this->maxVelocity && this->direction.y > 0) {
			this->velocity.y += direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_LEFT]))) {
		this->direction.x = -1;
		if (this->velocity.x > -this->maxVelocity && this->direction.x < 0) {
			this->velocity.x += direction.x * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_RIGHT]))) {
		this->direction.x = 1;
		if (this->velocity.x < this->maxVelocity && this->direction.x > 0) {
			this->velocity.x += direction.x * this->acceleration;
		}
	}

	// Apply Drag Force
	if (this->velocity.x > 0) {
		this->velocity.x -= this->stabalizingForce;
		this->velocity.x = std::max(0.f, this->velocity.x);
	}else if (this->velocity.x < 0) {
		this->velocity.x += this->stabalizingForce;
		this->velocity.x = std::min(0.f, this->velocity.x);
	}
	if (this->velocity.y > 0) {
		this->velocity.y -= this->stabalizingForce;
		this->velocity.y = std::max(0.f, this->velocity.y);
	}
	else if (this->velocity.y < 0) {
		this->velocity.y += this->stabalizingForce;
		this->velocity.y = std::min(0.f, this->velocity.y);
	}
	

	// Final movement
	this->sprite.move(this->velocity);
}
