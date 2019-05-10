#include <algorithm>
#include "Player.h"
#include "EnemyLifeform.h"

unsigned Player::playerId = 0;


//dArr<Texture> Player::shipBulletTextures;
dArr<Texture> Player::shipBodyTextures;
dArr<Texture> Player::shipMainGunTextures;
dArr<Texture> Player::shipLWingTextures;
dArr<Texture> Player::shipRWingTextures;
dArr<Texture> Player::shipCockpitTextures;
dArr<Texture> Player::shipAuraTextures;
dArr<Texture> Player::shipShieldTextures;
dArr<Texture> Player::powerupIndicatorTextures;

void Player::InitTextures() {
	Texture temp;

	// Body textures
	temp.loadFromFile("Textures/ship.png");
	Player::shipBodyTextures.Add(temp);

	// Main Gun textures
	temp.loadFromFile("Textures/Guns/gunStock06.png");
	Player::shipMainGunTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/gunDoubleStock06.png");
	Player::shipMainGunTextures.Add(temp);
	temp.loadFromFile("Textures/Guns/gunTripleStock07.png");
	Player::shipMainGunTextures.Add(temp);

	// Powerup indication textures
	temp.loadFromFile("Textures/Powerups/powerupRFIndicator.png");
	Player::powerupIndicatorTextures.Add(temp);
	temp.loadFromFile("Textures/Powerups/powerupXPIndicator.png");
	Player::powerupIndicatorTextures.Add(temp);
	temp.loadFromFile("Textures/Powerups/powerupAbsorberIndicator.png");
	Player::powerupIndicatorTextures.Add(temp);
	temp.loadFromFile("Textures/Powerups/powerupGrinderIndicator.png");
	Player::powerupIndicatorTextures.Add(temp);
	//temp.loadFromFile("Textures/Powerups/powerupPierce01Indicator.png");
	temp.loadFromFile("Textures/Powerups/powerupPierce02Indicator.png");
	Player::powerupIndicatorTextures.Add(temp);

	// Sheild texutures
	temp.loadFromFile("Textures/shield02.png");
	Player::shipShieldTextures.Add(temp);

	// Ship Parts textures
	std::string accessoriesBaseDir = "Textures/Ships/Player/";
	std::string accessories[] = {
	"leftwings.txt","rightwings.txt","auras.txt","cockpits.txt" };

	std::ifstream in;
	std::string accessoryFileName;
	for (int i = 0; i < 4; ++i)
	{
		in.open(accessoriesBaseDir + accessories[i]);
		if (in.is_open()) {
			while (getline(in, accessoryFileName)) {
				temp.loadFromFile(accessoryFileName);
				switch (i) {
				case 0:
					Player::shipLWingTextures.Add(Texture(temp));
					break;
				case 1:
					Player::shipRWingTextures.Add(Texture(temp));
					break;
				case 2:
					Player::shipAuraTextures.Add(Texture(temp));
					break;
				case 3:
					Player::shipCockpitTextures.Add(Texture(temp));
					break;
				}
			}
		}
		in.close();
	}
}

Player::Player(
	AudioManager* audioManager, KeyManager *keyManager
	/*int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE,
	int GAUSSCANNON,
	int SHIELD,
	int TOGGLESTATS,
	int CHANGE_LWING,
	int CHANGE_CPIT,
	int CHANGE_RWING,
	int CHANGE_AURA*/
) :level(1), lives(1), exp(0), hp(10), hpMax(10), hpAdded(10), shieldAdded(0.f), shieldRechargeRate(0.5f), statPoints(0), cooling(0), maneuverability(0), plating(0), power(0), damage(1), damageMax(2), score(0)
{
	this->audioManager = audioManager;
	// Stats
	this->expNext = 20 + static_cast<int>(
		(50/3)
		*((pow(level,3)-6
			* pow(level, 2)) + 17
			* level -12)
		);

	this->keyTimeMax = 8.f;
	this->keyTime = this->keyTimeMax;

	// Selectors
	this->lWingSelect = 1;
	this->rWingSelect = 1;
	this->cPitSelect = 1;
	this->auraSelect = 0;

	this->enemiesKilled = 0;
	this->score = 0;
	this->highestLevelAchieved = 1;
	this->totalSecondsAlive = 1;

	this->_initTextures();
	this->_initPlayerSettings();

	this->keyManager = keyManager;
	this->keyScheme = 0;

	// Set player controls
	this->controls.Add(this->keyManager->Up(this->keyScheme));
	this->controls.Add(this->keyManager->Down(this->keyScheme));
	this->controls.Add(this->keyManager->Left(this->keyScheme));
	this->controls.Add(this->keyManager->Right(this->keyScheme));
	this->controls.Add(this->keyManager->Fire(this->keyScheme));
	this->controls.Add(this->keyManager->GaussCannon(this->keyScheme));
	this->controls.Add(this->keyManager->Shield(this->keyScheme));
	this->controls.Add(this->keyManager->ToggleStats(this->keyScheme));
	this->controls.Add(Keyboard::Num1);
	this->controls.Add(Keyboard::Num2);
	this->controls.Add(Keyboard::Num3);
	this->controls.Add(Keyboard::Num4);
}

