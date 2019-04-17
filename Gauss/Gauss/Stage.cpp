#include "Stage.h"
#include <sstream>

// Save format
// rect rect rect rect pos pos bool bool
// Texturerect, vector2fPosition, colliderType, damageType 

dArr<Texture> Stage::backgroundTextures;
int Stage::numOfBackgrounds;

void Stage::InitTextures() {
	Texture temp;
	std::string line;
	std::ifstream fin;
	fin.open("Textures/Backgrounds/backgrounds.txt");

	if (fin.is_open()) {
		while (std::getline(fin, line)) {
			temp.loadFromFile(line.c_str());
			temp.setRepeated(true);
			Stage::backgroundTextures.Add(Texture(temp));
			line.clear();
		}
	}
	else {
		std::cout << "Could not open backgrounds file " << std::endl;
	}

	fin.close();

	Stage::numOfBackgrounds = Stage::backgroundTextures.Size();
}

Stage::Stage(unsigned long sizeX, unsigned long sizeY)
	:stageSizeX(sizeX),
	stageSizeY(sizeY),
	tileMatrix(stageSizeX),
	backgroundTiles(stageSizeX),
	enemySpawners(stageSizeX)
{
	this->stageSizeX = sizeX;
	this->stageSizeY = sizeY;
	this->scrollSpeed = 2.f;

	this->fromCol = 0;
	this->toCol = 0;
	this->fromRow = 0;
	this->toRow = 0;

	for (int i = 0; i < this->stageSizeX; i++)
	{
		this->tileMatrix.Add(TileArr<Tile>(this->stageSizeY), i);
		this->backgroundTiles.Add(TileArr<Tile>(this->stageSizeY), i);
		this->enemySpawners.Add(TileArr<EnemySpawner>(this->stageSizeY), i);
	}

	this->backgroundIndex = 0;
	this->backgroundRect.setSize(Vector2f(static_cast<float>(Gauss::BACKGROUND_SIZE), static_cast<float>(Gauss::BACKGROUND_SIZE)));
	this->backgroundRect.setTextureRect(IntRect(0, 0, Gauss::BACKGROUND_SIZE, Gauss::BACKGROUND_SIZE));
	this->backgroundRect.setTexture(&Stage::backgroundTextures[this->backgroundIndex]);
}


Stage::~Stage()
{
}

