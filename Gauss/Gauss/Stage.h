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
	Stage();
	virtual ~Stage();

	inline TileArr<TileArr<Tile>>& getTiles() { return this->tileMatrix; }
	
	void Update();
	void Draw(RenderTarget &renderTarget, View &view);
};

