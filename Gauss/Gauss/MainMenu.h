#pragma once

#include "GameObject.h"
#include "MenuButton.h"

class MainMenu
{
public:
	MainMenu();
	virtual ~MainMenu();

	// Init
	void Init();
	void InitButtons();

	// Update
	void Update(Vector2f &mousePos, const float &dt);
	void UpdateButtons(Vector2f &mousePos, const float &dt);
	
	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawButtons(RenderTarget &renderTarget);

	dArr<MenuButton> buttons;

	// statics
	enum buttonTypes {
		BTN_LADDER = 0, 
		BTN_SURVIVAL,
		BTN_EXIT
	};
};

