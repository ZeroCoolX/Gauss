#include <algorithm>
#include "Player.h"

unsigned Player::playerId = 0;

enum control_index {UP = 0, DOWN, LEFT, RIGHT, FIRE};

Player::Player(Texture *texture, Texture *bulletTexture,
	int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE
) :level(1), exp(0), expNext(100), hp(10), hpMax(10), damage(1), damageMax(2), score(0)
{
	// Assign ship
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(0.13f, 0.13f);
	
	// Assign bullet properties
	this->bulletTexture = bulletTexture;
	this->bulletSpeed = 2.f;
	this->bulletMaxSpeed = 50;
	this->bulletAcceleration = 1.f;

	// Setup timers
	this->shootTimerMax = 15;
	this->shootTimer = this->shootTimerMax;
	this->damageMax = 5;
	this->damageTimer = this->damageMax;

	// Set player controls
	this->controls[control_index::UP] = UP;
	this->controls[control_index::DOWN] = DOWN;
	this->controls[control_index::LEFT] = LEFT;
	this->controls[control_index::RIGHT] = RIGHT;
	this->controls[control_index::FIRE] = FIRE;

	this->maxVelocity = 15.f;
	this->acceleration = 1.f;
	this->stabalizingForce = 0.3f;

	this->playerNumber = Player::playerId;
	Player::playerId++;

	std::cout << "Player [" << this->playerNumber << "] Created\n";
}

Player::~Player()
{
}

void Player::Movement() {
	this->processPlayerInput();
}

void Player::Combat() {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::FIRE])) && this->shootTimer >= this->shootTimerMax)
	{
		this->bullets.push_back(Bullet(bulletTexture, this->playerCenter, Vector2f(1.f, 0.f), this->bulletSpeed, this->bulletMaxSpeed, this->bulletAcceleration));
		this->shootTimer = 0; // RESET TIMER
	}
}

void Player::Update(Vector2u windowBounds) {
	// Update timers
	if (this->shootTimer < this->shootTimerMax)
		this->shootTimer++;
	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer++;

	// Update sprite center
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2;

	this->Movement();
	this->Combat();
}

void Player::Draw(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->bullets.size(); ++i) {
		this->bullets[i].Draw(renderTarget);
	}

	renderTarget.draw(this->sprite);
}

void Player::processPlayerInput() {
	// Collect player input
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::UP]))) {
		this->direction.y = -1;
		if (this->velocity.y > -this->maxVelocity && this->direction.y < 0) {
			this->velocity.y += direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::DOWN]))) {
		this->direction.y = 1;
		if (this->velocity.y < this->maxVelocity && this->direction.y > 0) {
			this->velocity.y += direction.y * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::LEFT]))) {
		this->direction.x = -1;
		if (this->velocity.x > -this->maxVelocity && this->direction.x < 0) {
			this->velocity.x += direction.x * this->acceleration;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::RIGHT]))) {
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
