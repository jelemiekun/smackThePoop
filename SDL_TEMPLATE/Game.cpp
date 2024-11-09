#include "Game.h"
#include <iostream>
#include <string>

Game::Game() : gWindow(nullptr), running(false), 
				imgBackground(nullptr), imgHeart1(nullptr), imgHeart2(nullptr),
				imgHeart3(nullptr), imgStartBG(nullptr),
				controller1(nullptr), gameSounds(nullptr),
				character(nullptr), gFontTimer(nullptr), textTimer(nullptr),
				gameTimer(nullptr), poopBar(nullptr), poopFart(nullptr), flags(nullptr),
				poopFXTimer(nullptr), poopClickTimer(nullptr), imgGameOverBG(nullptr),
				textGameOver(nullptr), gFontGame(nullptr), gFontPlayAgain(nullptr),
				textPlayAgain(nullptr) {}

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

	gFontTimer = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZE_TIMER);
	if (gFontTimer == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	gFontGame = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZE_GAME);
	if (gFontGame == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	gFontPlayAgain = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZE_PLAY_AGAIN);
	if (gFontPlayAgain == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	textTimer = new Text;
	textTimer->setGFont(gFontTimer);

	textGameOver = new Text;
	textGameOver->setGFont(gFontGame);

	textPlayAgain = new Text;
	textPlayAgain->setGFont(gFontPlayAgain);

	imgBackground = new GameImage;
	imgBackground->loadFromFile(gRenderer, "assets/img/background.png");

	imgGameOverBG = new GameImage;
	imgGameOverBG->loadFromFile(gRenderer, "assets/img/gameOverBG.png");

	imgStartBG = new GameImage;
	imgStartBG->loadFromFile(gRenderer, "assets/img/startMenuBG.png");

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

	controller1 = new GameController;
	controller1->init();

	gameSounds = new GameSound;
	gameSounds->initMixer();
	gameSounds->loadMusic();
	gameSounds->loadSoundFX();

	gameTimer = new Timer;

	poopFXTimer = new Timer;

	poopClickTimer = new Timer;

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
	
	flags = new InputFlags;

	flags->playing = 0;
	flags->inGameOver = 0;
	flags->inStart = 1;

	running = true;
}

bool Game::isRunning() const {
	return running;
}