Player::~Player()
{
}

int Player::getDamage() const { 
	int damage = rand() % this->damageMax + this->damage;

	switch (this->currentWeapon) {
		case Player::LASER_GUN:
			// Accept regular damage
			break;
		case Player::MISSILE_LIGHT_GUN:
			damage *= 2;
			break;
		case Player::MISSILE_HEAVY_GUN:
			damage *= 4;
			break;
	}
	return damage; 
}

void Player::TakeDamage(int damage) {
	this->hp = std::max(0, this->hp - damage);
	this->damageTimer = 0;

	this->velocity.x += -this->normalizedDir.x * 10.f; // knockback amount
	this->velocity.y += -this->normalizedDir.y * 10.f; // knockback amount
}

bool Player::ChangeAccessories(const float &dt) {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_CHANGE_LWING]))) {
		this->lWingSelect = ++this->lWingSelect % ((int)Player::shipLWingTextures.Size() - 1);
		this->lWing.setTexture(Player::shipLWingTextures[this->lWingSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_CHANGE_RWING]))) {
		this->rWingSelect = ++this->rWingSelect % ((int)Player::shipRWingTextures.Size() - 1);
		this->rWing.setTexture(Player::shipRWingTextures[this->rWingSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_CHANGE_AURA]))) { // Don't allow anyone to change this
		this->auraSelect = ++this->auraSelect % ((int)Player::shipAuraTextures.Size() - 1);
		this->aura.setTexture(Player::shipAuraTextures[this->auraSelect]);
		return true;
	}else if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_CHANGE_CPIT]))) {
		this->cPitSelect = ++this->cPitSelect % ((int)Player::shipCockpitTextures.Size() - 1);
		this->cPit.setTexture(Player::shipCockpitTextures[this->cPitSelect]);
		return true;
	}
	return false;
}

void Player::Movement(const float &dt, View &view, const float scrollSpeed) {
	// Update normalized direction
	this->normalizedDir = this->normalize(this->velocity, this->vectorLength(this->velocity));

	this->_processPlayerInput(dt);

	// Move player
	const float speedPlusViewX = (scrollSpeed * dt * DeltaTime::dtMultiplier) + (this->velocity.x * dt * DeltaTime::dtMultiplier);
	const float speedPlusViewY = (this->velocity.y * dt * DeltaTime::dtMultiplier);
	this->sprite.move(speedPlusViewX, speedPlusViewY);

	this->_recalculatePlayerCenter();

	// Bounds check for window collision
	if (!(this->gameOverOverride || this->tutorialOverride)) {
		this->_checkBounds(view, this->warpVerticalBounds);
	}
}

