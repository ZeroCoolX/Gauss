#pragma once

#include "TileArr.h"
#include "Gauss.h"

class Stage
{
private:
	long stageSizeX;
	long stageSizeY;
	TileArr<TileArr<Tile>> tileMatrix;

	// Optimization
	int fromCol, toCol;
	int fromRow, toRow;

public:
	Stage(unsigned long sizeX, unsigned long sizeY);
	virtual ~Stage();

	inline TileArr<TileArr<Tile>>& getTiles() { return this->tileMatrix; }
	inline long getSizeX() const { return this->stageSizeX; }
	inline long getSizeY() const { return this->stageSizeY; }

	void Update();
	void Draw(RenderTarget &renderTarget, View &view);

	void AddTile(const Tile tile, unsigned row, unsigned col);
};

