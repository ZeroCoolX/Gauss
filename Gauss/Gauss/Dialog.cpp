#include "Dialog.h"

Dialog::Dialog(Font &font, RenderWindow *window)
{
	this->active = false;
	this->font = font;
	this->window = window;

	this->text.setFont(font);
	this->text.setCharacterSize(30);
	this->text.setFillColor(Color::White);
	this->text.setString("This is just a test to see if I can see anything. If you can see me - then woohoo!");

	// Load texture
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/dialogBackground.png");

	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x / 4.f), static_cast<float>(window->getSize().y / 6)));
	this->background.setFillColor(Color(255, 255, 255, 255));
	this->background.setTexture(&this->backgroundTexture);
	// Set position at the bottom middle
	this->background.setPosition(
		(this->window->getSize().x / 2) - this->background.getGlobalBounds().width / 2.f,
		this->window->getSize().y - this->background.getGlobalBounds().height
	);
}


Dialog::~Dialog()
{
}

void Dialog::Update(const float &dt) {

}

void Dialog::Draw(RenderTarget &renderTarget)
{
	renderTarget.draw(this->background);
	renderTarget.draw(this->text);
}