void Player::Combat(const float &dt) {
	const Vector2f direction = Vector2f(1.f, 0.f);
	
	// Main laser
	if (!this->disableLaser) {
		if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_FIRE])) && this->shootTimer >= this->shootTimerMax)
		{
			this->audioManager->PlaySound(AudioManager::AudioSounds::PLAYER_LASER);
			switch (this->currentWeapon) {
			case Player::LASER_GUN:
				this->_fireLaser(direction);
				break;
			case Player::MISSILE_LIGHT_GUN:
				this->_fireMissileLight(direction);
				break;
			case Player::MISSILE_HEAVY_GUN:
				this->_fireMissileHeavy(direction);
				break;
			}

			this->shootTimer = 0; // RESET TIMER
		}
	}

	// Gauss Cannon
	if (!this->disableGaussCannon) {
		if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_GAUSSCANNON])) && this->gaussChargeTimer >= this->gaussChargeTimerMax) {
			this->audioManager->PlaySound(AudioManager::AudioSounds::GAUSS_CANNON);
			// Fire a deadly gauss cannon shot
			this->gaussChargeTimer = 0.f;
			if (!this->colorFlashTutorialOverride) {
				this->playerGaussBar.setFillColor(this->gaussChargingColor);
			}
			this->_fireGaussCannon(direction);
			this->gaussReloaded = false;
			this->keyTime = 0;
		}
	}

	// Sheild
	if (this->shieldCharged()) {
		if (!this->colorFlashTutorialOverride) {
			this->playerShieldChargeCircle.setFillColor(this->shieldReadyColor);
		}
	}
	if (!this->disableSheild) {
		this->shieldActive = Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_SHIELD])) && this->shieldChargeTimer > 0;
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
	this->statsText.setString("[ " + std::to_string(this->getLevel()) + " ]");

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

	// Gauss Charge bar
	Vector2f gaussChargeBarPos = Vector2f(
		healthBarPos.x - this->playerGaussBar.getGlobalBounds().width - 10.f,
		healthBarPos.y + this->playerHealthBar.getGlobalBounds().height);
	this->playerGaussBar.setPosition(gaussChargeBarPos);
	float gaussCharge = this->gaussChargeTimer / this->gaussChargeTimerMax;
	this->playerGaussBar.setScale(1.f, gaussCharge);

	// Shield Charge circle
	Vector2f shieldChargePos = Vector2f(this->getPosition().x + this->getGlobalBounds().width, (this->getPosition().y - this->getGlobalBounds().height + (this->playerShieldChargeCircleBorder.getRadius() * 2)));
	this->playerShieldChargeCircle.setPosition(shieldChargePos);
	// Scale based off player experience to create a dynamic bar
	float circleScale = this->shieldChargeTimer / this->shieldChargeTimerMax;
	this->playerShieldChargeCircle.setScale(circleScale, circleScale);
	playerShieldChargeCircleBorder.setPosition(shieldChargePos);

	// Powerup Indicator
	if (this->isAnyPowerupActive()) {
		Vector2f powerupSpritePos = Vector2f(shieldChargePos.x - 45.f, shieldChargePos.y - 7.f);
		this->powerupSprite.setPosition(powerupSpritePos);
		const float indicatorAlpha = (this->powerupTimer / this->powerupTimerMax) * 255.f;
		this->powerupSprite.setColor(Color(this->powerupSprite.getColor().r, this->powerupSprite.getColor().g, this->powerupSprite.getColor().b, static_cast<int>(indicatorAlpha)));
	}

	if (this->PlayerShowStatsIsPressed()) {
		this->playerStatsHudText.setString(this->GetStatsAsString());
		this->playerStatsTextBox.setPosition(this->getPosition().x, this->getPosition().y + 150.f);
		this->playerStatsTextBox.setSize(Vector2f(this->playerStatsHudText.getGlobalBounds().width, this->playerStatsHudText.getGlobalBounds().height));
		this->playerStatsHudText.setPosition(this->playerStatsTextBox.getPosition());
	}
}

void Player::UpdateAccessories(const float &dt, const float scollSpeed) {
	// Update the position of the gun to track the player
	this->mainGunSprite.setPosition(
		this->mainGunSprite.getPosition().x,
		this->playerCenter.y);

	// Compensate after fire kickback
	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 6;
	if (this->mainGunSprite.getPosition().x < origin) {
		this->mainGunSprite.move((scollSpeed * dt * DeltaTime::dtMultiplier) + (this->mainGunReturnSpeed + this->velocity.x) * dt * DeltaTime::dtMultiplier, 0.f);

	}
	if (this->mainGunSprite.getPosition().x > origin) {
		this->mainGunSprite.setPosition(
			origin,
			this->playerCenter.y);
	}

	// Shield
	float shieldScale = ((this->shieldChargeTimer + (this->shieldChargeTimerMax / 2)) / this->shieldChargeTimerMax);
	if (shieldScale > 1.f) {
		shieldScale = 1.f;
	}
	this->deflectorShield.setScale(shieldScale, shieldScale);
	this->deflectorShield.setPosition(this->playerCenter);

	// Left Wing
	float xMovement = (this->velocity.x < 0 ? -abs(this->velocity.x) : abs(this->velocity.x / 3));
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
	this->aura.rotate(-15.f * dt * DeltaTime::dtMultiplier);

	// Cockpit
	this->cPit.setPosition(this->playerCenter.x - this->velocity.x / 3, this->playerCenter.y);
}

void Player::UpdatePowerups(const float &dt) {
	if (this->powerupTimer <= 0.f) {
		this->powerupRF = false;
		this->powerupXP = false;
		this->powerupAbsorb = false;
		this->powerupGrind = false;
		this->audioManager->StopSound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);
		this->powerupPiercingShot = false;
	}
}

