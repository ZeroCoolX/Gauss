#include "Enemy.h"
#include "Enums.h"

Enemy::Enemy(dArr<Texture> &textures,
	int type,
	Vector2u windowBounds,
	Vector2f scale,
	Vector2f direction,
	int hpMax,
	Vector2i damageRange,
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

	this->hpMax = hpMax;
	this->hp = this->hpMax;

	this->playerFollowNum = playerFollowNum;
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage(int damage){
	this->hp = std::max(0, (this->hp - damage));

	// Damage effect
	this->damageTimer = this->damageTimerMax;
}

void Enemy::Update(const float &dt, Vector2f playerPosition){
	Vector2f normalizedDir;
	float adjustedMoveSpeed;
	switch (this->type) {
		case GameEnums::E_MOVE_LEFT:
			this->sprite.move(this->direction.x * this->moveSpeed * dt * DeltaTime::dtMultiplier, this->direction.y * this->moveSpeed * dt * DeltaTime::dtMultiplier);
			break;
		case GameEnums::E_FOLLOW:
			// Get distance to see if we need to divebomb
			float distance = sqrt(pow(this->sprite.getPosition().x - playerPosition.x, 2) + pow(this->sprite.getPosition().y - playerPosition.x, 2));

			if (distance < divebombDistanceThreshold || this->divebombPlayer) {
				this->divebombPlayer = true;
				normalizedDir = this->_normalize(this->direction, this->_vectorLength(this->direction));
				adjustedMoveSpeed = (this->moveSpeed / 2);
			}
			else {
				// Get the direction from us to the player 
				this->direction.x = playerPosition.x - this->sprite.getPosition().x;
				this->direction.y = playerPosition.y - this->sprite.getPosition().y;
				// Normalize direction
				normalizedDir = this->_normalize(this->direction, this->_vectorLength(this->direction));
				// Angle needed to go from current facing dir to face player
				float angle = atan2(normalizedDir.y, normalizedDir.x) * 180.f / 3.14159265359f + 180.f;
				// Rotate
				this->sprite.setRotation(angle);
				adjustedMoveSpeed = (this->moveSpeed / 3);
			}

			this->sprite.move(normalizedDir.x * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier, normalizedDir.y * adjustedMoveSpeed * dt * DeltaTime::dtMultiplier);
			break;
	}

	if (this->damageTimer > 0.f) {
		this->damageTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		this->sprite.setColor(Color::Red);
		this->sprite.move(this->moveSpeed/2 * this->damageTimer * dt * DeltaTime::dtMultiplier, 0.f);
	}
	else {
		this->sprite.setColor(Color::White);
	}
}

void Enemy::Draw(RenderTarget &renderTarget){
	renderTarget.draw(this->sprite);
}
