#include "Tutorial.h"

Tutorial::Tutorial(Font &font, RenderWindow *window)
{
	this->currentStage = Tutorial::TutorialStage::INTRO;
	this->dialog = new Dialog(font, window);
}


Tutorial::~Tutorial()
{
	delete this->dialog;
}

void Tutorial::Update(const float &dt) {
	switch (currentStage) {
		case INTRO:
			this->updateIntro();
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

void Tutorial::updateIntro() {
	if (!this->dialog->isActive()) {
		this->dialog->activate();
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