bool Player::UpdateLeveling() {
	if (this->exp >= this->expNext) {
		this->audioManager->PlaySound(AudioManager::AudioSounds::LEVEL_UP);
		this->level++;
		this->highestLevelAchieved = this->level;
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
	this->shieldChargeTimerMax = 300.f + this->shieldAdded + (this->cooling * (this->maneuverability / 2));
	this->shieldRechargeRate = 0.5f + (static_cast<float>(this->cooling) / 10.f);
}

void Player::Update(View &view, const float &dt, const float scrollSpeed) {
	this->shootTimerMax = this->getCalculatedShootTimer();

	// Update timers
	if (this->cosmoEffectTimer < this->cosmoEffectTimerMax) {
		this->cosmoEffectTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		if (this->effectedByCosmo) {
			this->effectedByCosmo = false;
			switch (this->currentCosmoEffect) {
			case CosmoEffects::INVERT_CONTROLS_EFFECT:
				this->invertControlsEffectActive = false;
				this->ToggleInvertControlsEffect();
				break;
			case CosmoEffects::VERTICAL_WARP_EFFECT:
				this->ToggleWarpBoundsEffect(false);
				break;
			case CosmoEffects::SPEED_INCREASE_EFFECT:
				this->ToggleSpeedIncreaseEffect(false);
				break;
			case CosmoEffects::SPEED_DECREASE_EFFECT:
				this->ToggleSpeedDecreaseEffect(false);
				break;
			case CosmoEffects::SWAP_BADDIE_TYPES_EFFECT:
				this->ToggleBaddieSwap(false);
				break;
			case CosmoEffects::VISUAL_DISTORTION_EFFECT:
				break;
			}
		}
	}
	// Once the game over state is initiated count down and end
	// ONLY countdown then
	if (this->gameOverOverride && this->gameOverTimer > 0.f) {
		this->gameOverTimer -= 1.f * dt * DeltaTime::dtMultiplier;
	}

	if (this->shootTimer < this->shootTimerMax) {
		this->shootTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->damageTimer < this->damageTimerMax) {
		this->damageTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}

	if (this->powerupTimer >= 0.f) {
		this->powerupTimer -= 1.f * dt * DeltaTime::dtMultiplier;
	}

	if (this->gaussChargeTimer < this->gaussChargeTimerMax) {
		this->gaussChargeTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		if (!this->colorFlashTutorialOverride) {
			this->playerGaussBar.setFillColor(this->gaussReadyColor);
		}
		if (!this->gaussReloaded) {
			this->audioManager->PlaySound(AudioManager::AudioSounds::GAUSS_CANNON_RELOAD);
			this->gaussReloaded = true;
		}
	}

	// Depletes at rate n. Charges at rate n/2
	if (shieldActive) {
		this->shieldChargeTimer = std::max(0.f, this->shieldChargeTimer - 1.f * dt * DeltaTime::dtMultiplier);
	}
	// Make sure they let go of the key
	else if(!Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_SHIELD]))){
		this->shieldChargeTimer = std::min(this->shieldChargeTimerMax, this->shieldChargeTimer + this->shieldRechargeRate * dt * DeltaTime::dtMultiplier);
		if (!this->colorFlashTutorialOverride) {
			this->playerShieldChargeCircle.setFillColor(this->shieldChargingColor);
		}
	}

	this->Movement(dt, view, scrollSpeed);
	this->UpdateAccessories(dt, scrollSpeed);
	this->UpdatePowerups(dt);
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

	// Gauss Charge bar
	renderTarget.draw(this->playerGaussBar);

	// Shield charge circle border
	renderTarget.draw(this->playerShieldChargeCircleBorder);
	// Shield charge circle
	renderTarget.draw(this->playerShieldChargeCircle);

	if (this->PlayerShowStatsIsPressed()) {
		renderTarget.draw(this->playerStatsTextBox);
		renderTarget.draw(this->playerStatsHudText);
	}
}

void Player::Draw(RenderTarget &renderTarget) {
	// Only draw the aura if we have something to show
	if (this->powerupAbsorb || this->powerupGrind) {
		renderTarget.draw(this->aura);
	}

	for (size_t i = 0; i < this->bullets.Size(); ++i) {
		this->bullets[i].Draw(renderTarget);
	}

	renderTarget.draw(this->mainGunSprite);
	renderTarget.draw(this->sprite);

	renderTarget.draw(this->cPit);
	renderTarget.draw(this->lWing);
	renderTarget.draw(this->rWing);

	if (this->shieldActive) {
		renderTarget.draw(this->deflectorShield);
	}

	if (this->isAnyPowerupActive()) {
		renderTarget.draw(this->powerupSprite);
	}

	this->DrawUI(renderTarget);
}

