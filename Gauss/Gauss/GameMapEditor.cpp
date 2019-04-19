#include "GameMapEditor.h"

GameMapEditor::GameMapEditor(RenderWindow *window)
{
	this->window = window;

	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;
	this->tileToolSelect = Stage::TileType::FOREGROUND_TILE;
	this->stage = nullptr;
	this->backgroundIndex = 0;
	this->backgroundWidth = Gauss::BACKGROUND_SIZE;
	this->backgroundHeight = Gauss::BACKGROUND_SIZE;

	this->enemySpType = 0;
	this->enemySpInterval = 0;
	this->numOfEnemies = 0;
	this->enemySpRandPos = false;

	this->InitView();

	this->InitTextures();

	this->InitUI();

	this->InitMap();

	this->InitText();

	this->InitMenuButtons();
}

GameMapEditor::~GameMapEditor()
{
	delete this->stage;
}


void GameMapEditor::InitView() {
	this->mainView.setSize(Vector2f(this->window->getSize()));
	this->mainView.setCenter(Vector2f(
		this->window->getSize().x / 2.f,
		this->window->getSize().y / 2.f));
}

void GameMapEditor::InitMenuButtons() {
	MenuButton temp(this->font, "EnemySpawnerSelect", 12, Vector2f(600.f, 500.f), 0);

	this->buttons.Add(temp);
}

void GameMapEditor::InitTextures() {
}

void GameMapEditor::InitUI() {
	this->showTextureSelectUI = true;

	this->textureSelectedX = 0;
	this->textureSelectedY = 0;

	this->selector.setSize(Vector2f(static_cast<float>(Gauss::GRID_SIZE), static_cast<float>(Gauss::GRID_SIZE)));
	this->selector.setFillColor(Color::Transparent);
	this->selector.setOutlineColor(Color::Red);
	this->selector.setOutlineThickness(2.f);

	this->textureSelector.setTexture(Tile::tileTextures);
}

void GameMapEditor::InitMap() {
	this->stageName = "EMPTY";
	this->stage = new Stage(10, 10);
}

void GameMapEditor::InitText() {
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->selectorText.setFont(this->font);
	this->selectorText.setCharacterSize(14);
	this->selectorText.setFillColor(Color::White);
	this->selectorText.setPosition(Vector2f(this->mousePosWindow));

	this->enemySpText.setFont(this->font);
	this->enemySpText.setCharacterSize(14);
	this->enemySpText.setFillColor(Color::White);
	this->enemySpText.setPosition(Vector2f(this->mousePosWindow));
}

void GameMapEditor::UpdateMousePosition() {
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosWorld = this->window->mapPixelToCoords(this->mousePosWindow);

	if (this->showTextureSelectUI) {
#pragma warning(push)
#pragma warning(disable:4244)
		this->mousePosGrid = Vector2i(
			this->mousePosWindow.x / (Gauss::GRID_SIZE + 1),
			this->mousePosWindow.y / (Gauss::GRID_SIZE + 1)
		);
#pragma warning(pop)
	}
	else {
#pragma warning(push)
#pragma warning(disable:4244)
		this->mousePosGrid = Vector2i(
			std::max(0.f, this->mousePosWorld.x / Gauss::GRID_SIZE),
			std::max(0.f, this->mousePosWorld.y / Gauss::GRID_SIZE)
		);
#pragma warning(pop)
		// Max bounds check
		if (this->mousePosGrid.x >= this->stage->getSizeX()) {
			this->mousePosGrid.x = this->stage->getSizeX() - 1;
		}
		if (this->mousePosGrid.y >= this->stage->getSizeY()) {
			this->mousePosGrid.y = this->stage->getSizeY() - 1;
		}
	}
}

void GameMapEditor::UpdateView(const float &dt) {
	float mapMoveSpeed = 10.f;
	if (Keyboard::isKeyPressed(Keyboard::LShift)) {
		mapMoveSpeed = 30.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::W)) {
		this->mainView.move(0.f, -mapMoveSpeed * dt * DeltaTime::dtMultiplier);
	}else if (Keyboard::isKeyPressed(Keyboard::S)) {
		this->mainView.move(0.f, mapMoveSpeed * dt * DeltaTime::dtMultiplier);
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		this->mainView.move(-mapMoveSpeed * dt * DeltaTime::dtMultiplier, 0.f);
	}else if (Keyboard::isKeyPressed(Keyboard::D)) {
		this->mainView.move(mapMoveSpeed * dt * DeltaTime::dtMultiplier, 0.f);
	}
}

