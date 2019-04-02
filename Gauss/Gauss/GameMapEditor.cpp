#include "GameMapEditor.h"

GameMapEditor::GameMapEditor(RenderWindow *window)
{
	this->window = window;

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	this->keyTimeMax = 5.f;
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
	this->selector.setSize(Vector2f(static_cast<float>(Gauss::GRID_SIZE), static_cast<float>(Gauss::GRID_SIZE)));
	this->selector.setFillColor(Color::Transparent);
	this->selector.setOutlineColor(Color::Red);
	this->selector.setOutlineThickness(2.f);
}

void GameMapEditor::InitMap() {
	this->stage = new Stage(10, 10);
}

void GameMapEditor::UpdateMousePosition() {
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosWorld = this->window->mapPixelToCoords(this->mousePosWindow);
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

void GameMapEditor::UpdateView(const float &dt) {
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		this->mainView.move(0.f, -10.f * dt * DeltaTime::dtMultiplier);
	}else if (Keyboard::isKeyPressed(Keyboard::S)) {
		this->mainView.move(0.f, 10.f * dt * DeltaTime::dtMultiplier);
	}

	if (Keyboard::isKeyPressed(Keyboard::A)) {
		this->mainView.move(-10.f * dt * DeltaTime::dtMultiplier, 0.f);
	}else if (Keyboard::isKeyPressed(Keyboard::D)) {
		this->mainView.move(10.f * dt * DeltaTime::dtMultiplier, 0.f);
	}
}

void GameMapEditor::UpdateUI() {
	this->selector.setPosition(this->mousePosGrid.x * static_cast<float>(Gauss::GRID_SIZE), this->mousePosGrid.y * static_cast<float>(Gauss::GRID_SIZE));
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

	// Update Selector
	this->UpdateSelector();

	// Update UI
	this->UpdateUI();

	// view update
	this->UpdateView(dt);
}

void GameMapEditor::UpdateTimers(const float &dt) {
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void GameMapEditor::UpdateMap() {

}

void GameMapEditor::UpdateSelector() {
	if (Mouse::isButtonPressed(Mouse::Left) && this->keyTime >= this->keyTimeMax) {
		this->keyTime = 0.f;

		this->stage->AddTile(
			Tile(IntRect(0, 0, Gauss::GRID_SIZE, Gauss::GRID_SIZE), this->selector.getPosition(), false, false), 
			this->mousePosGrid.x, 
			this->mousePosGrid.y);
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

void GameMapEditor::DrawUIWindow() {

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

	// Draw UI - update view
	this->window->setView(this->window->getDefaultView());

	// Draw the window
	this->DrawUIWindow();

	// Set View
	this->window->setView(this->mainView);

	// Draw the view
	this->DrawUIView();

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

