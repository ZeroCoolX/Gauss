#include "Tutorial.h"
#include <string>
#include <cstring>


Tutorial::Tutorial(Font &font, RenderWindow *window, dArr<Player> *players)
{
	this->active = false;
	this->tutorialComplete = false;

	this->players = players;

	this->currentStage = Tutorial::TutorialStage::INTRO;
	this->dialog = new Dialog(font, window);

	this->textTimerMax = 1.f;
	this->textTimer = this->textTimerMax;

	this->sentenceDelayTimerMax = 200.f;
	this->sentenceDelayTimer = this->sentenceDelayTimerMax;

	this->hudFlashIndicatorTimerMax = 100.f;
	this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;

	this->xpGainTimerMax = 25.f;
	this->xpGainTimer = this->xpGainTimerMax;

	this->tutorialCompleteTimerMax = 100.f;
	this->tutorialCompleteTimer = this->tutorialCompleteTimerMax;

	this->currentText = "";
	this->currentTextIndex = 0;

	this->moveToNextStage = false;
	this->dialogueFinished = false;
	this->leftPressed = false;
	this->rightPressed = false;
	this->upPressed = false;
	this->downPressed = false;
	this->firePressed = false;
	this->gaussCannonPressed = false;
	this->shieldPressed = false;
	this->collectInput = false;
}


Tutorial::~Tutorial()
{
	delete this->dialog;
}