void GameMapEditor::UpdateUI() {
	if (this->showTextureSelectUI) {
		this->selector.setPosition(
			this->mousePosGrid.x * static_cast<float>(Gauss::GRID_SIZE + 1),
			this->mousePosGrid.y * static_cast<float>(Gauss::GRID_SIZE + 1));
	}
	else {
		this->selector.setPosition(
			this->mousePosGrid.x * static_cast<float>(Gauss::GRID_SIZE),
			this->mousePosGrid.y * static_cast<float>(Gauss::GRID_SIZE));
	}
}

void GameMapEditor::UpdateTimers(const float &dt) {
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void GameMapEditor::UpdateMap(const float &dt) {
	this->stage->Update(dt, this->mainView, true);
}

void GameMapEditor::UpdateControls() {
	// Show tilemap editor
	if (Keyboard::isKeyPressed(Keyboard::Tab) && this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;
		this->showTextureSelectUI = !this->showTextureSelectUI;
	}

	if (this->showTextureSelectUI) {
		// Texture select
		if (Mouse::isButtonPressed(Mouse::Left)) {
			this->textureSelectedX = this->mousePosGrid.x * (Gauss::GRID_SIZE + 1);
			this->textureSelectedY = this->mousePosGrid.y * (Gauss::GRID_SIZE + 1);
		}
	}
	// Update Selector
	else {
		this->UpdateAddRemoveTiles();
	}

	// New Stage
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::N) && this->keyTime >= this->keyTimeMax) {
		this->NewStage();
		this->keyTime = 0.f;
	}

	// Switch background/foreground tile
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::B) && this->keyTime >= this->keyTimeMax) {
		if (tileToolSelect == Stage::TileType::BACKGROUND_TILE) {
			tileToolSelect = Stage::TileType::FOREGROUND_TILE;
		}
		else {
			tileToolSelect = Stage::BACKGROUND_TILE;
		}
		this->keyTime = 0.f;
	}

	// Select background and size
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::W) && this->keyTime >= this->keyTimeMax) {
		this->SetBackground();
		this->keyTime = 0.f;
	}

	// Set Enemy Spawner 
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::E) && this->keyTime >= this->keyTimeMax) {
		this->SetEnemySpawner();
		this->keyTime = 0.f;
	}

	// Assign Enemy Spawner 
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::E) && this->keyTime >= this->keyTimeMax) {
		tileToolSelect = Stage::ENEMY_SPAWNER_TILE;
		this->keyTime = 0.f;
	}

	// Save stage
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::S) && this->keyTime >= this->keyTimeMax) {
		this->SaveStage();
		this->keyTime = 0.f;
	}

	// Load stage
	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::L) && this->keyTime >= this->keyTimeMax) {
		this->LoadStage();
		this->keyTime = 0.f;
	}
}

void GameMapEditor::UpdateText() {
	if (this->showTextureSelectUI) {
		this->selectorText.setPosition(Vector2f(this->mousePosWindow.x + 20.f, static_cast<float>(this->mousePosWindow.y)));
		this->enemySpText.setPosition(Vector2f(this->mousePosWindow.x + 20.f, this->mousePosWindow.y + 20.f));
	}
	else {
		this->selectorText.setPosition(Vector2f(this->mousePosWorld.x + 20.f, static_cast<float>(this->mousePosWorld.y)));
		this->enemySpText.setPosition(Vector2f(this->mousePosWorld.x + 20.f, this->mousePosWorld.y + 20.f));
	}

	if(this->tileToolSelect == Stage::TileType::BACKGROUND_TILE) {
		this->selectorText.setString("BACKGROUND_TILE");
	}
	else if(this->tileToolSelect == Stage::TileType::FOREGROUND_TILE){
		this->selectorText.setString("FOREGROUND_TILE");
	}
	else {
		this->selectorText.setString("ENEMY_SPAWNER_TILE");
	}

	this->enemySpText.setString(
		"Type: " + std::to_string(this->enemySpType) +
		"\nInterval: " + std::to_string(this->enemySpInterval) +
		"\nNum Enemies: " + std::to_string(this->numOfEnemies)
	);
}

void GameMapEditor::UpdateButtons() {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i].Update(this->mousePosWorld);
	}
}

