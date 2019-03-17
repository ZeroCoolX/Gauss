#include "TextTag.h"


TextTag::TextTag(Font *font, std::string text, const Color color, Vector2f position, unsigned int characterSize, float timerMax)
{
	this->text.setFont(*font);
	this->text.setCharacterSize(characterSize);
	this->text.setPosition(position);
	this->text.setString(text);
	this->text.setFillColor(color);

	// Again magic numbers
	this->speed = 2.f;
	this->direction.y = -1.f;
	this->timerMax = timerMax;
	this->timer = this->timerMax;
}

TextTag::~TextTag()
{
}

void TextTag::Update(const float &dt) {
	// Update timer
	if (this->timer >= 0) {
		this->timer -= 1.f * dt * DeltaTime::dtMultiplier;

		// You can do this because direction is a vector...
		//this->text.move(this->direction * this->speed * dt * DeltaTime::dtMultiplier);
		this->text.move(this->direction.x * this->speed * dt * DeltaTime::dtMultiplier, this->direction.y * this->speed * dt * DeltaTime::dtMultiplier);
	}
	else {
		// We are never deleteing these from the vector<TextTag>
		this->text.setFillColor(Color(0, 0, 0, 0));
	}


}
void TextTag::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->text);
}
