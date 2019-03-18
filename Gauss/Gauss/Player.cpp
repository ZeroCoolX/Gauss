#include <algorithm>
#include "Player.h"
#include "Enums.h"

unsigned Player::playerId = 0;

Player::Player(std::vector<Texture> &textureMap,
	dArr<Texture> &lWingTextureMap,
	dArr<Texture> &rWingTextureMap,
	dArr<Texture> &auraTextureMap,
	dArr<Texture> &cPitTextureMap,
	int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE
) :level(1), exp(0), hp(10), hpMax(10), statPoints(0), cooling(0), maneuverability(0), power(0), damage(1), damageMax(2), score(0)
{
	// Stats
	// Formula courtesy of Suraj Sharma and Tibia
	// https://www.youtube.com/watch?v=SPcLqoTbIWE&list=PL6xSOsbVA1eZUKRu_boTDdzu8pGsVNpTo&index=18
	// https://tibia.fandom.com/wiki/Experience_Formula
	this->expNext = 20 + static_cast<int>(
		(50/3)
		*((pow(level,3)-6
			* pow(level, 2)) + 17
			* level -12)
		);

	// Accesory Textures
	this->lWingTextureMap = &lWingTextureMap;
	this->rWingTextureMap = &rWingTextureMap;
	this->auraTextureMap = &auraTextureMap;
	this->cPitTextureMap = &cPitTextureMap;


	this->_initTextures(textureMap);
	this->_initPlayerSettings();

	// Set player controls
	this->controls[GameEnums::C_UP] = UP;
	this->controls[GameEnums::C_DOWN] = DOWN;
	this->controls[GameEnums::C_LEFT] = LEFT;
	this->controls[GameEnums::C_RIGHT] = RIGHT;
	this->controls[GameEnums::C_FIRE] = FIRE;
}

Player::~Player()
{
}

int Player::getDamage() const { 
	int damage = rand() % this->damageMax + this->damage;

	switch (this->currentWeapon) {
		case GameEnums::G_LASER:
			// Accept regular damage
			break;
		case GameEnums::G_MISSILE01:
			damage *= 2;
			break;
		case GameEnums::G_MISSILE02:
			damage *= 4;
			break;
	}
	return damage; 
}

bool Player::UpdateLeveling() {
	if (this->exp >= this->expNext) {
		this->level++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>(
			(50 / 3)
			*((pow(level, 3) - 6
				* pow(level, 2)) + 17
				* level - 12)
			);
		// Regenerate health for now
		this->hp = this->hpMax;
		
		return true;
	}
	return false;
}

void Player::UpdateAccessories(const float &dt) {
	// Update the position of the gun to track the player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x,
		this->playerCenter.y);

	// Compensate after fire kickback
	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 4;
	if (this->mainGunSprite.getPosition().x < origin) {
		this->mainGunSprite.move((this->mainGunReturnSpeed + this->velocity.x) * dt * DeltaTime::dtMultiplier, 0.f);

	}
	if (this->mainGunSprite.getPosition().x > origin) {
		this->mainGunSprite.setPosition(
			origin,
			this->playerCenter.y);
	}

	// Left Wing

	// Right Wing

	// Aura

	// Cockpit
}

void Player::Movement(const float &dt) {
	this->_processPlayerInput(dt);

	// Move player
	this->sprite.move(this->velocity * dt * DeltaTime::dtMultiplier);

	this->_recalculatePlayerCenter();
}

void Player::Combat(const float &dt) {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_FIRE])) && this->shootTimer >= this->shootTimerMax)
	{
		const Vector2f direction = Vector2f(1.f, 0.f);
		switch (this->currentWeapon) {
			case GameEnums::G_LASER:
				this->_fireLaser(direction);
				break;
			case GameEnums::G_MISSILE01:
				this->_fireMissileLight(direction);
				break;
			case GameEnums::G_MISSILE02:
				this->_fireMissileHeavy(direction);
				break;
		}

		this->shootTimer = 0; // RESET TIMER
	}
}

void Player::UpdateStatsUI() {
	Vector2f statsPos = Vector2f(this->getPosition().x, (this->getPosition().y - this->getGlobalBounds().height / 4));

	// Stats
	this->statsText.setPosition(statsPos);
	this->statsText.setString("[ " + std::to_string(this->playerNumber) + " ]					" + this->getHpAsString()
	+ "\n\n\n\n\n\n"
	+ std::to_string(this->getLevel()));

	// Exp Bar
	Vector2f expBarPos = Vector2f(this->getPosition().x + 15.f, (this->getPosition().y + this->getGlobalBounds().height + 12.f)); // Clean up magic numbers
	this->playerExpBar.setPosition(expBarPos);
	// Scale based off player experience to create a dynamic bar
	this->playerExpBar.setScale(static_cast<float>(this->getExp()) / static_cast<float>(getExpNext()), 1.f);
}

void Player::Update(Vector2u windowBounds, const float &dt) {
	// Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer += 1.f * dt * DeltaTime::dtMultiplier;
	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * DeltaTime::dtMultiplier;

	this->Movement(dt);
	this->UpdateAccessories(dt);
	this->Combat(dt);
	this->UpdateStatsUI();
}

void Player::DrawUI(RenderTarget &renderTarget) {
	// Stats
	renderTarget.draw(this->statsText);
	// Exp bar
	renderTarget.draw(this->playerExpBar);
}

