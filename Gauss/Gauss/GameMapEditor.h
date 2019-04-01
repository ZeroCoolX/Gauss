#pragma once

#include "Player.h"
#include "MoveLeftEnemy.h"
#include "TrackerEnemy.h"
#include "MoveLeftShootEnemy.h"
#include "ItemPickup.h"
#include "ItemUpgrade.h"
#include "Boss.h"
#include "WorldMap.h"
#include "Particle.h"
#include "Powerup.h"

class GameMapEditor
{
private:
	// GameEditor
	RenderWindow *window;
	View mainView;
	float keyTimeMax;
	float keyTime;
	bool fullscreen;

	// MOUSE 
	Vector2i mousePosWindow;
	Vector2f mousePosWorld;
	Vector2i mousePosGrid;

	// UI
	RectangleShape selector;

	// TEXT
	Font font;

	// MAP
	WorldMap worldMap;
	Stage stage;

public:
	GameMapEditor(RenderWindow *window);
	virtual ~GameMapEditor();

	// Accessors
	inline RenderWindow& getWindow() { return *this->window; }

	// Functions
	void InitView();
	void InitMapTextures();
	void InitTextures();
	void InitUI();
	void InitMap();

	void UpdateMousePosition();
	void UpdateView(const float &dt);
	void UpdateUI();
	void Update(const float &dt);
	void UpdateTimers(const float &dt);
	void UpdateMap();
	void UpdateWallColliders(const float &dt, int playerIndex);

	void Draw();
	void DrawUI();
	void DrawMap();

	void ToggleFullscreen();
};

