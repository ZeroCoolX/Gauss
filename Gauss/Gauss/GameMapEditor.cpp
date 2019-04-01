#include "GameMapEditor.h"

GameMapEditor::GameMapEditor(RenderWindow *window)
{
	this->window = window;

	this->InitView();

	this->InitTextures();

	// Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");


	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	this->InitUI();
	this->InitMap();
}

GameMapEditor::~GameMapEditor()
{
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
	
}

void GameMapEditor::InitMap() {

}

void GameMapEditor::UpdateView() {
	//this->mainView.setCenter(this->players[0].getPosition());
}

void GameMapEditor::Update(const float &dt) {

	// Keytime update
	this->UpdateTimers(dt);

	// Fullscreen check
	this->ToggleFullscreen();

	// Update Map
	this->UpdateMap();

	// view update
	this->UpdateView();
}

void GameMapEditor::UpdateTimers(const float &dt) {
	if (this->keyTime < this->keyTimeMax) {
		this->keyTime += 1.f * dt * DeltaTime::dtMultiplier;
	}
}

void GameMapEditor::UpdateMap() {

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

void GameMapEditor::DrawMap() {
	this->stage.Draw(*this->window, this->mainView);
}

void GameMapEditor::Draw() {
	this->window->clear();

	// Set View
	this->window->setView(this->mainView);

	// Draw Map
	this->DrawMap();

	// Draw UI - update view
	this->window->setView(this->window->getDefaultView());

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

