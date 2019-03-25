#include <algorithm>
#include "Player.h"
#include "Enums.h"

unsigned Player::playerId = 0;

Player::Player(std::vector<Texture> &textureMap,
	dArr<Texture> &mainGunTextureMap,
	dArr<Texture> &lWingTextureMap,
	dArr<Texture> &rWingTextureMap,
	dArr<Texture> &auraTextureMap,
	dArr<Texture> &cPitTextureMap,
	int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE,
	int GAUSSCANNON,
	int TOGGLESTATS,
	int CHANGE_LWING,
	int CHANGE_CPIT,
	int CHANGE_RWING,
	int CHANGE_AURA
) :level(1), exp(0), hp(10), hpMax(10), hpAdded(10), statPoints(0), cooling(0), maneuverability(0), plating(0), power(0), damage(1), damageMax(2), score(0)
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

	this->keyTimeMax = 8.f;
	this->keyTime = this->keyTimeMax;

	// Accesory Textures
	this->lWingTextureMap = &lWingTextureMap;
	this->rWingTextureMap = &rWingTextureMap;
	this->auraTextureMap = &auraTextureMap;
	this->cPitTextureMap = &cPitTextureMap;

	// Main Gun Textures
	this->mainGunTextureMap = &mainGunTextureMap;

	// Selectors
	this->lWingSelect = 1;
	this->rWingSelect = 1;
	this->cPitSelect = 1;
	this->auraSelect = 0;


	this->_initTextures(textureMap);
	this->_initPlayerSettings();

	// Set player controls
	this->controls.Add(UP);
	this->controls.Add(DOWN);
	this->controls.Add(LEFT);
	this->controls.Add(RIGHT);
	this->controls.Add(FIRE);
	this->controls.Add(GAUSSCANNON);
	this->controls.Add(TOGGLESTATS);
	this->controls.Add(CHANGE_LWING);
	this->controls.Add(CHANGE_CPIT);
	this->controls.Add(CHANGE_RWING);
	this->controls.Add(CHANGE_AURA);
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

void Player::TakeDamage(int damage) {
	this->hp -= damage;

	this->damageTimer = 0;

	this->velocity.x += -this->normalizedDir.x * 10.f; // knockback amount
	this->velocity.y += -this->normalizedDir.y * 10.f; // knockback amount
}

bool Player::UpdateLeveling() {
	if (this->exp >= this->expNext) {
		this->level++;
		this->addStatPoint();

		this->exp -= this->expNext;
		this->expNext = static_cast<int>(
			(50 / 3)
			*((pow(level, 3) - 6
				* pow(level, 2)) + 17
				* level - 12)
			);

		this->cooling++;
		this->plating++;
		this->power++;
		this->maneuverability++;

		this->UpdateStats();
		this->hp = this->hpMax;

		return true;
	}
	return false;
}

void Player::UpdateStats() {
	this->hpMax = this->hpAdded + (this->plating * 5);
	this->damageMax = 2 + (this->power * 2);
	this->damage = 1 + power;
}

bool Player::ChangeAccessories(const float &dt) {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::CHANGE_LWING]))) {
		this->lWingSelect = ++this->lWingSelect % ((int)(*this->lWingTextureMap).Size() - 1);
		this->lWing.setTexture((*this->lWingTextureMap)[this->lWingSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::CHANGE_RWING]))) {
		this->rWingSelect = ++this->rWingSelect % ((int)(*this->rWingTextureMap).Size() - 1);
		this->rWing.setTexture((*this->rWingTextureMap)[this->rWingSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::CHANGE_AURA]))) {
		this->auraSelect = ++this->auraSelect % ((int)(*this->auraTextureMap).Size() - 1);
		this->aura.setTexture((*this->auraTextureMap)[this->auraSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::CHANGE_CPIT]))) {
		this->cPitSelect = ++this->cPitSelect % ((int)(*this->cPitTextureMap).Size() - 1);
		this->cPit.setTexture((*this->cPitTextureMap)[this->cPitSelect]);
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
	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 6;
	if (this->mainGunSprite.getPosition().x < origin) {
		this->mainGunSprite.move((this->mainGunReturnSpeed + this->velocity.x) * dt * DeltaTime::dtMultiplier, 0.f);

	}
	if (this->mainGunSprite.getPosition().x > origin) {
		this->mainGunSprite.setPosition(
			origin,
			this->playerCenter.y);
	}

	// Left Wing
	float xMovement = (this->velocity.x < 0 ? -abs(this->velocity.x) : abs(this->velocity.x/3));
	float yMovement = (this->velocity.x < 0 ? abs(this->velocity.x) : -abs(this->velocity.x / 4));

	this->lWing.setPosition(
		this->playerCenter.x + xMovement,
		this->playerCenter.y - yMovement);

	// Right Wing
	this->rWing.setPosition(
		this->playerCenter.x + xMovement,
		this->playerCenter.y + yMovement);

	// Aura
	this->aura.setPosition(this->playerCenter.x, this->playerCenter.y);
	this->aura.rotate(5.f * dt * DeltaTime::dtMultiplier);

	// Cockpit
	this->cPit.setPosition(this->playerCenter.x - this->velocity.x / 3, this->playerCenter.y);
}

