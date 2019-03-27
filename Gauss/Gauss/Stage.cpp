#include "Stage.h"


// Save format
// rect rect rect rect pos pos bool bool
// Texturerect, vector2fPosition, colliderType, damageType 

int Stage::gridSize = 50;

Stage::Stage() 
	:stageSizeX(100), 
	stageSizeY(100), 
	tileMatrix(stageSizeX)
{
	this->stageSizeX = 100;
	this->stageSizeY = 100;

	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;

	for (int i = 0; i < this->stageSizeX; i++)
	{
		this->tileMatrix.Add(TileArr<Tile>(this->stageSizeY), i);
	}
}


Stage::~Stage()
{
}

void Stage::Update() {

}

// Dependent on the view
void Stage::Draw(RenderTarget &renderTarget, View &view) {
	// TODO: IF THINGS BREAK ITS THE STATIC_CAST<INT>
	this->fromCol = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / Stage::gridSize);
	if (this->fromCol <= 0) {
		this->fromCol = 0;
	}
	if (this->fromCol >= this->stageSizeX) {
		this->fromCol = this->stageSizeX;
	}

	this->toCol = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / Stage::gridSize + 1);
	if (this->toCol <= 0) {
		this->toCol = 0;
	}
	if (this->toCol >= this->stageSizeX) {
		this->toCol = this->stageSizeX;
	}


	this->fromRow = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / Stage::gridSize);
	if (this->fromRow <= 0) {
		this->fromRow = 0;
	}
	if (this->fromRow >= this->stageSizeY) {
		this->fromRow = this->stageSizeY;
	}

	this->toRow = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / Stage::gridSize + 1);
	if (this->toRow <= 0) {
		this->toRow = 0;
	}
	if (this->toRow >= this->stageSizeY) {
		this->toRow = this->stageSizeY;
	}

	for (int i = this->fromCol; i < this->toCol; i++)
	{
		for (int j = this->fromRow; j < this->toRow; j++)
		{
			if (!this->tileMatrix[i].IsNull(j)) {
				this->tileMatrix[i][j].Draw(renderTarget);
			}
		}
	}
}
