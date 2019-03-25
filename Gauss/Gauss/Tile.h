#pragma once

#include "GameObject.h"

class Tile
{
private:
	// Core
	bool colliderType;
	bool damageType;
	int damage;

	// Positioning
	Vector2f position;

	// Visuals
	Sprite sprite;

public:
	Tile(IntRect textureRect, 
		Vector2f position,
		bool collisionType, 
		bool damageType );
	virtual ~Tile();

	inline const Vector2f getPos() const { return this->sprite.getPosition(); }
	inline const FloatRect getBounds() const { return this->sprite.getGlobalBounds(); }
	inline const bool isDamageType() const { return this->damageType; }
	inline const bool isColliderType() const { return this->colliderType; }
	inline void setPos(float x, float y) { this->sprite.setPosition(Vector2f(x, y)); }

	void Update();
	void Draw(RenderTarget &renderTarget);

	// Statics
	static Texture tileTextures;

};

