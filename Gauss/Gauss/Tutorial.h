#pragma once
#include "GameObject.h"
#include "Dialog.h"
#include "Player.h"

class Tutorial
{
public:
	enum TutorialStage {
		INTRO = 0,
		HORIZONTAL,
		VERTICAL,
		LASER,
		GAUSS_CANNON,
		SHIELD
	};

private:
	RenderWindow *window;
	TutorialStage currentStage;
	Dialog* dialog;

	// Pointer to the player controls
	dArr<Player> *players;

	// Indicates ready to move to next stage
	bool moveToNextStage;
	bool dialogueFinished;

	// Tutorial specific keypress triggers
	bool leftPressed;
	bool rightPressed;
	bool upPressed;
	bool downPressed;
	bool firePressed;
	bool gaussCannonPressed;
	bool shieldPressed;

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

	void displayText(const float &dt);
	void updateIntro(const float &dt);
	void updateHorizontal(const float &dt);
	void updateVertical(const float &dt);
	void updateLaser(const float &dt);
	void updateGaussCannon(const float &dt);
	void updateShield(const float &dt);


public:
	Tutorial(Font &font, RenderWindow *window, dArr<Player> *players);
	virtual ~Tutorial();

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);

	static dArr<dArr<std::string>> dialogTexts;
	static void InitTexts();
};

