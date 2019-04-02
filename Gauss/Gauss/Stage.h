#pragma once

#include "TileArr.h"
#include "Gauss.h"

class Stage
{
private:
	long stageSizeX;
	long stageSizeY;
	TileArr<TileArr<Tile>> tileMatrix;
	dArr<Sprite> backgrounds;

	// Optimization
	int fromCol, toCol;
	int fromRow, toRow;

public:
	Stage(unsigned long sizeX, unsigned long sizeY);
	virtual ~Stage();

	inline TileArr<TileArr<Tile>>& getTiles() { return this->tileMatrix; }
	inline long getSizeX() const { return this->stageSizeX; }
	inline long getSizeY() const { return this->stageSizeY; }

	void UpdateBackground(const float &dt, Vector2f relativeOrigin);
	void Update();
	void Draw(RenderTarget &renderTarget, View &view);

	void AddTile(const Tile tile, long row, long col);
	void RemoveTile(long row, long col);
	void SaveStage(std::string fileName);
	void LoadStage(std::string fileName);
};


