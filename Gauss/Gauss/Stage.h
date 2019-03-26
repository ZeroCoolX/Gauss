#pragma once

#include "Tile.h"

class Stage
{
private:
	long stageSizeX;
	long stageSizeY;
	dArr< dArr<Tile> > tileMatrix;
	
	// Optimization
	int fromCol, toCol;
	int fromRow, toRow;

public:
	Stage();
	virtual ~Stage();

	inline dArr<dArr<Tile>> getTiles() { return this->tileMatrix; }
	
	void Update();
	void Draw(RenderTarget &renderTarget, View &view);

	// Statics
	static int gridSize;
};

