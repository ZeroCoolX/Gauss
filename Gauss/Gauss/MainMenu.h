#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"

class MainMenu
{
private:
	bool active = true;

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

	// Init
	void Init();
	void InitButtons();
	void InitBackground();

	// Update
	void Update(const float &dt);
	void UpdateButtons(const float &dt);
	void UpdateMousePosition();
	
	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawButtons(RenderTarget &renderTarget);
	void DrawBackground(RenderTarget &renderTarget);

	dArr<MenuButton*> buttons;

	// statics
	enum ButtonTypes {
		BTN_LADDER = 0, 
		BTN_SURVIVAL,
		BTN_EXIT
	};
};

