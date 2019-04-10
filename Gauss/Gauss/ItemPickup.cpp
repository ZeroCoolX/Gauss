#include "ItemPickup.h"

// Static define
dArr<Texture> ItemPickup::pickupTextures;

void ItemPickup::InitTextures() {
	Texture temp;

	// Init pickups textures
	temp.loadFromFile("Textures/Pickups/hpSupply.png");
	ItemPickup::pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileSupply.png");
	ItemPickup::pickupTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Pickups/missileHSupply.png");
	ItemPickup::pickupTextures.Add(Texture(temp));
	ItemPickup::numberOfPickups = ItemPickup::pickupTextures.Size();
}

int ItemPickup::numberOfPickups;

ItemPickup::~ItemPickup()
{
}


void ItemPickup::Update(const float &dt, Vector2f target) {
	Consumable::Update(dt);
}

void ItemPickup::Draw(RenderTarget &renderTarget) {
	Consumable::Draw(renderTarget);
}

void ItemPickup::Consume(dArr<TextTag> &tags, Font *font, Player *player) {
	switch (this->type) {
		case GameEnums::ITEM_HEALTH:
		{
			const int gainedHp = player->getHpMax() / 5;
			if (player->gainHp(gainedHp)) {
				tags.Add(
					TextTag(
						font, Vector2f(player->getPosition().x + player->getGlobalBounds().width / 4,
							player->getPosition().y - player->getGlobalBounds().height),
						"+" + std::to_string(gainedHp) + " HP", Color::Green,
						Vector2f(0.f, 1.f),
						24, 40.f, true
					)
				);
			}
			else {
				if (player->gainExp(10)) {
					tags.Add(
						TextTag(
							font, Vector2f(player->getPosition().x + player->getGlobalBounds().width / 4,
								player->getPosition().y + player->getGlobalBounds().height),
							"LEVEL UP!", Color::Cyan,
							Vector2f(0.f, 1.f),
							36, 40.f, true
						)
					);
				}
				else {
					tags.Add(
						TextTag(
							font, Vector2f(player->getPosition().x + player->getGlobalBounds().width / 4,
								player->getPosition().y + player->getGlobalBounds().height),
							"+" + std::to_string(10) + " EXP", Color::Cyan,
							Vector2f(0.f, 1.f),
							24, 40.f, true
						)
					);
				}
			}
			break;
		}
		case GameEnums::ITEM_MISSILE:
		{
			break;
		}
		case GameEnums::ITEM_MISSILE_HEAVY:
		{
			break;
		}
	}
}
