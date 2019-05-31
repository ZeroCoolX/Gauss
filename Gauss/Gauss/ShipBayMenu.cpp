#include "ShipBayMenu.h"


ShipBayMenu::ShipBayMenu(Font &font, RenderWindow *window)
{
	this->pressTimeMax = 10.f;
	this->pressTime = 0.f;
	this->active = false;
	this->selectionUpdatedNeeded = false;
	this->font = font;
	this->window = window;
	this->shipSelection = 0;
	this->Init();
}


ShipBayMenu::~ShipBayMenu()
{
}

void ShipBayMenu::Init() {
	this->InitBackground();
	this->InitButtons();
}


void ShipBayMenu::InitButtons() {
	const int adder = 60;
	for (size_t i = 0; i < 12; i++)
	{
		this->buttons.Add(new MenuButton(i+1, this->font, "", 18, Vector2f(250.f, 115.f + (adder * i)), 4));
	}

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONFIRM, this->font, "Confirm", 18, Vector2f(265.f, 950.f), 0));
}

void ShipBayMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/shipSelect.png");
	this->background.setTexture(&this->backgroundTexture);
}

void ShipBayMenu::Update(const float &dt) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
}

// For now - this is rather ugly
void ShipBayMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);
		if (this->buttons[i]->IsPressed() && this->pressTime >= this->pressTimeMax) {
			this->pressTime = 0.f;
			if (this->buttons[i]->getId() == BTN_CONFIRM) {
				this->_confirmSelection();
				return;
			}
			else {
				this->buttons[this->shipSelection]->resetColor();
				this->shipSelection = i;
				this->selectionUpdatedNeeded = true;
				break;
			}
		}
	}

	this->buttons[shipSelection]->changeColor(
		Color(0, 200, 0, 255), 
		Color(0, 255, 0, 255),
		Color(0, 50, 0, 255));
}

void ShipBayMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void ShipBayMenu::UpdateMousePosition() {
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

void ShipBayMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
	this->DrawButtons(renderTarget);
	//this->DrawShipParts(renderTarget);
}

void ShipBayMenu::DrawButtons(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Draw(renderTarget);
	}
}

void ShipBayMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}

void ShipBayMenu::DrawShipParts(RenderTarget &renderTarget) {
	renderTarget.draw(this->mainGunSprite);
	renderTarget.draw(this->body);

	renderTarget.draw(this->cPit);
	renderTarget.draw(this->lWing);
	renderTarget.draw(this->rWing);
}

void ShipBayMenu::_confirmSelection() {
	this->active = false;
	this->selectionUpdatedNeeded = false;
	this->pressTime = 0.f;
}