void Player::Movement(const float &dt, Vector2u windowBounds) {
	// Update normalized direction
	this->normalizedDir = this->normalize(this->velocity, this->vectorLength(this->velocity));

	this->_processPlayerInput(dt);

	// Move player
	this->sprite.move(this->velocity * dt * DeltaTime::dtMultiplier);

	this->_recalculatePlayerCenter();

	// Bounds check for window collision
	this->_checkBounds(windowBounds, false);
}

void Player::Combat(const float &dt) {
	const Vector2f direction = Vector2f(1.f, 0.f);

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_FIRE])) && this->shootTimer >= this->shootTimerMax)
	{
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

	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::C_GAUSSCANNON])) && this->gaussChargeTimer >= this->gaussChargeTimerMax) {
		// Fire a deadly gauss cannon shot
		this->gaussChargeTimer = 0.f;
		this->playerGaussChargeCircle.setFillColor(this->gaussChargingColor);
		this->_fireGaussCannon(direction);
		this->keyTime = 0;
	}

	if (this->isDamageCooldown()) {
		if ((int)this->damageTimer % 2 == 0) {
			this->lWing.setColor(Color::Red);
			this->rWing.setColor(Color::Red);
			this->cPit.setColor(Color::Red);
		}
		else {
			this->lWing.setColor(Color::White);
			this->rWing.setColor(Color::White);
			this->cPit.setColor(Color::White);
		}
	}
	else {
		this->lWing.setColor(Color::White);
		this->rWing.setColor(Color::White);
		this->cPit.setColor(Color::White);
	}
}

void Player::UpdateStatsUI() {
	Vector2f statsPos = Vector2f(this->getPosition().x, (this->getPosition().y - this->getGlobalBounds().height));

	// Stats
	this->statsText.setPosition(statsPos);
	this->statsText.setString("[ " + std::to_string(this->playerNumber) + " ]"
	+ "\n\n\n\n\n\n\n\n\n"
	+ std::to_string(this->getLevel()));

	// Exp Bar
	Vector2f expBarPos = Vector2f(this->getPosition().x + 15.f, (this->getPosition().y + this->getGlobalBounds().height + 45.f));
	this->playerExpBar.setPosition(expBarPos);
	// Scale based off player experience to create a dynamic bar
	this->playerExpBar.setScale(static_cast<float>(this->getExp()) / static_cast<float>(getExpNext()), 1.f);
	this->playerExpBarBox.setPosition(Vector2f(expBarPos.x - 0.5f, expBarPos.y - 0.5f));

	// Health Bar
	Vector2f healthBarPos = Vector2f(expBarPos.x, expBarPos.y + this->playerExpBar.getGlobalBounds().height * 2.5f);
	this->playerHealthBar.setPosition(healthBarPos);
	// Scale based off player experience to create a dynamic bar
	this->playerHealthBar.setScale(static_cast<float>(this->getHp()) / static_cast<float>(getHpMax()), 1.f);
	this->playerHealthBarBox.setPosition(Vector2f(healthBarPos.x - 0.5f, healthBarPos.y - 0.5f));

	// Gauss Cannon Charge circle
	Vector2f gaussChargePos = Vector2f(this->getPosition().x + this->getGlobalBounds().width, (this->getPosition().y - this->getGlobalBounds().height + (this->playerGaussChargeCircleBorder.getRadius() * 2)));
	this->playerGaussChargeCircle.setPosition(gaussChargePos);
	// Scale based off player experience to create a dynamic bar
	float circleScale = this->gaussChargeTimer / this->gaussChargeTimerMax;
	this->playerGaussChargeCircle.setScale(circleScale, circleScale);
	playerGaussChargeCircleBorder.setPosition(gaussChargePos);

	if (this->PlayerShowStatsIsPressed()) {
		this->playerStatsHudText.setString(this->GetStatsAsString());
		this->playerStatsTextBox.setPosition(this->getPosition().x, this->getPosition().y + 150.f);
		this->playerStatsTextBox.setSize(Vector2f(this->playerStatsHudText.getGlobalBounds().width, this->playerStatsHudText.getGlobalBounds().height));
		this->playerStatsHudText.setPosition(this->playerStatsTextBox.getPosition());
	}
}

