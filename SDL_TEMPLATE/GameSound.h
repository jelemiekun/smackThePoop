#pragma once
#include "SDL_mixer.h"

enum class ClassSoundFXState {
	relief,
	grunt,
	hitPlayer,
	hitPoop,
	poopStart,
	poopEnd,
	fart
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

	ClassSoundFXState FXState;

public:
	GameSound();
	~GameSound();

	void initMixer();
	void loadSoundFX();
	void loadMusic();
	void playMusic();
	void setSoundFX(ClassSoundFXState state);
	void playSoundFX();
	void close();
};

