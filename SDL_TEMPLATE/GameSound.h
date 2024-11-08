#pragma once
#include "SDL_mixer.h"

class GameSound {
private:
	Mix_Music* gMusic;

	Mix_Chunk* gFXRelief;
	Mix_Chunk* gFXGrunt;
	Mix_Chunk* gFXHitPlayer;
	Mix_Chunk* gFXHitPoop;
	Mix_Chunk* gFXPoopStart;
	Mix_Chunk* gFXPoopEnd;
	Mix_Chunk* gFXFart;

public:
	GameSound();
	~GameSound();

	void initMixer();
	void loadSoundFX();
	void loadMusic();
	void close();
};

