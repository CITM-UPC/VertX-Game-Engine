#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "../Engine/p2List.h"
#include "Log.h"


ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled), music(NULL)
{}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	bool ret = true;

	// Remove unnecessary SDL_Init(0)

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// Load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	// Adjust the number of channels according to your needs
	Mix_AllocateChannels(16);  // Change the value based on your requirements

	effectMusicPlayed = false;

	return ret;
}

update_status ModuleAudio::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::Update()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;

	for (item = fx.getFirst(); item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}


void ModuleAudio::playFirstSong(const char* filePath, int durationInSeconds) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		// handle error
		return;
	}

	Mix_Music* music = Mix_LoadMUS(filePath);
	if (!music) {
		// handle error
		return;
	}

	// Start playing the first song
	if (Mix_PlayMusic(music, 0) == -1) {
		// handle error
		Mix_FreeMusic(music);
		return;
	}

	// Record the start time
	Uint32 startTime = SDL_GetTicks();

	// Wait for the specified duration
	while (SDL_GetTicks() - startTime < durationInSeconds * 1000) {
		SDL_Delay(100);  // Adjust this delay as needed
	}

	// Stop playing the first song
	Mix_HaltMusic();
	Mix_FreeMusic(music);
}

void ModuleAudio::playSoundEffect(const char* filePath, int repeatCount, double volumeLevel) {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		// handle error
		return;
	}

	Mix_Chunk* soundEffect = Mix_LoadWAV(filePath);
	if (!soundEffect) {
		// handle error
		return;
	}

	// Map the volume level to the valid range [0, 128]
	int volume = static_cast<int>(volumeLevel * MIX_MAX_VOLUME);

	// Play the sound effect
	int channel = Mix_PlayChannel(-1, soundEffect, repeatCount);
	if (channel == -1) {
		// handle error
		Mix_FreeChunk(soundEffect);
		return;
	}

	// Set the volume for the channel
	Mix_Volume(channel, volume);

	// Optionally, you may want to wait until the sound effect finishes playing before freeing it
	while (Mix_Playing(channel) != 0) {
		SDL_Delay(10);  // Add a small delay to avoid a busy-wait loop
	}

	// Free the allocated sound effect
	Mix_FreeChunk(soundEffect);
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time, int effectChannel)
{
	bool ret = true;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic((int)(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// This call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		ret = false;
	}
	else
	{
		// Check if the effect channel is valid
		if (effectChannel >= 0)
		{
			// Stop any existing sound on the specified channel
			Mix_HaltChannel(effectChannel);
		}

		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
		
				ret = false;
			}
		}
	}

	return ret;
}



// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat, int channel)
{
	bool ret = false;

	Mix_Chunk* chunk = NULL;

	if (fx.at(id - 1, chunk) == true)
	{
		Mix_PlayChannel(channel, chunk, repeat);
		ret = true;
	}

	return ret;
}