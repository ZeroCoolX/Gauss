#include "AudioManager.h"


AudioManager::AudioManager()
{
	sounds.Add(this->LoadSound("Audio/Sounds/player_laser02.ogg", 50.f));
	sounds.Add(this->LoadSound("Audio/Sounds/gauss_cannon.ogg", 50.f));
	sounds.Add(this->LoadSound("Audio/Sounds/gauss_cannon_ready.ogg", 50.f));
	sounds.Add(this->LoadSound("Audio/Sounds/button_click.ogg", 50.f));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_death01_short.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_death01.ogg"));

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::MENU]->openFromFile("Audio/Music/Music_Boss.ogg");
}


AudioManager::~AudioManager()
{
}


AudioManager::SoundInfo* AudioManager::LoadSound(const std::string& path, float volume) {
	SoundInfo* soundInfo = new SoundInfo(path, volume);

	if (!soundInfo->soundBuffer->loadFromFile(soundInfo->filePath)) {
		delete soundInfo->soundBuffer;
		soundInfo->soundBuffer = nullptr;
		std::cerr << "! Failed to load sound: " << soundInfo->filePath << std::endl;
	}
	soundInfo->sound->setBuffer(*soundInfo->soundBuffer);

	return soundInfo;
}

void AudioManager::PlaySound(AudioManager::AudioSounds sIndex) {
	this->sounds[sIndex]->sound->play();
}

void AudioManager::PlayMusic(AudioManager::AudioMusic mIndex) {
	this->musicContainer[mIndex]->play();
}

void AudioManager::StopMusic(AudioManager::AudioMusic mIndex) {
	this->musicContainer[mIndex]->stop();
}
