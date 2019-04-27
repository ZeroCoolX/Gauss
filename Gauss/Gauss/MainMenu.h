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

	float pressTimeMax;
	float pressTime = this->pressTimeMax;

	Font font;
	Texture backgroundTexture;
	RectangleShape background;
	RenderWindow *window;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

public:
	MainMenu(Font &font, RenderWindow *window);
	virtual ~MainMenu();

	inline bool isActive() const { return this->active; }
	inline const bool onCosmosPress() { return this->active && this->playCosmos; }
	inline const bool onCampaignPress() { return this->active && this->playCampaign; }
	inline const bool onInfinitePress() { return this->active && this->playInfinite; }
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
		BTN_EXIT
	};
};

