#pragma once

#include "Tile.h"

class Stage
{
private:
	dArr< dArr<Tile> > tileMatrix;


public:
	Stage();
	virtual ~Stage();

	inline dArr<dArr<Tile>> getTiles() { return this->tileMatrix; }
	
	void Update();
	void Draw(RenderTarget &renderTarget, 
		int fromCol, int toCol, 
		int fromRow, int toRow);

};

