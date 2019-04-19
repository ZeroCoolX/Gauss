#include "MainMenu.h"



MainMenu::MainMenu()
{
	this->Init();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init() {
	this->InitButtons();
}

void MainMenu::InitButtons() {
	// heh
}

void MainMenu::Update(Vector2f &mousePos, const float &dt) {
	this->UpdateButtons(mousePos, dt);
}

void MainMenu::UpdateButtons(Vector2f &mousePos, const float &dt) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i].Update(mousePos);
	}
}

void MainMenu::Draw(RenderTarget &renderTarget) {
	this->DrawButtons(renderTarget);
}

void MainMenu::DrawButtons(RenderTarget &renderTarget) {
	for (size_t i = 0; i < this->buttons.Size(); i++)
	{
		this->buttons[i].Draw(renderTarget);
	}
}
