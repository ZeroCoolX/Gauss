#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"

class ShipBayMenu
{
private:
	bool active;

	int shipSelection;

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

	dArr<MenuButton*> buttons;

	void _confirmSelection();

public:
	ShipBayMenu(Font &font, RenderWindow *window);
	virtual ~ShipBayMenu();

	inline bool isActive() const { return this->active; }
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

	enum ButtonTypes {
		BTN_CONFIRM = 13
	};
};

