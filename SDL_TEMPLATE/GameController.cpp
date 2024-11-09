#include "GameController.h"
#include <iostream>

static unsigned short int numController = 0;

GameController::GameController() : gGameController(nullptr) {}

GameController::~GameController() {}

void GameController::init() {
	gGameController = SDL_GameControllerOpen(numController);

	if (gGameController == nullptr) {
		std::cout << "Warning: Unable to open game controller " << numController << ": " << SDL_GetError() << '\n';
	} else {
		std::cout << "Opened controller " << numController << '\n';
		++numController;
	}
}

SDL_GameController* GameController::getGameController() const {
	return gGameController;
}

void GameController::close() {
	SDL_GameControllerClose(gGameController);
}