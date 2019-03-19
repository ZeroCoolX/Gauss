#include "Enemy.h"
#include "Enums.h"

Enemy::Enemy(dArr<Texture> &textures,
	int type,
	Vector2u windowBounds,
	Vector2f scale,
	Vector2f moveDirection,
	int scalar,
	int playerFollowNum)
{
	this->type = type;

	this->textures = &textures;

	if (this->type >= (int)(*this->textures).Size()) {
		std::cout << "No texture for type: " << this->type << ". ERROR!" << std::endl;
		// Default
		this->type = 0;
	}
	this->sprite.setTexture((*this->textures)[this->type]);
	this->sprite.setScale(scale);

	const float xPos((float)windowBounds.x);
	const float yPos((float)(rand() % windowBounds.y + this->sprite.getGlobalBounds().height));
	this->sprite.setPosition(xPos, yPos);

	this->damageTimerMax = 3.f;
	this->damageTimer = 0;
	this->damageRange = damageRange;
	this->divebombDistanceThreshold = rand() % 700 + 300;

	this->moveDirection = moveDirection;

	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->hpMax = (rand() % 5 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 3 + 1)*scalar, (rand() % 2 + 1)*scalar);
			this->moveSpeed = rand() % 15 + 5.f;
			break;
		case GameEnums::E_FOLLOW:
			this->hpMax = (rand() % 3 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 2 + 1)*scalar, (rand() % 1 + 1)*scalar);
			this->moveSpeed = rand() % 8 + 3.f;
		case GameEnums::E_MOVE_LEFT_SHOOT:
			this->hpMax = (rand() % 3 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 2 + 1)*scalar, (rand() % 1 + 1)*scalar);
			this->moveSpeed = rand() % 10 + 3.f;
		default:
			this->hpMax = (rand() % 3 + 1) * scalar;
			this->damageRange = Vector2i((rand() % 2 + 1)*scalar, (rand() % 1 + 1)*scalar);
			this->moveSpeed = rand() % 10 + 3.f;
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
		{
			this->sprite.move(this->moveDirection.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->moveDirection.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
			this->normalizedMoveDirection = this->_normalize(this->moveDirection, this->_vectorLength(this->moveDirection));
		}
		break;
		case GameEnums::E_FOLLOW:
		{
			// Get distance to see if we need to divebomb
			const float distance = sqrt(pow(this->sprite.getPosition().x - playerPosition.x, 2) + pow(this->sprite.getPosition().y - playerPosition.x, 2));

			if (distance < divebombDistanceThreshold || this->divebombPlayer) {
				this->divebombPlayer = true;
				this->normalizedMoveDirection = this->_normalize(this->moveDirection, this->_vectorLength(this->moveDirection));
				adjustedMoveSpeed = this->moveSpeed;
			}
			else {
				// Get the direction from us to the player 
				this->moveDirection.x = playerPosition.x - this->sprite.getPosition().x;
				this->moveDirection.y = playerPosition.y - this->sprite.getPosition().y;
				// Normalize direction
				this->normalizedMoveDirection = this->_normalize(this->moveDirection, this->_vectorLength(this->moveDirection));
				// Angle needed to go from current facing dir to face player
				float angle = atan2(this->normalizedMoveDirection.y, this->normalizedMoveDirection.x) * 180.f / 3.14159265359f + 180.f;
				// Rotate
				this->sprite.setRotation(angle);
				adjustedMoveSpeed = this->moveSpeed * 0.75f;
			}

			this->sprite.move(this->normalizedMoveDirection.x * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier, this->normalizedMoveDirection.y * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier);
		}
		break;
		case GameEnums::E_MOVE_LEFT_SHOOT:
		{
			this->lookDirection.x = playerPosition.x - this->sprite.getPosition().x;
			this->lookDirection.y = playerPosition.y - this->sprite.getPosition().y;
			this->normalizedLookDirection = this->_normalize(this->lookDirection, this->_vectorLength(this->lookDirection));

			// Rotate
			this->sprite.setRotation(atan2(this->normalizedLookDirection.y, this->normalizedLookDirection.x) * 180.f / 3.14159265359f + 180.f);

			this->sprite.move(this->moveDirection.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->moveDirection.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
			this->normalizedMoveDirection = this->_normalize(this->moveDirection, this->_vectorLength(this->moveDirection));
		}
		break;
	}

	if (this->damageTimer > 0.f) {
		this->sprite.setColor(Color::Red);
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		// Knockback
		this->sprite.move(this->moveSpeed * -this->normalizedMoveDirection.x * this->damageTimer * dt * DeltaTime::dtMultiplier,
			this->moveSpeed * -this->normalizedMoveDirection.y * this->damageTimer * dt * DeltaTime::dtMultiplier);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

void Enemy::Draw(RenderTarget &renderTarget){
	renderTarget.draw(this->sprite);
}
