#include "Stage.h"


// Save format
// rect rect rect rect pos pos bool bool
// Texturerect, vector2fPosition, colliderType, damageType 

int Stage::gridSize = 50;

Stage::Stage()
{
	this->stageSizeX = 100;
	this->stageSizeY = 100;

	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;

	for (int i = 0; i < this->stageSizeX; i++)
	{
		this->tileMatrix.Add(dArr<Tile>());
		for (int j = 0; j < this->stageSizeY; j++)
		{
			this->tileMatrix[i].Add(Tile(IntRect(0,0,50,50), Vector2f(static_cast<float>(i * Stage::gridSize), static_cast<float>(j * Stage::gridSize)), 0, 0));
		}
	}
}


Stage::~Stage()
{
}

void Stage::Update() {

}

// Dependent on the view
void Stage::Draw(RenderTarget &renderTarget, View &view) {
	// TODO: can probably refactor this
	this->fromCol = (view.getCenter().x - view.getSize().x / 2) / Stage::gridSize;
	if (this->fromCol <= 0) {
		this->fromCol = 0;
	}
	if (this->fromCol >= this->stageSizeX) {
		this->fromCol = this->stageSizeX;
	}

	this->toCol = (view.getCenter().x + view.getSize().x / 2) / Stage::gridSize + 1;
	if (this->toCol <= 0) {
		this->toCol = 0;
	}
	if (this->toCol >= this->stageSizeX) {
		this->toCol = this->stageSizeX;
	}


	this->fromRow = (view.getCenter().y - view.getSize().y / 2) / Stage::gridSize;
	if (this->fromRow <= 0) {
		this->fromRow = 0;
	}
	if (this->fromRow >= this->stageSizeY) {
		this->fromRow = this->stageSizeY;
	}

	this->toRow = (view.getCenter().y + view.getSize().y / 2) / Stage::gridSize + 1;
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
			this->tileMatrix[i][j].Draw(renderTarget);
		}
	}
}
