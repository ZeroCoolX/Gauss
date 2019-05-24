#include "MainMenu.h"


MainMenu::MainMenu(Font &font, RenderWindow *window)
{
	this->pressTimeMax = 10.f;
	this->pressTime = this->pressTimeMax;
	this->active = true;
	this->currentSummaryPanel = SummaryPanels::BACKSTORY;
	this->currentInfoPanel = 0;
	this->font = font;
	this->window = window;
	this->infoPanelTimerMax = 800.f;
	this->infoPanelTimer = 0.f;
	this->summaryPanelFadeTimerMax = 255.f;
	this->summaryPanelFadeTimer = this->summaryPanelFadeTimerMax;
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

	this->buttons.Add(new MenuButton(MainMenu::BTN_TUTORIAL, this->font, "Tutorial - Strongly Recommended", 18, Vector2f(100.f, 350.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_CAMPAIGN, this->font, "Play Campaign", 18, Vector2f(100.f, 450.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_INFINITE, this->font, "Play Infinite Invasion", 18, Vector2f(100.f, 550.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_COSMOS, this->font, "Play Cosmos", 18, Vector2f(100.f, 650.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_KEYBIND, this->font, "Change Keybinding", 18, Vector2f(100.f, 750.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_SHIPBAY, this->font, "Ship Bay", 18, Vector2f(100.f, 850.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_EXIT, this->font, "Quit", 18, Vector2f(100.f, 950.f), 0));
	this->buttons.Add(new MenuButton(MainMenu::BTN_CREDITS, this->font, "Credits", 15, Vector2f(this->window->getSize().x - 150.f, 25.f), 6));
	this->buttons.Add(new MenuButton(MainMenu::BTN_INFO_CYCLE_BACK, this->font, "prev", 18, Vector2f((this->window->getSize().x / 2.f) + 360, 1000.f), 5));
	this->buttons.Add(new MenuButton(MainMenu::BTN_INFO_CYCLE_FWD, this->font, "next", 18, Vector2f((this->window->getSize().x / 2.f) + 600, 1000.f), 5));
}

void MainMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/background.png");
	this->background.setTexture(&this->backgroundTexture);

	this->panelTextures[InfoPanels::PROMO].loadFromFile("Textures/Backgrounds/UI/InfoPanels/promo.png");
	this->panelTextures[InfoPanels::POWERUPS].loadFromFile("Textures/Backgrounds/UI/InfoPanels/powerups.png");
	this->panelTextures[InfoPanels::UPGRADES].loadFromFile("Textures/Backgrounds/UI/InfoPanels/upgrades.png");
	this->panelTextures[InfoPanels::ENEMIES].loadFromFile("Textures/Backgrounds/UI/InfoPanels/enemies.png");
	this->panelTextures[SummaryPanels::CAMPAIGN].loadFromFile("Textures/Backgrounds/UI/SummaryPanels/CampaignSummaryPanel.png");
	this->panelTextures[SummaryPanels::COSMOS].loadFromFile("Textures/Backgrounds/UI/SummaryPanels/CosmosSummaryPanel.png");
	this->panelTextures[SummaryPanels::INFINITE].loadFromFile("Textures/Backgrounds/UI/SummaryPanels/InfiniteSummaryPanel.png");
	this->panelTextures[SummaryPanels::BACKSTORY].loadFromFile("Textures/Backgrounds/UI/SummaryPanels/GaussBackstoryPanel2.png");

	// Setup info panel
	this->infoPanel.setTexture(this->panelTextures[this->currentInfoPanel]);
	this->infoPanel.setOrigin(
		this->infoPanel.getGlobalBounds().width / 2,
		this->infoPanel.getGlobalBounds().height / 2
	);
	this->infoPanel.setPosition(Vector2f((this->window->getSize().x / 2.f) + 550, (this->window->getSize().y / 2.f) + 50.f));

	// Setup Summary panel
	this->summaryPanel.setTexture(this->panelTextures[this->currentSummaryPanel]);
	this->summaryPanel.setOrigin(
		this->summaryPanel.getGlobalBounds().width / 2,
		this->summaryPanel.getGlobalBounds().height / 2
	);
	this->summaryPanel.setPosition(Vector2f((this->window->getSize().x / 2.f) - 100, (this->window->getSize().y / 2.f) + 50.f));
}

void MainMenu::Update(const float &dt) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateInfoPanels(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
	//this->UpdateSummaryPanels(dt);
}

// For now - this is rather ugly
void MainMenu::UpdateButtons(const float &dt) {
	const Color summaryPanelColor = this->summaryPanel.getColor();
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
				case MainMenu::BTN_KEYBIND:
					this->changeKeybind = true;
					return;
				case MainMenu::BTN_SHIPBAY:
					this->enterShipBay = true;
					return;
				case MainMenu::BTN_INFO_CYCLE_FWD:
					// Temporarily set the info panel very high so the user has time to read
					this->infoPanelTimer = 0.f;
					this->infoPanelTimerMax = 800.f;
					this->_cycleInfoPanel(1);
					return;
				case MainMenu::BTN_INFO_CYCLE_BACK:
					// Temporarily set the info panel very high so the user has time to read
					this->infoPanelTimer = 0.f;
					this->infoPanelTimerMax = 800.f;
					this->_cycleInfoPanel(-1);
					return;
				case MainMenu::BTN_CREDITS:
					this->enterCredits = true;
					return;
				case MainMenu::BTN_EXIT:
					this->window->close();
					return;
			}
		}
		else {
			if (this->buttons[i]->IsHover()) {

				switch (this->buttons[i]->getId()) {
				case MainMenu::BTN_CAMPAIGN:
					if (this->currentSummaryPanel != SummaryPanels::CAMPAIGN) {
						this->summaryPanelFadeTimer = 0.f;
						this->summaryPanel.setColor(Color(summaryPanelColor.r, summaryPanelColor.g, summaryPanelColor.b, 255));
						this->currentSummaryPanel = SummaryPanels::CAMPAIGN;
						this->_setSummaryPanel();
					}
					return;
				case MainMenu::BTN_INFINITE:
					if (this->currentSummaryPanel != SummaryPanels::INFINITE) {
						this->summaryPanelFadeTimer = 0.f;
						this->summaryPanel.setColor(Color(summaryPanelColor.r, summaryPanelColor.g, summaryPanelColor.b, 255));
						this->currentSummaryPanel = SummaryPanels::INFINITE;
						this->_setSummaryPanel();
					}
					return;
				case MainMenu::BTN_COSMOS:
					if (this->currentSummaryPanel != SummaryPanels::COSMOS) {
						this->summaryPanelFadeTimer = 0.f;
						this->summaryPanel.setColor(Color(summaryPanelColor.r, summaryPanelColor.g, summaryPanelColor.b, 255));
						this->currentSummaryPanel = SummaryPanels::COSMOS;
						this->_setSummaryPanel();
					}
					return;
				}
			}
		}
	}
	this->currentSummaryPanel = SummaryPanels::BACKSTORY;
	this->_setSummaryPanel();
	if (this->summaryPanelFadeTimer < this->summaryPanelFadeTimerMax) {
		this->summaryPanelFadeTimer += 1.f * dt * DeltaTime::dtMultiplier;
		this->summaryPanel.setColor(Color(summaryPanelColor.r, summaryPanelColor.g, summaryPanelColor.b, static_cast<Uint8>(this->summaryPanelFadeTimer)));
	}
	else {
		this->summaryPanel.setColor(Color(summaryPanelColor.r, summaryPanelColor.g, summaryPanelColor.b, 255));
	}
}

void MainMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void MainMenu::UpdateInfoPanels(const float &dt) {
	if (this->infoPanelTimer < this->infoPanelTimerMax) {
		this->infoPanelTimer += 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		this->_cycleInfoPanel(1);
		this->infoPanelTimerMax = 400.f;
		this->infoPanelTimer = 0.f;
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

	renderTarget.draw(this->summaryPanel);

	renderTarget.draw(this->infoPanel);
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
	this->changeKeybind = false;
	this->enterShipBay = false;
	this->enterCredits = false;
	this->pressTime = 0.f;
	this->infoPanelTimerMax = 400.f;
	this->infoPanelTimer = 0.f;
	this->currentSummaryPanel = SummaryPanels::BACKSTORY;
	this->currentInfoPanel = InfoPanels::PROMO;
}

void MainMenu::_cycleInfoPanel(int direction) {
	if (this->currentInfoPanel + direction < 0) {
		this->currentInfoPanel = 3;
	}
	else {
		this->currentInfoPanel = (this->currentInfoPanel + direction) % 4;
	}
	this->infoPanel.setTexture(this->panelTextures[this->currentInfoPanel]);
}

void MainMenu::_setSummaryPanel() {
	this->summaryPanel.setTexture(this->panelTextures[this->currentSummaryPanel]);
}

