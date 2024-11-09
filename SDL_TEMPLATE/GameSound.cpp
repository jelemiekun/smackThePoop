#include "GameSound.h"
#include <iostream>

GameSound::GameSound() : gMusicCurrent(nullptr), gMusicStartMenu(nullptr), 
					gMusicPlaying(nullptr), gMusicGameOver(nullptr),
					gFXRelief(nullptr), 	gFXGrunt(nullptr),
					gFXHitPlayer(nullptr),	gFXHitPoop(nullptr), 
					gFXPoopStart(nullptr),	gFXPoopEnd(nullptr),  
					gFXFart(nullptr){};

GameSound::~GameSound() {}

void GameSound::initMixer() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cout << "Warning: Failed to setup music controller: " << Mix_GetError() << '\n';
	}
}

void GameSound::loadMusics() {
	gMusicStartMenu = Mix_LoadMUS("assets/music/startMenu.mp3");

	if (gMusicStartMenu == nullptr)
		std::cout << "Failed to load the music: " << Mix_GetError() << '\n';
	else std::cout << "Music start menu loaded" << '\n';

	gMusicPlaying = Mix_LoadMUS("assets/music/playing.mp3");

	if (gMusicPlaying == nullptr)
		std::cout << "Failed to load the music: " << Mix_GetError() << '\n';
	else std::cout << "Music playing loaded" << '\n';

	gMusicGameOver = Mix_LoadMUS("assets/music/gameOver.mp3");

	if (gMusicGameOver == nullptr)
		std::cout << "Failed to load the music: " << Mix_GetError() << '\n';
	else std::cout << "Music game over loaded" << '\n';
}

void GameSound::loadSoundFX() {
	gFXRelief = Mix_LoadWAV("assets/fx/relief.wav");

	if (gFXRelief == nullptr)
		std::cout << "Failed to load fx relief: " << Mix_GetError() << '\n';
	else
		std::cout << "FX relief loaded." << '\n';

	gFXGrunt = Mix_LoadWAV("assets/fx/grunt.wav");

	if (gFXGrunt == nullptr)
		std::cout << "Failed to load fx grunt: " << Mix_GetError() << '\n';
	else
		std::cout << "FX grunt loaded." << '\n';

	gFXHitPlayer = Mix_LoadWAV("assets/fx/hitPlayer.wav");

	if (gFXHitPlayer == nullptr)
		std::cout << "Failed to load fx hitPlayer: " << Mix_GetError() << '\n';
	else
		std::cout << "FX hitPlayer loaded." << '\n';

	gFXHitPoop = Mix_LoadWAV("assets/fx/hitPoop.wav");

	if (gFXHitPoop == nullptr)
		std::cout << "Failed to load fx hitPoop: " << Mix_GetError() << '\n';
	else
		std::cout << "FX hitPoop loaded." << '\n';

	gFXPoopStart = Mix_LoadWAV("assets/fx/poopStart.wav");

	if (gFXPoopStart == nullptr)
		std::cout << "Failed to load fx poopStart: " << Mix_GetError() << '\n';
	else
		std::cout << "FX poopStart loaded." << '\n';

	gFXPoopEnd = Mix_LoadWAV("assets/fx/poopEnd.wav");

	if (gFXPoopEnd == nullptr)
		std::cout << "Failed to load fx poopEnd: " << Mix_GetError() << '\n';
	else
		std::cout << "FX poopEnd loaded." << '\n';

	gFXFart = Mix_LoadWAV("assets/fx/fart.wav");

	if (gFXFart == nullptr) 
		std::cout << "Failed to load fx fart: " << Mix_GetError() << '\n';
	else
		std::cout << "FX fart loaded." << '\n';

	Mix_VolumeChunk(gFXRelief, 100);
	Mix_VolumeChunk(gFXGrunt, 100);
	Mix_VolumeChunk(gFXHitPlayer, 124);
	Mix_VolumeChunk(gFXHitPoop, 100);
	Mix_VolumeChunk(gFXPoopStart, 100);
	Mix_VolumeChunk(gFXPoopEnd, 70);
	Mix_VolumeChunk(gFXFart, 70);
}

void GameSound::setMusic(ClassMusic music) {
	switch (music) {
	case ClassMusic::startMenu:	gMusicCurrent = gMusicStartMenu; break;
	case ClassMusic::playing: gMusicCurrent = gMusicPlaying; break;
	case ClassMusic::gameOver: gMusicCurrent = gMusicGameOver;  break;
	default: break;
	}
}

void GameSound::playMusic() {
	Mix_VolumeMusic(35);
	Mix_FadeInMusic(gMusicCurrent, -1, 1500);
}

void GameSound::stopMusic() {
	Mix_FadeOutMusic(500);
}

void GameSound::setSoundFX(ClassSoundFXState state) {
	FXState = state;
}

void GameSound::playSoundFX() {
	switch (FXState) {
	case ClassSoundFXState::relief:
		Mix_PlayChannel(-1, gFXRelief, 0);
		break;
	case ClassSoundFXState::grunt:
		Mix_PlayChannel(-1, gFXGrunt, 0);
		break;
	case ClassSoundFXState::hitPlayer:
		Mix_PlayChannel(-1, gFXHitPlayer, 0);
		break;
	case ClassSoundFXState::hitPoop:
		Mix_PlayChannel(-1, gFXHitPoop, 0);
		break;
	case ClassSoundFXState::poopStart:
		Mix_PlayChannel(-1, gFXPoopStart, 0);
		break;
	case ClassSoundFXState::poopEnd:
		Mix_PlayChannel(-1, gFXPoopEnd, 0);
		break;
	case ClassSoundFXState::fart:
		Mix_PlayChannel(-1, gFXFart, 0);
		break;
	default:
		break;
	}
}

void GameSound::close() {
	Mix_FreeChunk(gFXRelief);
	Mix_FreeChunk(gFXGrunt);
	Mix_FreeChunk(gFXHitPlayer);
	Mix_FreeChunk(gFXHitPoop);
	Mix_FreeChunk(gFXPoopStart);
	Mix_FreeChunk(gFXPoopEnd);
	Mix_FreeChunk(gFXFart);

	Mix_FreeMusic(gMusicCurrent);
	Mix_FreeMusic(gMusicStartMenu);
	Mix_FreeMusic(gMusicPlaying);
	Mix_FreeMusic(gMusicGameOver);

	Mix_Quit();
}