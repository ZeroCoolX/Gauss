#include "MainMenu.h"


MainMenu::MainMenu(Font &font, RenderWindow *window)
{
	this->pressTimeMax = 10.f;
	this->pressTime = this->pressTimeMax;
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

	this->buttons.Add(new MenuButton(MainMenu::BTN_TUTORIAL, this->font, "Tutorial", 18, Vector2f(150.f, 400.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_CAMPAIGN, this->font, "Play Campaign", 18, Vector2f(150.f, 500.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_INFINITE, this->font, "Play Infinite Invasion", 18, Vector2f(150.f, 600.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_COSMOS, this->font, "Play Cosmos", 18, Vector2f(150.f, 700.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_EXIT, this->font, "Quit", 18, Vector2f(150.f, 850.f), 0));
}

void MainMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/background.png");
	this->background.setTexture(&this->backgroundTexture);
}

void MainMenu::Update(const float &dt) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
}

// For now - this is rather ugly
void MainMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);
		if (this->buttons[i]->IsPressed() && this->pressTime >= this->pressTimeMax) {
			this->pressTime = 0.f;
			switch (this->buttons[i]->getId()) {
				case MainMenu::BTN_TUTORIAL:
					this->playTutorial = true;
					return;
				case MainMenu::BTN_CAMPAIGN:
					this->playCampaign = true;
					return;
				case MainMenu::BTN_INFINITE:
					this->playInfinite = true;
					return;
				case MainMenu::BTN_COSMOS:
					this->playCosmos = true;
					return;
				case MainMenu::BTN_EXIT:
					this->window->close();
					return;
			}
		}
	}
}

void MainMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
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

void MainMenu::Reset() {
	this->deactivate();
	this->playCosmos = false;
	this->playCampaign = false;
	this->playInfinite = false;
	this->playTutorial = false;
	this->pressTime = 0.f;
}

