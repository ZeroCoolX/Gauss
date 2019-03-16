#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include "DeltaTime.h"

using namespace sf;

class Bullet
{
private:
	Texture *texture;
	Sprite sprite;

	Vector2f velocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;


public:
	Bullet(Texture *texture, 
		Vector2f scale,
		Vector2f position,
		Vector2f direction,
		float initialVelocity,
		float maxVelocity, 
		float acceleration);
	virtual ~Bullet();

	// Accessors
	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f& getPosition() const { return this->sprite.getPosition(); }

	// Functions
	void Movement(const float &dt);
	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

