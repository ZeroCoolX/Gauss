#pragma once

#include "GameObject.h"
#include "Gauss.h"

class GameOverMenu
{
private:
	bool active = false;

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
	void activate() { this->active = true; }
	void deactivate() { this->active = false; }

	// Init
	void Init();
	void InitBackground();

	// Update
	void Update(const float &dt);
	void UpdateMousePosition();

	// Draw
	void Draw(RenderTarget &renderTarget);
	void DrawBackground(RenderTarget &renderTarget);
};

