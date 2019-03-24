#include "Boss.h"


Boss::~Boss()
{
}

void Boss::Movement() {

}

void Boss::Update(const float &dt, Vector2f target) {

	// UPDATE GUNS
	for (size_t i = 0; i < this->guns.Size(); i++)
	{
		this->guns[i].Update(dt);
	}

	// UPDATE BULLETS
	for (size_t i = 0; i < this->bullets.Size(); i++)
	{
		this->bullets[i].Update(dt, target);
	}
}

void Boss::Draw(RenderTarget &renderTarget) {
	renderTarget.draw(this->sprite);

	// DRAW GUNS
	for (size_t i = 0; i < this->guns.Size(); i++)
	{
		this->guns[i].Draw(renderTarget);
	}

	// DRAW BULLETS
	for (size_t i = 0; i < this->bullets.Size(); i++)
	{
		this->bullets[i].Draw(renderTarget);
	}
}