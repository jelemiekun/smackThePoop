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

	ClassSoundFXState FXState;

public:
	GameSound();
	~GameSound();

	void initMixer();
	void loadSoundFX();
	void loadMusics();
	void setMusic(ClassMusic music);
	void playMusic();
	void setMusicVolume(uint8_t volume);
	void stopMusic();
	void setSoundFX(ClassSoundFXState state);
	void setsfxVolume(uint8_t volume);
	void playSoundFX();
	void close();
};

