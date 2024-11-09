#include "Game.h"
#include <iostream>
#include <string>

Game::Game() : gWindow(nullptr), running(false), 
				imgBackground(nullptr), imgHeart1(nullptr), imgHeart2(nullptr),
				imgHeart3(nullptr),
				controller1(nullptr), gameSounds(nullptr),
				character(nullptr), gFont(nullptr), textTimer(nullptr),
				gameTimer(nullptr), poopBar(nullptr), poopFart(nullptr), flags(nullptr),
				poopFXTimer(nullptr), poopClickTimer(nullptr) {}

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

	gameTimer = new Timer;
	gameTimer->setStartingTime(20000);
	gameTimer->startTimer();

	poopFXTimer = new Timer;

	poopClickTimer = new Timer;

	textTimer = new Text;
	textTimer->setGFont(gFont);

	constexpr int TIMER_RECT_X_ALLOWANCE = 20;
	constexpr int TIMER_RECT_Y_ALLOWANCE = 8;
	constexpr int timerRectWidth = 110;
	timerRect = new SDL_Rect;
	timerRect->x = SCREEN_WIDTH - timerRectWidth - TIMER_RECT_X_ALLOWANCE;
	timerRect->y = TIMER_RECT_Y_ALLOWANCE;
	timerRect->w = timerRectWidth;
	timerRect->h = 50;

	poopBar = new PoopBar;
	poopBar->init(gRenderer);
	poopBar->setXY(150, 10);

	
	poopFart = new PoopFart;
	poopFart->loadFromFile(gRenderer, "assets/img/poopFart.png");
	poopFart->init();
	poopFart->initSpriteSheet();

	gameSounds->playMusic();
	
	flags = new InputFlags;
	flags->playing = 1;
	flags->poopInProgress = 0;
	flags->poopFinished = 1;
	flags->FXInProgress = 0;
	flags->FXFinished = 1;
	flags->isFXPoopFinished = 1;
	flags->kindOfPoop = 0;
	flags->takeDamage = 0;
	flags->takeDamageInProgress = 0;
	flags->takeDamageFinished = 1;
	flags->animateSlap = 0;
	flags->animateSlapInProgress = 0;
	flags->animateSlapFinished = 1;

	running = true;
}

bool Game::isRunning() const {
	return running;
}

void Game::input() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		} else {
			if (flags->playing) {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case PRIMARY_BUTTON:
						if (flags->poopFinished) {
							flags->poopInProgress = 1;
							flags->poopFinished = 0;
							flags->FXInProgress = 1;
							flags->FXFinished = 0;
						}
						break;
					case LEFT_BUTTON:
						if (flags->poopInProgress) {
							if (poopClickTimer->isFinish()) {
								flags->takeDamage = 1;
							} else {
								switch (flags->kindOfPoop) {
								case 0: case 2: case 3: {
									if (flags->takeDamageFinished && flags->animateSlapFinished) {
										flags->takeDamage = 1;
										flags->takeDamageInProgress = 1;
										flags->takeDamageFinished = 0;
									}
								}
									  break;
								case 1: {
									if (flags->animateSlapFinished && flags->takeDamageFinished) {
										flags->animateSlap = 1;
										flags->animateSlapInProgress = 1;
										flags->animateSlapFinished = 0;
									}
								}
									  break;
								default: break;
								}
							}
						}
						break;
					case RIGHT_BUTTON:
						if (flags->poopInProgress) {
							if (poopClickTimer->isFinish()) {
								flags->takeDamage = 1;
							} else {
								switch (flags->kindOfPoop) {
								case 0: case 1: case 3: {
									if (flags->takeDamageFinished && flags->animateSlapFinished) {
										flags->takeDamage = 1;
										flags->takeDamageInProgress = 1;
										flags->takeDamageFinished = 0;
									}
								}
									  break;
								case 2: {
									if (flags->animateSlapFinished && flags->takeDamageFinished) {
										flags->animateSlap = 1;
										flags->animateSlapInProgress = 1;
										flags->animateSlapFinished = 0;
									}
								}
									  break;
								default: break;
								}
							}
						}
						break;
					default:
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
					case PRIMARY_BUTTON:
						break;
					case LEFT_BUTTON:
						break;
					case RIGHT_BUTTON:
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void Game::update() {
	if (flags->FXInProgress && flags->poopInProgress) {
		poopFart->generateRandomNumber();
		int state = poopFart->getToReleaseState();
		ClassSoundFXState temp;

		if (state == 0 || state == 1 || state == 2) temp = ClassSoundFXState::poopStart;
		else if (state == 5) temp = ClassSoundFXState::fart;

		switch (state) {
		case 0: flags->kindOfPoop = 0; break;
		case 1: flags->kindOfPoop = 1; break;
		case 2: flags->kindOfPoop = 2; break;
		case 5: flags->kindOfPoop = 3; break;
		default: break;
		}

		gameSounds->setSoundFX(temp);
		gameSounds->playSoundFX();

		flags->FXInProgress = 0;

		poopFXTimer->setStartingTime(500);
		poopFXTimer->startTimer();

		poopClickTimer->setStartingTime(500);
		poopClickTimer->startTimer();

	}

	if (flags->poopInProgress && !flags->animateSlap && !flags->takeDamage &&
		(flags->kindOfPoop == 1 || flags->kindOfPoop == 2) && poopClickTimer->isFinish()) {
		flags->takeDamage = 1;
		flags->takeDamageInProgress = 1;
		flags->takeDamageFinished = 0;
	}

	if (flags->takeDamage && flags->takeDamageInProgress) {
		if (heartStates.heart3) {
			heartStates.heart3 = 0;
		} else {
			if (heartStates.heart2) heartStates.heart2 = 0;
			else heartStates.heart1 = 0;
		}
		flags->takeDamageInProgress = 0;
	}

	if (flags->animateSlap && flags->animateSlapInProgress) {
		std::cout << "nice!" << '\n';
		flags->animateSlapInProgress = 0;
	}

	Uint32 temp1 = poopClickTimer->getRawTime();
	Uint32 temp2 = poopFXTimer->getRawTime();

	if ((flags->kindOfPoop == 0 || flags->kindOfPoop == 1 || flags->kindOfPoop == 2)
		&& flags->poopInProgress && poopFXTimer->isFinish() && flags->isFXPoopFinished) {
		gameSounds->setSoundFX(ClassSoundFXState::poopEnd);
		gameSounds->playSoundFX();
		flags->isFXPoopFinished = 0;
	}
}

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

	
	character->animate(gRenderer, flags->takeDamage);

	SDL_Color black = { 0, 0, 0, 255 };
	textTimer->loadFromRenderedText(gRenderer, gameTimer->getTimeInFormat(), black, timerRect );

	poopBar->render(gRenderer);

	if (flags->poopInProgress) {
		poopFart->render(gRenderer);

		if (poopFart->isFinishRendering()) {
			flags->poopInProgress = 0;
			flags->poopFinished = 1;
			flags->isFXPoopFinished = 1;
			flags->takeDamage = 0;
			flags->takeDamageInProgress = 0;
			flags->takeDamageFinished = 1;
			flags->animateSlap = 0;
			flags->animateSlapInProgress = 0;
			flags->animateSlapFinished = 1;
		}
	}

	SDL_RenderPresent(gRenderer);

	flags->playing = !gameTimer->isFinish();
	flags->playing = !isGameOver();

	if (!flags->playing) gameTimer->stopTimer();
}

bool Game::isGameOver() const {
	return !heartStates.heart1;
}

void Game::close() {
	controller1->close();
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gameSounds->close();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}