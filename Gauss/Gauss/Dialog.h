#pragma once
#include "GameObject.h"

class Dialog
{
private:
	bool active = false;
	Font font;
	float currentTextWidth;
	Text text;
	Texture backgroundTexture;
	RectangleShape background;
	RenderWindow *window;

public:
	Dialog(Font &font, RenderWindow *window);
	virtual ~Dialog();

	inline void activate() { this->active = true; }
	inline void disable() { this->active = false; }
	inline const bool isActive() const { return this->active; }
	inline float getCurrentTextWidth() { return this->currentTextWidth; }
	inline void incrementCurrentTextWidth() { this->currentTextWidth += this->text.getCharacterSize(); }
	inline void resetCurrentTextWidth() { this->currentTextWidth = 0.f; }
	inline const bool isWithinBorderEdge()const { 
		return (this->currentTextWidth < (this->background.getSize().x + this->background.getGlobalBounds().width - 15.f));
	}
	inline void setText(std::string newText) { this->text.setString(newText); }

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

