#include "Tutorial.h"

// statics
dArr<dArr<std::string>> Tutorial::dialogTexts;

void Tutorial::InitTexts() {
	dArr<std::string> tempText(5);
	// Intro
	tempText.Add("Welcome to the Gaussian Fleet comrade.");
	tempText.Add("Before we send you off to the front lines we need to make sure your ship is operating at max capacity.");
	tempText.Add("Don't want that bad boy malfunctioning during the middle of a firefight.");
	tempText.Add("Follow my orders and you'll be on your way to protecting humanity in no time");
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

	this->textTimerMax = 100.f;
	this->textTimer = this->textTimerMax;

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
				this->currentText = Tutorial::dialogTexts[TutorialStage::INTRO][this->currentTextIndex];
				this->dialog->setText(currentText);
				++this->currentTextIndex;
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
