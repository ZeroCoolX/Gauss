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
	switch (this->type) {
	case GameEnums::UT_DOUBLE_RAY:
		if (player->getGunLevel() < GameEnums::MGT_MAIN_GUN02) {
			player->SetGunLevel(GameEnums::MGT_MAIN_GUN02);
		}
		else {
			return;
		}
		break;
	case GameEnums::UT_TRIPLE_RAY:
		if (player->getGunLevel() < GameEnums::MGT_MAIN_GUN03) {
			player->SetGunLevel(GameEnums::MGT_MAIN_GUN03);
		}
		else {
			return;
		}
		break;
	case GameEnums::UT_PIERCING_SHOT:
		player->enablePiercingShot();
		break;
	case GameEnums::UT_SHEILD:
		player->enableSheild();
		break;
	case GameEnums::UT_HEALTH_TANK:
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
