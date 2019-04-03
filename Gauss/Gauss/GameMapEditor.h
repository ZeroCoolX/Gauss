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
	const std::string MAP_FILEPATH = "Stages/";

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

	bool showTextureSelectUI;
	Sprite textureSelector;
	int textureSelectedX;
	int textureSelectedY;

	// TEXT
	Font font;

	// MAP
	std::string stageName;
	Stage *stage;

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

	void UpdateAddRemoveTiles();
	void UpdateMousePosition();
	void UpdateView(const float &dt);
	void UpdateUI();
	void Update(const float &dt);
	void UpdateTimers(const float &dt);
	void UpdateMap();
	void UpdateControls();
	void UpdateWallColliders(const float &dt, int playerIndex);

	void Draw();
	void DrawUIWindow();
	void DrawUIView();
	void DrawMap();

	void ToggleFullscreen();
	void NewStage();
	void SaveStage();
	void LoadStage();
};