// Order matters!
void Player::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->aura);

	for (size_t i = 0; i < this->bullets.Size(); ++i) {
		this->bullets[i].Draw(renderTarget);
	}

	renderTarget.draw(this->mainGunSprite);
	renderTarget.draw(this->sprite);

	renderTarget.draw(this->cPit);
	renderTarget.draw(this->lWing);
	renderTarget.draw(this->rWing);

	this->DrawUI(renderTarget);
}

void Player::InitUI(Text t) {
	this->statsText = t;

	this->playerExpBar.setSize(Vector2f(90.f, 8.5)); // TODO: magic numbers need to go away
	this->playerExpBar.setFillColor(Color(0, 90, 200, 200));
}

Bullet& Player::BulletAt(unsigned index) {
	if (index < 0 || index > this->bullets.Size()) {
		throw "OutOfBoundsBullet - Player::BulletAt";
	}
	return this->bullets[index];
}

void Player::RemoveBullet(unsigned index) {
	if (index < 0 || index > this->bullets.Size()) {
		throw "OutOfBoundsBullet - Player::RemoveBullet";
	}
	return this->bullets.Remove(index);
}

void Player::_processPlayerInput(const float &dt) {
	// Collect player input
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_UP]))) {
		this->direction.y = -1;
		if (this->velocity.y > -this->maxVelocity && this->direction.y < 0) {
			this->velocity.y += direction.y * this->acceleration * dt * DeltaTime::dtMultiplier;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_DOWN]))) {
		this->direction.y = 1;
		if (this->velocity.y < this->maxVelocity && this->direction.y > 0) {
			this->velocity.y += direction.y * this->acceleration * dt * DeltaTime::dtMultiplier;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_LEFT]))) {
		this->direction.x = -1;
		if (this->velocity.x > -this->maxVelocity && this->direction.x < 0) {
			this->velocity.x += direction.x * this->acceleration * dt * DeltaTime::dtMultiplier;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_RIGHT]))) {
		this->direction.x = 1;
		if (this->velocity.x < this->maxVelocity && this->direction.x > 0) {
			this->velocity.x += direction.x * this->acceleration * dt * DeltaTime::dtMultiplier;
		}
	}

	// Apply Drag Force
	if (this->velocity.x > 0) {
		this->velocity.x -= this->stabalizingForce * dt * DeltaTime::dtMultiplier;
		this->velocity.x = std::max(0.f, this->velocity.x);
	}else if (this->velocity.x < 0) {
		this->velocity.x += this->stabalizingForce * dt * DeltaTime::dtMultiplier;
		this->velocity.x = std::min(0.f, this->velocity.x);
	}
	if (this->velocity.y > 0) {
		this->velocity.y -= this->stabalizingForce * dt * DeltaTime::dtMultiplier;
		this->velocity.y = std::max(0.f, this->velocity.y);
	}
	else if (this->velocity.y < 0) {
		this->velocity.y += this->stabalizingForce * dt * DeltaTime::dtMultiplier;
		this->velocity.y = std::min(0.f, this->velocity.y);
	}
}



void Player::_initTextures(std::vector <Texture> &textureMap) {
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

	this->lWing.setTexture((*this->lWingTextureMap)[0]);
	this->rWing.setTexture((*this->rWingTextureMap)[0]);
	this->aura.setTexture((*this->auraTextureMap)[0]);
	this->cPit.setTexture((*this->cPitTextureMap)[0]);
}

void Player::_initPlayerSettings() {

	// Bullet settings
	this->bulletSpeed = 2.f;
	this->bulletMaxSpeed = 60;
	this->bulletAcceleration = 1.f;

	// Setup timers
	this->shootTimerMax = 20.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 5.f;
	this->damageTimer = this->damageTimerMax;

	// Movement settings
	this->maxVelocity = 15.f;
	this->acceleration = 1.f;
	this->stabalizingForce = 0.3f;

	// WEAPON
	this->currentWeapon = GameEnums::G_LASER;

	// UPGRADES
	this->mainGunLevel = GameEnums::DEFAULT_LASER;
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;

	// Number of players for co-op
	this->playerNumber = Player::playerId + 1;
	Player::playerId++;
}

void Player::_recalculatePlayerCenter() {
	// Update sprite center
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2;
}

void Player::_fireLaser(const Vector2f direction) {
	// Create Laser
	switch (this->mainGunLevel) {
	case GameEnums::DEFAULT_LASER:
		this->bullets.Add(
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
	this->mainGunSprite.move(-mainGunKickback, 0.f); // Should arguably be .setPosition() instead of move...
}

void Player::_fireMissileLight(const Vector2f direction) {
	// Create Missile
	this->bullets.Add(
		Bullet(missile01ProjectileTexture,
			missileScale,
			Vector2f(this->playerCenter.x, this->playerCenter.y - (this->sprite.getGlobalBounds().height / 2)),
			direction,
			this->bulletSpeed, this->bulletMaxSpeed, this->bulletAcceleration)
	);
	if (dualMissiles01) {
		this->bullets.Add(
			Bullet(missile01ProjectileTexture,
				missileScale,
				Vector2f(this->playerCenter.x, this->playerCenter.y + (this->sprite.getGlobalBounds().height / 2)),
				direction,
				this->bulletSpeed, this->bulletMaxSpeed, this->bulletAcceleration)
		);
	}
}

void Player::_fireMissileHeavy(const Vector2f direction) {
	if (dualMissiles02) {
		// setup
	}
}
