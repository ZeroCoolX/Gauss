#include "Stage.h"


// Save format
// rect rect rect rect pos pos bool bool
// Texturerect, vector2fPosition, colliderType, damageType 

Stage::Stage(unsigned long sizeX, unsigned long sizeY)
	:stageSizeX(sizeX),
	stageSizeY(sizeY),
	tileMatrix(stageSizeX),
	backgroundTiles(stageSizeX)
{
	this->stageSizeX = sizeX;
	this->stageSizeY = sizeY;

	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;

	for (int i = 0; i < this->stageSizeX; i++)
	{
		this->tileMatrix.Add(TileArr<Tile>(this->stageSizeY), i);
		this->backgroundTiles.Add(TileArr<Tile>(this->stageSizeY), i);
	}
}


Stage::~Stage()
{
}

void Stage::AddTile(const Tile tile, long row, long col, bool bkg) {
	if (row >= this->stageSizeX || col >= this->stageSizeY) {
		throw("Error: OUT OF BOUNDS: Stage::AddTile()");
	}

	if (bkg) {
		if (this->backgroundTiles[row].IsNull(col)) {
			this->backgroundTiles[row].Add(Tile(tile.getTexRect(), tile.getPos(), false, false), col);
			this->backgroundTiles[row][col].changeColorTo(Color(100, 100, 100, 255));
		}
		else {
			std::cout << "Alread background tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
	else {
		if (this->tileMatrix[row].IsNull(col)) {
			this->tileMatrix[row].Add(tile, col);
		}
		else {
			std::cout << "Alread tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
}

void Stage::RemoveTile(long row, long col, bool bkg) {
	if (row >= this->stageSizeX || col >= this->stageSizeY) {
		throw("Error: OUT OF BOUNDS: Stage::RemoveTile()");
	}
	if (bkg) {
		if (!this->backgroundTiles[row].IsNull(col)) {
			this->backgroundTiles[row].Remove(col);
		}
		else {
			std::cout << "No background tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
	else {
		if (!this->tileMatrix[row].IsNull(col)) {
			this->tileMatrix[row].Remove(col);
		}
		else {
			std::cout << "No tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
}

void Stage::SaveStage(std::string filename) {
	std::ofstream fout;
	fout.open(filename.c_str());
	if (fout.is_open()) {
		// Save map size
		fout << std::to_string(this->stageSizeX) << MAP_FILE_DELIM;
		fout << std::to_string(this->stageSizeY) << MAP_FILE_DELIM;

		// Save background path
		fout << "NONE" << MAP_FILE_DELIM;

		for (int i = 0; i < this->stageSizeX; i++)
		{
			for (int j = 0; j < this->stageSizeY; j++)
			{
				if (!this->tileMatrix[i].IsNull(j)) {
					fout << this->tileMatrix[i][j].GetAsString() << MAP_FILE_DELIM;
				}
			}
		}
	}
	else {
		std::cout << "Could not open map file: " << filename << std::endl;
	}

	fout.close();
}

bool Stage::LoadStage(std::string filename) {
	std::ifstream fin;
	bool successLoading = false;

	// Open file
	fin.open("Stages/" + filename);
	if (fin.is_open()) {
		unsigned sizeX = 0;
		unsigned sizeY = 0;
		std::string backgroundPath;

		int rectLeft = 0;
		int rectTop = 0;
		int rectWidth = 0;
		int rectHeight = 0;
		int gridPosX = 0;
		int gridPosY = 0;
		bool isCollider = 0;
		bool isDamaging = 0;
		int damage = 0;

		fin >> sizeX;
		this->stageSizeX = sizeX;
		fin >> sizeY;
		this->stageSizeY = sizeY;

		fin >> backgroundPath;
		this->backgroundTexture.loadFromFile(backgroundPath);
		this->background1.setTexture(this->backgroundTexture);
		this->background2.setTexture(this->backgroundTexture);

		// Clear the old stage
		this->tileMatrix.ResizeClear(this->stageSizeX);
		for (int i = 0; i < this->stageSizeX; i++)
		{
			this->tileMatrix.Add(TileArr<Tile>(stageSizeY), i);
		}

		// Load new Stage
		while (
			fin >> rectLeft >> rectTop
			>> rectWidth >> rectHeight
			>> gridPosX >> gridPosY
			>> isCollider >> isDamaging >> damage
			) {

			this->tileMatrix[gridPosX].Add(
				Tile(
					IntRect(rectLeft, rectTop, rectWidth, rectHeight), 
					Vector2f(gridPosX * Gauss::GRID_SIZE, gridPosY * Gauss::GRID_SIZE), 
					isCollider, 
					isDamaging), gridPosY
			);
		}
		fin.ignore();
		successLoading = true;
		std::cout << "Success loading" << std::endl;
	}
	else {
		std::cout << "File failed to open " << std::endl;
		successLoading = false;
	}
	
	// Load map size

	// Set tilearr size

	// Load backgrounds

	// Load tiles

	fin.close();
	return successLoading;
}

void Stage::UpdateBackground(const float &dt, Vector2f relativeOrigin) {

}

void Stage::Update(const float &dt, Vector2f relativeOrigin, int offset) {
	// TODO: IF THINGS BREAK ITS THE STATIC_CAST<INT>
	this->fromCol = static_cast<int>((relativeOrigin.x - offset) / Gauss::GRID_SIZE);
	if (this->fromCol <= 0) {
		this->fromCol = 0;
	}
	if (this->fromCol >= this->stageSizeX) {
		this->fromCol = this->stageSizeX;
	}

	this->toCol = static_cast<int>((relativeOrigin.x + offset) / Gauss::GRID_SIZE);
	if (this->toCol <= 0) {
		this->toCol = 0;
	}
	if (this->toCol >= this->stageSizeX) {
		this->toCol = this->stageSizeX;
	}


	this->fromRow = static_cast<int>((relativeOrigin.y - offset) / Gauss::GRID_SIZE);
	if (this->fromRow <= 0) {
		this->fromRow = 0;
	}
	if (this->fromRow >= this->stageSizeY) {
		this->fromRow = this->stageSizeY;
	}

	this->toRow = static_cast<int>((relativeOrigin.y + offset) / Gauss::GRID_SIZE);
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
			//if (!this->backgroundTiles[i].IsNull(j)) {
			//	this->backgroundTiles[i][j].Update(dt);
			//}

			if (!this->tileMatrix[i].IsNull(j)) {
				this->tileMatrix[i][j].Update(dt);
			}
		}
	}
}

// Dependent on the view
void Stage::Draw(RenderTarget &renderTarget, View &view) {
	// TODO: IF THINGS BREAK ITS THE STATIC_CAST<INT>
	this->fromCol = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / Gauss::GRID_SIZE);
	if (this->fromCol <= 0) {
		this->fromCol = 0;
	}
	if (this->fromCol >= this->stageSizeX) {
		this->fromCol = this->stageSizeX;
	}

	this->toCol = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / Gauss::GRID_SIZE + 1);
	if (this->toCol <= 0) {
		this->toCol = 0;
	}
	if (this->toCol >= this->stageSizeX) {
		this->toCol = this->stageSizeX;
	}


	this->fromRow = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / Gauss::GRID_SIZE);
	if (this->fromRow <= 0) {
		this->fromRow = 0;
	}
	if (this->fromRow >= this->stageSizeY) {
		this->fromRow = this->stageSizeY;
	}

	this->toRow = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / Gauss::GRID_SIZE + 1);
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
			if (!this->backgroundTiles[i].IsNull(j)) {
				this->backgroundTiles[i][j].Draw(renderTarget);
			}

			if (!this->tileMatrix[i].IsNull(j)) {
				this->tileMatrix[i][j].Draw(renderTarget);
			}
		}
	}
}
