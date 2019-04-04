#pragma once

#include "TileArr.h"
#include "Gauss.h"

class Stage
{
private:
	const std::string MAP_FILE_DELIM = " ";
	long stageSizeX;
	long stageSizeY;
	TileArr<TileArr<Tile>> tileMatrix;
	TileArr<TileArr<Tile>> backgroundTiles;

	Texture backgroundTexture;
	Sprite background1;
	Sprite background2;

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
	void Update(const float &dt, Vector2f relativeOrigin, int offset);
	void Draw(RenderTarget &renderTarget, View &view);

	void AddTile(const Tile tile, long row, long col, bool bkg = false);
	void RemoveTile(long row, long col, bool bkg = false);
	void SaveStage(std::string fileName);
	bool LoadStage(std::string fileName);
};


