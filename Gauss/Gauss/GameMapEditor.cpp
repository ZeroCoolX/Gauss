#include "GameMapEditor.h"

GameMapEditor::GameMapEditor(RenderWindow *window)
{
	this->window = window;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;
	this->backgroundTile = false;
	this->stage = nullptr;
	this->backgroundIndex = 0;
	this->backgroundWidth = Gauss::BACKGROUND_SIZE;
	this->backgroundHeight = Gauss::BACKGROUND_SIZE;

	this->InitView();

	this->InitTextures();

	this->InitUI();
	this->InitMap();
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


void GameMapEditor::InitTextures() {
	Stage::InitTextures();
	Tile::InitTextures();
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

	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::N) && this->keyTime >= this->keyTimeMax) {
		this->NewStage();
		this->keyTime = 0.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::B) && this->keyTime >= this->keyTimeMax) {
		this->backgroundTile = !this->backgroundTile;
		this->keyTime = 0.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::S) && this->keyTime >= this->keyTimeMax) {
		this->SaveStage();
		this->keyTime = 0.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::L) && this->keyTime >= this->keyTimeMax) {
		this->LoadStage();
		this->keyTime = 0.f;
	}
}

void GameMapEditor::UpdateAddRemoveTiles() {
	if (Mouse::isButtonPressed(Mouse::Left)) {
		this->keyTime = 0.f;

		this->stage->AddTile(
			Tile(
				IntRect(this->textureSelectedX, this->textureSelectedY, Gauss::GRID_SIZE, Gauss::GRID_SIZE), 
				Vector2f(static_cast<float>(this->mousePosGrid.x * Gauss::GRID_SIZE), static_cast<float>(this->mousePosGrid.y* Gauss::GRID_SIZE)), 
				false, 
				false
			),
			this->mousePosGrid.x, 
			this->mousePosGrid.y, this->backgroundTile);
	}
	else if (Mouse::isButtonPressed(Mouse::Right)) {
		this->stage->RemoveTile(this->mousePosGrid.x, this->mousePosGrid.y, this->backgroundTile);
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

	// Update UI
	this->UpdateUI();

	// view update
	this->UpdateView(dt);
}

void GameMapEditor::DrawUIWindow() {
	this->window->draw(this->textureSelector);
	this->window->draw(this->selector);
}

void GameMapEditor::DrawUIView() {
	this->window->draw(this->selector);
}

void GameMapEditor::DrawMap() {
	this->stage->Draw(*this->window, this->mainView, true);
}

void GameMapEditor::Draw() {
	this->window->clear();

	// Set View
	this->window->setView(this->mainView);

	// Draw Map
	this->DrawMap();

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

