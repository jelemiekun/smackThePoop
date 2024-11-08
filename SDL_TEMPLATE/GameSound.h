#pragma once
#include "SDL_mixer.h"

struct SoundFXState {
	unsigned int fx : 3;
};

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

	SoundFXState FXState = { 0 };

public:
	GameSound();
	~GameSound();

	void initMixer();
	void loadSoundFX();
	void loadMusic();
	void playMusic();
	void setSoundFX(Uint8 state);
	void playSoundFX();
	void close();
};

