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
	this->direction = direction; // (1,0)
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
		// You can do this because direction is a vector...
		//this->text.move(this->direction * this->speed * dt * DeltaTime::dtMultiplier);
		this->text.move(this->direction.x * curSpeed * dt * DeltaTime::dtMultiplier, this->direction.y * curSpeed * dt * DeltaTime::dtMultiplier);
	}
	else {
		// We are never deleteing these from the vector<TextTag>
		this->text.setFillColor(Color(0, 0, 0, 0));
	}


}
void TextTag::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->text);
}
