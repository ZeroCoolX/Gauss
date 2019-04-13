#pragma once

#include "GameObject.h"

class MenuButton
{
private:
	Sprite sprite;
	Text text;

	bool idle;
	bool pressing;
	bool hovering;

	Color colorIdle;
	Color colorHover;
	Color colorPressed;

public:
	MenuButton(Font &font, 
		std::string text, 
		const unsigned int charSize,
		Vector2f pos, 
		int textureIndex);

	virtual ~MenuButton();

	void Update(const float &dt, Vector2f mousePos);
	void Draw(RenderTarget &renderTarget);

	bool IsPressed();
	bool IsHover();
	bool IsIdle();

	// Statics
	static dArr<Texture> textures;
	static int numOfTextures;

	static void InitTextures();
};

