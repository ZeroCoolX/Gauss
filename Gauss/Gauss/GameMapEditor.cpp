#include "GameMapEditor.h"

GameMapEditor::GameMapEditor(RenderWindow *window)
{
	this->window = window;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	this->stage = nullptr;

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

void GameMapEditor::InitMapTextures() {
	Tile::tileTextures.loadFromFile("Textures/Map/textureSheet.png");
}

void GameMapEditor::InitTextures() {
	this->InitMapTextures();
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

void GameMapEditor::UpdateMap() {

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

	if (Keyboard::isKeyPressed(Keyboard::N) && this->keyTime >= this->keyTimeMax) {
		this->NewMap();
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
			this->mousePosGrid.y);
	}
	else if (Mouse::isButtonPressed(Mouse::Right)) {
		this->stage->RemoveTile(this->mousePosGrid.x, this->mousePosGrid.y);
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
	this->UpdateMap();

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
	this->stage->Draw(*this->window, this->mainView);
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

void GameMapEditor::NewMap() {
	unsigned mapSizeX = 0;
	unsigned mapSizeY = 0;
	std::cout << "New Map\n\n" << std::endl;

	std::cout << "Map Name :>";
	std::getline(std::cin, this->stageName);

	std::cout << "Map size X :>";
	std::cin >> mapSizeX;

	std::cout << "Map size Y :>";
	std::cin >> mapSizeY;

	delete this->stage;
	this->stage = new Stage(mapSizeX, mapSizeY);

	std::cin.ignore(100, '\n');
}

