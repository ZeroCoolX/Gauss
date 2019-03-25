#include "Stage.h"



Stage::Stage()
{
	Tile tile(IntRect(0, 0, 100, 50), Vector2f(500.f, 50.f), false, false);
	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 100; j++)
		{
			this->tileMatrix[i].Add(tile);
		}
	}
}


Stage::~Stage()
{
}

void Stage::Update() {

}

void Stage::Draw(RenderTarget &renderTarget,
	int fromCol, int toCol,
	int fromRow, int toRow) {

	for (int i = fromCol; i < toCol; i++)
	{
		for (int j = fromRow; j < toRow; j++)
		{
			this->tileMatrix[i][j].Draw(renderTarget);
		}
	}
}
