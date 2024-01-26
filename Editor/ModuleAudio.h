#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "../Engine/p2List.h"
#include <SDL2/SDL_mixer.h>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time);

	void playFirstSong(const char* filePath, int durationInSeconds);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	void playSoundEffect(const char* filePath, int repeatCount, double volumeLevel);


	

private:
	Mix_Music* music;

	Mix_Chunk* soundEffect;
	
	p2List<Mix_Chunk*>	fx;
};

#endif // __ModuleAudio_H__