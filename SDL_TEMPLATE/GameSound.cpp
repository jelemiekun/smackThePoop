#include "GameSound.h"
#include <iostream>

GameSound::GameSound() : gMusic(nullptr), 
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

void GameSound::loadMusic() {
	gMusic = Mix_LoadMUS("assets/music/music.mp3");

	if (gMusic == nullptr)
		std::cout << "Failed to load the music: " << Mix_GetError() << '\n';
	else
		std::cout << "Music loaded" << '\n';
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
}

void GameSound::playMusic() {
	Mix_FadeInMusic(gMusic, -1, 5000);
}

void GameSound::setSoundFX(Uint8 state) {
	FXState.fx = { state };
}

void GameSound::playSoundFX() {
	switch (FXState.fx) {
	case 0:
		Mix_PlayChannel(-1, gFXRelief, 0);
		break;
	case 1:
		Mix_PlayChannel(-1, gFXGrunt, 0);
		break;
	case 2:
		Mix_PlayChannel(-1, gFXHitPlayer, 0);
		break;
	case 3:
		Mix_PlayChannel(-1, gFXHitPoop, 0);
		break;
	case 4:
		Mix_PlayChannel(-1, gFXPoopStart, 0);
		break;
	case 5:
		Mix_PlayChannel(-1, gFXPoopEnd, 0);
		break;
	case 6:
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

	Mix_FreeMusic(gMusic);

	Mix_Quit();
}