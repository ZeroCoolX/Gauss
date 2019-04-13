#pragma once

#include "GameObject.h"

class MainMenu
{
public:
	MainMenu();
	virtual ~MainMenu();

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