void Stage::AddTile(const Tile tile, long row, long col, int tileType) {
	if (row >= this->stageSizeX || col >= this->stageSizeY) {
		throw("Error: OUT OF BOUNDS: Stage::AddTile()");
	}

	if (tileType == TileType::BACKGROUND_TILE) {
		if (this->backgroundTiles[row].IsNull(col)) {
			this->backgroundTiles[row].Add(Tile(tile.getTexRect(), tile.getPos(), false, false), col);
			this->backgroundTiles[row][col].changeColorTo(Color(100, 100, 100, 255));
		}
		else {
			std::cout << "Alread background tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
	else if (tileType == TileType::FOREGROUND_TILE) {
		if (this->tileMatrix[row].IsNull(col)) {
			this->tileMatrix[row].Add(tile, col);
		}
		else {
			std::cout << "Alread foreground tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
}

void Stage::RemoveTile(long row, long col, int tileType) {
	if (row >= this->stageSizeX || col >= this->stageSizeY) {
		throw("Error: OUT OF BOUNDS: Stage::RemoveTile()");
	}
	if (tileType == TileType::BACKGROUND_TILE) {
		if (!this->backgroundTiles[row].IsNull(col)) {
			this->backgroundTiles[row].Remove(col);
		}
		else {
			std::cout << "No background tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
	else if (tileType == TileType::FOREGROUND_TILE) {
		if (!this->tileMatrix[row].IsNull(col)) {
			this->tileMatrix[row].Remove(col);
		}
		else {
			std::cout << "No tile existing at [" << row << "][" << col << "]" << std::endl;
		}
	}
}

void Stage::AddEnemySpawner(const EnemySpawner es, long row, long col) {
	if (this->enemySpawners[row].IsNull(col)) {
		this->enemySpawners[row].Add(es, col);
	}
	else {
		std::cout << "Alread enemy spawner tile existing at [" << row << "][" << col << "]" << std::endl;
	}
}

void Stage::RemoveEnemySpawner(long row, long col) {
	if (row >= this->stageSizeX || col >= this->stageSizeY) {
		throw("Error: OUT OF BOUNDS: Stage::RemoveEnemySpawner()");
	}
	if (!this->enemySpawners[row].IsNull(col)) {
			this->enemySpawners[row].Remove(col);
	}
	else {
		std::cout << "No Enemy Spawner tile existing at [" << row << "][" << col << "]" << std::endl;
	}
}

void Stage::SaveStage(std::string filename) {
	std::ofstream fout;
	fout.open(filename.c_str());
	if (fout.is_open()) {
		// Save map size
		fout << std::to_string(this->stageSizeX) << MAP_FILE_DELIM;
		fout << std::to_string(this->stageSizeY) << MAP_FILE_DELIM;

		// Save background
		fout << this->backgroundIndex 
			<< MAP_FILE_DELIM << static_cast<int>(this->backgroundRect.getGlobalBounds().width) 
			<< MAP_FILE_DELIM << static_cast<int>(this->backgroundRect.getGlobalBounds().height);

		fout << "\n";//breaker

		// Foreground tiles
		for (int i = 0; i < this->stageSizeX; i++)
		{
			// Foreground tiles
			for (int j = 0; j < this->stageSizeY; j++)
			{
				if (!this->tileMatrix[i].IsNull(j)) {
					fout << this->tileMatrix[i][j].GetAsString() << MAP_FILE_DELIM;
				}
			}
		}

		fout << "\n";// breaker

		// Background tiles
		for (int i = 0; i < this->stageSizeX; i++)
		{
			// Background tiles
			for (int j = 0; j < this->stageSizeY; j++)
			{
				if (!this->backgroundTiles[i].IsNull(j)) {
					fout << this->backgroundTiles[i][j].GetAsString() << MAP_FILE_DELIM;
				}
			}
		}

		fout << "\n";// breaker

		// Enemy Spawners
		for (int i = 0; i < this->stageSizeX; i++)
		{
			// Enemy Spawners
			for (int j = 0; j < this->stageSizeY; j++)
			{
				if (!this->enemySpawners[i].IsNull(j)) {
					fout << this->enemySpawners[i][j].GetAsString() << MAP_FILE_DELIM;
				}
			}
		}

	}
	else {
		std::cout << "Could not open map file: " << filename << std::endl;
	}

	fout.close();
}

bool Stage::LoadStage(std::string filename, View &view) {
	std::ifstream fin;
	bool successLoading = false;
	std::stringstream ss;
	std::string line;

	// Map
	unsigned sizeX = 0;
	unsigned sizeY = 0;

	// Background
	int bgWidth;
	int bgHeight;

	// Open file
	fin.open("Stages/" + filename);
	if (fin.is_open()) {

		// Load the header - LINE 1
		std::getline(fin, line);
		ss.str(line);

		ss >> sizeX;
		this->stageSizeX = sizeX;
		ss >> sizeY;
		this->stageSizeY = sizeY;

		this->backgroundIndex = 0;
		ss >> this->backgroundIndex >> bgWidth >> bgHeight;

		this->backgroundRect.setSize(Vector2f(static_cast<float>(bgWidth), static_cast<float>(bgHeight)));
		this->backgroundRect.setTexture(&Stage::backgroundTextures[this->backgroundIndex]);
		this->backgroundRect.setTextureRect(IntRect(0, 0, bgWidth, bgHeight));
		this->backgrounds.Add(this->backgroundRect);

		// Clear and Resize map to size
		this->tileMatrix.ResizeClear(this->stageSizeX);
		this->backgroundTiles.ResizeClear(this->stageSizeX);
		this->enemySpawners.ResizeClear(this->stageSizeX);

		for (int i = 0; i < this->stageSizeX; i++)
		{
			this->tileMatrix.Add(TileArr<Tile>(stageSizeY), i);
			this->backgroundTiles.Add(TileArr<Tile>(stageSizeY), i);
			this->enemySpawners.Add(TileArr<EnemySpawner>(stageSizeY), i);
		}

		line.clear();
		ss.clear();

		// Tiles
		int rectLeft = 0;
		int rectTop = 0;
		int rectWidth = 0;
		int rectHeight = 0;
		int gridPosX = 0;
		int gridPosY = 0;
		bool isCollider = 0;
		bool isDamaging = 0;

		// Load the foreground - LINE 2
		std::getline(fin, line);
		ss.str(line);
		while (
			ss >> rectLeft >> rectTop
			>> rectWidth >> rectHeight
			>> gridPosX >> gridPosY
			>> isCollider >> isDamaging
			) {

			this->tileMatrix[gridPosX].Add(
				Tile(
					IntRect(rectLeft, rectTop, rectWidth, rectHeight),
					Vector2f(static_cast<float>(gridPosX * Gauss::GRID_SIZE), static_cast<float>(gridPosY * Gauss::GRID_SIZE)),
					isCollider,
					isDamaging), gridPosY
			);
		}

		line.clear();
		ss.clear();

		// Load the background - LINE 3
		std::getline(fin, line);
		ss.str(line);
		while (
			ss >> rectLeft >> rectTop
			>> rectWidth >> rectHeight
			>> gridPosX >> gridPosY
			>> isCollider >> isDamaging
			) {

			this->backgroundTiles[gridPosX].Add(
				Tile(
					IntRect(rectLeft, rectTop, rectWidth, rectHeight),
					Vector2f(static_cast<float>(gridPosX * Gauss::GRID_SIZE), static_cast<float>(gridPosY * Gauss::GRID_SIZE)),
					isCollider,
					isDamaging), gridPosY
			);
			this->backgroundTiles[gridPosX][gridPosY].changeColorTo(Gauss::BACKGROUND_COLOR);
		}

		// Enemy Spawner
		int enemySpType = 0;
		int enemySpInterval = 0;
		int enemySpNumOfEnemies = 0;
		bool randSpawnPos = 0;

		line.clear();
		ss.clear();

		//// Load the enemy spawners
		std::getline(fin, line);
		ss.str(line);
		while (
			ss >> gridPosX 
			>> gridPosY 
			>> randSpawnPos
			>> enemySpType
			>> enemySpInterval 
			>> enemySpNumOfEnemies
			) {

			this->enemySpawners[gridPosX].Add(
				EnemySpawner(
					Vector2i(gridPosX, gridPosY),
					randSpawnPos,
					enemySpType,
					enemySpInterval,
					enemySpNumOfEnemies), gridPosY
			);
		}

		successLoading = true;
		std::cout << "Success loading" << std::endl;
	}
	else {
		std::cout << "File failed to open " << std::endl;
		successLoading = false;
	}

	fin.close();
	return successLoading;
}

void Stage::UpdateBackground(const float &dt, View &view) {
	float backgroundRightEdge = 0.f;
	float viewLeftEdge = 0.f;
	float viewRightEdge = 0.f;
	// Parallax scrolling simulation
	for (size_t i = 0; i < this->backgrounds.Size(); i++)
	{
		this->backgrounds[i].move(-this->scrollSpeed * 0.6f * dt * DeltaTime::dtMultiplier , 0.f);

		backgroundRightEdge = this->backgrounds[i].getPosition().x + this->backgrounds[i].getGlobalBounds().width;
		viewLeftEdge = view.getCenter().x - view.getSize().x / 2.f;
		viewRightEdge = view.getCenter().x + view.getSize().x / 2.f;

		if (this->backgrounds.Size() < 3 && backgroundRightEdge <= viewRightEdge) {
			this->backgroundRect.setPosition(backgroundRightEdge, this->backgroundRect.getPosition().y);
			this->backgrounds.Add(this->backgroundRect);
		}
		else if(backgroundRightEdge <= viewLeftEdge){
			this->backgrounds.Remove(i);
			break;
		}
	}
}

void Stage::Update(const float &dt, View &view, bool editor) {
	//this->fromCol = static_cast<int>((view.getCenter().x - view.getSize().x / 2) / Gauss::GRID_SIZE);
	//if (this->fromCol <= 0) {
	//	this->fromCol = 0;
	//}
	//if (this->fromCol >= this->stageSizeX) {
	//	this->fromCol = this->stageSizeX;
	//}

	//this->toCol = static_cast<int>((view.getCenter().x + view.getSize().x / 2) / Gauss::GRID_SIZE + 1);
	//if (this->toCol <= 0) {
	//	this->toCol = 0;
	//}
	//if (this->toCol >= this->stageSizeX) {
	//	this->toCol = this->stageSizeX;
	//}


	//this->fromRow = static_cast<int>((view.getCenter().y - view.getSize().y / 2) / Gauss::GRID_SIZE);
	//if (this->fromRow <= 0) {
	//	this->fromRow = 0;
	//}
	//if (this->fromRow >= this->stageSizeY) {
	//	this->fromRow = this->stageSizeY;
	//}

	//this->toRow = static_cast<int>((view.getCenter().y + view.getSize().y / 2) / Gauss::GRID_SIZE + 1);
	//if (this->toRow <= 0) {
	//	this->toRow = 0;
	//}
	//if (this->toRow >= this->stageSizeY) {
	//	this->toRow = this->stageSizeY;
	//}


	//// Update tiles
	//for (int i = this->fromCol; i < this->toCol; i++)
	//{
	//	for (int j = this->fromRow; j < this->toRow; j++)
	//	{
	//		// Update background tiles
	//		if (!this->backgroundTiles[i].IsNull(j)) {
	//			this->backgroundTiles[i][j].Update(dt);
	//		}

	//		// Update foreground tiles
	//		if (!this->tileMatrix[i].IsNull(j)) {
	//			this->tileMatrix[i][j].Update(dt);
	//		}

	//		// Update enemy Spawners tiles
	//		if (!this->enemySpawners[i].IsNull(j)) {
	//			this->enemySpawners[i][j].Update(dt);
	//		}

	//		//this->UpdateBackground(dt, view, i, j);
	//	}
	//}
	if (!editor) {
		this->UpdateBackground(dt, view);
	}
}

void Stage::Draw(RenderTarget &renderTarget, View &view, bool editor, Font &font) {
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

	if (editor) {
		renderTarget.draw(this->backgroundRect);
	}
	else {
		// Draw background image
		for (size_t i = 0; i < this->backgrounds.Size(); i++)
		{
			renderTarget.draw(this->backgrounds[i]);
		}
	}

	// Draw tiles
	for (int i = this->fromCol; i < this->toCol; i++)
	{
		for (int j = this->fromRow; j < this->toRow; j++)
		{
			// Draw background tiles
			if (!this->backgroundTiles[i].IsNull(j)) {
				this->backgroundTiles[i][j].Draw(renderTarget);
			}

			// Draw foreground tiles
			if (!this->tileMatrix[i].IsNull(j)) {
				this->tileMatrix[i][j].Draw(renderTarget);
			}

			// Draw enemy spawners tiles
			if (!this->enemySpawners[i].IsNull(j)) {
				this->enemySpawners[i][j].Draw(renderTarget, font);
			}
		}
	}
}

void Stage::SetBackground(const int index, const int width, const int height) {
	if (index < 0 || index >= Stage::numOfBackgrounds) {
		std::cout << "ERROR: No such background for index " << index << std::endl;
		return;
	}
	this->backgroundIndex = index;
	this->backgroundRect.setSize(Vector2f(static_cast<float>(width), static_cast<float>(height)));
	this->backgroundRect.setTextureRect(IntRect(0, 0, width, height));
	this->backgroundRect.setTexture(&Stage::backgroundTextures[this->backgroundIndex]);
}

void Stage::SetBackgroundSize(float width, float height) {
	if (width < 0 || height < 0) {
		std::cout << "ERROR: Invalid size parameters " << width << ", " << height << std::endl;
		width = static_cast<float>(Gauss::BACKGROUND_SIZE);
		height = static_cast<float>(Gauss::BACKGROUND_SIZE);
	}
	this->backgroundRect.setSize(Vector2f(width, height));
}

void Stage::Reset(View &view) {
	this->backgrounds.Clear();
	this->backgroundRect.setPosition(Vector2f(
		view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2
	));
	this->backgrounds.Add(this->backgroundRect);

	// Reset enemy spawners
}