void Player::InitUI(Text t) {
	this->statsText = t;

	// EXP bad
	this->playerExpBarColor = Color(0, 90, 200, 200);
	this->playerExpBar.setSize(Vector2f(90.f, 8.5)); 
	this->playerExpBar.setFillColor(this->playerExpBarColor);

	// Health bar
	this->playerHealthBarColor = Color(180, 0, 18, 200);
	this->playerHealthBar.setSize(Vector2f(90.f, 8.0));
	this->playerHealthBar.setFillColor(this->playerHealthBarColor);

	// Setup Bar Boxes
	this->playerHealthBarBox.setSize(Vector2f(91.f, 9.5));
	this->playerHealthBarBox.setFillColor(Color(255, 255, 255, 0));
	this->playerHealthBarBox.setOutlineThickness(1);
	this->playerHealthBarBox.setOutlineColor(Color(255, 255, 255, 10));

	this->playerExpBarBox.setSize(Vector2f(91.f, 9.5));
	this->playerExpBarBox.setFillColor(Color(255, 255, 255, 0));
	this->playerExpBarBox.setOutlineThickness(1);
	this->playerExpBarBox.setOutlineColor(Color(255, 255, 255, 10));


	// Gauss cannon charge bar
	this->gaussReadyColor = Color(0, 220, 93, 150);
	this->gaussChargingColor = Color(0, 86, 37, 150);

	this->playerGaussBar.setSize(Vector2f(8.5f, -30.f));
	this->playerGaussBar.setFillColor(this->gaussChargingColor);

	// Shield charge circle
	this->shieldReadyColor = Color(250, 17, 200, 150);
	this->shieldChargingColor = Color(145, 49, 125, 150);

	this->playerShieldChargeCircle.setRadius(10);
	this->playerShieldChargeCircle.setFillColor(this->shieldChargingColor);
	this->playerShieldChargeCircle.setOrigin(Vector2f(this->playerShieldChargeCircle.getRadius(), this->playerShieldChargeCircle.getRadius()));

	this->playerShieldChargeCircleBorder.setRadius(10);
	this->playerShieldChargeCircleBorder.setFillColor(Color(255, 255, 255, 0));
	this->playerShieldChargeCircleBorder.setOutlineColor(Color(255, 255, 255, 10));
	this->playerShieldChargeCircleBorder.setOutlineThickness(1);
	this->playerShieldChargeCircleBorder.setOrigin(Vector2f(this->playerShieldChargeCircleBorder.getRadius(), this->playerShieldChargeCircleBorder.getRadius()));

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

void Player::UpgradeGunLevel(int gunLevel) {
	this->audioManager->PlaySound(AudioManager::AudioSounds::UPGRADE_COLLECT);
	this->SetGunLevel(gunLevel);
}

void Player::SetGunLevel(int gunLevel) {
	this->mainGunLevel = gunLevel;
	if (this->mainGunLevel >= (int)Player::shipMainGunTextures.Size()) {
		std::cout << "ERROR! No texture for gun level " << gunLevel << std::endl;
	}
	else {
		this->mainGunSprite.setTexture(Player::shipMainGunTextures[this->mainGunLevel]);
	}
}

void Player::Reset() {
	// Reset sprite
	this->sprite.setPosition(Vector2f(500.f, 300.f));

	this->_recalculatePlayerCenter();
	
	// Reset stats
	this->hpMax = 10;
	this->hp = this->hpMax;
	this->hpAdded = 10;
	this->shieldAdded = 0.f;
	this->shieldRechargeRate = 0.5f;
	this->level = 1;
	this->exp = 0;
	this->expNext = 20;
	this->statPoints = 0;
	this->maneuverability = 0;
	this->cooling = 0;
	this->power = 0;
	this->plating = 0;
	this->score = 0;
	this->enemiesKilled = 0;
	this->score = 0;
	this->highestLevelAchieved = 1;
	this->totalSecondsAlive = 1;
	this->damageMax = 2;
	this->UpdateStats();

	// Reset game over state
	this->gameOverOverride = false;
	this->gameOverTimerMax = 150.f;
	this->gameOverTimer = this->gameOverTimerMax;

	// Reset Physics
	this->velocity.x = 0;
	this->velocity.y = 0;
	this->warpVerticalBounds = false;

	// Reset upgrades
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->sheild = false;
	this->shieldActive = false;
	this->powerupPiercingShot = false;
	this->powerupRF = false;
	this->powerupXP = false;
	this->powerupAbsorb = false;
	this->powerupGrind = false;
	this->audioManager->StopSound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);
	this->upgradesAcquired.Clear();

	// Reset weapons
	this->currentWeapon = Player::LASER_GUN;
	this->SetGunLevel(Player::DEFAULT_LASER);
	this->bullets.Clear();
	this->mainGunLevel = Player::DEFAULT_LASER;

	// Reset Timers
	this->effectedByCosmo = false;
	this->invertControlsEffectActive = false;
	this->cosmoEffectTimerMax = 500.f;
	this->cosmoEffectTimer = this->cosmoEffectTimerMax;
	this->shootTimerMax = this->getCalculatedShootTimer();
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;
	this->gaussChargeTimerMax = 500.f;
	this->gaussChargeTimer = 0.f;
	this->gaussReloaded = false;
	this->shieldChargeTimerMax = 300.f + (this->cooling * 5) + (this->maneuverability / 2);
	this->shieldChargeTimer = this->shieldChargeTimerMax;
	// Powerups Timer
	this->powerupTimerMax = 500.f;
	this->powerupTimer = this->powerupTimerMax;

	// Undo any tutorial overrides
	this->enableAllControls();
}
// Over time these two menthods do the same thing ^ and v
void Player::ResetOnLifeLost(View &view) {
	// Reset sprite
	this->sprite.setPosition(view.getCenter());
	this->_recalculatePlayerCenter();

	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 6;
	this->mainGunSprite.setPosition(
		origin,
		this->playerCenter.y);

	// Reset Physics
	this->velocity.x = 0;
	this->velocity.y = 0;

	// Reset game over state
	this->gameOverOverride = false;
	this->gameOverTimerMax = 150.f;
	this->gameOverTimer = this->gameOverTimerMax;

	// Reset upgrades
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->sheild = false;
	this->shieldActive = false;
	this->powerupPiercingShot = false;
	this->powerupRF = false;
	this->powerupXP = false;
	this->powerupAbsorb = false;
	this->powerupGrind = false;
	this->audioManager->StopSound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);

	this->upgradesAcquired.Clear();

	// Reset weapons
	this->currentWeapon = Player::LASER_GUN;
	this->SetGunLevel(Player::DEFAULT_LASER);
	this->bullets.Clear();
	this->mainGunLevel = Player::DEFAULT_LASER;

	// Reset Timers
	this->shootTimerMax = this->getCalculatedShootTimer();
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;
	this->gaussChargeTimerMax = 500.f;
	this->gaussChargeTimer = 0.f;
	this->gaussReloaded = false;
	this->shieldChargeTimerMax = 300.f + (this->cooling * 5) + (this->maneuverability / 2);
	this->shieldChargeTimer = this->shieldChargeTimerMax;
	// Powerups Timer
	this->powerupTimerMax = 500.f;
	this->powerupTimer = this->powerupTimerMax;

	this->hp = this->hpMax;
	this->UpdateStats();

	// Undo any tutorial overrides
	this->enableAllControls();
}