void Tutorial::InitTexts() {
	dArr<std::string> tempText(4);

	// TutorialStage::INTRO
	tempText.Add("Welcome to the Gaussian Fleet comrade - Hands off those controls!");
	tempText.Add("Before we send you off to the front lines we need to make sure your Gauss is operating at max capacity.");
	tempText.Add("Don't want that bad boy malfunctioning during the middle of a firefight.");
	tempText.Add("Follow my orders and you'll be on your way to protecting humanity in no time soldier!");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::HORIZONTAL
	tempText.Add("First we need to ensure the horizontal acceleration thrusters are working properly.");
	tempText.Add("Try moving your Gauss forward (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_RIGHT]) + ") and backwards (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_LEFT]) + ") by activating each thruster. \n Your Gauss will accelerate at different velocities if you TAP vs HOLD the direction");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::VERTICAL
	tempText.Add("Excellent, green across the board on this side.");
	tempText.Add("Now let's check the vertical acceleration thrusters are primed.");
	tempText.Add("Try moving your Gauss up (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_UP]) + ") and down (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_DOWN]) + ") by activating each thruster. \n\n Your Gauss will accelerate at different velocities if you TAP vs HOLD the direction");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::LASER
	tempText.Add("Well done comrad. Mastering the movement controls of your Gauss will give you that extra edge in battle.");
	tempText.Add("Remember that TAPPING (moement) vs HOLDING (movement) will yield difference accelerations.");
	tempText.Add("TAP the direction to move a very small amount and HOLD the direction to accelerate very quickly.");
	tempText.Add("Your Gauss is equipped with an electro magnetic laser as its main fire mode");
	tempText.Add("This laser is your most reliable way to cleanup the universe of alien scum.");
	tempText.Add("Fire your laser (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_FIRE]) + ") to make sure the engineers planet side didn't mess anything up.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::GAUSS_CANNON
	tempText.Add("Good...At least they did something right.");
	tempText.Add("The laser will never run out of ammo, and will never overheat - so fire at will.");
	tempText.Add("You can fire a single shot by TAPPING (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_FIRE]) + ") or achieve automatic fire by HOLDING (SPACE).");
	tempText.Add("Each Gauss is also equipped with an insancely powerful magnetic acceleration cannon - or MAC.");
	tempText.Add("The MAC needs to powerup and can only be shot once fully charged.");
	tempText.Add("Fire the MAC by pressing (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_GAUSSCANNON]) + ").");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::SHIELD
	tempText.Add("WOOOhhoo I could feel the power of that shot from all the way up here!");
	tempText.Add("The MAC will plow through multiple enemies so use it when you're in a tight spot or want to completely clear the area in front of you.");
	tempText.Add("The Gauss is also equipped with a deflector shield.");
	tempText.Add("This shield will REFLECT enemy projectiles and minimize the damage done to head on collisions.");
	tempText.Add("Using the deflector shield will drain its charge.");
	tempText.Add("If used for too long, it will burst and need time to charge back up.");
	tempText.Add("The shield will shrink over time to indicate when its losing too much charge.");
	tempText.Add("Reflecting enemy projectiles will not imapact the charge.");
	tempText.Add("But deflecting head on collisions will destroy 1/3 of the current charge.");
	tempText.Add("Activate your shield by pressing (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_SHIELD]) + ")");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::SHIELD_HOLD
	tempText.Add("When the shield is depleted you cannot protect yourself so watch the size!");
	tempText.Add("Now deplete the shield fully by holding (" + KeyManager::KeyName((*((*this->players)[0].getControls()))[Player::CONTROL_SHIELD]) + ") until to bursts.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::HEALTH_HUD
	tempText.Add("The Gauss has a Heads up Display which is projected directly on the ship.");
	tempText.Add("Its health is indicated by the red bar underneath it.");
	tempText.Add("If this bar hits 0 you turn into space dust - so watch it.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::SHIELD_HUD
	tempText.Add("The current shield charge is indicated by a pink circle near the top right of the Gauss.");
	tempText.Add("It will similarly grow and shrink to show the available charge.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::GAUSS_CHARGE_HUD
	tempText.Add("The MAC indicator is on the bottom left of the Gauss as indicated by a green bar.");
	tempText.Add("It shows you the time needed to reload between MAC shots.");
	tempText.Add("A sound will also emit upon full reload of the MAC to let you know when it can be fired again.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::XP_HUD
	tempText.Add("The XP indicator is the blue bar above the health bar.");
	tempText.Add("This bar will fill as you gain XP by killing aliens.");
	tempText.Add("When completely filled you will LEVEL UP which increases the stats about your Gauss as well as refils your health.");
	tempText.Add("These stats: Cooling, Power, Plating, and Maneuverability directly effect the damage, health, and XP gain of your Gauss.");
	this->dialogTexts.Add(tempText);
	tempText.Clear();

	// TutorialStage::TUTORIAL_OVER
	tempText.Add("Just remember these few key battle tips...");
	tempText.Add("TAP the direction to move a very small amount and HOLD the direction to accelerate very quickly.");
	tempText.Add("Leveling up refills your Gauss's health so if you're going down on health, go up on killing!");
	tempText.Add("Lastly, don't forget to use the reflect shield to reflect enemy projectiles, and to give you some extra buffer if enemy collision is inevitable.");
	tempText.Add("Alright soldier, time to cut the chit-chat and get out there!");
	this->dialogTexts.Add(tempText);
}

void Tutorial::Update(const float &dt) {
	if (!this->active) {
		return;
	}

	switch (this->currentStage) {
		case INTRO:
			this->updateIntro(dt);
			break;
		case HORIZONTAL:
			this->updateHorizontal(dt);
			break;
		case VERTICAL:
			this->updateVertical(dt);
			break;
		case LASER:
			this->updateLaser(dt);
			break;
		case GAUSS_CANNON:
			this->updateGaussCannon(dt);
			break;
		case SHIELD:
			this->updateShield(dt);
			break;
		case SHIELD_HOLD:
			this->updateShieldHold(dt);
			break;
		case HEALTH_HUD:
			this->updateHealthHud(dt);
			break;
		case SHIELD_HUD:
			this->updateShieldHud(dt);
			break;
		case GAUSS_CHARGE_HUD:
			this->updateGaussChargeHud(dt);
			break;
		case XP_HUD:
			this->updateXpChargeHud(dt);
			break;
		case TUTORIAL_OVER:
			(*this->players)[0].disableAllControls();
			this->updateTutorialOver(dt);
			break;
		default:
			(*this->players)[0].enableAllControls();
			(*this->players)[0].deactivateColorFlashTutorialOverride();
			break;
	}
	if (this->moveToNextStage) {
		this->dialogueFinished = false;
		this->moveToNextStage = false;
		this->collectInput = false;
	}
}

void Tutorial::Draw(RenderTarget &renderTarget) {
	if (!this->active) {
		return;
	}

	if (this->dialog->isActive()) {
		this->dialog->Draw(renderTarget);
	}
}

void Tutorial::displayText(const float &dt) {
	if (this->textTimer > 0.f) {
		this->textTimer -= 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		if (this->currentTextIndex < static_cast<int>(Tutorial::dialogTexts[this->currentStage].Size())) {
			// Populate the current text characters into the array
			if (textCharacters.Size() == 0) 
			{
				this->textTimer = this->textTimerMax;
				
				std::string str = Tutorial::dialogTexts[this->currentStage][this->currentTextIndex];

				// declaring character array 
				char *cstr = new char[str.size() + 1];
				str.copy(cstr, str.size() + 1);
				cstr[str.size()] = '\0';

				for (size_t i = 0; i < str.size(); i++) {
					textCharacters.Add(cstr[i]);
				}

				this->currentTextCharIndex = 0;

				// Release memory
				delete[] cstr;
			}
			else {
				// Loop through all the characters if we still have more to show
				if (this->currentTextCharIndex < static_cast<int>(this->textCharacters.Size())) {
					if (!this->dialog->isWithinBorderEdge() && textCharacters[this->currentTextCharIndex] == ' ') {
						this->currentText += "\n";
						this->dialog->resetCurrentTextWidth();
					}
					else {
						this->currentText += textCharacters[this->currentTextCharIndex];
						this->dialog->incrementCurrentTextWidth();
					}
					this->dialog->setText(currentText);
					++this->currentTextCharIndex;
					this->textTimer = this->textTimerMax;
				}
				// End of the text line, so move to the next one
				else {
					this->collectInput = this->isLastSentence();

					if (this->sentenceDelayTimer > 0.f) {
						this->sentenceDelayTimer -= 1.f * dt * DeltaTime::dtMultiplier;
					}
					else {
						this->textTimer = this->textTimerMax;
						this->dialog->resetCurrentTextWidth();
						this->textCharacters.Clear();
						this->currentText = "";
						++this->currentTextIndex;
						this->sentenceDelayTimer = this->sentenceDelayTimerMax;
					}
				}
			}
		}
		else {
			this->dialogueFinished = true;
			this->currentTextCharIndex = 0;
			this->currentTextIndex = 0;
			this->dialog->resetCurrentTextWidth();
			this->textCharacters.Clear();
			this->currentText = "";
			this->sentenceDelayTimer = this->sentenceDelayTimerMax;
		}
	}
}

bool Tutorial::isLastSentence() {
	return (this->currentTextIndex + 1) >= static_cast<int>(Tutorial::dialogTexts[this->currentStage].Size());
}

void Tutorial::updateIntro(const float &dt) {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);
		if (this->dialogueFinished) {
			this->currentStage = TutorialStage::HORIZONTAL;
			this->moveToNextStage = true;
			this->dialogueFinished = false;
		}
	}
}

void Tutorial::updateTutorialOver(const float &dt) {
	if (!this->dialog->isActive() && !this->dialogueFinished) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);
		if (this->dialogueFinished) {
			this->dialog->disable();
			(*this->players)[0].overrideControlsForTutorial();

			if (this->tutorialCompleteTimer > 0.f) {
				this->tutorialCompleteTimer -= 1.f * dt * DeltaTime::dtMultiplier;
			}
			else {
				this->tutorialComplete = true;
			}
		}
	}
}

