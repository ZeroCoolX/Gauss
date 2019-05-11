#pragma once

#include "GameObject.h"

class MenuButton
{
private:
	int ID;

	Sprite sprite;
	Text text;

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
		int textureIndex);

	virtual ~MenuButton();

	inline std::string getName() { return this->text.getString(); }
	inline int getId() const { return this->ID; }
	inline void updateText(std::string newText) { this->text.setString(newText); }
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