void Player::AddStatPointRandom() {
	this->audioManager->PlaySound(AudioManager::AudioSounds::UPGRADE_COLLECT);
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
	return Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_TOGGLE_STATS]));
}

std::string Player::GetStatsAsString() {
	return
		"Health: " + std::to_string(this->hp) + "/" + std::to_string(this->hpMax) + " (+ " + std::to_string(this->hpAdded) + ")" +
		"\nDamage: " + std::to_string(this->damage) + "/" + std::to_string(this->damageMax) +
		"\nPower: " + std::to_string(this->power) +
		"\nPlating: " + std::to_string(this->plating) +
		"\nManeuverability: " + std::to_string(this->maneuverability) +
		"\nCooling: " + std::to_string(this->cooling) +
		"\nShield Capacity: " + std::to_string(this->shieldChargeTimerMax) +
		"\nShield Recharge Rate: " + std::to_string(this->shieldRechargeRate);
}

std::string Player::ApplyCosmoEffect() {
	std::string effectText = "";

	if (this->effectedByCosmo) {
		// we're already effected so disregard this for now
		return effectText;
	}

	int cosmoEffect = rand() % 5; // number of possible cosmo effecs

	this->effectedByCosmo = true;
	this->cosmoEffectTimer = 0.f;

	// do some stuff
	switch (cosmoEffect) {
	case CosmoEffects::INVERT_CONTROLS_EFFECT:
		this->ToggleInvertControlsEffect();
		this->currentCosmoEffect = CosmoEffects::INVERT_CONTROLS_EFFECT;
		this->invertControlsEffectActive = true;
		this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_INVERT_CONTROLS);
		effectText = "UP = DOWN and DOWN = UP!";
		break;
	case CosmoEffects::VERTICAL_WARP_EFFECT:
		this->currentCosmoEffect = CosmoEffects::VERTICAL_WARP_EFFECT;
		this->ToggleWarpBoundsEffect(true);
		this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_WARP);
		effectText = "VERTICAL bounds TELEPORT!";
		break;
	case CosmoEffects::SPEED_INCREASE_EFFECT:
		this->currentCosmoEffect = CosmoEffects::SPEED_INCREASE_EFFECT;
		this->ToggleSpeedIncreaseEffect(true);
		this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_SPEED_UP);
		effectText = "SPEED INCREASED!";
		break;
	case CosmoEffects::SPEED_DECREASE_EFFECT:
		this->currentCosmoEffect = CosmoEffects::SPEED_DECREASE_EFFECT;
		this->ToggleSpeedDecreaseEffect(true);
		this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_SPEED_DOWN);
		effectText = "SPEED DECREASED!";
		break;
	case CosmoEffects::SWAP_BADDIE_TYPES_EFFECT:
		this->currentCosmoEffect = CosmoEffects::SWAP_BADDIE_TYPES_EFFECT;
		this->ToggleBaddieSwap(true);
		this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_BADDIE_SWAP);
		effectText = "BADDIE SWAP!";
		break;
	case CosmoEffects::VISUAL_DISTORTION_EFFECT:
		this->currentCosmoEffect = CosmoEffects::VISUAL_DISTORTION_EFFECT;
		effectText = "I DON'T KNOW YET!!!!";
		break;
	}
	return effectText;
}

void Player::ResetAllCosmoEffects() {
	if(this->invertControlsEffectActive){
		this->ToggleInvertControlsEffect();
	}
	this->ToggleWarpBoundsEffect(false);
	this->ToggleSpeedIncreaseEffect(false);
	this->ToggleSpeedDecreaseEffect(false);
	this->ToggleBaddieSwap(false);
}

