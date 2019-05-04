#pragma once
#include "GameObject.h"

class Dialog
{
private:
	bool active = false;
	Font font;
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
	inline void setText(std::string newText) { this->text.setString(newText); }

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

