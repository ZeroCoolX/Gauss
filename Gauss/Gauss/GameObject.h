#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include "DeltaTime.h"
#include "dArr.h"
#include "MathUtil.h"

using namespace sf;

class GameObject
{
protected:
	Texture *texture;
	Sprite sprite;

public:
	GameObject(Texture *texture, Vector2f spriteScale) {
		this->texture = texture;
		this->sprite.setTexture(*this->texture);
		this->sprite.setScale(spriteScale);
	}
	virtual ~GameObject() {}

	inline FloatRect getGlobalBounds() const { return this->sprite.getGlobalBounds(); }

	// Abstract methods
	virtual void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f)) = 0;
	virtual void Draw(RenderTarget &renderTarget) = 0;
};