void Player::ToggleInvertControlsEffect() {
	int temp;
	temp = this->controls[Player::CONTROL_UP];
	this->controls[Player::CONTROL_UP] = this->controls[Player::CONTROL_DOWN];
	this->controls[Player::CONTROL_DOWN] = temp;
}

void Player::ToggleWarpBoundsEffect(bool warp) {
	this->warpVerticalBounds = warp;
}

void Player::ToggleSpeedIncreaseEffect(bool on) {
	if (on) {
		this->maxVelocity *= 2;
		this->acceleration = 1.5f;
	}
	else {
		this->maxVelocity = 15.f;
		this->acceleration = 1.f;
	}
}

void Player::ToggleSpeedDecreaseEffect(bool on) {
	if (on) {
		this->maxVelocity /= 2;
	}
	else {
		this->maxVelocity = 15.f;
	}
}

void Player::ToggleBaddieSwap(bool swap) {
	EnemyLifeform::hotSwap = swap;
}

void Player::ActivateGameEnd(Vector2f pos) {
	// Set player into the middle of the screen
	this->sprite.setPosition(pos);
	this->_recalculatePlayerCenter();

	const float origin = this->playerCenter.x + this->sprite.getGlobalBounds().width / 6;
	this->mainGunSprite.setPosition(
		origin,
		this->playerCenter.y);

	// turn off player input for WASD
	this->gameOverOverride = true;
}

