#include "ItemUpgrade.h"

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
	case GameEnums::UPGRADE_STATPOINT:
		player->AddStatPointRandom();
		break;
	case GameEnums::UPGRADE_DOUBLE_RAY:
		if (player->getGunLevel() < GameEnums::MGT_MAIN_GUN02) {
			player->SetGunLevel(GameEnums::MGT_MAIN_GUN02);
		}
		else {
			return;
		}
		break;
	case GameEnums::UPGRADE_TRIPLE_RAY:
		if (player->getGunLevel() < GameEnums::MGT_MAIN_GUN03) {
			player->SetGunLevel(GameEnums::MGT_MAIN_GUN03);
		}
		else {
			return;
		}
		break;
	case GameEnums::UPGRADE_PIERCING_SHOT:
		player->enablePiercingShot();
		break;
	case GameEnums::UPGRADE_SHEILD:
		player->enableSheild();
		break;
	case GameEnums::UPGRADE_HEALTH_TANK:
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
	return this->type != GameEnums::UPGRADE_STATPOINT && this->type != GameEnums::UPGRADE_HEALTH_TANK;
}

