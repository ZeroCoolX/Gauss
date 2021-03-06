#include "Dialog.h"

Dialog::Dialog(Font &font, RenderWindow *window)
{
	this->active = false;
	this->font = font;
	this->window = window;

	// Load texture
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/GaussDialog.png");

	this->background.setSize(Vector2f(static_cast<float>(this->window->getDefaultView().getSize().x / 3.f), static_cast<float>(this->window->getDefaultView().getSize().y / 4)));
	this->background.setFillColor(Color(255, 255, 255, 255));
	this->background.setTexture(&this->backgroundTexture);
	// Set position at the bottom middle
	this->background.setPosition(
		(this->window->getDefaultView().getSize().x / 2.f) - (this->background.getGlobalBounds().width / 2.f), 
		this->window->getDefaultView().getSize().y - this->background.getGlobalBounds().height
	);
	
	this->text.setFont(font);
	this->text.setCharacterSize(25);
	this->text.setFillColor(Color::White);
	this->text.setString("This is just a test to see if I can see anything. \nIf you can see me - then woohoo!");
	this->text.setPosition(
		this->background.getPosition().x + 40.f,//(this->background.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->background.getPosition().y + 90.f//(this->background.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
	);

	this->currentTextWidth = 0.f;
}


Dialog::~Dialog()
{
}

void Dialog::Update(const float &dt) {

}

void Dialog::Draw(RenderTarget &renderTarget)
{
	if (this->active) {
		renderTarget.draw(this->background);
		renderTarget.draw(this->text);
	}
}

