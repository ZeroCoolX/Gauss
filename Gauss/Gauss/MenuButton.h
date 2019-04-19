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

