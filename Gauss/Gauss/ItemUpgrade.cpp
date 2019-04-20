#include "ItemUpgrade.h"

dArr<Texture> ItemUpgrade::upgradeTextures;
void ItemUpgrade::InitTextures() {
	Texture temp;

	temp.loadFromFile("Textures/Upgrades/statpoint.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/healthtank.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/doubleray.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/tripleray.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/piercingshot.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Upgrades/shield.png");
	ItemUpgrade::upgradeTextures.Add(Texture(temp));
	ItemUpgrade::numberOfUpgrades = ItemUpgrade::upgradeTextures.Size();
}

int ItemUpgrade::numberOfUpgrades;

ItemUpgrade::~ItemUpgrade()
{
}

void ItemUpgrade::Update(const float &dt, Vector2f target) {
	Consumable::Update(dt);
}

void ItemUpgrade::Draw(RenderTarget &renderTarget) {
	Consumable::Draw(renderTarget);
}

void ItemUpgrade::Consume(dArr<TextTag> &tags, Font *font, Player *player) {
	// Add the upgrade if we need to
	if (_shouldAddUpgrade() && !player->hasUpgrade(this->type)) {
		player->getAcquiredUpgrades().Add(this->type);
	}

	switch (this->type) {
	case ItemUpgrade::STATPOINT:
		player->AddStatPointRandom();
		break;
	case ItemUpgrade::DOUBLE_RAY:
		if (player->getGunLevel() < Player::LEVEL_2_LASER) {
			player->SetGunLevel(Player::LEVEL_2_LASER);
		}
		else {
			return;
		}
		break;
	case ItemUpgrade::TRIPLE_RAY:
		if (player->getGunLevel() < Player::LEVEL_3_LASER) {
			player->SetGunLevel(Player::LEVEL_3_LASER);
		}
		else {
			return;
		}
		break;
	case ItemUpgrade::PIERCING_SHOT:
		player->enablePiercingShot();
		break;
	case ItemUpgrade::SHEILD:
		player->upgradeShield();
		break;
	case ItemUpgrade::HEALTH_TANK:
		player->upgradeHP();
		break;
	}

	tags.Add(
		TextTag(
			font,
			Vector2f(player->getPosition()),
			this->text, this->color,
			this->moveDir,
			this->charSize, this->lifetime, this->accelerate
		)
	);
}

bool ItemUpgrade::_shouldAddUpgrade() {
	return this->type != ItemUpgrade::STATPOINT && this->type != ItemUpgrade::HEALTH_TANK && this->type != ItemUpgrade::SHEILD;
}

