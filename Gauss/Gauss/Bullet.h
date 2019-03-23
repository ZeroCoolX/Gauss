#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
private:
	// Movement
	Vector2f velocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;

	void _constantMovement(const float &dt);
	void _accelerationMovement(const float &dt);

public:
	Bullet(Texture *texture,
		Vector2f scale,
		Vector2f position,
		Vector2f direction,
		float initialVelocity,
		float maxVelocity,
		float acceleration) : GameObject(texture, scale) {

		// Init movement
		this->maxVelocity = maxVelocity;
		this->acceleration = acceleration;
		this->direction = direction;
		this->velocity = Vector2f(
			initialVelocity * this->direction.x,
			initialVelocity * this->direction.y);

		// Init origin and position
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2,
			(this->sprite.getGlobalBounds().height / 2) + 20); // 20 is a compensation...I swear these sprites used to line up perfectly...After refactoring it seems 10 pixels have changed. Idk how....
		this->sprite.setPosition(position);
		this->sprite.setRotation(atan2(this->direction.y, this->direction.x) * 180.f / MathUtil::PI);
	}
	virtual ~Bullet();

	// Accessors
	inline const Vector2f& getPosition() const { return this->sprite.getPosition(); }
	inline void setPosition(Vector2f position) { this->sprite.setPosition(position); }

	// Functions
	void Move(const float &dt);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);
};

