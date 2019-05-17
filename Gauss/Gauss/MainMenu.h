#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"

class MainMenu
{
private:
	bool active = true;
	bool playCosmos = false;
	bool playCampaign = false;
	bool playInfinite = false;
	bool playTutorial = false;
	bool changeKeybind = false;
	bool enterShipBay = false;

	float pressTimeMax;
	float pressTime = this->pressTimeMax;

	Font font;
	Texture backgroundTexture;
	RectangleShape background;

	Texture infoPanelTextures[4];
	Sprite infoPanel;
	int currentInfoPanel;

	RenderWindow *window;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

	void _cycleInfoPanel();

public:
	MainMenu(Font &font, RenderWindow *window);
	virtual ~MainMenu();

	inline bool isActive() const { return this->active; }
	inline const bool onCosmosPress() { return this->active && this->playCosmos; }
	inline const bool onCampaignPress() { return this->active && this->playCampaign; }
	inline const bool onTutorialPress() { return this->active && this->playTutorial; }
	inline const bool onInfinitePress() { return this->active && this->playInfinite; }
	inline const bool onKeybindingPress() { return this->active && this->changeKeybind; }
	inline const bool onShipBayPress() { return this->active && this->enterShipBay; }
	void activate() { this->active = true; }
	void deactivate() { this->active = false; }

	// Init
	void Init();
	void InitButtons();
	void InitBackground();

	// Update
	void Update(const float &dt);
	void UpdateButtons(const float &dt);
	void UpdateMousePosition();
	void UpdateTimers(const float &dt);
	
	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawButtons(RenderTarget &renderTarget);
	void DrawBackground(RenderTarget &renderTarget);

	void Reset();

	dArr<MenuButton*> buttons;

	// statics
	enum ButtonTypes {
		BTN_CAMPAIGN = 0, 
		BTN_INFINITE,
		BTN_COSMOS,
		BTN_EXIT,
		BTN_TUTORIAL,
		BTN_KEYBIND,
		BTN_SHIPBAY,
		BTN_INFO_CYCLE
	};

	enum InfoPanels {
		PROMO = 0,
		POWERUPS,
		UPGRADES,
		ENEMIES
	};
};

