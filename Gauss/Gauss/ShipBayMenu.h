#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"
#include "Player.h"

class ShipBayMenu
{
private:
	bool active;

	int shipSelection;
	bool selectionUpdatedNeeded;

	float pressTimeMax;
	float pressTime;

	Font font;
	Texture backgroundTexture;
	RectangleShape background;
	RenderWindow *window;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

	// SHIP
	Sprite body;
	Sprite mainGunSprite;
	Sprite lWing;
	Sprite rWing;
	Sprite cPit;

	dArr<MenuButton*> buttons;



	void _confirmSelection();

public:
	ShipBayMenu(Font &font, RenderWindow *window);
	virtual ~ShipBayMenu();

	inline const int getShipSelection() { this->selectionUpdatedNeeded = false; return this->shipSelection; }
	inline const bool selectionUpdateNeeded() const { return this->selectionUpdatedNeeded; }

	inline bool isActive() const { return this->active; }
	void activate(int shipSkin = 0) { this->active = true; this->shipSelection = shipSkin; }
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
	void DrawShipParts(RenderTarget &renderTarget);

	enum ButtonTypes {
		BTN_CONFIRM = 13
	};
};

