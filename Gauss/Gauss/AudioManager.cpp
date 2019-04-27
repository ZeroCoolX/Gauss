#include "AudioManager.h"


AudioManager::AudioManager()
{
	sounds.Add(this->LoadSound("Audio/Sounds/Shot.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/GaussCannon.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/ExplosionBlast.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/ExplosionFireBlast.ogg"));

	this->musicContainer.Add(new Music());
	//this->musicContainer[AudioManager::AudioMusic::MENU]->openFromFile("Audio/Music/Music_Boss.ogg");
}


AudioManager::~AudioManager()
{
}


AudioManager::SoundInfo* AudioManager::LoadSound(const std::string& path) {
	SoundInfo* soundInfo = new SoundInfo(path, 50.f);

	if (!soundInfo->soundBuffer->loadFromFile(soundInfo->filePath)) {
		delete soundInfo->soundBuffer;
		soundInfo->soundBuffer = nullptr;
		std::cerr << "! Failed to load sound: " << soundInfo->filePath << std::endl;
	}
	soundInfo->sound->setBuffer(*soundInfo->soundBuffer);

	return soundInfo;
}

void AudioManager::PlaySound(AudioManager::AudioSounds sIndex) {
	std::cout << "Playing sound" << std::endl;
	this->sounds[sIndex]->sound->play();
}

void AudioManager::PlayMusic(AudioManager::AudioMusic mIndex) {
	std::cout << "Playing music" << std::endl;
	this->musicContainer[mIndex]->play();
}

void AudioManager::StopMusic(AudioManager::AudioMusic mIndex) {
	std::cout << "Stopping music" << std::endl;
	this->musicContainer[mIndex]->stop();
}