void Player::_processPlayerInput(const float &dt) {
	if (this->gameOverOverride || this->tutorialOverride) {
		this->velocity.y = 0.f;
		this->velocity.x = this->maxVelocity * 10.f * dt * DeltaTime::dtMultiplier;
		return;
	}

	// Collect player input

	// Vertical
	if (!this->disableVertical) {
		if (this->warpVerticalBounds) {
			if (this->velocity.y > -this->maxVelocity) {
				this->velocity.y += -1.f * this->acceleration * dt * DeltaTime::dtMultiplier;
			}
		}
		else {
			if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_UP]))) {
				this->direction.y = -1;
				if (this->velocity.y > -this->maxVelocity && this->direction.y < 0) {
					this->velocity.y += direction.y * this->acceleration * dt * DeltaTime::dtMultiplier;
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_DOWN]))) {
				this->direction.y = 1;
				if (this->velocity.y < this->maxVelocity && this->direction.y > 0) {
					this->velocity.y += direction.y * this->acceleration * dt * DeltaTime::dtMultiplier;
				}
			}
		}
	}

	// Horizontal
	if (!this->disableHorizontal) {
		if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_LEFT]))) {
			this->direction.x = -1;
			if (this->velocity.x > -this->maxVelocity && this->direction.x < 0) {
				this->velocity.x += direction.x * this->acceleration * dt * DeltaTime::dtMultiplier;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[Player::CONTROL_RIGHT]))) {
			this->direction.x = 1;
			if (this->velocity.x < this->maxVelocity && this->direction.x > 0) {
				this->velocity.x += direction.x * this->acceleration * dt * DeltaTime::dtMultiplier;
			}
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

void Player::_initTextures() {
	// Assign ship
	this->sprite.setTexture(Player::shipBodyTextures[Player::DEFAULT_SHIP_BODY]);
	this->sprite.setScale(0.1f, 0.1f);
	this->sprite.setColor(Color(10, 10, 10, 255));
	this->sprite.setPosition(Vector2f(500.f, 300.f));

	this->_recalculatePlayerCenter();

	// Assign main gun
	this->mainGunSprite.setTexture(Player::shipMainGunTextures[Player::DEFAULT_LASER]);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2);
	this->mainGunSprite.rotate(90);
	const float originX = this->playerCenter.x + this->sprite.getGlobalBounds().width / 6;
	// Update the position of the gun to track the player
	this->mainGunSprite.setPosition(
		originX,
		this->playerCenter.y);

	// Assign sheild
	this->deflectorShield.setTexture(Player::shipShieldTextures[Player::DEFAULT_SHIELD]);
	this->deflectorShield.setOrigin(this->deflectorShield.getGlobalBounds().width / 2.f, this->deflectorShield.getGlobalBounds().height / 2.f);
	this->deflectorShield.setPosition(this->sprite.getPosition());

	// Accessories
	// Left wing
	this->lWing.setTexture(Player::shipLWingTextures[this->lWingSelect]);
	this->lWing.setOrigin(this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2);
	this->lWing.setPosition(this->playerCenter);
	this->lWing.setRotation(90.f);
	this->lWing.setScale(0.7f, 0.7f);

	// Right wing
	this->rWing.setTexture(Player::shipRWingTextures[this->rWingSelect]);
	this->rWing.setOrigin(this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2);
	this->rWing.setPosition(this->playerCenter);
	this->rWing.setRotation(90.f);
	this->rWing.setScale(0.7f, 0.7f);

	// Aura
	this->aura.setTexture(Player::shipAuraTextures[this->auraSelect]);
	this->aura.setPosition(this->playerCenter);
	this->aura.setScale(0.85f, 0.85f);
	this->aura.setOrigin(100.f, 100.f);

	// Cockpit
	this->cPit.setTexture(Player::shipCockpitTextures[this->cPitSelect]);
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

	// Setup cosmo properties
	this->invertControlsEffectActive = false;
	this->effectedByCosmo = false;
	this->cosmoEffectTimerMax = 500.f;
	this->cosmoEffectTimer = this->cosmoEffectTimerMax;

	// Setup timers
	this->shootTimerMax = this->getCalculatedShootTimer();
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;
	this->gaussChargeTimerMax = 500.f;
	this->gaussChargeTimer = 0.f;
	this->gaussReloaded = false;
	this->shieldChargeTimerMax = 300.f + (this->cooling * 5) + (this->maneuverability / 2);
	this->shieldChargeTimer = this->shieldChargeTimerMax;
	// Powerups Timer
	this->powerupTimerMax = 500.f;
	this->powerupTimer = this->powerupTimerMax;

	// Movement settings
	this->maxVelocity = 15.f;
	this->acceleration = 1.f;
	this->stabalizingForce = 0.3f;
	this->warpVerticalBounds = false;

	// Reset game over state
	this->gameOverOverride = false;
	this->tutorialOverride = false;
	this->gameOverTimerMax = 150.f;
	this->gameOverTimer = this->gameOverTimerMax;

	// WEAPON
	this->currentWeapon = Player::LASER_GUN;

	// UPGRADES
	this->SetGunLevel(Player::DEFAULT_LASER);
	this->dualMissiles01 = false;
	this->dualMissiles02 = false;
	this->sheild = false;
	this->shieldActive = false;
	this->powerupPiercingShot = false;
	this->powerupRF = false;
	this->powerupXP = false;
	this->powerupAbsorb = false;
	this->powerupGrind = false;
	this->audioManager->StopSound(AudioManager::AudioSounds::POWERUP_GRIND_IDLE);

	this->enableAllControls();
	this->deactivateColorFlashTutorialOverride();

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
	float yOffset = this->mainGunLevel == Player::DEFAULT_LASER ? 0.f : this->mainGunLevel > Player::LEVEL_2_LASER ? 30.f : 15.f;
	for (int i = 0; i <= this->mainGunLevel; i++)
	{
		this->bullets.Add(
			Bullet(Bullet::LASER,
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
			Bullet(Bullet::GAUSS_CANNON,
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
		Bullet(Bullet::MISSILE,
			missileScale,
			Vector2f(this->playerCenter.x, this->playerCenter.y - (this->sprite.getGlobalBounds().height / 2)),
			direction,
			this->bulletSpeed, this->bulletMaxSpeed, this->getDamage(), false, this->bulletAcceleration)
	);
	if (dualMissiles01) {
		this->bullets.Add(
			Bullet(Bullet::MISSILE,
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

void Player::_checkBounds(View &view, bool warpVertical) {
	float viewLeft = view.getCenter().x - (view.getSize().x / 2.f);
	float viewRight = view.getCenter().x + (view.getSize().x / 2.f);

	if (this->getPosition().x <= viewLeft) {// LEFT BOUNDS
		this->sprite.setPosition(viewLeft + 50.f/*offset so the player is not pinned to wall*/, this->sprite.getPosition().y);
		this->velocity.x = 0.f;
	}
	else if (this->getPosition().x + this->sprite.getGlobalBounds().width >= viewRight) { // RIGHT BOUNDS
		this->sprite.setPosition(viewRight - this->sprite.getGlobalBounds().width, this->sprite.getPosition().y);
		this->velocity.x = 0.f;
	}

	float viewTop = view.getCenter().y - (view.getSize().y/ 2.f);
	float viewBottom = view.getCenter().y + (view.getSize().y / 2.f);

	if (warpVertical) {
		if (this->getPosition().y + this->sprite.getGlobalBounds().height <= viewTop) { // TOP BOUNDS
			this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_WARP);
			this->sprite.setPosition(this->sprite.getPosition().x, viewBottom - this->sprite.getGlobalBounds().height);
		}
		else if (this->getPosition().y >= viewBottom) { // BOTTOM BOUNDS
			this->audioManager->PlaySound(AudioManager::AudioSounds::COSMO_WARP);
			this->sprite.setPosition(this->sprite.getPosition().x, viewTop);
		}
	}
	else {
		if (this->getPosition().y <= viewTop) { // TOP BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, viewTop);
			this->velocity.y = 0.f;
		}
		else if (this->getPosition().y + this->sprite.getGlobalBounds().height >= viewBottom) { // BOTTOM BOUNDS
			this->sprite.setPosition(this->sprite.getPosition().x, viewBottom - this->sprite.getGlobalBounds().height);
			this->velocity.y = 0.f;
		}
	}
}

