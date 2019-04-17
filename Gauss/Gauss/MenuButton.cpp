#include "MenuButton.h"

dArr<Texture> MenuButton::textures;
int MenuButton::numOfTextures;

void MenuButton::InitTextures() {
	Texture temp;

	temp.loadFromFile("Textures/Buttons/button02.png");
	MenuButton::textures.Add(temp);

	MenuButton::numOfTextures = MenuButton::textures.Size();
}

MenuButton::MenuButton(Font &font, 
	std::string text, 
	const unsigned int charSize,
	Vector2f pos, 
	int textureIndex)
{
	this->text.setFont(font);
	this->text.setCharacterSize(charSize);
	this->text.setString(text);

	// Set colors
	this->colorIdle = Color(200, 200, 200, 240);
	this->colorHover = Color(255, 255, 255, 240);
	this->colorPressed = Color(50, 50, 50, 240);

	this->idle = true;
	this->hovering = false;
	this->pressing = false;

	if (textureIndex < 0 || textureIndex >= this->numOfTextures) {
		std::cout << "Error: No such button texture for index " << textureIndex << std::endl;
	}
	else {
		this->sprite.setTexture(MenuButton::textures[textureIndex]);
	}
	this->sprite.setPosition(pos);
	this->sprite.setColor(this->colorIdle);
	this->text.setPosition(this->sprite.getPosition());
}


MenuButton::~MenuButton()
{
}

void MenuButton::Update(Vector2f mousePos) {
	// Set Idle state
	this->idle = !this->sprite.getGlobalBounds().contains(mousePos);

	// Set Hover and pressing state
	if (this->sprite.getGlobalBounds().contains(mousePos)) {
		this->pressing = Mouse::isButtonPressed(Mouse::Left);
		this->hovering = !this->pressing;
	}else{
		this->pressing = false;
		this->hovering = false;
	}

	if (this->idle) {
		this->sprite.setColor(this->colorIdle);
	}
	else if (this->hovering) {
		this->sprite.setColor(this->colorHover);
	}
	else if (this->pressing) {
		this->sprite.setColor(this->colorPressed);
	}
}

void MenuButton::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);
	renderTarget.draw(this->text);
}

bool MenuButton::IsPressed() {
	return this->pressing;
}

bool MenuButton::IsHover() {
	return this->hovering;
}

bool MenuButton::IsIdle() {
	return this->idle;
}