#include "GameOverMenu.h"

dArr<Texture> GameOverMenu::backgroundTextures;
void GameOverMenu::InitTextures() {
	Texture temp;

	temp.loadFromFile("Textures/Backgrounds/UI/genericGameOverBackground02.png");
	GameOverMenu::backgroundTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Backgrounds/UI/cosmosGameOverBackground.png");
	GameOverMenu::backgroundTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Backgrounds/UI/campaignGameOverBackground.png");
	GameOverMenu::backgroundTextures.Add(Texture(temp));
	temp.loadFromFile("Textures/Backgrounds/UI/campaignBeatScreen.png");
	GameOverMenu::backgroundTextures.Add(Texture(temp));
}

GameOverMenu::GameOverMenu(Font &font, RenderWindow *window)
{
	this->pressTimeMax = 10.f;
	this->pressTime = this->pressTimeMax;
	this->active = false;
	this->font = font;
	this->window = window;
	this->background.setSize(Vector2f(static_cast<float>(this->window->getDefaultView().getSize().x), static_cast<float>(this->window->getDefaultView().getSize().y)));
	this->background.setFillColor(Color(255, 255, 255, 255));
	this->background.setTexture(&GameOverMenu::backgroundTextures[0]);
}


GameOverMenu::~GameOverMenu()
{
}

void GameOverMenu::LoadButtons(int buttonTextureIndex) {
	this->buttons.Add(new MenuButton(GameOverMenu::BTN_REDEPLOY, this->font, "Redeploy", 18, Vector2f(50.f, 650.f), buttonTextureIndex));
	this->buttons.Add(new MenuButton(GameOverMenu::BTN_MENU, this->font, "Menu", 18, Vector2f(50.f, 750.f), buttonTextureIndex));
	this->buttons.Add(new MenuButton(GameOverMenu::BTN_EXIT, this->font, "Quit", 18, Vector2f(50.f, 850.f), buttonTextureIndex));
}


void GameOverMenu::LoadGameOverBackground(Backgrounds backgroundIndex, int buttonIndex) {
	this->background.setTexture(&GameOverMenu::backgroundTextures[backgroundIndex]);
	this->buttons.Clear();
	LoadButtons(buttonIndex);
}


void GameOverMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);
		if (this->buttons[i]->IsPressed() && this->pressTime >= this->pressTimeMax) {
			this->pressTime = 0.f;
			switch (this->buttons[i]->getId()) {
			case GameOverMenu::BTN_REDEPLOY:
				this->redeploy = true;
				return;
			case GameOverMenu::BTN_MENU:
				this->menu = true;
				return;
			case GameOverMenu::BTN_EXIT:
				this->window->close();
				return;
			}
		}
	}
}

// Update
void GameOverMenu::Update(const float &dt) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
}

void GameOverMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
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
#pragma warning(disable:4244)
	// Max bounds check
	if (this->mousePosGrid.x >= window->getDefaultView().getSize().x) {
		this->mousePosGrid.x = window->getDefaultView().getSize().x - 1;
	}
	if (this->mousePosGrid.y >= window->getDefaultView().getSize().y) {
		this->mousePosGrid.y = window->getDefaultView().getSize().y - 1;
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
	this->pressTime = 0.f;
}
