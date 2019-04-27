#include "GameOverMenu.h"

GameOverMenu::GameOverMenu(Font &font, RenderWindow *window)
{
	//this->callbackFunction = callback;
	this->active = false;
	this->font = font;
	this->window = window;
	this->Init();
}


GameOverMenu::~GameOverMenu()
{
}

void GameOverMenu::InitButtons() {

	this->buttons.Add(new MenuButton(GameOverMenu::BTN_REDEPLOY, this->font, "Redeploy", 18, Vector2f(50.f, 550.f), 1));
	this->buttons.Add(new MenuButton(GameOverMenu::BTN_MENU, this->font, "Menu", 18, Vector2f(50.f, 650.f), 1));
	this->buttons.Add(new MenuButton(GameOverMenu::BTN_EXIT, this->font, "Quit", 18, Vector2f(50.f, 750.f), 1));
}

// Init
void GameOverMenu::Init() {
	this->InitBackground();
	this->InitButtons();
}

void GameOverMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/genericGameOverBackground.png");
	this->background.setTexture(&this->backgroundTexture);
	this->background.setFillColor(Color(255, 255, 255, 255));
}

void GameOverMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);
		if (this->buttons[i]->IsPressed()) {
			switch (this->buttons[i]->getId()) {
			case GameOverMenu::BTN_REDEPLOY:
				this->redeploy = true;
				break;
			case GameOverMenu::BTN_MENU:
				this->menu = true;
				break;
			case GameOverMenu::BTN_EXIT:
				this->window->close();
				break;
			}
		}
	}
}

// Update
void GameOverMenu::Update(const float &dt) {
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
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

void GameOverMenu::DrawButtons(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Draw(renderTarget);
	}
}

// Draw
void GameOverMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
	this->DrawButtons(renderTarget);
}
void GameOverMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}

void GameOverMenu::Reset() {
	this->deactivate();
	this->redeploy = false;
	this->menu = false;
}
