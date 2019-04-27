#pragma once

#include "GameObject.h"
#include "MenuButton.h"
#include "Gauss.h"

class GameOverMenu
{
private:
	bool active = false;
	bool redeploy = false;
	bool menu = false;

	Font font;
	Texture backgroundTexture;
	RectangleShape background;
	RenderWindow *window;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

public:
	GameOverMenu(Font &font, RenderWindow *window);
	virtual ~GameOverMenu();

	inline bool isActive() const { return this->active; }
	inline const bool onMenuPress() { return this->active && this->menu; }
	inline const bool onRedeployPress() { return this->active && this->redeploy; }
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

	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawButtons(RenderTarget &renderTarget);
	void DrawBackground(RenderTarget &renderTarget);

	void Reset();

	dArr<MenuButton*> buttons;

	// statics
	enum ButtonTypes {
		BTN_REDEPLOY = 0,
		BTN_MENU,
		BTN_EXIT
	};
};

