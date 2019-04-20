#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
private:
	bool gaussProjectile = false;
	bool playerProjectile = false;

	// Movement
	Vector2f velocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;

	int type;

	int damage;

	void _constantMovement(const float &dt);
	void _accelerationMovement(const float &dt);

public:
	Bullet(int type,
		Vector2f scale,
		Vector2f position,
		Vector2f direction,
		float initialVelocity,
		float maxVelocity,
		int damage,
		bool gaussShot = false,
		float acceleration = 0.f) : GameObject(&bulletTextures[type], scale) {

		this->type = type;

		// Damage
		this->damage = damage;

		// Init movement
		this->maxVelocity = maxVelocity;
		this->acceleration = acceleration;
		this->direction = direction;
		this->velocity = Vector2f(
			initialVelocity * this->direction.x,
			initialVelocity * this->direction.y);

		// Init origin and position
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2,
			(this->sprite.getGlobalBounds().height / 2) + 20);
		this->sprite.setPosition(position);
		this->sprite.setRotation(atan2(this->direction.y, this->direction.x) * 180.f / MathUtil::PI);
	}
	virtual ~Bullet();

	// Accessors
	inline const Vector2f& getPosition() const { return this->sprite.getPosition(); }
	inline void setPosition(Vector2f position) { this->sprite.setPosition(position); }
	inline const int& getDamage() const { return this->damage; }
	inline const bool& isGaussShot() const { return this->gaussProjectile; }
	inline const bool& isPlayerProjectile() const { return this->playerProjectile; }
	inline void setPlayerProjectile(bool playerProj) { this->playerProjectile = playerProj; }
	inline Vector2f getNormDir() { return this->normalize(this->velocity, this->vectorLength(this->velocity)); }
	inline Vector2f getVelocity() { return this->velocity; }
	inline float getMaxVelocity() { return this->maxVelocity; }
	inline void reverseDirection() { this->velocity.x *= -1; this->velocity.y *= -1; }

	// Vector Utility
	inline float vectorLength(Vector2f v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }
	inline Vector2f normalize(Vector2f v, float length) {
		if (length == 0) { return Vector2f(0.f, 0.f); }
		return (v / length);
	}

	// Functions
	void Move(const float &dt);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);

	// Statics
	enum BulletTypes{ 
		LASER = 0, 
		MISSILE, 
		GAUSS_CANNON, 
		MISSILE_HEAVY, 
		ORB_RED,
		ORB_BLUE
	};
	static dArr<Texture> bulletTextures;

	static void InitTextures();
};

