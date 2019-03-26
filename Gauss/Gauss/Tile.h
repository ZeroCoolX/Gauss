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

	inline const FloatRect getBounds() const { return this->sprite.getGlobalBounds(); }
	inline const bool isDamageType() const { return this->damageType; }
	inline const bool isColliderType() const { return this->colliderType; }
	inline void setPos(float x, float y) { this->sprite.setPosition(Vector2f(x, y)); }
	inline const Vector2f getPos() const { return this->sprite.getPosition(); }
	inline void setTexRect(int x, int y, int width, int height) { this->sprite.setTextureRect(IntRect(x, y, width, height)); }
	inline void setDamageType(bool damageType) { this->damageType = damageType; }
	inline void setColliderType(bool colliderType) { this->colliderType = colliderType; }

	void Update();
	void Draw(RenderTarget &renderTarget);

	// Statics
	static Texture tileTextures;

};

