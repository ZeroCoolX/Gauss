#include "CreditsMenu.h"



CreditsMenu::CreditsMenu(Font &font, RenderWindow *window){
	this->pressTimeMax = 10.f;
	this->pressTime = 0.f;
	this->active = false;
	this->font = font;
	this->window = window;
	this->Init();
}


CreditsMenu::~CreditsMenu()
{
	delete this->backButton;
}

void CreditsMenu::Init() {
	this->InitBackground();
	this->InitButtons();
}


void CreditsMenu::InitButtons() {
	this->backButton = new MenuButton(0, this->font, "Back", 18, Vector2f(this->window->getSize().x - 150.f, 25.f), 6);
}

void CreditsMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/credits.png");
	this->background.setTexture(&this->backgroundTexture);
}

void CreditsMenu::Update(const float &dt) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
}

// For now - this is rather ugly
void CreditsMenu::UpdateButtons(const float &dt) {
	this->backButton->Update(this->mousePosWorld);
	if (this->backButton->IsPressed() && this->pressTime >= this->pressTimeMax) {
		this->pressTime = 0.f;
		this->active = false;
	}
}

void CreditsMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void CreditsMenu::UpdateMousePosition() {
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

void CreditsMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
	this->DrawButtons(renderTarget);
}

void CreditsMenu::DrawButtons(RenderTarget &renderTarget) {
	this->backButton->Draw(renderTarget);
}

void CreditsMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}

