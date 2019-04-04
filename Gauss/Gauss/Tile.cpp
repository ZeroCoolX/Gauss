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
	this->damage = 0;

	// Sprite sheet
	this->sprite.setTexture(Tile::tileTextures);
	this->sprite.setTextureRect(textureRect);
	this->sprite.setPosition(this->position);
}


Tile::~Tile()
{
}

std::string Tile::GetAsString() const {
	return
		// IntRect
		std::to_string(this->sprite.getTextureRect().left) + " " +
		std::to_string(this->sprite.getTextureRect().top) + " " +
		std::to_string(this->sprite.getTextureRect().width) + " " +
		std::to_string(this->sprite.getTextureRect().height) + " " +
		// Vector2f Position
		std::to_string(static_cast<int>(this->sprite.getPosition().x / Gauss::GRID_SIZE)) + " " +
		std::to_string(static_cast<int>(this->sprite.getPosition().y / Gauss::GRID_SIZE)) + " " +
		// Stats
		std::to_string(this->colliderType) + " " +
		std::to_string(this->damageType) + " " +
		std::to_string(this->damage);
}


void Tile::Update(const float &dt) {

}

void Tile::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}
