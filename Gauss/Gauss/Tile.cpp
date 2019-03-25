#include "Tile.h"

Texture Tile::tileTextures;

Tile::Tile(IntRect textureRect,
	Vector2f position,
	bool colliderType, 
	bool damageType)
{
	this->position = position;
	this->colliderType = colliderType;
	this->damageType = damageType;

	// Sprite sheet
	this->sprite.setTexture(Tile::tileTextures);
	this->sprite.setTextureRect(textureRect);
	this->sprite.setPosition(this->position);
}


Tile::~Tile()
{
}

void Tile::Update() {

}

void Tile::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
