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

	std::string currentText;
	int currentTextIndex;

	dArr<char> textCharacters;
	int currentTextCharIndex;

	// time in between each character appearing on screen
	float textTimer;
	float textTimerMax;
	// time in between each sentence being written out
	float sentenceDelayTimer;
	float sentenceDelayTimerMax;

	void updateIntro(const float &dt);
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

	static dArr<dArr<std::string>> dialogTexts;
	static void InitTexts();
};

