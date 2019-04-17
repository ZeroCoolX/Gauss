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
#include "MenuButton.h"

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
	int tileToolSelect;

	int backgroundIndex;
	int backgroundWidth;
	int backgroundHeight;

	// Enemy Spawner
	Vector2i enemySpPositionGrid;
	bool enemySpRandPos;
	int enemySpType;
	int enemySpInterval;
	int numOfEnemies;
	float enemySpTimerMax;

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
	Text selectorText;
	Text enemySpText;

	dArr<MenuButton> buttons;

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
	void InitMenuButtons();
	void InitTextures();
	void InitUI();
	void InitMap();
	void InitText();

	void UpdateAddRemoveTiles();
	void UpdateMousePosition();
	void UpdateView(const float &dt);
	void UpdateUI();
	void Update(const float &dt);
	void UpdateTimers(const float &dt);
	void UpdateMap(const float &dt);
	void UpdateControls();
	void UpdateText();
	void UpdateButtons();
	void UpdateWallColliders(const float &dt, int playerIndex);

	void Draw();
	void DrawText();
	void DrawUIWindow();
	void DrawUIView();
	void DrawMap();

	void ToggleFullscreen();
	void NewStage();
	void SaveStage();
	void LoadStage();
	void SetBackground();
	void SetEnemySpawner();
};

