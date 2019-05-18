#include "Powerup.h"

dArr<Texture> Powerup::powerupTextures;
void Powerup::InitTextures() {
	Texture temp;

	temp.loadFromFile("Textures/Powerups/powerupRF.png");
	Powerup::powerupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Powerups/powerupXP.png");
	Powerup::powerupTextures.Add(Texture(temp));	
	temp.loadFromFile("Textures/Powerups/powerupPierce02.png");
	Powerup::powerupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Powerups/powerupAbsorber.png");
	Powerup::powerupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Powerups/powerupGrinder.png");
	Powerup::powerupTextures.Add(Texture(temp));

	Powerup::numberOfPowerups = Powerup::powerupTextures.Size();
}

int Powerup::numberOfPowerups;

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
	switch (this->type) {
		case RF:
			player->enablePowerupRF();
			break;
		case XP2:
			player->enablePowerupXP();
			break;
		case ABSORB:
			player->enablePowerupAbsorb();
			break;
		case GRIND:
			player->enablePowerupGrind();
			break;
		case PIERCE:
			player->enablePowerupPiercingShot();
			break;
	}
}
