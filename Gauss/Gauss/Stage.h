#pragma once

#include "TileArr.h"
#include "Gauss.h"
#include "EnemySpawner.h"

class Stage
{
private:
	const std::string MAP_FILE_DELIM = " ";
	long stageSizeX;
	long stageSizeY;
	float scrollSpeed;
	TileArr<TileArr<Tile>> tileMatrix;
	TileArr<TileArr<Tile>> backgroundTiles;
	TileArr<TileArr<EnemySpawner>> enemySpawners;

	int backgroundIndex;
	RectangleShape backgroundRect;
	dArr<RectangleShape> backgrounds;

	// Optimization
	int fromCol, toCol;
	int fromRow, toRow;

public:
	Stage(unsigned long sizeX, unsigned long sizeY);
	virtual ~Stage();

	inline TileArr<TileArr<Tile>>& getTiles() { return this->tileMatrix; }
	inline TileArr<TileArr<EnemySpawner>>& getEnemySpawners() { return this->enemySpawners; }
	inline long getSizeX() const { return this->stageSizeX; }
	inline long getSizeY() const { return this->stageSizeY; }
	inline float getScrollSpeed() const { return this->scrollSpeed; }

	void UpdateBackground(const float &dt, View &view);
	void Update(const float &dt, View &view, bool editor);
	void Draw(RenderTarget &renderTarget, View &view, bool editor, Font &font);

	void AddEnemySpawner(const EnemySpawner es, long row, long col);
	void RemoveEnemySpawner(long row, long col);
	void AddTile(const Tile tile, long row, long col, int tileType);
	void RemoveTile(long row, long col, int tileType);
	void SaveStage(std::string fileName);
	bool LoadStage(std::string fileName, View &view);
	void SetBackground(const int index, const int width, const int height);
	void SetBackgroundSize(float width, float height);
	void Reset(View &view);

	static dArr<Texture> backgroundTextures;
	static int numOfBackgrounds;

	static void InitTextures();

	enum TileType { FOREGROUND_TILE = 0, BACKGROUND_TILE, ENEMY_SPAWNER_TILE };
};


