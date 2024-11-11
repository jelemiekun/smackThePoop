#include "Game.h"
#include <iostream>
#include <string>

Game::Game() : gWindow(nullptr), running(false), 
				imgBackground(nullptr), imgHeart1(nullptr), imgHeart2(nullptr),
				imgHeart3(nullptr), imgStartBG(nullptr), timerRect(nullptr),
				controller1(nullptr), gameSounds(nullptr), imgSettingsBG(nullptr),
				character(nullptr), gFontTimer(nullptr), textTimer(nullptr),
				gameTimer(nullptr), poopBar(nullptr), poopFart(nullptr), flags(nullptr),
				poopFXTimer(nullptr), poopClickTimer(nullptr), imgGameOverBG(nullptr),
				textGameOver(nullptr), gFontGame(nullptr), gFontPlayAgain(nullptr),
				textPlayAgain(nullptr), textSM1(nullptr), textSM2(nullptr), textSM3(nullptr) {}

Game::~Game() {}

void Game::initIcon() {
	std::string path = "assets/img/app.ico";
	SDL_Surface* iconSurface = IMG_Load(path.c_str());

	if (iconSurface == nullptr) {
		std::cout << "Failed to load icon: " << IMG_GetError() << '\n';
		return;
	}

	SDL_SetWindowIcon(gWindow, iconSurface);
	SDL_FreeSurface(iconSurface);
}

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

	initIcon();

	textTimer = new Text;
	textTimer->setGFont(gFontTimer);

	textGameOver = new Text;
	textGameOver->setGFont(gFontGame);

	textPlayAgain = new Text;
	textPlayAgain->setGFont(gFontPlayAgain);

	textSM1 = new Text;
	textSM1->setGFont(gFontPlayAgain);

	textSM2 = new Text;
	textSM2->setGFont(gFontPlayAgain);

	textSM3 = new Text;
	textSM3->setGFont(gFontPlayAgain);

	imgBackground = new GameImage;
	imgBackground->loadFromFile(gRenderer, "assets/img/background.png");

	imgGameOverBG = new GameImage;
	imgGameOverBG->loadFromFile(gRenderer, "assets/img/gameOverBG.png");

	imgStartBG = new GameImage;
	imgStartBG->loadFromFile(gRenderer, "assets/img/startMenuBG.png");


	imgSettingsBG = new GameImage;
	imgSettingsBG->loadFromFile(gRenderer, "assets/img/settingsBG.png");

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
	gameSounds->loadMusics();
	gameSounds->loadSoundFX();

	gameSounds->setMusic(ClassMusic::startMenu);
	gameSounds->playMusic();

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
	flags->inSettings = 0;

	flags->win = 0;

	flags->readyToChangeMusic = 0;
	
	flags->SMinPlayClick = 0;
	flags->SMinSettClick = 0;
	flags->SMinQuitClick = 0;

	resetMouseflags();

	gameTimer->resetTimer();
	gameTimer->setStartingTime(ALLOWANCE_TIME_GAMEPLAY);
	gameTimer->startTimer();
	gameTimer->getTimeInFormat();
	gameTimer->stopTimer();

	heartStates = { 1, 1, 1 };

	running = true;
}

bool Game::isRunning() const {
	return running;
}

