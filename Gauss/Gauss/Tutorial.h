#pragma once
#include "GameObject.h"
#include "Dialog.h"

class Tutorial
{
public:
	enum TutorialStage {
		INTRO = 0,
		HORIZONTAL,
		VETICAL,
		LASER,
		GAUSS_CANNON,
		SHIELD
	};

private:
	RenderWindow *window;
	TutorialStage currentStage;
	Dialog* dialog;

	void updateIntro();
	void updateHorizontal();
	void updateVertical();
	void updateLaser();
	void updateGaussCannon();
	void updateShield();


public:
	Tutorial(Font &font, RenderWindow *window);
	virtual ~Tutorial();

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
};

