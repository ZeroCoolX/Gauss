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
		SHIELD,
		SHIELD_HOLD,
		HEALTH_HUD,
		SHIELD_HUD,
		GAUSS_CHARGE_HUD,
		XP_HUD,
		TUTORIAL_OVER
	};

private:
	dArr<dArr<std::string>> dialogTexts;

	bool active;
	bool tutorialComplete;

	RenderWindow *window;
	TutorialStage currentStage;
	Dialog* dialog;

	// Pointer to the player controls
	dArr<Player> *players;

	// Indicates ready to move to next stage
	bool moveToNextStage;
	bool dialogueFinished;
	bool collectInput;

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
	// time in between each flash indicating a part of the hud
	float hudFlashIndicatorTimer;
	float hudFlashIndicatorTimerMax;
	// timer for forced xp gain
	float xpGainTimer;
	float xpGainTimerMax;
	// time until we go back to the menu on complete
	float tutorialCompleteTimerMax;;
	float tutorialCompleteTimer;

	void displayText(const float &dt);
	void updateIntro(const float &dt);
	void updateHorizontal(const float &dt);
	void updateVertical(const float &dt);
	void updateLaser(const float &dt);
	void updateGaussCannon(const float &dt);
	void updateShield(const float &dt);
	void updateShieldHold(const float &dt);
	void updateHealthHud(const float &dt);
	void updateShieldHud(const float &dt);
	void updateGaussChargeHud(const float &dt);
	void updateXpChargeHud(const float &dt);
	void updateTutorialOver(const float &dt);
	bool isLastSentence();

public:
	Tutorial(Font &font, RenderWindow *window, dArr<Player> *players);
	virtual ~Tutorial();

	void InitTexts();

	void Update(const float &dt);
	void Draw(RenderTarget &renderTarget);
	void Reset();
	inline void Activate() {
		this->active = true;
		this->tutorialComplete = false;
		(*this->players)[0].disableAllControls();
		(*this->players)[0].activateColorFlashTutorialOverride();
		this->InitTexts();
	}
	inline const bool IsTutorialComplete() const { return this->active && this->tutorialComplete; }
};

