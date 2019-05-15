#include "KeyBindingMenu.h"

KeyBindingMenu::KeyBindingMenu(Font &font, RenderWindow *window, KeyManager *keyManager)
{
	this->keyManager = keyManager;
	this->pressTimeMax = 10.f;
	this->pressTime = 0.f;
	this->keyTimeMax = 2.f;
	this->keyTime = this->keyTimeMax;
	this->active = false;
	this->keybindPolling = false;
	this->keyRefreshNeeded = false;
	this->keybindPollingId = -1;
	this->font = font;
	this->window = window;

	this->lastChangeText.setFont(this->font);
	this->lastChangeText.setCharacterSize(25);
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

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_UP, this->font, KeyManager::KeyName(this->keyManager->Up(0)), 18, Vector2f(125.f, 105.f), 5));
	this->buttonKeyNames.Add("UP");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_DOWN, this->font, KeyManager::KeyName(this->keyManager->Down(0)), 18, Vector2f(125.f, 185), 5));
	this->buttonKeyNames.Add("DOWN");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_LEFT, this->font, KeyManager::KeyName(this->keyManager->Left(0)), 18, Vector2f(125.f, 275), 5));
	this->buttonKeyNames.Add("LEFT");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_RIGHT, this->font, KeyManager::KeyName(this->keyManager->Right(0)), 18, Vector2f(125.f, 375), 5));
	this->buttonKeyNames.Add("RIGHT");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_LASER, this->font, KeyManager::KeyName(this->keyManager->Fire(0)), 18, Vector2f(125.f, 460), 5));
	this->buttonKeyNames.Add("LASER");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_MAC, this->font, KeyManager::KeyName(this->keyManager->Mac(0)), 18, Vector2f(125.f, 560), 5));
	this->buttonKeyNames.Add("MAC");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_SHIELD, this->font, KeyManager::KeyName(this->keyManager->Shield(0)), 15, Vector2f(125.f, 645), 5));
	this->buttonKeyNames.Add("SHIELD");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_CONTROL_STATS, this->font, KeyManager::KeyName(this->keyManager->ToggleStats(0)), 15, Vector2f(125.f, 735), 5));
	this->buttonKeyNames.Add("STATS");

	this->buttons.Add(new MenuButton(ButtonTypes::BTN_BACK, this->font, "Confirm", 18, Vector2f(125.f, 850.f), 0));
	this->buttons.Add(new MenuButton(ButtonTypes::BTN_RESET, this->font, "Reset To Defaults", 18, Vector2f(450.f, 850.f), 0));
}

void KeyBindingMenu::InitBackground() {
	this->background.setSize(Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y)));
	this->backgroundTexture.loadFromFile("Textures/Backgrounds/UI/keybinding02.png");
	this->background.setTexture(&this->backgroundTexture);
}

void KeyBindingMenu::Update(const float &dt, const Event *event) {
	// Presstime update
	this->UpdateTimers(dt);
	this->UpdateMousePosition();
	this->UpdateButtons(dt);
	if (this->active) {
		this->UpdateKeybindPolling(dt, event);
	}
}

void KeyBindingMenu::UpdateKeybindPolling(const float &dt, const Event *event) {
	if (this->keybindPolling && this->keybindPollingId >= 0 && this->keyTime >= this->keyTimeMax && this->pressTime >= this->pressTimeMax) {
		// Key pressed
		if (event->type == Event::KeyReleased || event->type == Event::KeyPressed || event->type == Event::MouseButtonPressed)
		{
			this->keybindPolling = false;
			const int key = KeyMouseBoard::key(event);

			if (KeyManager::KeyName(key) == "UNSUPPORTED") {
			//if (KeyManager::KeyName(event->key.code) == "UNSUPPORTED") {
				this->lastChangeText.setString("There is no supported key mapping for that key");
			}
			else {
				this->lastChangeText.setString("Changed key " + this->buttonKeyNames[this->keybindPollingId] + ": \tfrom \t" + this->buttons[this->keybindPollingId]->getName() + " \tto \t" + KeyManager::KeyName(key) + "");
				this->keyRefreshNeeded = true;
				this->buttons[this->keybindPollingId]->updateText(KeyManager::KeyName(key));
				this->keyManager->SetKey(0, this->buttonKeyNames[this->keybindPollingId], key);
				//this->buttons[this->keybindPollingId]->updateText(KeyManager::KeyName(event->key.code));
				//this->keyManager->SetKey(0, this->buttonKeyNames[this->keybindPollingId], event->key.code);
			}
			this->buttons[this->keybindPollingId]->resetColor();
			this->keybindPollingId = -1;
			this->lastChangeText.setPosition(Vector2f(425.f - (this->lastChangeText.getGlobalBounds().width / 2.f), 950.f));
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
				this->_goBack();
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

	this->keyManager->SetKey(0, "LASER", KeyManager::DefaultControls::DEF_LASER);
	this->buttons[BTN_CONTROL_LASER]->updateText(KeyManager::KeyName(KeyManager::DefaultControls::DEF_LASER));

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
	this->lastChangeText.setPosition(Vector2f(425.f - (this->lastChangeText.getGlobalBounds().width / 2.f), 950.f));

	// Reset all button colors just in case
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->resetColor();
	}
}

void KeyBindingMenu::_goBack() {
	this->active = false;
	this->pressTime = 0.f;
	this->keyTime = 0.f;
	this->keyRefreshNeeded = false;
	this->keybindPolling = false;
	this->keybindPollingId = -1;
	this->lastChangeText.setString("");

	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i]->resetColor();
	}

	this->keyManager->SaveBindingsToFile();
}