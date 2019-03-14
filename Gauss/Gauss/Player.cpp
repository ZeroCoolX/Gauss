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
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::UP])))
		this->sprite.move(0.f, -10.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::DOWN])))
		this->sprite.move(0.f, 10.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::LEFT])))
		this->sprite.move(-10.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::RIGHT])))
		this->sprite.move(10.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_index::FIRE])))
	{
		// Fix position so that the missile spawns in the middle of the ship (sprite height / 2)
		// Not perfect at the moment but better
		Vector2f fixedPos = Vector2f(this->sprite.getPosition().x, this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height / 2));
		this->bullets.push_back(Bullet(bulletTexture, fixedPos));
	}
}
void Player::Update() {
	this->Movement();

	for (size_t i = 0; i < this->bullets.size(); ++i) {
		this->bullets[i].Update();
	}
}

void Player::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);

	for (size_t i = 0; i < this->bullets.size(); ++i) {
		this->bullets[i].Draw(renderTarget);
	}
}
