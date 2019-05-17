#pragma once

#include "GameObject.h"

class MenuButton
{
private:
	int ID;

	Sprite sprite;
	Text text;

	bool locked;
	bool idle;
	bool pressing;
	bool hovering;

	Color colorIdleDefault;
	Color colorIdle;
	Color colorHover;
	Color colorPressed;

public:
	MenuButton(
		int id,
		Font &font,
		std::string text,
		const unsigned int charSize,
		Vector2f pos,
		int textureIndex,
		bool locked = false);

	virtual ~MenuButton();

	inline std::string getName() { return this->text.getString(); }
	inline int getId() const { return this->ID; }
	inline void updateText(std::string newText) { 
		this->text.setString(newText);
		this->text.setPosition(
			this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
			this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
		);
	}
	inline void changeColor(Color c) { this->colorIdle = c; }
	inline void resetColor() { this->colorIdle = this->colorIdleDefault; }

	void Update(Vector2f mousePos);
	void Draw(RenderTarget &renderTarget);

	bool IsPressed();
	bool IsHover();
	bool IsIdle();

	// Statics
	static dArr<Texture> textures;
	static int numOfTextures;

	static void InitTextures();
};

