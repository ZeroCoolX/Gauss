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
	sounds.Add(this->LoadSound("Audio/Sounds/grinder_idle.ogg", 15.f, true));
	sounds.Add(this->LoadSound("Audio/Sounds/health_pickup.ogg"));
	sounds.Add(this->LoadSound("Audio/Sounds/level_up_robovoice.ogg"));
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
	this->musicContainer[AudioManager::AudioMusic::MENU]->setLoop(true);
	this->musicContainer[AudioManager::AudioMusic::MENU]->setVolume(50.f);
	this->musicContainer[AudioManager::AudioMusic::MENU]->openFromFile("Audio/Music/menu.ogg");

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::CAMPAIGN]->setLoop(false);
	this->musicContainer[AudioManager::AudioMusic::CAMPAIGN]->openFromFile("Audio/Music/campaign.ogg");

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::INF_COS_01]->setLoop(true);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_01]->setVolume(25.f);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_01]->openFromFile("Audio/Music/infinite_cosmos_01-trimmed.ogg");

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::INF_COS_02]->setLoop(true);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_02]->setVolume(25.f);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_02]->openFromFile("Audio/Music/infinite_cosmos_02-trimmed.ogg");

	this->musicContainer.Add(new Music());
	this->musicContainer[AudioManager::AudioMusic::INF_COS_03]->setLoop(true);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_03]->setVolume(25.f);
	this->musicContainer[AudioManager::AudioMusic::INF_COS_03]->openFromFile("Audio/Music/infinite_cosmos_03-trimmed.ogg");
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

void AudioManager::PlayMusic(int mIndex) {
	this->musicContainer[mIndex]->play();
}

void AudioManager::StopMusic(int mIndex) {
	this->musicContainer[mIndex]->stop();
}

void AudioManager::PauseMusic(int mIndex) {
	this->musicContainer[mIndex]->pause();
}

void AudioManager::UnpauseMusic(int mIndex) {
	if (this->musicContainer[mIndex]->getStatus() == Music::Paused) {
		this->musicContainer[mIndex]->play();
	}
}