void Tutorial::updateHorizontal(const float &dt) {
	if (this->collectInput) {
		// start looking for keypresses and hide the dialogue
		(*this->players)[0].setDisableHorizontal(false);

		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::CONTROL_LEFT]
		)) {
			this->leftPressed = true;
		}
		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::CONTROL_RIGHT]
		)) {
			this->rightPressed = true;
		}
	}

	if (this->dialogueFinished) {
		if (this->leftPressed && this->rightPressed) {
			this->currentStage = TutorialStage::VERTICAL;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateVertical(const float &dt) {
	if (this->collectInput) {
		// start looking for keypresses and hide the dialogue
		(*this->players)[0].setDisableVertical(false);

		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::CONTROL_UP]
		)) {
			this->upPressed = true;
		}
		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::CONTROL_DOWN]
		)) {
			this->downPressed = true;
		}
	}

	if (this->dialogueFinished) {
		if (this->upPressed && this->downPressed) {
			this->currentStage = TutorialStage::LASER;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateLaser(const float &dt) {
	if (this->collectInput) {
		// start looking for keypresses and hide the dialogue
		(*this->players)[0].setDisableLaser(false);


		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::Controls::CONTROL_FIRE]
		)) {
			this->firePressed = true;
		}
	}

	if (this->dialogueFinished) {
		if (this->firePressed) {
			this->currentStage = TutorialStage::GAUSS_CANNON;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateGaussCannon(const float &dt) {
	if (this->collectInput) {
		// start looking for keypresses and hide the dialogue
		(*this->players)[0].setDisableGaussCannon(false);

		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::Controls::CONTROL_GAUSSCANNON]
		)) {
			this->gaussCannonPressed = true;
		}
	}

	if (this->dialogueFinished) {
		if (this->gaussCannonPressed) {
			this->currentStage = TutorialStage::SHIELD;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateShield(const float &dt) {
	if (this->collectInput) {
		// start looking for keypresses and hide the dialogue
		(*this->players)[0].setDisableShield(false);

		if (KeyMouseBoard::isPressed(
			(*((*this->players)[0].getControls()))[Player::Controls::CONTROL_SHIELD]
		)) {
			this->shieldPressed = true;
		}
	}

	if (this->dialogueFinished) {
		if (this->shieldPressed) {
			this->currentStage = TutorialStage::SHIELD_HOLD;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateShieldHold(const float &dt) {
	if (this->dialogueFinished) {
		// start looking for keypresses and hide the dialogue
		if (this->dialog->isActive()) {
			this->dialog->disable();
		}

		if ((*this->players)[0].getShieldChargeTimer() <= 0.f) {
			this->currentStage = TutorialStage::HEALTH_HUD;
			this->moveToNextStage = true;
		}
	}
	else {
		if (!this->dialog->isActive()) {
			this->dialog->activate();
			// Clear any previous text
			this->currentText = "";
			this->currentTextIndex = 0;
		}
		else {
			this->displayText(dt);
		}
	}
}

void Tutorial::updateHealthHud(const float &dt) {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);

		if (this->dialogueFinished) {
			(*this->players)[0].resetPlayerHealthBarColor();
			this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;
			this->currentStage = TutorialStage::SHIELD_HUD;
			this->moveToNextStage = true;
			this->dialogueFinished = false;
		}
		else {
			if (this->hudFlashIndicatorTimer > 0.f) {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
			}
			else {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
				(*this->players)[0].activateColorFlashTutorialOverride();
				(*this->players)[0].flashPlayerHealthBar(static_cast<int>(this->hudFlashIndicatorTimer*-1));
			}
		}
	}
}

void Tutorial::updateShieldHud(const float &dt) {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);

		if (this->dialogueFinished) {
			(*this->players)[0].resetPlayerShieldColor();
			this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;
			this->currentStage = TutorialStage::GAUSS_CHARGE_HUD;
			this->moveToNextStage = true;
			this->dialogueFinished = false;
		}
		else {
			if (this->hudFlashIndicatorTimer > 0.f) {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
			}
			else {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
				(*this->players)[0].activateColorFlashTutorialOverride();
				(*this->players)[0].flashPlayerShield(static_cast<int>(this->hudFlashIndicatorTimer*-1));
			}
		}
	}
}

void Tutorial::updateGaussChargeHud(const float &dt) {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);

		if (this->dialogueFinished) {
			(*this->players)[0].resetPlayerGaussCannonColor();
			this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;
			this->currentStage = TutorialStage::XP_HUD;
			this->moveToNextStage = true;
			this->dialogueFinished = false;
		}
		else {
			if (this->hudFlashIndicatorTimer > 0.f) {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
			}
			else {
				this->hudFlashIndicatorTimer -= 0.5f * dt * DeltaTime::dtMultiplier;
				(*this->players)[0].activateColorFlashTutorialOverride();
				(*this->players)[0].flashPlayerGaussCannon(static_cast<int>(this->hudFlashIndicatorTimer));
			}
		}
	}
}

