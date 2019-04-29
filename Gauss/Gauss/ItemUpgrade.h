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
		case ItemUpgrade::STATPOINT:
			this->text = "RANDOM STAT POINT UPGRADE";
			break;
		case ItemUpgrade::DOUBLE_RAY:
			this->text = "DOUBLE RAY UPGRADE";
			break;
		case ItemUpgrade::TRIPLE_RAY:
			this->text = "TRIPLE RAY UPGRADE";
			break;
		case ItemUpgrade::SHEILD:
			this->text = "PERMANENT SHEILD UPGRADE";
			break;
		case ItemUpgrade::HEALTH_TANK:
			this->text = "PERMANENT HEALTH UPGRADE";
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

	enum Type { 
		STATPOINT = 0, 
		HEALTH_TANK, 
		DOUBLE_RAY, 
		TRIPLE_RAY, 
		SHEILD 
	};
};