void GameMapEditor::UpdateAddRemoveTiles() {
	if (Mouse::isButtonPressed(Mouse::Left)) {
		if (this->tileToolSelect == Stage::TileType::ENEMY_SPAWNER_TILE) {
			this->enemySpPositionGrid = this->mousePosGrid;
			this->stage->AddEnemySpawner(
				EnemySpawner(
					this->enemySpPositionGrid,
					this->enemySpRandPos,
					this->enemySpVelocity,
					this->enemySpType,
					this->enemySpInterval,
					this->numOfEnemies
				),
				this->mousePosGrid.x,
				this->mousePosGrid.y
			);
		}
		else {
			this->stage->AddTile(
				Tile(
					IntRect(this->textureSelectedX, this->textureSelectedY, Gauss::GRID_SIZE, Gauss::GRID_SIZE),
					Vector2f(static_cast<float>(this->mousePosGrid.x * Gauss::GRID_SIZE), static_cast<float>(this->mousePosGrid.y* Gauss::GRID_SIZE)),
					false,
					false
				),
				this->mousePosGrid.x,
				this->mousePosGrid.y, this->tileToolSelect);
		}
	}
	else if (Mouse::isButtonPressed(Mouse::Right)) {
		if (this->tileToolSelect == Stage::TileType::ENEMY_SPAWNER_TILE) {
			this->stage->RemoveEnemySpawner(this->mousePosGrid.x, this->mousePosGrid.y);
		}
		else {
			this->stage->RemoveTile(this->mousePosGrid.x, this->mousePosGrid.y, this->tileToolSelect);
		}
	}
}

void GameMapEditor::UpdateWallColliders(const float &dt, int playerIndex) {
	//for (size_t i = 0; i < this->tiles.Size(); i++)
	//{
	//	if (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
	//		while (this->players[playerIndex].collidesWith(this->tiles[i].getBounds())) {
	//			this->players[playerIndex].move(
	//				20.f * -1.f * this->players[playerIndex].getNormDir().x,
	//				20.f * -1.f * this->players[playerIndex].getNormDir().y
	//			);
	//		}

	//		this->players[playerIndex].resetVelocity();
	//	}
	//}
}

void GameMapEditor::Update(const float &dt) {
	// Keytime update
	this->UpdateTimers(dt);

	// Mouse update
	this->UpdateMousePosition();

	// Fullscreen check
	this->ToggleFullscreen();

	// Update Map
	this->UpdateMap(dt);

	// General controls
	this->UpdateControls();

	// Text update
	this->UpdateText();

	// Buttons update
	this->UpdateButtons();

	// Update UI
	this->UpdateUI();

	// view update
	this->UpdateView(dt);
}

void GameMapEditor::DrawText() {
	this->window->draw(this->selectorText);
	this->window->draw(this->enemySpText);
}

void GameMapEditor::DrawUIWindow() {
	this->window->draw(this->textureSelector);
	this->window->draw(this->selector);

}

void GameMapEditor::DrawUIView() {
	this->window->draw(this->selector);
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i].Draw(*this->window);
	}
}

void GameMapEditor::DrawMap() {
	this->stage->Draw(*this->window, this->mainView, true, this->font);
}

void GameMapEditor::Draw() {
	this->window->clear();

	// Set View
	this->window->setView(this->mainView);

	// Draw Map
	this->DrawMap();

	// Draw Text
	this->DrawText();

	// Draw the window
	if (this->showTextureSelectUI) {
		// Draw UI - update view
		this->window->setView(this->window->getDefaultView());
		this->DrawUIWindow();
	}
	else {
		// Set View
		this->window->setView(this->mainView);
		// Draw the view
		this->DrawUIView();
	}

	// Draw everything to the window
	this->window->display();
}

void GameMapEditor::ToggleFullscreen() {
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;

		this->window->close();
		this->fullscreen = !this->fullscreen;
		this->window->create(sf::VideoMode(1920, 1080), "Map Editor", this->fullscreen ? Style::Fullscreen : Style::Default);
	}
}

void GameMapEditor::NewStage() {
	unsigned mapSizeX = 0;
	unsigned mapSizeY = 0;
	std::cout << "New Stage\n\n" << std::endl;

	std::cout << "Stage Name :>";
	std::getline(std::cin, this->stageName);
	this->stageName.append(".smap");


	std::cout << "Stage size X :>";
	std::cin >> mapSizeX;
	while (std::cin.fail() || mapSizeX <= 0) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Stage size X :>";
		std::cin >> mapSizeX;
	}

	std::cout << "Stage size Y :>";
	std::cin >> mapSizeY;
	while (std::cin.fail() || mapSizeY <= 0) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Stage size Y :>";
		std::cin >> mapSizeY;
	}

	std::cin.ignore(100, '\n');
	std::cout << "\n";

	delete this->stage;
	this->stage = new Stage(mapSizeX, mapSizeY);

	std::cout << "Stage " << this->stageName << " created!" << "\n\n";
}

