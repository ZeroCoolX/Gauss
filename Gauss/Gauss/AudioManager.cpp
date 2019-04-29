#include "AudioManager.h"


AudioManager::AudioManager()
{
	sounds.Add(this->LoadSound("Audio/Sounds/player_laser02.ogg", 85.f));
	sounds.Add(this->LoadSound("Audio/Sounds/gauss_cannon.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/gauss_cannon_ready.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/button_click.ogg", 75.f));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_death01_short.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_death01.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/absorb.ogg", 75.f));
	sounds.Add(this->LoadSound("Audio/Sounds/game_over.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/grinder_grinding.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/grinder_idle.ogg", 100.f, true));
	sounds.Add(this->LoadSound("Audio/Sounds/health_pickup.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/level_up_robovoice.ogg", 90.f));
	sounds.Add(this->LoadSound("Audio/Sounds/powerup_collected.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/upgrade_collected.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/warp_cosmo.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_swap_cosmo.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/speed_decrease_cosmo.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/speed_increase_cosmo.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/invert_controls_cosmo.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/reflect_projectile.ogg", 75.f));
	sounds.Add(this->LoadSound("Audio/Sounds/enemy_bullet_impact.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/cosmo_impact.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/baddie_death01_short.ogg"));

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::MENU]->openFromFile("Audio/Music/Music_Boss.ogg");
}


AudioManager::~AudioManager()
{
}


AudioManager::SoundInfo* AudioManager::LoadSound(const std::string& path, float volume, bool loop) {
	SoundInfo* soundInfo = new SoundInfo(path, volume, 1.f, loop);

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

void AudioManager::StopSound(AudioManager::AudioSounds sIndex) {
	this->sounds[sIndex]->sound->stop();
}

void AudioManager::PlayMusic(AudioManager::AudioMusic mIndex) {
	this->musicContainer[mIndex]->play();
}

void AudioManager::StopMusic(AudioManager::AudioMusic mIndex) {
	this->musicContainer[mIndex]->stop();
}