void Player::Update(Vector2u windowBounds, const float &dt) {
	// Update timers
	if (this->shootTimer < this->shootTimerMax) {
		this->shootTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->damageTimer < this->damageTimerMax) {
		this->damageTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}

	if (this->gaussChargeTimer < this->gaussChargeTimerMax) {
		this->gaussChargeTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		this->playerGaussChargeCircle.setFillColor(this->gaussReadyColor);
	}

	this->Movement(dt, windowBounds);
	this->UpdateAccessories(dt);
	this->Combat(dt);
	this->UpdateStatsUI();
}

void Player::DrawUI(RenderTarget &renderTarget) {
	// Stats
	renderTarget.draw(this->statsText);

	// Exp bar backing
	renderTarget.draw(this->playerExpBarBox);
	// Exp bar
	renderTarget.draw(this->playerExpBar);

	// Health bar backing
	renderTarget.draw(this->playerHealthBarBox);
	// Health bar
	renderTarget.draw(this->playerHealthBar);

	// Gauss charge circle border
	renderTarget.draw(this->playerGaussChargeCircleBorder);
	// Gauss charge circle
	renderTarget.draw(this->playerGaussChargeCircle);

	if (this->PlayerShowStatsIsPressed()) {
		renderTarget.draw(this->playerStatsTextBox);
		renderTarget.draw(this->playerStatsHudText);
	}
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

	// EXP bad
	this->playerExpBar.setSize(Vector2f(90.f, 8.5)); // TODO: magic numbers need to go away
	this->playerExpBar.setFillColor(Color(0, 90, 200, 200));

	// Health bar
	this->playerHealthBar.setSize(Vector2f(90.f, 8.0));
	this->playerHealthBar.setFillColor(Color(180, 0, 18, 200));

	// Setup Bar Boxes
	this->playerHealthBarBox.setSize(Vector2f(91.f, 9.5));
	this->playerHealthBarBox.setFillColor(Color(255, 255, 255, 0));
	this->playerHealthBarBox.setOutlineThickness(1);
	this->playerHealthBarBox.setOutlineColor(Color(255, 255, 255, 10));

	this->playerExpBarBox.setSize(Vector2f(91.f, 9.5));
	this->playerExpBarBox.setFillColor(Color(255, 255, 255, 0));
	this->playerExpBarBox.setOutlineThickness(1);
	this->playerExpBarBox.setOutlineColor(Color(255, 255, 255, 10));

	// Gauss cannon circle
	this->gaussReadyColor = Color(0, 220, 93, 150);
	this->gaussChargingColor = Color(0, 86, 37, 150);

	this->playerGaussChargeCircle.setRadius(10);
	this->playerGaussChargeCircle.setFillColor(this->gaussChargingColor);
	this->playerGaussChargeCircle.setOrigin(Vector2f(this->playerGaussChargeCircle.getRadius(), this->playerGaussChargeCircle.getRadius()));

	this->playerGaussChargeCircleBorder.setRadius(10);
	this->playerGaussChargeCircleBorder.setFillColor(Color(255, 255, 255, 0));
	this->playerGaussChargeCircleBorder.setOutlineColor(Color(255, 255, 255, 10));
	this->playerGaussChargeCircleBorder.setOutlineThickness(1);
	this->playerGaussChargeCircleBorder.setOrigin(Vector2f(this->playerGaussChargeCircleBorder.getRadius(), this->playerGaussChargeCircleBorder.getRadius()));

	// Stats HUD text
	this->playerStatsHudText.setFont(*this->statsText.getFont());
	this->playerStatsHudText.setFillColor(Color::White);
	this->playerStatsHudText.setCharacterSize(16);
	this->playerStatsHudText.setString("NONE");

	this->playerStatsTextBox.setFillColor(Color(50, 50, 50, 100));
	this->playerStatsTextBox.setOutlineThickness(1.f);
	this->playerStatsTextBox.setOutlineColor(Color(255, 255, 255, 200));

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

void Player::SetGunLevel(int gunLevel) {
	this->mainGunLevel = gunLevel;
	if (this->mainGunLevel >= (int)(*this->mainGunTextureMap).Size()) {
		std::cout << "ERROR! No texture for gun level " << gunLevel << std::endl;
	}
	else {
		this->mainGunSprite.setTexture((*this->mainGunTextureMap)[this->mainGunLevel]);
		// Hack right now since I don't like how the level 3 gun looks
		if (this->mainGunLevel == GameEnums::LEVEL_3_LASER) {
			this->mainGunSprite.setScale(0.75f, 0.75f);
		}
	}
}

void Player::Reset() {
	// Reset sprite
	this->sprite.setPosition(Vector2f(100.f, 100.f));
	
	// Reset stats
	this->hpMax = 10;
	this->hp = this->hpMax;
	this->level = 1;
	this->exp = 0;
	this->expNext = 20;
	this->statPoints = 0;
	this->maneuverability = 0;
	this->cooling = 0;
	this->power = 0;
	this->plating = 0;
	this->score = 0;

	// Reset upgrades
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->sheild = false;
	this->piercingShot = false;

	// Reset weapons
	this->currentWeapon = GameEnums::G_LASER;
	this->SetGunLevel(GameEnums::DEFAULT_LASER);
	this->bullets.Clear();

	// Reset Timers
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = this->damageTimerMax;
}

void Player::AddStatPointRandom() {
	int r = rand() % 4;
	switch (r) {
	case 0:
		this->power++;
		break;
	case 1:
		this->maneuverability++;
		break;
	case 2:
		this->cooling++;
		break;
	case 3:
		this->plating++;
		break;
	}

	this->UpdateStats();
}

bool Player::PlayerShowStatsIsPressed() {
	return Keyboard::isKeyPressed(Keyboard::Key(this->controls[GameEnums::TOGGLE_STATS]));
}

std::string Player::GetStatsAsString() {
	return 
		"Level: " + std::to_string(this->level) +
		"\nExp: " + std::to_string(this->exp) + "/" + std::to_string(this->expNext) +
		"\nStatpoints: " + std::to_string(this->statPoints) +
		"\nHealth: " + std::to_string(this->hp) + "/" + std::to_string(this->hpMax) + " (+ " + std::to_string(this->hpAdded) + ")" +
		"\nDamage: " + std::to_string(this->damage) + "/" + std::to_string(this->damageMax) +
		"\nScore: " + std::to_string(this->score) +
		"\nPower: " + std::to_string(this->power) +
		"\nPlating: " + std::to_string(this->plating) +
		"\nManeuverability: " + std::to_string(this->maneuverability) +
		"\nCooling: " + std::to_string(this->cooling);

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
	this->sprite.setScale(0.1f, 0.1f);
	this->sprite.setColor(Color(10, 10, 10, 255));

	// Assign main gun
	this->mainGunSprite.setTexture((*this->mainGunTextureMap)[GameEnums::MGT_MAIN_GUN]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2);
	this->mainGunSprite.rotate(90);

	// Assign bullet properties
	this->laserProjectileTexture = &textureMap[GameEnums::T_LASER01];
	this->gaussCannonProjectileTexture = &textureMap[GameEnums::T_GAUSSCANNON01];
	this->missile01ProjectileTexture = &textureMap[GameEnums::T_MISSILE01];

	// Accessories
	// Left wing
	this->lWing.setTexture((*this->lWingTextureMap)[this->lWingSelect]);
	this->lWing.setOrigin(this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2);
	this->lWing.setPosition(this->playerCenter);
	this->lWing.setRotation(90.f);
	this->lWing.setScale(0.7f, 0.7f);

	// Right wing
	this->rWing.setTexture((*this->rWingTextureMap)[this->rWingSelect]);
	this->rWing.setOrigin(this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2);
	this->rWing.setPosition(this->playerCenter);
	this->rWing.setRotation(90.f);
	this->rWing.setScale(0.7f, 0.7f);

	// Aura
	this->aura.setTexture((*this->auraTextureMap)[this->auraSelect]);
	this->aura.setOrigin(this->aura.getGlobalBounds().width / 2,
		this->aura.getGlobalBounds().height / 2);
	this->aura.setPosition(this->playerCenter);
	this->aura.setRotation(90.f);
	this->aura.setScale(0.7f, 0.7f);

	// Cockpit
	this->cPit.setTexture((*this->cPitTextureMap)[this->cPitSelect]);
	this->cPit.setOrigin(this->cPit.getGlobalBounds().width / 2,
		this->cPit.getGlobalBounds().height / 2);
	this->cPit.setPosition(this->playerCenter);
	this->cPit.setRotation(90.f);
	this->cPit.setScale(0.7f, 0.7f);
}

void Player::_initPlayerSettings() {

	// Bullet settings
	this->bulletSpeed = 2.f;
	this->bulletMaxSpeed = 60;
	this->bulletAcceleration = 1.f;

	// Setup timers
	this->shootTimerMax = 20.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;
	this->gaussChargeTimerMax = 500.f;
	this->gaussChargeTimer = 0.f;

	// Movement settings
	this->maxVelocity = 15.f;
	this->acceleration = 1.f;
	this->stabalizingForce = 0.3f;

	// WEAPON
	this->currentWeapon = GameEnums::G_LASER;

	// UPGRADES
	this->SetGunLevel(GameEnums::DEFAULT_LASER);
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->sheild = false;
	this->piercingShot = false;

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
	float yOffset = this->mainGunLevel == GameEnums::DEFAULT_LASER ? 0.f : this->mainGunLevel > GameEnums::LEVEL_2_LASER ? 30.f : 15.f;
	std::cout << "yOffset = " << yOffset << std::endl;
	for (int i = 0; i <= this->mainGunLevel; i++)
	{
		this->bullets.Add(
			Bullet(laserProjectileTexture,
				laserBulletScale,
				Vector2f(
					this->playerCenter.x + (this->mainGunSprite.getGlobalBounds().width / 2) + (i == 0 && yOffset == 30.f ? 50.f : -20), 
					this->playerCenter.y + (yOffset == 15 ? yOffset : i > 0 ? yOffset : 0.f)),
				direction,
				this->bulletMaxSpeed, this->bulletMaxSpeed, this->getDamage(), false, 0.f)
		);
		yOffset *= -1;
	}
	// Animate gun
	this->mainGunSprite.move(-mainGunKickback, 0.f);
}

void Player::_fireGaussCannon(const Vector2f direction) {
	this->bullets.Add(
			Bullet(gaussCannonProjectileTexture,
				gaussCannonProjectileScale,
				Vector2f(
					this->playerCenter.x + (this->mainGunSprite.getGlobalBounds().width / 2),this->playerCenter.y),
				direction,
				this->bulletMaxSpeed, this->bulletMaxSpeed, this->getDamage() * 10, true, 0.f)
		);
	// Animate gun
	this->mainGunSprite.move(-mainGunKickback, 0.f);
}

void Player::_fireMissileLight(const Vector2f direction) {
	// Create Missile
	this->bullets.Add(
		Bullet(missile01ProjectileTexture,
			missileScale,
			Vector2f(this->playerCenter.x, this->playerCenter.y - (this->sprite.getGlobalBounds().height / 2)),
			direction,
			this->bulletSpeed, this->bulletMaxSpeed, this->getDamage(), false, this->bulletAcceleration)
	);
	if (dualMissiles01) {
		this->bullets.Add(
			Bullet(missile01ProjectileTexture,
				missileScale,
				Vector2f(this->playerCenter.x, this->playerCenter.y + (this->sprite.getGlobalBounds().height / 2)),
				direction,
				this->bulletSpeed, this->bulletMaxSpeed, this->getDamage(), false, this->bulletAcceleration)
		);
	}
}

void Player::_fireMissileHeavy(const Vector2f direction) {
	if (dualMissiles02) {
		// setup
	}
}

void Player::_checkBounds(Vector2u windowBounds, bool warpVertical) {
	if (this->getPosition().x <= 0) {// LEFT BOUNDS
		this->sprite.setPosition(0.f, this->sprite.getPosition().y);
		this->velocity.x = 0.f;
	}
	else if (this->getPosition().x + this->sprite.getGlobalBounds().width >= windowBounds.x) { // RIGHT BOUNDS
		this->sprite.setPosition(windowBounds.x - this->sprite.getGlobalBounds().width, this->sprite.getPosition().y);
		this->velocity.x = 0.f;
	}

	if (warpVertical) {
		if (this->getPosition().y + this->sprite.getGlobalBounds().height <= 0) { // TOP BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, (float)windowBounds.y);
		}
		else if (this->getPosition().y >= windowBounds.y) { // BOTTOM BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, 0.f - this->sprite.getGlobalBounds().height);
		}
	}
	else {
		if (this->getPosition().y <= 0) { // TOP BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, 0.f);
			this->velocity.y = 0.f;
		}
		else if (this->getPosition().y + this->sprite.getGlobalBounds().height >= windowBounds.y) { // BOTTOM BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, windowBounds.y - this->sprite.getGlobalBounds().height);
			this->velocity.y = 0.f;
		}
	}
}
