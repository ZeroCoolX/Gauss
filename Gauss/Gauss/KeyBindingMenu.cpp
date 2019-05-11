#include "KeyBindingMenu.h"

KeyBindingMenu::KeyBindingMenu(Font &font, RenderWindow *window, KeyManager *keyManager)
{
	this->keyManager = keyManager;
	this->pressTimeMax = 10.f;
	this->pressTime = this->pressTimeMax;
	this->keyTimeMax = 2.f;
	this->keyTime = this->keyTimeMax;
	this->active = false;
	this->keybindPolling = true;
	this->keybindPollingId = -1;
	this->font = font;
	this->window = window;

	this->lastChangeText.setFont(this->font);
	this->lastChangeText.setCharacterSize(30);
	this->lastChangeText.setFillColor(Color::White);
	this->lastChangeText.setString("");

	this->Init();
}


KeyBindingMenu::~KeyBindingMenu()
{
}


void KeyBindingMenu::Init() {
	this->InitBackground();
	this->InitButtons();
}

void KeyBindingMenu::InitButtons() {

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_UP, this->font, "W", 18, Vector2f(300.f, 75.f), 0));
	this->buttonKeyNames.Add("UP");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_DOWN, this->font, "S", 18, Vector2f(300.f, 175), 0));
	this->buttonKeyNames.Add("DOWN");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_LEFT, this->font, "A", 18, Vector2f(300.f, 275.f), 0));
	this->buttonKeyNames.Add("LEFT");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_RIGHT, this->font, "D", 18, Vector2f(300.f, 375.f), 0));
	this->buttonKeyNames.Add("RIGHT");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_FIRE, this->font, "SPACE", 18, Vector2f(300.f, 475.f), 0));
	this->buttonKeyNames.Add("FIRE");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_MAC, this->font, "Q", 18, Vector2f(300.f, 590.f), 0));
	this->buttonKeyNames.Add("MAC");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_SHIELD, this->font, "RALT", 18, Vector2f(300.f, 695.f), 0));
	this->buttonKeyNames.Add("SHIELD");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_STATS, this->font, "TAB", 18, Vector2f(300.f, 805.f), 0));
	this->buttonKeyNames.Add("TOGGLESTATS");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_BACK, this->font, "Back", 18, Vector2f(window->getSize().x - 500.f, 100.f), 0));
	this->buttons.Add(new MenuButton(ButtonTypes::BTN_RESET, this->font, "Reset To Defaults", 18, Vector2f(window->getSize().x - 500.f, 300.f), 0));
}

void KeyBindingMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/keybindingBackground2.png");
	this->background.setTexture(&this->backgroundTexture);
}

void KeyBindingMenu::Update(const float &dt, const Event *event) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
	this->UpdateKeybindPolling(dt, event);
}

void KeyBindingMenu::UpdateKeybindPolling(const float &dt, const Event *event) {
	if (this->keybindPolling && this->keybindPollingId >= 0 && this->keyTime >= this->keyTimeMax) {
		// Key pressed
		if (event->type == Event::KeyReleased || event->type == Event::KeyPressed)
		{
			if (event->key.code == Keyboard::Escape) {
				this->keybindPolling = false;
			}
			this->keybindPolling = false;
			this->lastChangeText.setString("Changed key " + this->buttonKeyNames[this->keybindPollingId] + ": \tfrom \t" + this->buttons[this->keybindPollingId]->getName() + " \tto \t" + KeyManager::KeyName(event->key.code) + "");
			this->lastChangeText.setPosition(Vector2f((this->window->getSize().x / 2.f) - (this->lastChangeText.getGlobalBounds().width / 2.f), this->window->getSize().y - 50.f));
			this->buttons[this->keybindPollingId]->updateText(KeyManager::KeyName(event->key.code));
			this->buttons[this->keybindPollingId]->resetColor();
			this->keyManager->SetKey(0, this->buttonKeyNames[this->keybindPollingId], event->key.code);
			this->keybindPollingId = -1;
		}
	}
}

void KeyBindingMenu::UpdateButtons(const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Update(this->mousePosWorld);

		if (this->buttons[i]->IsPressed() && this->pressTime >= this->pressTimeMax) {

			if (this->buttons[i]->getId() == ButtonTypes::BTN_RESET) {
				this->_resetControlToDefault();
				return;
			}
			else if(this->buttons[i]->getId() == ButtonTypes::BTN_BACK){
				return;
			}

			this->pressTime = 0.f;

			this->keybindPollingId = this->buttons[i]->getId();
			this->keybindPolling = !this->keybindPolling;
			if (this->keybindPolling) {
				this->buttons[i]->changeColor(Color::Red);
			}
			else {
				this->buttons[i]->resetColor();
			}
		}
	}
}

void KeyBindingMenu::UpdateTimers(const float &dt) {
	if (this->pressTime < this->pressTimeMax) {
		this->pressTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void KeyBindingMenu::UpdateMousePosition() {
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

void KeyBindingMenu::Draw(RenderTarget &renderTarget) {
	this->DrawBackground(renderTarget);
	this->DrawButtons(renderTarget);
	renderTarget.draw(this->lastChangeText);
}

void KeyBindingMenu::DrawButtons(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->Draw(renderTarget);
	}
}

void KeyBindingMenu::DrawBackground(RenderTarget &renderTarget) {
	renderTarget.draw(this->background);
}

void KeyBindingMenu::_resetControlToDefault() {
	this->keyManager->SetKey(0, "UP", KeyManager::DefaultControls::DEF_UP);
	this->buttons[BTN_CONTROL_UP]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_UP));

	this->keyManager->SetKey(0, "DOWN", KeyManager::DefaultControls::DEF_DOWN);
	this->buttons[BTN_CONTROL_DOWN]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_DOWN));

	this->keyManager->SetKey(0, "LEFT", KeyManager::DefaultControls::DEF_LEFT);
	this->buttons[BTN_CONTROL_LEFT]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_LEFT));

	this->keyManager->SetKey(0, "RIGHT", KeyManager::DefaultControls::DEF_RIGHT);
	this->buttons[BTN_CONTROL_RIGHT]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_RIGHT));

	this->keyManager->SetKey(0, "FIRE", KeyManager::DefaultControls::DEF_FIRE);
	this->buttons[BTN_CONTROL_FIRE]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_FIRE));

	this->keyManager->SetKey(0, "MAC", KeyManager::DefaultControls::DEF_MAC);
	this->buttons[BTN_CONTROL_MAC]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_MAC));

	this->keyManager->SetKey(0, "SHIELD", KeyManager::DefaultControls::DEF_SHIELD);
	this->buttons[BTN_CONTROL_SHIELD]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_SHIELD));

	this->keyManager->SetKey(0, "STATS", KeyManager::DefaultControls::DEF_STATS);
	this->buttons[BTN_CONTROL_STATS]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_STATS));

	// Reset all polling just in case
	this->keybindPolling = false;
	this->keybindPollingId = -1;

	// Update history
	this->lastChangeText.setString("Reset keys to defaults");
	this->lastChangeText.setPosition(Vector2f((this->window->getSize().x / 2.f) - (this->lastChangeText.getGlobalBounds().width / 2.f), this->window->getSize().y - 50.f));

	// Reset all button colors just in case
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->resetColor();
	}
}