void GameMapEditor::SaveStage() {
	std::cout << "Saving Stage: " << this->stageName << std::endl;
	
	std::ifstream fin;
	const std::string stagePath = MAP_FILEPATH + this->stageName;

	fin.open(stagePath);
	if (fin.is_open()) {
		std::cout << "File " << this->stageName << " already exists. Overwrite? (1)YES / (0)NO:>";

		int overwrite = 0;
		// Function-ize
		std::cin >> overwrite;
		while (std::cin.fail() || overwrite > 1) {
			std::cout << "Invalid input...\n";
			std::cin.clear();
			std::cin.ignore(100, '\n');

			std::cout << "File already exists. Overwrite? (1)YES / (0)NO:>";
			std::cin >> overwrite;
		}
		std::cin.ignore(100, '\n');
		std::cout << "\n";

		if (overwrite) {
			this->stage->SaveStage(stagePath);
			std::cout << "Stage " << this->stageName << " overwritten and saved successfully." << std::endl;
		}
		else {
			std::cout << "Do not overwrite!" << std::endl;
		}
	}else {
		this->stage->SaveStage(stagePath);
		std::cout << "Stage " << this->stageName << " saved successfully." << std::endl;
	}

	fin.close();
}

void GameMapEditor::LoadStage() {

	std::string loadFilename = "";
	std::cout << "File name (without extention) :>";
	std::getline(std::cin, loadFilename);
	loadFilename.append(".smap");

	if (this->stage->LoadStage(loadFilename, this->mainView)) {
		this->stageName = loadFilename;
		std::cout << "Stage: " << this->stageName << " successfully loaded" << std::endl;
	}
	else {
		std::cout << "Failed to load stage " << loadFilename << std::endl;
	}
}

void GameMapEditor::SetBackground() {
	std::cout << "* - Background Select - *" << std::endl;
	std::cout << " Background index [0," << (Stage::numOfBackgrounds - 1) << "] (-1 to exit) :>";
	int choice;
	std::cin >> choice;
	while (std::cin.fail() || choice >= Stage::numOfBackgrounds || (choice < 0 && choice != -1)) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << " Background index [0," << (Stage::numOfBackgrounds - 1) << "] (-1 to exit) :>";
		std::cin >> choice;
	}

	if (choice > 0) {
		this->backgroundIndex = choice;
	}

	std::cout << "* - Background Size - *" << std::endl;
	std::cout << " Background size X :>";
	std::cin >> choice;
	while (std::cin.fail() || choice < Gauss::BACKGROUND_SIZE) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << " Background size X :>";
		std::cin >> choice;
	}
	this->backgroundWidth = choice;

	std::cout << " Background size Y :>";
	std::cin >> choice;
	while (std::cin.fail() || choice < Gauss::BACKGROUND_SIZE) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << " Background size Y :>";
		std::cin >> choice;
	}
	this->backgroundHeight = choice;

	this->stage->SetBackground(this->backgroundIndex, this->backgroundWidth, this->backgroundHeight);

	std::cin.ignore(100, '\n');
	std::cout << "\n";
}

void GameMapEditor::SetEnemySpawner() {
	std::cout << "SetEnemySpawner " << "\n\n";
	std::cout << "Random Pos? (0/1) :>";
	int randSpawn = 0;
	std::cin >> randSpawn;
	while (std::cin.fail() || randSpawn < 0 || randSpawn > 1) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Random Pos? (0/1) :>";
		std::cin >> randSpawn;
	}
	this->enemySpRandPos = randSpawn;

	std::cout << "Type (-1 for random) :>";
	std::cin >> this->enemySpType;
	while (std::cin.fail() || this->enemySpType > EnemyLifeform::nrOfEnemyTypes-1) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Type (-1 for random) :>";
		std::cin >> this->enemySpType;
	}

	std::cout << "Velocity (-1 for random) :>";
	std::cin >> this->enemySpVelocity;
	while (std::cin.fail() || this->enemySpVelocity < -1) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Velocity (-1 for random):>";
		std::cin >> this->enemySpVelocity;
	}

	std::cout << "Level Interval :>";
	std::cin >> this->enemySpInterval;
	while (std::cin.fail()) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Level Interval :>";
		std::cin >> this->enemySpInterval;
	}

	std::cout << "Number of enemies :>";
	std::cin >> this->numOfEnemies;
	while (std::cin.fail()) {
		std::cout << "Invalid input...\n";
		std::cin.clear();
		std::cin.ignore(100, '\n');

		std::cout << "Number of enemies :>";
		std::cin >> this->numOfEnemies;
	}

	std::cin.clear();
	std::cin.ignore(100, '\n');
}

