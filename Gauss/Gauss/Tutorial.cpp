#include "Tutorial.h"
#include <string>
#include <cstring>

// statics
dArr<dArr<std::string>> Tutorial::dialogTexts;

void Tutorial::InitTexts() {
	dArr<std::string> tempText(5);
	// Intro
	tempText.Add("Welcome to the Gaussian Fleet comrade.");
	tempText.Add("Before we send you off to the front lines we need to make sure your ship is operating at max capacity.");
	tempText.Add("Don't want that bad boy malfunctioning during thhgf ghhh gjj h hjh jhhjhjh hh jh hj hhe middle of a firefight.");
	tempText.Add("Follow my orders and you'll be on your way to protecting humanity in no time Follow my orders and you'll be on your way to protecting humanity in no time Follow my orders and you'll be on your way to protecting humanity in no time");
	Tutorial::dialogTexts.Add(tempText);
	// Horizontal instructions
	tempText.Add("HORIZONTAL asdfsgdsfjhjg");
	tempText.Add("Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Vertical instructions
	tempText.Add("VERTICAL asdfsgdsfjhjg");
	tempText.Add("Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
	// Laser instructions
	tempText.Add("FIRE  to the Gaussian Fleet comrade.");
	tempText.Add("Before dfhgghdfghdgherating at max capacity.");
	tempText.Add("Don't wantdfhfghfghring the middle of a firefight.");
	Tutorial::dialogTexts.Add(tempText);
}

Tutorial::Tutorial(Font &font, RenderWindow *window)
{
	this->currentStage = Tutorial::TutorialStage::INTRO;
	this->dialog = new Dialog(font, window);

	this->textTimerMax = 1.f;
	this->textTimer = this->textTimerMax;

	this->sentenceDelayTimerMax = 5.f;
	this->sentenceDelayTimer = this->sentenceDelayTimerMax;

	this->currentText = "";
	this->currentTextIndex = 0;
}


Tutorial::~Tutorial()
{
	delete this->dialog;
}

void Tutorial::Update(const float &dt) {
	switch (currentStage) {
		case INTRO:
			this->updateIntro(dt);
			break;
		case HORIZONTAL:
			break;
		case VETICAL:
			break;
		case LASER:
			break;
		case GAUSS_CANNON:
			break;
		case SHIELD:
			break;
	}
}

void Tutorial::Draw(RenderTarget &renderTarget) {
	if (this->dialog->isActive()) {
		this->dialog->Draw(renderTarget);
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
		if (this->textTimer > 0.f) {
			this->textTimer -= 1.f * dt * DeltaTime::dtMultiplier;
		}
		else {
			this->textTimer = this->textTimerMax;
			if (this->currentTextIndex < Tutorial::dialogTexts[TutorialStage::INTRO].Size()) {
				// Populate the current text characters into the array
				if (textCharacters.Size() == 0) {
					std::string str = Tutorial::dialogTexts[TutorialStage::INTRO][this->currentTextIndex];

					// declaring character array 
					char *cstr = new char[str.size() + 1];
					str.copy(cstr, str.size() + 1);
					cstr[str.size()] = '\0';

					for (int i = 0; i < str.size(); i++) {
						textCharacters.Add(cstr[i]);
					}

					this->currentTextCharIndex = 0;

					// Release memory
					delete [] cstr;
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
		}
	}
}
void Tutorial::updateHorizontal() {

}
void Tutorial::updateVertical() {

}
void Tutorial::updateLaser() {

}
void Tutorial::updateGaussCannon() {

}
void Tutorial::updateShield() {

}
