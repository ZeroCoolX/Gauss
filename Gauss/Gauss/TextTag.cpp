#include "TextTag.h"


TextTag::TextTag(Font *font, std::string text, const Color color, Vector2f position, Vector2f direction, unsigned int characterSize, float timerMax, bool accelerate)
{
	this->text.setFont(*font);
	this->text.setCharacterSize(characterSize);
	this->text.setPosition(position);
	this->text.setString(text);
	this->text.setFillColor(color);

	// Again magic numbers
	this->speed = 2.f;
	this->direction = direction;
	this->timerMax = timerMax;
	this->timer = this->timerMax;
	this->accelerate = accelerate;
}

TextTag::~TextTag()
{
}

void TextTag::Update(const float &dt) {
	// Update timer
	if (this->timer >= 0) {
		this->timer -= 1.f * dt * DeltaTime::dtMultiplier;

		const float curSpeed = this->speed * 
			(this->accelerate && this->timer > this->timerMax / 2) ? 5.f : 1.f;

		this->text.move(this->direction * this->speed * dt * DeltaTime::dtMultiplier);
	}
	else {
		this->text.setFillColor(Color(0, 0, 0, 0));
	}


}
void TextTag::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->text);
}
