#include "MainMenu.h"



MainMenu::MainMenu(Font &font)
{
	this->font = font;
	this->Init();
}


MainMenu::~MainMenu()
{
}

void MainMenu::Init() {
	this->InitButtons();
}

void MainMenu::InitButtons() {
	//this->buttons.Add(MenuButton(this->font, 
	//	ButtonTypes::BTN_SURVIVAL,
	//	"Survival",
	//	12, 
	//	Vector2f(0.f, 0.f)))
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
