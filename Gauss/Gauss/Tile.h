#pragma once

#include "GameObject.h"
#include "Gauss.h"

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
		bool damageType);
	virtual ~Tile();

	inline const FloatRect getBounds() const { return this->sprite.getGlobalBounds(); }
	inline const IntRect getTexRect() const { return this->sprite.getTextureRect(); }
	inline const bool isDamageType() const { return this->damageType; }
	inline const bool isColliderType() const { return this->colliderType; }
	inline void setPos(float x, float y) { this->sprite.setPosition(Vector2f(x, y)); }
	inline void move(float x, float y) { this->sprite.move(x, y); }
	inline const Vector2f getPos() const { return this->sprite.getPosition(); }
	inline void setTexRect(int x, int y, int width, int height) { this->sprite.setTextureRect(IntRect(x, y, width, height)); }
	inline void setDamageType(bool damageType) { this->damageType = damageType; }
	inline void setColliderType(bool colliderType) { this->colliderType = colliderType; }
	inline void changeColorTo(const Color color) { this->sprite.setColor(color); }

	std::string GetAsString() const;
	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);

	// Statics
	static Texture tileTextures;

};

