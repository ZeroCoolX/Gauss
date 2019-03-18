#include "Enemy.h"
#include "Enums.h"

Enemy::Enemy(dArr<Texture> &textures,
	int type,
	Vector2u windowBounds,
	Vector2f scale,
	Vector2f direction,
	int scalar,
	int playerFollowNum)
{
	this->type = type;

	this->textures = &textures;
	this->sprite.setTexture((*this->textures)[this->type]);
	this->sprite.setScale(scale);

	const float xPos((float)windowBounds.x);
	const float yPos((float)(rand() % windowBounds.y + this->sprite.getGlobalBounds().height));
	this->sprite.setPosition(xPos, yPos);

	this->damageTimerMax = 3.f;
	this->damageTimer = 0;
	this->damageRange = damageRange;
	this->divebombDistanceThreshold = rand() % 700 + 300;

	this->direction = direction;

	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->hpMax = (rand() % 5 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 3 + 1)*scalar, (rand() % 2 + 1)*scalar);
			this->moveSpeed = rand() % 15 + 5.f;
			break;
		case GameEnums::E_FOLLOW:
			this->hpMax = (rand() % 3 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 2 + 1)*scalar, (rand() % 1 + 1)*scalar);
			this->moveSpeed = rand() % 6 + 3.f;
		default:
			this->moveSpeed = 10.f;
			break;
	}
	this->hp = this->hpMax;

	this->playerFollowNum = playerFollowNum;
}

Enemy::~Enemy()
{
}

void Enemy::Collision() {
	this->damageTimer = this->damageTimerMax;
}

void Enemy::TakeDamage(int damage){
	this->hp = std::max(0, (this->hp - damage));

	// Damage effect
	this->damageTimer = this->damageTimerMax;
}

void Enemy::Update(const float &dt, Vector2f playerPosition){
	float adjustedMoveSpeed;
	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->sprite.move(this->direction.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->direction.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
			this->normalizedDir = this->_normalize(this->direction, this->_vectorLength(this->direction));
			break;
		case GameEnums::E_FOLLOW:
			// Get distance to see if we need to divebomb
			float distance = sqrt(pow(this->sprite.getPosition().x - playerPosition.x, 2) + pow(this->sprite.getPosition().y - playerPosition.x, 2));

			if (distance < divebombDistanceThreshold || this->divebombPlayer) {
				this->divebombPlayer = true;
				this->normalizedDir = this->_normalize(this->direction, this->_vectorLength(this->direction));
				adjustedMoveSpeed = this->moveSpeed;
			}
			else {
				// Get the direction from us to the player 
				this->direction.x = playerPosition.x - this->sprite.getPosition().x;
				this->direction.y = playerPosition.y - this->sprite.getPosition().y;
				// Normalize direction
				this->normalizedDir = this->_normalize(this->direction, this->_vectorLength(this->direction));
				// Angle needed to go from current facing dir to face player
				float angle = atan2(this->normalizedDir.y, this->normalizedDir.x) * 180.f / 3.14159265359f + 180.f;
				// Rotate
				this->sprite.setRotation(angle);
				adjustedMoveSpeed = this->moveSpeed * 0.75f;
			}

			this->sprite.move(this->normalizedDir.x * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier, this->normalizedDir.y * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier);
			break;
	}

	if (this->damageTimer > 0.f) {
		this->sprite.setColor(Color::Red);
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		// Knockback
		this->sprite.move(this->moveSpeed * -this->normalizedDir.x * this->damageTimer * dt * DeltaTime::dtMultiplier,
			this->moveSpeed * -this->normalizedDir.y * this->damageTimer * dt * DeltaTime::dtMultiplier);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

void Enemy::Draw(RenderTarget &renderTarget){
	renderTarget.draw(this->sprite);
}
