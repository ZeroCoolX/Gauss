#include "MainMenu.h"


MainMenu::MainMenu(Font &font, RenderWindow *window)
{
	this->active = true;
	this->font = font;
	this->window = window;
	this->Init();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init() {
	this->InitBackground();
	this->InitButtons();
}

void MainMenu::InitButtons() {

	this->buttons.Add(new MenuButton(MainMenu::BTN_SURVIVAL, this->font, "Play Gauss", 18, Vector2f(150.f, 400.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_EXIT, this->font, "Quit", 18, Vector2f(150.f, 550.f), 0));
}

void MainMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/background.png");
	this->background.setTexture(&this->backgroundTexture);
}

void MainMenu::Update(const float &dt) {
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
}

// For now - this is rather ugly
void MainMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);
		if (this->buttons[i]->IsPressed()) {
			switch (this->buttons[i]->getId()) {
				case MainMenu::BTN_LADDER:
				case MainMenu::BTN_SURVIVAL:
					this->active = false;
					break;
				case MainMenu::BTN_EXIT:
					this->window->close();
					break;
			}
		}
	}
}

void MainMenu::UpdateMousePosition() {
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

void MainMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
	this->DrawButtons(renderTarget);
}

void MainMenu::DrawButtons(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Draw(renderTarget);
	}
}

void MainMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}