void Tutorial::updateXpChargeHud(const float &dt) {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);

		if (this->dialogueFinished) {
			this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;
			this->currentStage = TutorialStage::TUTORIAL_OVER;
			this->moveToNextStage = true;
			this->dialogueFinished = false;
		}
		else {
			if (this->xpGainTimer > 0.f) {
				this->xpGainTimer -= 1.f * dt * DeltaTime::dtMultiplier;
			}
			else {
				this->xpGainTimer = this->xpGainTimerMax;
				if ((*this->players)[0].getLevel() == 1) {
					(*this->players)[0].gainExp(1);
				}
				(*this->players)[0].UpdateStatsUI();
			}
		}
	}
}

void Tutorial::Reset() {
	this->active = false;
	this->tutorialComplete = false;

	(*this->players)[0].enableAllControls();
	(*this->players)[0].deactivateColorFlashTutorialOverride();
	
	this->textTimerMax = 1.f;
	this->textTimer = this->textTimerMax;

	this->sentenceDelayTimerMax = 200.f;
	this->sentenceDelayTimer = this->sentenceDelayTimerMax;

	this->hudFlashIndicatorTimerMax = 100.f;
	this->hudFlashIndicatorTimer = this->hudFlashIndicatorTimerMax;

	this->xpGainTimerMax = 25.f;
	this->xpGainTimer = this->xpGainTimerMax;

	float tutorialCompleteTimerMax = 100.f;
	float tutorialCompleteTimer = this->tutorialCompleteTimerMax;

	this->currentText = "";
	this->currentTextIndex = 0;

	this->moveToNextStage = false;
	this->dialogueFinished = false;
	this->leftPressed = false;
	this->rightPressed = false;
	this->upPressed = false;
	this->downPressed = false;
	this->firePressed = false;
	this->gaussCannonPressed = false;
	this->shieldPressed = false;
	this->collectInput = false;
}
