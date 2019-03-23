#include "Consumable.h"

Consumable::~Consumable()
{
}

bool Consumable::CollidesWith(const FloatRect rect) {
	return this->sprite.getGlobalBounds().intersects(rect);
}

void Consumable::Update(const float &dt, Vector2f target) {
	if (this->lifetimeTimer < this->lifetimeTimerMax) {
		this->lifetimeTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	this->sprite.rotate(5.f * dt * DeltaTime::dtMultiplier);
}

void Consumable::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
}

void Consumable::Consume(dArr<TextTag> &tags, Font *font, Player *player) {
	std::cout << "ERROR! No Consumable::Consume() base implementation" << std::endl;
	throw "ERROR! No Consumable::Consume() base implementation";
}

