#include "ShipBayMenu.h"


ShipBayMenu::ShipBayMenu(Font &font, RenderWindow *window)
{
	this->pressTimeMax = 10.f;
	this->pressTime = this->pressTimeMax;
	this->active = true;
	this->font = font;
	this->window = window;
	this->shipSelection = 0;
	this->Init();
}


ShipBayMenu::~ShipBayMenu()
{
}

void ShipBayMenu::Init() {
	this->InitShipParts();
	this->InitBackground();
	this->InitButtons();
}

void ShipBayMenu::InitShipParts() {
	this->body.setTexture(Player::shipBodyTextures[Player::DEFAULT_SHIP_BODY]);
	this->body.setScale(0.3f, 0.3f);
	this->body.setColor(Color(10, 10, 10, 255));
	this->body.setPosition(Vector2f(700.f, 700.f));

	const Vector2f shipCenter = Vector2f(
		this->body.getPosition().x + this->body.getGlobalBounds().width / 2, 
		this->body.getPosition().y + this->body.getGlobalBounds().height / 2
	);

	this->mainGunSprite.setTexture(Player::shipMainGunTextures[Player::DEFAULT_LASER]);
	//this->mainGunSprite.setScale(0.1f, 0.1f);
	this->mainGunSprite.setOrigin(
		this->mainGunSprite.getGlobalBounds().width / 2,
		this->mainGunSprite.getGlobalBounds().height / 2);
	this->mainGunSprite.setRotation(90.f);
	const float originX = shipCenter.x + this->body.getGlobalBounds().width / 6;
	this->mainGunSprite.setPosition(
		originX,
		shipCenter.y);
	this->mainGunSprite.setScale(1.3f, 1.3f);

	this->lWing.setTexture(Player::shipLWingTextures[this->shipSelection]);
	this->lWing.setOrigin(this->lWing.getGlobalBounds().width / 2,
		this->lWing.getGlobalBounds().height / 2);
	this->lWing.setPosition(shipCenter);
	this->lWing.setRotation(90.f);
	this->lWing.setScale(1.f, 1.f);

	this->rWing.setTexture(Player::shipRWingTextures[this->shipSelection]);
	this->rWing.setOrigin(this->rWing.getGlobalBounds().width / 2,
		this->rWing.getGlobalBounds().height / 2);
	this->rWing.setPosition(shipCenter);
	this->rWing.setRotation(90.f);
	this->rWing.setScale(1.f, 1.f);

	this->cPit.setTexture(Player::shipCockpitTextures[this->shipSelection]);
	this->cPit.setOrigin(this->cPit.getGlobalBounds().width / 2,
		this->cPit.getGlobalBounds().height / 2);
	this->cPit.setPosition(shipCenter);
	this->cPit.setRotation(90.f);
	this->cPit.setScale(1.f, 1.f);
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

				this->lWing.setTexture(Player::shipLWingTextures[this->shipSelection]);
				this->rWing.setTexture(Player::shipLWingTextures[this->shipSelection]);
				this->cPit.setTexture(Player::shipLWingTextures[this->shipSelection]);
				break;
			}
		}
	}

	this->buttons[shipSelection]->changeColor(Color::Green);
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
	this->pressTime = 0.f;
}