void Game::input() {
	static constexpr SDL_Rect GOBGyesR1 = { 141, 380, 146, 90 };
	static constexpr SDL_Rect GOBGyesR2 = { 133, 398, 163, 56 };
	static constexpr SDL_Rect GOBGnoR1 = { 141 + 221, 380, 146, 90 };
	static constexpr SDL_Rect GOBGnoR2 = { 133 + 221, 398, 163, 56 };
	bool GOBGoutsideYes;
	bool GOBGoutsideNo;


	int x;
	int y;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		} else {
			// if in game over menu
			if (LIMIT && flags->inGameOver && !flags->playing && !flags->inStart) {
				switch (event.type) {
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;

					GOBGoutsideYes = x < GOBGyesR1.x || x > GOBGyesR1.x + GOBGyesR1.w || y < GOBGyesR1.y || y > GOBGyesR1.y + GOBGyesR1.h ||
									x < GOBGyesR2.x || x > GOBGyesR2.x + GOBGyesR2.w || y < GOBGyesR2.y || y > GOBGyesR2.y + GOBGyesR2.h;
					GOBGoutsideNo = x < GOBGnoR1.x || x > GOBGnoR1.x + GOBGnoR1.w || y < GOBGnoR1.y || y > GOBGnoR1.y + GOBGnoR1.h ||
									x < GOBGnoR2.x || x > GOBGnoR2.x + GOBGnoR2.w || y < GOBGnoR2.y || y > GOBGnoR2.y + GOBGnoR2.h;

					if (GOBGoutsideYes && GOBGoutsideNo) {
						flags->GOBGoutside = 1;
					} else {
						flags->GOBGoutside = 0;

						if (!GOBGoutsideYes) flags->GOBGinside = 0;
						if (!GOBGoutsideNo) flags->GOBGinside = 2;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (flags->GOBGinside == 0) flags->GOBGinside = 1;
					if (flags->GOBGinside == 2) flags->GOBGinside = 3;
					break;
				case SDL_MOUSEBUTTONUP:
					if (flags->GOBGinside == 1) {
						flags->inGameOver = 0;
						flags->inStart = 0;
						flags->playing = 0;
						flags->GOBGinside = 0;
					}
					if (flags->GOBGinside == 3) {
						flags->inGameOver = 0;
						flags->inStart = 1;
						flags->playing = 0;
						flags->GOBGinside = 2;
					}
					break;
				default:
					break;
				}
			}

			// if in start menu
			if (LIMIT && !flags->inGameOver && !flags->playing && flags->inStart) {

			}

			// if playing
			if (LIMIT && flags->playing && !flags->inStart && !flags->inGameOver) {
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

		poopFXTimer->setStartingTime(ALLOWANCE_TIME_TO_PLAY_POOP_END);
		poopFXTimer->startTimer();

		poopClickTimer->setStartingTime(ALLOWANCE_TIME_TO_CLICK_PLAYER);
		poopClickTimer->startTimer();

	}

	if (flags->poopInProgress && !flags->animateSlap && !flags->takeDamage &&
		(flags->kindOfPoop == 1 || flags->kindOfPoop == 2) && poopClickTimer->isFinish()) {
		flags->takeDamage = 1;
		flags->takeDamageInProgress = 1;
		flags->takeDamageFinished = 0;
	}

	if (flags->takeDamage && flags->takeDamageInProgress) {
		gameSounds->setSoundFX(ClassSoundFXState::hitPlayer);
		gameSounds->playSoundFX();

		if (heartStates.heart3) {
			heartStates.heart3 = 0;
		} else {
			if (heartStates.heart2) heartStates.heart2 = 0;
			else heartStates.heart1 = 0;
		}
		flags->takeDamageInProgress = 0;
	}

	if (flags->animateSlap && flags->animateSlapInProgress) {
		gameSounds->setSoundFX(ClassSoundFXState::hitPoop);
		gameSounds->playSoundFX();

		int newReleaseState = 3;
		if (flags->kindOfPoop == 1) newReleaseState = 3;
		else if (flags->kindOfPoop == 2) newReleaseState = 4;
		poopFart->setReleaseState(newReleaseState);

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

	SDL_Color black = { 0, 0, 0, 255 };
	textTimer->loadFromRenderedText(gRenderer, gameTimer->getTimeInFormat(), black, timerRect );

	character->animate(gRenderer, flags->takeDamage);
	poopBar->render(gRenderer);

	if (flags->poopInProgress) {
		poopFart->render(gRenderer);

		if (poopFart->isFinishRendering()) {
			if (flags->poopInProgress && flags->kindOfPoop == 0 &&
				!flags->takeDamage && !flags->animateSlap) {
				poopBar->poopReleased();
			}

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

	if (LIMIT && !(!flags->playing && !flags->inGameOver && !flags->inStart)) {
		if (flags->playing && !flags->inStart && !flags->inGameOver) {
			flags->playing = !gameTimer->isFinish();
			flags->playing = !isGameOver();

			if (!flags->playing) {
				flags->inGameOver = 1;
				gameTimer->stopTimer();
			}
		}
	}

	bool inGameOver = LIMIT && !flags->playing && !flags->inStart && flags->inGameOver;
	bool inStartMenu = !flags->playing && flags->inStart && !flags->inGameOver;
	bool playAgain = !flags->playing && !flags->inStart && !flags->inGameOver;
	
	std::cout << flags->playing << ", " << flags->inStart << ", " << flags->inGameOver << '\n';
	//std::cout << inGameOver << ", " << inStartMenu << ", " << playAgain << '\n';
	if (inStartMenu) startMenu();
	if (playAgain) startGame();
	if (inGameOver) gameOver();

	SDL_RenderPresent(gRenderer);
}

bool Game::isGameOver() const {
	return !heartStates.heart1;
}

void Game::startGame() {
	flags->playing = 1;
	flags->inStart = 0;
	flags->inGameOver = 0;
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
	flags->GOBGoutside = 1;
	flags->GOBGinside = 0;
	flags->SMoutside = 1;
	flags->SMinPlay = 0;
	flags->SMinSett = 0;
	flags->SMinQuit = 0;


	if (Mix_PlayingMusic()) gameSounds->stopMusic();
	gameSounds->playMusic();

	gameTimer->resetTimer();
	gameTimer->setStartingTime(ALLOWANCE_TIME_GAMEPLAY);
	gameTimer->startTimer();

	heartStates = { 1, 1, 1 };
}

void Game::startMenu() {
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
	SDL_Rect whiteTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &whiteTransparent);

	int wSM = 0;
	int hSM = 0;

	SDL_QueryTexture(imgStartBG->mTexture, NULL, NULL, &wSM, &hSM);

	SDL_Rect srcRect = { (wSM / 7) * 0, 0, wSM / 7, hSM };

	if (!flags->SMoutside) {
		if (flags->SMinPlay) srcRect.x = (wSM / 7) * 1;
		if (flags->SMinSett) srcRect.x = (wSM / 7) * 3;
		if (flags->SMinQuit) srcRect.x = (wSM / 7) * 5;
	}

	SDL_Rect SMDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 60, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 40,
	400, 500 };

	imgStartBG->srcRect = &srcRect;
	imgStartBG->render(gRenderer, &SMDstRect);
}

void Game::gameOver() {
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
	SDL_Rect whiteTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &whiteTransparent);

	int wGOBG = 0;
	int hGOBG = 0;

	SDL_QueryTexture(imgGameOverBG->mTexture, NULL, NULL, &wGOBG, &hGOBG);

	SDL_Rect srcRect = { (wGOBG / 5) * 0, 0, wGOBG / 5, hGOBG };

	if (!flags->GOBGoutside) {
		switch (flags->GOBGinside) {
		case 0: srcRect.x = (wGOBG / 5) * 1; break;
		case 1: srcRect.x = (wGOBG / 5) * 2; break;
		case 2: srcRect.x = (wGOBG / 5) * 3; break;
		case 3: srcRect.x = (wGOBG / 5) * 4; break;
		default: break;
		}
	}

	SDL_Rect gameOverDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 15, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 20,
	500, 400 };

	imgGameOverBG->srcRect = &srcRect;
	imgGameOverBG->render(gRenderer, &gameOverDstRect);

	SDL_Color blackGOBG = { 0, 0, 0, 255 };
	SDL_Rect dstRectGameOver = { 145, 150 , 370, 150 };
	textGameOver->loadFromRenderedText(gRenderer, "GAME OVER", blackGOBG, &dstRectGameOver);

	SDL_Rect dstRectPlayAgain = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 3), (SCREEN_HEIGHT / 2) + 10,
		350, 40 };
	textPlayAgain->loadFromRenderedText(gRenderer, "DO YOU WANT TO PLAY AGAIN?", blackGOBG, &dstRectPlayAgain);
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