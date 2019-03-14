#include "Player.h"

enum control_type {UP = 0, DOWN, LEFT, RIGHT, FIRE};

Player::Player(Texture *texture,
	int UP, 
	int DOWN,
	int LEFT, 
	int RIGHT,
	int FIRE
) :level(1), exp(0), expNext(100), hp(10), hpMax(10), damage(1), damageMax(2), score(0)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);

	// Set player controls
	this->controls[control_type::UP] = UP;
	this->controls[control_type::DOWN] = DOWN;
	this->controls[control_type::LEFT] = LEFT;
	this->controls[control_type::RIGHT] = RIGHT;
	this->controls[control_type::FIRE] = FIRE;
}

Player::~Player()
{
}

void Player::Movement() {
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_type::UP])))
		std::cout << "W" << "\n";
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_type::DOWN])))
		std::cout << "S" << "\n";
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_type::LEFT])))
		std::cout << "A" << "\n";
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_type::RIGHT])))
		std::cout << "R" << "\n";
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[control_type::FIRE])))
		std::cout << "SPACE" << "\n";
}

void Player::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
void Player::Update() {
	this->Movement();
}
