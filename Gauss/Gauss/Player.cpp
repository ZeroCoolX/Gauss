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
	
	// Assign bullet
	this->bulletTexture = bulletTexture;

	// Setup timers
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = this->damageMax;

	// Set player controls
	this->controls[control_index::UP] = UP;
	this->controls[control_index::DOWN] = DOWN;
	this->controls[control_index::LEFT] = LEFT;
	this->controls[control_index::RIGHT] = RIGHT;
	this->controls[control_index::FIRE] = FIRE;

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
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::UP])))
		this->sprite.move(0.f, -10.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::DOWN])))
		this->sprite.move(0.f, 10.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::LEFT])))
		this->sprite.move(-10.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::RIGHT])))
		this->sprite.move(10.f, 0.f);
}
