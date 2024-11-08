#include "Game.h"
#include <iostream>
#include <string>

Game::Game() : gWindow(nullptr), running(false), 
				imgBackground(nullptr), imgHeart1(nullptr), imgHeart2(nullptr),
				imgHeart3(nullptr),
				controller1(nullptr), gameSounds(nullptr), isPlaying(false),
				character(nullptr), gFont(nullptr), textTimer(nullptr) {}

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
		std::cout << "Renderer created" << '\n';

	if (TTF_Init() == -1)
		std::cout << "SDL_TTF could not initialize: " << TTF_GetError() << '\n';
	else
		std::cout << "SDL_TTF initialized." << '\n';

	gFont = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZE);
	if (gFont == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	imgBackground = new GameImage;
	imgBackground->loadFromFile(gRenderer, "assets/img/background.png");

	imgHeart1 = new Heart;
	imgHeart1->loadFromFile(gRenderer, "assets/img/heart.png");
	imgHeart1->getSrcRect()->w = imgHeart1->getSrcRect()->w / 2;

	imgHeart2 = new Heart;
	imgHeart2->loadFromFile(gRenderer, "assets/img/heart.png");
	imgHeart2->getSrcRect()->w = imgHeart2->getSrcRect()->w / 2;

	imgHeart3 = new Heart;
	imgHeart3->loadFromFile(gRenderer, "assets/img/heart.png");
	imgHeart3->getSrcRect()->w = imgHeart3->getSrcRect()->w / 2;

	character = new Character;
	character->loadFromFile(gRenderer, "assets/img/player.png");
	character->init();

	heartStates = { 1, 1, 1 };

	controller1 = new GameController;
	controller1->init();

	gameSounds = new GameSound;
	gameSounds->initMixer();
	gameSounds->loadMusic();
	gameSounds->loadSoundFX();

	textTimer = new Text;
	textTimer->setGFont(gFont);

	constexpr int TIMER_RECT_X_ALLOWANCE = 20;
	constexpr int timerRectWidth = 110;
	timerRect = new SDL_Rect;
	timerRect->x = SCREEN_WIDTH - timerRectWidth - TIMER_RECT_X_ALLOWANCE;
	timerRect->y = 15;
	timerRect->w = timerRectWidth;
	timerRect->h = 50;

	gameSounds->playMusic();
	

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

int texts = 0;

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderClear(gRenderer);

	constexpr int HEART_X_ALLOWANCE = 20;
	constexpr int HEART_Y_ALLOWANCE = 15;
	constexpr int HEART_GAP = 3;

	SDL_Rect heart1 = { 0 + HEART_X_ALLOWANCE, HEART_Y_ALLOWANCE,
		imgHeart1->getSrcRect()->w / 7, imgHeart1->getSrcRect()->h / 7 };

	SDL_Rect heart2 = { (imgHeart2->getSrcRect()->w / 7,
		imgHeart2->getSrcRect()->h / 7) + HEART_X_ALLOWANCE + (HEART_GAP * 1), HEART_Y_ALLOWANCE,
		imgHeart2->getSrcRect()->w / 7, imgHeart2->getSrcRect()->h / 7 };

	SDL_Rect heart3 = { (imgHeart2->getSrcRect()->w / 7,
		imgHeart2->getSrcRect()->h / 7) * 2 + HEART_X_ALLOWANCE + (HEART_GAP * 2), HEART_Y_ALLOWANCE,
		imgHeart3->getSrcRect()->w / 7, imgHeart3->getSrcRect()->h / 7 };


	imgBackground->render(gRenderer, nullptr);
	imgHeart1->render(gRenderer, &heart1, heartStates.heart1);
	imgHeart2->render(gRenderer, &heart2, heartStates.heart2);
	imgHeart3->render(gRenderer, &heart3, heartStates.heart3);

	SDL_Rect rectCharacter = { 0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT };
	rectCharacter.x = (SCREEN_WIDTH / 2) - (rectCharacter.w / 2);
	character->animate(gRenderer, &rectCharacter);

	SDL_Color black = { 0, 0, 0, 255 };
	textTimer->loadFromRenderedText(gRenderer, std::to_string(texts), black, timerRect );

	texts++;

	SDL_RenderPresent(gRenderer);
}

void Game::close() {
	controller1->close();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gameSounds->close();
	IMG_Quit();
	SDL_Quit();
}