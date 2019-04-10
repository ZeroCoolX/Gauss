#pragma once

#include "Consumable.h"

using namespace sf;

class ItemUpgrade : public Consumable
{
private:
	std::string text;

	bool _shouldAddUpgrade();
	void _initText() {
		switch (this->type) {
		case GameEnums::UPGRADE_STATPOINT:
			this->text = "RANDOM STAT POINT UPGRADE";
			break;
		case GameEnums::UPGRADE_DOUBLE_RAY:
			this->text = "DOUBLE RAY UPGRADE";
			break;
		case GameEnums::UPGRADE_TRIPLE_RAY:
			this->text = "TRIPLE RAY UPGRADE";
			break;
		case GameEnums::UPGRADE_PIERCING_SHOT:
			this->text = "PIERCING SHOT UPGRADE";
			break;
		case GameEnums::UPGRADE_SHEILD:
			this->text = "SHEILD UPGRADE";
			break;
		case GameEnums::UPGRADE_HEALTH_TANK:
			this->text = "PERMANANT HEALTH UPGRADE";
			break;
		}
	}

public:
	ItemUpgrade(Vector2f position, int type, float lifetime) : Consumable(&upgradeTextures[type], Vector2f(0.75f, 0.75f)) {
		this->type = type;
		this->sprite.setPosition(position);

		this->lifetimeTimerMax = lifetime;
		this->lifetimeTimer = 0.f;

		// SPRITE SETTINGS
		this->color = Color::Yellow;
		this->moveDir = Vector2f(1.f, 0.f);
		this->charSize = 40;
		this->lifetime = 100;
		this->accelerate = true;

		this->_initText();
	}
	virtual ~ItemUpgrade();


	void Consume(dArr<TextTag> &tags, Font *font, Player *player);
	void Update(const float &dt, Vector2f target = Vector2f(0.f, 0.f));
	void Draw(RenderTarget &renderTarget);

	// Statics
	static dArr<Texture> upgradeTextures;
	static void InitTextures();

	static int numberOfUpgrades;
};

