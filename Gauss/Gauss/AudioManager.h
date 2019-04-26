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
		SoundInfo(const std::string& path){
			this->filePath = path;
			this->sound = new Sound();
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

