#include "Game.h"
#include <iostream>

Game::Game() : gWindow(nullptr), gRenderer(nullptr), running(false), 
				imgBackground(nullptr), imgHeart(nullptr),
				controller1(nullptr), gameSounds(nullptr) {}

Game::~Game() {}

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to initialize SDL subsystems: " << SDL_GetError() << '\n';
		return;
	} else {
		std::cout << "SDL subsystems initialized." << '\n';
	}
	
	gWindow = SDL_CreateWindow(SCREEN_TITLE, SCREEN_X_POS, SCREEN_Y_POS, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_FLAGS);

	if (gWindow == nullptr) {
		std::cout << "Failed to created window: " << SDL_GetError() << '\n';
		return;
	} else {
		std::cout << "Window created" << '\n';
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (gRenderer == nullptr)
		std::cout << "Failed to create renderer: " << SDL_GetError() << '\n';
	else
		std::cout << "Renderer created";

	imgBackground = new GameImage;
	imgBackground->loadFromFile(gRenderer, "assets/img/background.png");

	imgHeart = new GameImage;
	imgHeart->loadFromFile(gRenderer, "assets/img/heart.png");

	controller1 = new GameController;
	controller1->init();

	gameSounds = new GameSound;
	gameSounds->initMixer();
	gameSounds->loadMusic();
	gameSounds->loadSoundFX();

	running = true;
}

bool Game::isRunning() const {
	return running;
}

void Game::input() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		if (controller1->getGameController() != nullptr) {
			controller1->input();
		}
	}
}

void Game::update() {
}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	SDL_Rect heart = { 0, 0, imgHeart->getSrcRect()->w / 7, imgHeart->getSrcRect()->h / 7};

	imgBackground->render(gRenderer, nullptr);
	imgHeart->render(gRenderer, &heart);

	SDL_RenderPresent(gRenderer);
}

void Game::close() {
	gameSounds->close();
	controller1->close();
}