void Game::input() {
	/*
	SMP - Start Menu Play,
	SMS - Start Menu Settings
	SMQ - Start Menu quit
	*/
	static constexpr SDL_Rect SMP1 = { 243, 223, 169, 86 };
	static constexpr SDL_Rect SMP2 = { 228, 239, 200, 55 };
	static constexpr SDL_Rect SMS1 = { 149 + 48, 380 - 55, 254, 86 };
	static constexpr SDL_Rect SMS2 = { 134 + 48, 398 - 58, 284, 56 };
	static constexpr SDL_Rect SMQ1 = { 243, 426, 169, 86 };
	static constexpr SDL_Rect SMQ2 = { 228, 442, 200, 55 };
	bool SMoutsidePlay = true;
	bool SMoutsideSett = true;
	bool SMoutsideQuit = true;

	static constexpr SDL_Rect GOBGyesR1 = { 141, 380, 146, 90 };
	static constexpr SDL_Rect GOBGyesR2 = { 133, 398, 163, 56 };
	static constexpr SDL_Rect GOBGnoR1 = { 141 + 221, 380, 146, 90 };
	static constexpr SDL_Rect GOBGnoR2 = { 133 + 221, 398, 163, 56 };
	bool GOBGoutsideYes = true;
	bool GOBGoutsideNo = true;

	int x = 0;
	int y = 0;

	SDL_GetMouseState(&x, &y);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		} else {
			if (!flags->inGameOver && !flags->playing && !flags->inStart && flags->inSettings) {

			}

			// if in game over menu
			if (LIMIT && flags->inGameOver && !flags->playing && !flags->inStart && !flags->inSettings) {
				switch (event.type) {
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;

					GOBGoutsideYes = (x < GOBGyesR1.x || x > GOBGyesR1.x + GOBGyesR1.w || y < GOBGyesR1.y || y > GOBGyesR1.y + GOBGyesR1.h) ||
									(x < GOBGyesR2.x || x > GOBGyesR2.x + GOBGyesR2.w || y < GOBGyesR2.y || y > GOBGyesR2.y + GOBGyesR2.h);
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
					if (event.button.button == SDL_BUTTON_LEFT && flags->GOBGoutside == 0) {
						if (flags->GOBGinside == 0) flags->GOBGinside = 1;
						if (flags->GOBGinside == 2) flags->GOBGinside = 3;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT && flags->GOBGoutside == 0) {
						// Play again after game over
						if (flags->GOBGinside == 1) {
							flags->inGameOver = 0;
							flags->inStart = 0;
							flags->playing = 0;
							flags->inSettings = 0;
							flags->GOBGinside = 0;
						}
						// After game over, go to start menu
						if (flags->GOBGinside == 3) {
							flags->inGameOver = 0;
							flags->inStart = 1;
							flags->playing = 0;
							flags->inSettings = 0;
							flags->GOBGinside = 2;
							flags->readyToChangeMusic = 1;
						}
					}
					break;
				default:
					break;
				}
			}

			// if in start menu
			if (LIMIT && !flags->inGameOver && !flags->playing && flags->inStart && !flags->inSettings) {
				switch (event.type) {
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;

					SMoutsidePlay = x < SMP1.x || x > SMP1.x + SMP1.w || y < SMP1.y || y > SMP1.y + SMP1.h ||
						x < SMP2.x || x > SMP2.x + SMP2.w || y < SMP2.y || y > SMP2.y + SMP2.h;
					SMoutsideSett = x < SMS1.x || x > SMS1.x + SMS1.w || y < SMS1.y || y > SMS1.y + SMS1.h ||
						x < SMS2.x || x > SMS2.x + SMS2.w || y < SMS2.y || y > SMS2.y + SMS2.h;
					SMoutsideQuit = x < SMQ1.x || x > SMQ1.x + SMQ1.w || y < SMQ1.y || y > SMQ1.y + SMQ1.h ||
						x < SMQ2.x || x > SMQ2.x + SMQ2.w || y < SMQ2.y || y > SMQ2.y + SMQ2.h;

					if (SMoutsidePlay && SMoutsideSett && SMoutsideQuit) {
						flags->SMoutside = 1;
					} else {
						flags->SMoutside = 0;

						if (!SMoutsidePlay) { flags->SMinPlay = 1; flags->SMinSett = 0; flags->SMinQuit = 0; }
						if (!SMoutsideSett) { flags->SMinPlay = 0; flags->SMinSett = 1; flags->SMinQuit = 0; }
						if (!SMoutsideQuit) { flags->SMinPlay = 0; flags->SMinSett = 0; flags->SMinQuit = 1; }
					}
					break;
				case SDL_MOUSEBUTTONDOWN: {
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (flags->SMinPlay) flags->SMinPlayClick = 1; // If play is beging clicked
						if (flags->SMinSett) flags->SMinSettClick = 1; // If settings is beging clicked
						if (flags->SMinQuit) flags->SMinQuitClick = 1; // If quit is beging clicked
					}
				}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (flags->SMinPlay) {
							// From start menu, play
							flags->SMinPlayClick = 0;
							flags->playing = 0;
							flags->inStart = 0;
							flags->inGameOver = 0;
							flags->inSettings = 0;
						}
						if (flags->SMinSett) {
							// From start menu, go to settings
							flags->SMinSettClick = 0;
							flags->playing = 0;
							flags->inStart = 0;
							flags->inGameOver = 0;
							flags->inSettings = 1;
						}
						// Quit
						if (flags->SMinQuit) { flags->SMinQuitClick = 0; running = false; }
					}
				default:
					break;
				}
			}

			// if playing
			if (LIMIT && flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings) {
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
	// Handle inputs if currently playing
	if (flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings) {
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

	// If LIMIT is on while playing
	if (LIMIT && flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings) {
		flags->win = poopBar->getPoopRemaining() == 0;

		// If player win
		if (flags->win) {
			gameTimer->stopTimer();
			flags->playing = 0;
			flags->inStart = 0;
			flags->inGameOver = 1;
			flags->inSettings = 0;
			flags->readyToChangeMusic = 1;
		}
	}

	// If LIMIT is toggled and not currently playing
	if (LIMIT && !(!flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings)) {
		//  If currently playing, check if game is finished 
		if (flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings) {
			flags->playing = !gameTimer->isFinish(); // Toggle off if timer runs out

			if (flags->playing)
				flags->playing = !isGameOver(); // Toggle off if no hearts left

			// If playing is toggled off, go to game over and stop the timer
			if (!flags->playing) {
				flags->inGameOver = 1;
				gameTimer->stopTimer();
				flags->readyToChangeMusic = 1; // Change music
			}
		}
	}

	bool inGameOver = LIMIT && !flags->playing && !flags->inStart && flags->inGameOver && !flags->inSettings;
	bool inStartMenu = !flags->playing && flags->inStart && !flags->inGameOver && !flags->inSettings;
	bool playAgain = !flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings;
	bool inSettings = !flags->playing && !flags->inStart && !flags->inGameOver && flags->inSettings;
	
	// std::cout << flags->playing << ", " << flags->inStart << ", " << flags->inGameOver << '\n';
	// std::cout << inGameOver << ", " << inStartMenu << ", " << playAgain << '\n';
	if (inStartMenu) startMenu();
	if (playAgain) startGame();
	if (inGameOver) gameOver();
	if (inSettings) settings();

	SDL_RenderPresent(gRenderer);
}

bool Game::isGameOver() const {
	return !heartStates.heart1;
}

void Game::startGame() {
	if (Mix_PlayingMusic() && flags->readyToChangeMusic) {
		gameSounds->stopMusic();

		gameSounds->setMusic(ClassMusic::playing);
		gameSounds->playMusic();

		flags->readyToChangeMusic = 0;
	}

	gameSounds->setMusic(ClassMusic::playing);
	gameSounds->playMusic();

	restartFlags();

	gameTimer->resetTimer();
	gameTimer->setStartingTime(ALLOWANCE_TIME_GAMEPLAY + 1000);
	gameTimer->startTimer();

	heartStates = { 1, 1, 1 };

	poopBar->resetPoopBar();
}

void Game::startMenu() {
	std::cout << "meow" << '\n';
	if (Mix_PlayingMusic() && flags->readyToChangeMusic) {
		gameSounds->stopMusic();

		gameSounds->setMusic(ClassMusic::startMenu);
		gameSounds->playMusic();

		flags->readyToChangeMusic = 0;

		heartStates = { 1, 1, 1 };

		gameTimer->resetTimer();
		gameTimer->setStartingTime(ALLOWANCE_TIME_GAMEPLAY + 500);
		gameTimer->startTimer();
		gameTimer->getTimeInFormat();
		gameTimer->stopTimer();

		poopBar->resetPoopBar();
		poopBar->render(gRenderer);
	}

	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
	SDL_Rect whiteTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &whiteTransparent);

	int wSM = 0;
	int hSM = 0;

	SDL_QueryTexture(imgStartBG->mTexture, NULL, NULL, &wSM, &hSM);

	SDL_Rect srcRect = { (wSM / 7) * 0, 0, wSM / 7, hSM };

	if (!flags->SMoutside) {
		if (flags->SMinPlay) { 
			if (flags->SMinPlayClick) srcRect.x = (wSM / 7) * 2; 
			else srcRect.x = (wSM / 7) * 1;
		}

		if (flags->SMinSett) {
			if (flags->SMinSettClick) srcRect.x = (wSM / 7) * 4;
			else srcRect.x = (wSM / 7) * 3;
		}

		if (flags->SMinQuit) { 
			if (flags->SMinQuitClick) srcRect.x = (wSM / 7) * 6;
			else srcRect.x = (wSM / 7) * 5; 
		}
	}

	SDL_Rect SMDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 60, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 40,
	400, 500 };

	imgStartBG->srcRect = &srcRect;
	imgStartBG->render(gRenderer, &SMDstRect);

	SDL_Color blackSM = { 0, 0, 0, 255 };
	SDL_Rect dstRectSM1 = { 178, 100 , 280, 50 };
	textSM1->loadFromRenderedText(gRenderer, "SMACK", blackSM, &dstRectSM1);

	SDL_Rect dstRectSM2 = { 290, 145, 60, 20 };
	textSM2->loadFromRenderedText(gRenderer, "THE", blackSM, &dstRectSM2);

	SDL_Color brownSM = { 99, 51, 0, 255 };
	SDL_Rect dstRectSM3 = { 180, 155 , 290, 60 };
	textSM1->loadFromRenderedText(gRenderer, "P O O P", brownSM, &dstRectSM3);
}

