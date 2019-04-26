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
			this->sound = new Sound();
			this->sound->setVolume(volume);
			this->sound->setPitch(pitch);
			this->sound->setLoop(loop);

			this->soundBuffer = new SoundBuffer();
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
		SHOT_DEFAULT = 0,
		GAUSS_CANNON,
		EXPLOSION01,
		EXPLOSION02
	};

	enum AudioMusic {
		MENU = 0,
		DEFAULT
	};

public:
	AudioManager();
	~AudioManager();

	SoundInfo* LoadSound(const std::string& path);
	void PlaySound(AudioSounds sIndex);
	void PlayMusic(AudioMusic mIndex);
	void StopMusic(AudioMusic mIndex);
};

