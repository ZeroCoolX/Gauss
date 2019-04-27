#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(Font &font, RenderWindow *window)
{
	this->active = true;
	this->font = font;
	this->window = window;
	this->Init();
}


GameOverMenu::~GameOverMenu()
{
}

// Init
void GameOverMenu::Init() {
	this->InitBackground();
}

void GameOverMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/genericGameOverBackground.png");
	this->background.setTexture(&this->backgroundTexture);
	this->background.setFillColor(Color(255, 255, 255, 255));
}

// Update
void GameOverMenu::Update(const float &dt) {
	//if (Keyboard::isKeyPressed(Keyboard::U))
	//{
	//	sf::Color rectColor = this->background.getFillColor();
	//	if (rectColor != sf::Color::White) {
	//		this->background.setFillColor(rectColor + sf::Color(1, 1, 1, 0));
	//	}
	//}

	//if (Keyboard::isKeyPressed(Keyboard::Y))
	//{
	//	sf::Color rectColor = this->background.getFillColor();
	//	if (rectColor != sf::Color::Black) {
	//		this->background.setFillColor(rectColor - sf::Color(1, 1, 1, 0));
	//	}
	//}

	this->UpdateMousePosition();
}

void GameOverMenu::UpdateMousePosition() {
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosWorld = this->window->mapPixelToCoords(this->mousePosWindow);

#pragma warning(push)
#pragma warning(disable:4244)
	this->mousePosGrid = Vector2i(
		std::max(0.f, this->mousePosWorld.x / Gauss::GRID_SIZE),
		std::max(0.f, this->mousePosWorld.y / Gauss::GRID_SIZE)
	);
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4018)
	// Max bounds check
	if (this->mousePosGrid.x >= window->getSize().x) {
		this->mousePosGrid.x = window->getSize().x - 1;
	}
	if (this->mousePosGrid.y >= window->getSize().y) {
		this->mousePosGrid.y = window->getSize().y - 1;
	}
#pragma warning(pop)
}

// Draw
void GameOverMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
}
void GameOverMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}