void Game::gameOver() {
	if (Mix_PlayingMusic() && flags->readyToChangeMusic) {
		gameSounds->stopMusic();

		if (flags->win) gameSounds->setMusic(ClassMusic::win);
		else gameSounds->setMusic(ClassMusic::gameOver);
		gameSounds->playMusic();

		flags->readyToChangeMusic = 0;

		resetMouseflags();
	}

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

	SDL_Rect dstRectGameOver;
	std::string message;

	if (flags->win) { dstRectGameOver = { 140, 150 , 370, 150 }; message = "YOU WIN!"; }
	else { dstRectGameOver = { 145, 150 , 370, 150 }; message = "GAME OVER"; }
	textGameOver->loadFromRenderedText(gRenderer, message, blackGOBG, &dstRectGameOver);

	SDL_Rect dstRectPlayAgain = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 3), (SCREEN_HEIGHT / 2) + 10,
		350, 40 };
	textPlayAgain->loadFromRenderedText(gRenderer, "DO YOU WANT TO PLAY AGAIN?", blackGOBG, &dstRectPlayAgain);
}

void Game::settings() {

}

void Game::restartFlags() {
	flags->playing = 1;
	flags->inStart = 0;
	flags->inGameOver = 0;
	flags->inSettings = 0;
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
	flags->readyToChangeMusic = 0;
	flags->win = 0;

	resetMouseflags();
}

void Game::resetMouseflags() {
	flags->GOBGoutside = 1;
	flags->GOBGinside = 0;
	flags->SMoutside = 1;
	flags->SMinPlay = 0;
	flags->SMinSett = 0;
	flags->SMinQuit = 0;
	flags->SMinPlayClick = 0;
	flags->SMinSettClick = 0;
	flags->SMinQuitClick = 0;
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