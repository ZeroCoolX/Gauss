#pragma once

#include "GameObject.h"

class AudioManager
{
public:
	class SoundInfo {
	public:
		std::string filePath;
		bool playing;
		Sound* sound;
		SoundBuffer* soundBuffer; 
		
	public:
		SoundInfo(const std::string& path, float volume = 100.f, float pitch = 1.f, bool loop = false) {
			// Init audio filepath
			this->filePath = path;

			// Init pointers
			this->soundBuffer = new SoundBuffer();
			this->sound = new Sound();
			// Init sound properties
			this->sound->setVolume(volume);
			this->sound->setPitch(pitch);
			this->sound->setLoop(loop);
		}

		~SoundInfo() {
			delete sound;
			delete soundBuffer;
		}
	};

private:
	dArr<Music*> musicContainer;
	dArr<SoundInfo*> sounds;

public:
	enum AudioSounds {
		PLAYER_LASER = 0,
		GAUSS_CANNON,
		GAUSS_CANNON_RELOAD,
		BUTTON_CLICK,
		BADDIE_DEATH_01,
		PLAYER_DEATH,
		POWERUP_ABSORB,
		GAME_OVER,
		POWERUP_GRIND,
		POWERUP_GRIND_IDLE,
		HEALTH_PICKUP,
		LEVEL_UP,
		POWERUP_COLLECT,
		UPGRADE_COLLECT,
		COSMO_WARP,
		COSMO_BADDIE_SWAP,
		COSMO_SPEED_DOWN,
		COSMO_SPEED_UP,
		COSMO_INVERT_CONTROLS,
		REFLECT_BULLET,
		ENEMY_BULLET_IMPACT,
		COSMO_ENEMY_IMPACT,
		PLAYER_ENEMY_IMPACT
	};

	enum AudioMusic {
		MENU = 0,
		DEFAULT
	};

public:
	AudioManager();
	~AudioManager();

	SoundInfo* LoadSound(const std::string& path, float volume = 100.f, bool loop = false);
	void PlaySound(AudioSounds sIndex);
	void StopSound(AudioSounds sIndex);
	void PlayMusic(AudioMusic mIndex);
	void StopMusic(AudioMusic mIndex);
};

