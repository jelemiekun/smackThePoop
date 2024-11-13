#pragma once
#include "SDL_mixer.h"

enum class ClassSoundFXState {
	relief,
	grunt,
	hitPlayer,
	hitPoop,
	poopStart,
	poopEnd,
	fart,
	countDownBeep
};

enum class ClassMusic {
	startMenu,
	playing,
	gameOver,
	win
};

class GameSound {
private:

	Mix_Music* gMusicCurrent;
	Mix_Music* gMusicStartMenu;
	Mix_Music* gMusicPlaying;
	Mix_Music* gMusicGameOver;
	Mix_Music* gMusicWin;

	Mix_Chunk* gFXRelief;
	Mix_Chunk* gFXGrunt;
	Mix_Chunk* gFXHitPlayer;
	Mix_Chunk* gFXHitPoop;
	Mix_Chunk* gFXPoopStart;
	Mix_Chunk* gFXPoopEnd;
	Mix_Chunk* gFXFart;
	Mix_Chunk* gFXBeep;

	ClassSoundFXState FXState;

public:
	GameSound();
	~GameSound();

	void initMixer();
	void loadSoundFX();
	void loadMusics();
	void setMusic(const ClassMusic &music);
	void playMusic();
	void stopMusic();
	void setSoundFX(const ClassSoundFXState &state);
	void setFXsVolume(const uint16_t &volume);
	void playSoundFX();
	void close();
};

