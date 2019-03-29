#include "Powerup.h"

dArr<Texture> Powerup::powerupTextures;

Powerup::~Powerup()
{
}

void Powerup::Update(const float &dt, Vector2f target) {
	Consumable::Update(dt);
}

void Powerup::Draw(RenderTarget &renderTarget) {
	Consumable::Draw(renderTarget);
}

void Powerup::Consume(dArr<TextTag> &tags, Font *font, Player *player) {
	std::cout << "Powerup acquired" << std::endl;
}
