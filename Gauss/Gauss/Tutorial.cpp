#include "Tutorial.h"
#include <string>
#include <cstring>

// statics
dArr<dArr<std::string>> Tutorial::dialogTexts;

void Tutorial::InitTexts() {
	dArr<std::string> tempText(4);
	// Intro
	tempText.Add("Welcome to the Gaussian Fleet comrade.");
	tempText.Add("Before we send you off to the front lines we need to make sure your ship is operating at max capacity.");
	tempText.Add("Don't want that bad boy malfunctioning during thhgf ghhh gjj h hjh jhhjhjh hh jh hj hhe middle of a firefight.");
	tempText.Add("Follow my orders and you'll be on your way to protecting humanity in no time Follow my orders and you'll be on your way to protecting humanity in no time Follow my orders and you'll be on your way to protecting humanity in no time");
	Tutorial::dialogTexts.Add(tempText);
	// Horizontal instructions
	tempText.Clear();
	tempText.Add("HORIZONTAL asdfsgdsfjhjg");
	tempText.Add("HORIZONTAL Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("HORIZONTAL Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Vertical instructions
	tempText.Clear();
	tempText.Add("VERTICAL asdfsgdsfjhjg");
	tempText.Add("VERTICAL Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("VERTICAL Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Laser instructions
	tempText.Clear();
	tempText.Add("LASER asdfsgdsfjhjg");
	tempText.Add("LASER Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("LASER Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Gauss Cannon instructions
	tempText.Clear();
	tempText.Add("GAUSS_CANNON asdfsgdsfjhjg");
	tempText.Add("GAUSS_CANNON Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("GAUSS_CANNON Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Shield instructions
	tempText.Clear();
	tempText.Add("SHIELD asdfsgdsfjhjg");
	tempText.Add("SHIELD Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("SHIELD Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
}

Tutorial::Tutorial(Font &font, RenderWindow *window, dArr<Player> *players)
{
	this->players = players;
	(*this->players)[0].disableAllControls();

	this->currentStage = Tutorial::TutorialStage::INTRO;
	this->dialog = new Dialog(font, window);

	this->textTimerMax = 1.f;
	this->textTimer = this->textTimerMax;

	this->sentenceDelayTimerMax = 5.f;
	this->sentenceDelayTimer = this->sentenceDelayTimerMax;

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
}


Tutorial::~Tutorial()
{
	delete this->dialog;
}

void Tutorial::Update(const float &dt) {
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
	}
	if (this->moveToNextStage) {
		this->dialogueFinished = false;
		this->moveToNextStage = false;
	}
}

void Tutorial::Draw(RenderTarget &renderTarget) {
	if (this->dialog->isActive()) {
		this->dialog->Draw(renderTarget);
	}
}

void Tutorial::displayText(const float &dt) {
	if (this->textTimer > 0.f) {
		this->textTimer -= 1.f * dt * DeltaTime::dtMultiplier;
	}
	else {
		this->textTimer = this->textTimerMax;
		if (this->currentTextIndex < Tutorial::dialogTexts[this->currentStage].Size()) {
			// Populate the current text characters into the array
			if (textCharacters.Size() == 0) {
				std::string str = Tutorial::dialogTexts[this->currentStage][this->currentTextIndex];

				// declaring character array 
				char *cstr = new char[str.size() + 1];
				str.copy(cstr, str.size() + 1);
				cstr[str.size()] = '\0';

				for (int i = 0; i < str.size(); i++) {
					textCharacters.Add(cstr[i]);
				}

				this->currentTextCharIndex = 0;

				// Release memory
				delete[] cstr;
			}
			else {
				// Loop through all the characters if we still have more to show
				if (this->currentTextCharIndex < this->textCharacters.Size()) {
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
				}
				// End of the text line, so move to the next one
				else {
					if (this->sentenceDelayTimer > 0.f) {
						this->sentenceDelayTimer -= 1.f * dt * DeltaTime::dtMultiplier;
					}
					else {
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
		}
	}
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
		}
	}
}

void Tutorial::updateHorizontal(const float &dt) {
	if (this->dialogueFinished) {
		// start looking for keypresses and hide the dialogue
		if (this->dialog->isActive()) {
			this->dialog->disable();
			(*this->players)[0].setDisableHorizontal(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::CONTROL_LEFT]
		))) {
			this->leftPressed = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::CONTROL_RIGHT]
		))) {
			this->rightPressed = true;
		}
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
	if (this->dialogueFinished) {
		// start looking for keypresses and hide the dialogue
		if (this->dialog->isActive()) {
			this->dialog->disable();
			(*this->players)[0].setDisableVertical(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::CONTROL_UP]
		))) {
			this->upPressed = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::CONTROL_DOWN]
		))) {
			this->downPressed = true;
		}
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
	if (this->dialogueFinished) {
		// start looking for keypresses and hide the dialogue
		if (this->dialog->isActive()) {
			this->dialog->disable();
			(*this->players)[0].setDisableLaser(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::Controls::CONTROL_FIRE]
		))) {
			this->firePressed = true;
		}
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
	if (this->dialogueFinished) {
		// start looking for keypresses and hide the dialogue
		if (this->dialog->isActive()) {
			this->dialog->disable();
			(*this->players)[0].setDisableGaussCannon(false);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key(
			(*((*this->players)[0].getControls()))[Player::Controls::CONTROL_GAUSSCANNON]
		))) {
			this->gaussCannonPressed = true;
		}
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
	if (!this->dialog->isActive()) {
		this->dialog->activate();
		(*this->players)[0].setDisableShield(false);
		// Clear any previous text
		this->currentText = "";
		this->currentTextIndex = 0;
	}
	else {
		this->displayText(dt);
		if (this->dialogueFinished) {
			//this->currentStage = TutorialStage::HORIZONTAL;
			//this->moveToNextStage = true;
		}
	}
}
