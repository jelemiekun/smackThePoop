#include "Game.h"
#include <iostream>
#include <string>

constexpr static const char* SCREEN_TITLE = "Smack The Poop";
constexpr static int SCREEN_X_POS = SDL_WINDOWPOS_CENTERED;
constexpr static int SCREEN_Y_POS = SDL_WINDOWPOS_CENTERED;
constexpr static Uint16 SCREEN_WIDTH = 640;
constexpr static Uint16 SCREEN_HEIGHT = 640;
constexpr static Uint32 SCREEN_FLAGS = SDL_WINDOW_SHOWN;
constexpr static bool LIMIT = true;

constexpr static auto PRIMARY_BUTTON = SDLK_l;
constexpr static auto LEFT_BUTTON = SDLK_j;
constexpr static auto RIGHT_BUTTON = SDLK_k;
constexpr static auto PAUSE_BUTTON = SDLK_ESCAPE;

Game::Game() : gWindow(nullptr), running(false), 
				imgBackground(nullptr), imgHeart1(nullptr), imgHeart2(nullptr),
				imgHeart3(nullptr), imgStartBG(nullptr), timerRect(nullptr),
				controller1(nullptr), gameSounds(nullptr), imgSettingsBG(nullptr),
				character(nullptr), gFontTimer(nullptr), textTimer(nullptr),
				gameTimer(nullptr), poopBar(nullptr), poopFart(nullptr), flags(nullptr),
				poopFXTimer(nullptr), poopClickTimer(nullptr), imgGameOverBG(nullptr),
				textGameOver(nullptr), gFontGame(nullptr), gFontPlayAgain(nullptr),
				textPlayAgain(nullptr), textSM1(nullptr), textSM2(nullptr), textSM3(nullptr),
				sliderHandleMusicVol(nullptr), sliderHandleSFXVol(nullptr), 
				sliderMusicVol(nullptr), sliderSFXVol(nullptr), sliderMusicDragging(nullptr),
				musicVolume(nullptr), imgButtons(nullptr), prmryBtnOpacity(nullptr),
				imgPauseBG(nullptr), textPause(nullptr), imgNoteBG(nullptr) {}

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

	gFontTimer = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZES::SIZE_TIMER);
	if (gFontTimer == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	gFontGame = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZES::SIZE_GAME);
	if (gFontGame == nullptr)
		std::cout << "Failed to load font: " << TTF_GetError() << '\n';
	else
		std::cout << "Font yubold loaded." << '\n';

	gFontPlayAgain = TTF_OpenFont("assets/Yubold.ttf", FONT_SIZES::SIZE_PLAY_AGAIN);
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

	textPause = new Text;
	textPause->setGFont(gFontPlayAgain);

	imgBackground = new GameImage;
	imgBackground->loadFromFile(gRenderer, "assets/img/background.png");

	imgGameOverBG = new GameImage;
	imgGameOverBG->loadFromFile(gRenderer, "assets/img/gameOverBG.png");

	imgStartBG = new GameImage;
	imgStartBG->loadFromFile(gRenderer, "assets/img/startMenuBG.png");


	imgSettingsBG = new GameImage;
	imgSettingsBG->loadFromFile(gRenderer, "assets/img/settingsBG.png");

	imgButtons = new GameImage;
	imgButtons->loadFromFile(gRenderer, "assets/img/buttons.png");

	imgPauseBG = new GameImage;
	imgPauseBG->loadFromFile(gRenderer, "assets/img/pauseBG.png");

	imgNoteBG = new GameImage;
	imgNoteBG->loadFromFile(gRenderer, "assets/img/noteBG.png");

	prmryBtnOpacity = new uint8_t;
	leftBtnOpacity = new uint8_t;
	rightBtnOpacity = new uint8_t;

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
	character->init(SCREEN_WIDTH, SCREEN_HEIGHT);

	controller1 = new GameController;
	controller1->init();

	musicVolume = new uint16_t;
	*musicVolume = MIX_MAX_VOLUME / 2;

	SFXVolume = new uint16_t;
	*SFXVolume = MIX_MAX_VOLUME / 2;

	gameSounds = new GameSound;
	gameSounds->initMixer();
	gameSounds->loadMusics();
	gameSounds->loadSoundFX();
	gameSounds->setFXsVolume(*SFXVolume);
	gameSounds->setMusic(ClassMusic::startMenu);
	gameSounds->playMusic();

	sliderHandleMusicVol = new SDL_Rect;
	sliderHandleMusicVol->x = 405;
	sliderHandleMusicVol->y = 185;
	sliderHandleMusicVol->w = 10;
	sliderHandleMusicVol->h = 20;
	
	sliderMusicVol = new SDL_Rect;
	sliderMusicVol->x = 340;
	sliderMusicVol->y = 190;
	sliderMusicVol->w = 150;
	sliderMusicVol->h = 10;

	sliderMusicDragging = new bool;
	*sliderMusicDragging = false;

	sliderHandleSFXVol = new SDL_Rect;
	sliderHandleSFXVol->x = 405;
	sliderHandleSFXVol->y = 235;
	sliderHandleSFXVol->w = 10;
	sliderHandleSFXVol->h = 20;

	sliderSFXVol = new SDL_Rect;
	sliderSFXVol->x = 340;
	sliderSFXVol->y = 240;
	sliderSFXVol->w = 150;
	sliderSFXVol->h = 10;

	sliderSFXDragging = new bool;
	*sliderSFXDragging = false;

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
	poopBar->init(gRenderer, SCREEN_WIDTH);
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
	flags->inPause = 0;

	flags->win = 0;

	flags->readyToChangeMusic = 0;
	
	flags->SMinPlayClick = 0;
	flags->SMinSettClick = 0;
	flags->SMinQuitClick = 0;

	flags->NOTEoutside = 1;
	flags->NOTEokayClick = 0;
	flags->doneReadingNotes = 0;

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

	constexpr static SDL_Rect PRESUME1 = { 149 + 48, 223, 254, 86 };
	constexpr static SDL_Rect PRESUME2 = { 134 + 48, 239, 284, 55 };
	constexpr static SDL_Rect PRESTART1 = { 149 + 48, 380 - 55, 254, 86 };
	constexpr static SDL_Rect PRESTART2 = { 134 + 48, 398 - 58, 284, 56 };
	constexpr static SDL_Rect PQUIT1 = { 149 + 48, 426, 254, 86 };
	constexpr static SDL_Rect PQUIT2 = { 134 + 48, 442, 284, 55 };
	bool PauseOutsideResume = true;
	bool PauseOutsideStart = true;
	bool PauseOutsideQuit = true;

	constexpr static SDL_Rect NOTE1 = { 149 + 48, 426, 254, 86 };
	constexpr static SDL_Rect NOTE2 = { 134 + 48, 442, 284, 55 };

	int x = 0;
	int y = 0;

	static constexpr SDL_Rect SETTBack = { 122, 108, 68, 40 };
	int volume = MIX_MAX_VOLUME / 2;
	bool SETToutsideBack = true;

	SDL_GetMouseState(&x, &y);

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		} else {
			
			// If reading notes
			if (!flags->doneReadingNotes) {
				if (event.type == SDL_MOUSEMOTION) {
					x = event.motion.x;
					y = event.motion.y;

					flags->NOTEoutside = (x < NOTE1.x || x > NOTE1.x + NOTE1.w || y < NOTE1.y || y > NOTE1.y + NOTE1.h) ||
						(x < NOTE2.x || x > NOTE2.x + NOTE2.w || y < NOTE2.y || y > NOTE2.y + NOTE2.h);


				} else if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (!flags->NOTEoutside) flags->NOTEokayClick = 1;
				} else if (event.type == SDL_MOUSEBUTTONUP) {
					if (flags->NOTEokayClick && !flags->NOTEoutside) {
						flags->NOTEoutside = 1;
						flags->NOTEokayClick = 0;
						flags->doneReadingNotes = 1;

						flags->SMoutside = 1;
						flags->SMinPlay = 0;
						flags->SMinSett = 0;
						flags->SMinQuit = 0;
						flags->SMinPlayClick = 0;
						flags->SMinSettClick = 0;
						flags->SMinQuitClick = 0;
					} else {
						flags->NOTEokayClick = 0;
					}
				}
				return;
			}

			// If in settings
			if (!flags->inGameOver && !flags->playing && !flags->inStart && flags->inSettings && !flags->inPause) {
				if (event.type == SDL_MOUSEMOTION) {
					x = event.motion.x;
					y = event.motion.y;

					SETToutsideBack = x < SETTBack.x || x > SETTBack.x + SETTBack.w || y < SETTBack.y || y > SETTBack.y + SETTBack.h;

					flags->SETinBack = !SETToutsideBack;

					if (*sliderMusicDragging) {
						// Move the music slider handle with the mouse
						sliderHandleMusicVol->x = event.motion.x - sliderHandleMusicVol->w / 2;

						// Constrain the music handle to the slider track
						if (sliderHandleMusicVol->x < sliderMusicVol->x) {
							sliderHandleMusicVol->x = sliderMusicVol->x;
						} else if (sliderHandleMusicVol->x > sliderMusicVol->x + sliderMusicVol->w - sliderHandleMusicVol->w) {
							sliderHandleMusicVol->x = sliderMusicVol->x + sliderMusicVol->w - sliderHandleMusicVol->w;
						}

						// Map music handle position to volume range (0 to MIX_MAX_VOLUME)
						volume = (sliderHandleMusicVol->x - sliderMusicVol->x) * MIX_MAX_VOLUME / (sliderMusicVol->w - sliderHandleMusicVol->w);

						// Set the music volume
						Mix_VolumeMusic(volume);
					}
					if (*sliderSFXDragging) {
						// Move the music slider handle with the mouse
						sliderHandleSFXVol->x = event.motion.x - sliderHandleSFXVol->w / 2;

						// Constrain the music handle to the slider track
						if (sliderHandleSFXVol->x < sliderSFXVol->x) {
							sliderHandleSFXVol->x = sliderSFXVol->x;
						} else if (sliderHandleSFXVol->x > sliderSFXVol->x + sliderSFXVol->w - sliderHandleSFXVol->w) {
							sliderHandleSFXVol->x = sliderSFXVol->x + sliderSFXVol->w - sliderHandleSFXVol->w;
						}

						// Map music handle position to volume range (0 to MIX_MAX_VOLUME)
						volume = (sliderHandleSFXVol->x - sliderSFXVol->x) * MIX_MAX_VOLUME / (sliderSFXVol->w - sliderHandleSFXVol->w);

						gameSounds->setFXsVolume(volume);
					}
				} else if (event.type == SDL_MOUSEBUTTONDOWN) {
					if (event.button.button == SDL_BUTTON_LEFT &&
						event.button.x >= sliderHandleSFXVol->x && event.button.x <= sliderHandleSFXVol->x + sliderHandleSFXVol->w &&
						event.button.y >= sliderHandleSFXVol->y && event.button.y <= sliderHandleSFXVol->y + sliderHandleSFXVol->h) {
						*sliderSFXDragging = true;  // Start dragging if mouse is down inside music slider handle
					}
					if (event.button.button == SDL_BUTTON_LEFT &&
						event.button.x >= sliderHandleMusicVol->x && event.button.x <= sliderHandleMusicVol->x + sliderHandleMusicVol->w &&
						event.button.y >= sliderHandleMusicVol->y && event.button.y <= sliderHandleMusicVol->y + sliderHandleMusicVol->h) {
						*sliderMusicDragging = true;  // Start dragging if mouse is down inside music slider handle
					}
				} else if (event.type == SDL_MOUSEBUTTONUP) {
					if (flags->SETinBack) {
						flags->inGameOver = 0;
						flags->inStart = 1;
						flags->playing = 0;
						flags->inSettings = 0;
					}

					if (event.button.button == SDL_BUTTON_LEFT) {
						// From settings, go to start menu
						

						*sliderMusicDragging = false;  // Stop dragging on mouse up music handle
						*sliderSFXDragging = false;  // Stop dragging on mouse up SFX handle
					}
				}
			}

			// if in game over menu
			if (LIMIT && flags->inGameOver && !flags->playing && !flags->inStart && !flags->inSettings && !flags->inPause) {
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
			if (LIMIT && !flags->inGameOver && !flags->playing && flags->inStart && !flags->inSettings && !flags->inPause) {
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
							flags->SMinSett = 0;
							flags->playing = 0;
							flags->inStart = 0;
							flags->inGameOver = 0;
							flags->inSettings = 0;
						}
						if (flags->SMinSett) {
							// From start menu, go to settings
							flags->SMinSettClick = 0;
							flags->SMinSett = 0;
							flags->playing = 0;
							flags->inStart = 0;
							flags->inGameOver = 0;
							flags->inSettings = 1;
							flags->SETinBack = 0;
						}
						// Quit
						if (flags->SMinQuit) { flags->SMinQuitClick = 0; running = false; }
					}
				default:
					break;
				}
			}

			// if playing and not paused
			if (LIMIT && flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings && !flags->inPause) {
				switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case PRIMARY_BUTTON:
						flags->clickedButtonPrmry = 1;
						if (flags->poopFinished) {
							flags->poopInProgress = 1;
							flags->poopFinished = 0;
							flags->FXInProgress = 1;
							flags->FXFinished = 0;
						}
						break;
					case LEFT_BUTTON:
						flags->clickedButtonLeft = 1;
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
						flags->clickedButtonRight = 1;
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
					case PAUSE_BUTTON:
						if (flags->pauseButtonClickFinished) {
							flags->inPause = 1;
							flags->pauseButtonClickFinished = 0;
							flags->pauseButtonClickInProgress = 1;

							gameTimer->pauseTimer();
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
					case PAUSE_BUTTON:
						flags->pauseButtonClickFinished = 1;
						flags->pauseButtonClickInProgress = 0;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}
			}

			// IF playing and paused
			if (LIMIT && flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings && flags->inPause) {
				switch (event.type) {
				case SDL_MOUSEMOTION:
					x = event.motion.x;
					y = event.motion.y;

					PauseOutsideResume = x < PRESUME1.x || x > PRESUME1.x + PRESUME1.w || y < PRESUME1.y || y > PRESUME1.y + PRESUME1.h ||
						x < PRESUME2.x || x > PRESUME2.x + PRESUME2.w || y < PRESUME2.y || y > PRESUME2.y + PRESUME2.h;
					PauseOutsideStart = x < PRESTART1.x || x > PRESTART1.x + PRESTART1.w || y < PRESTART1.y || y > PRESTART1.y + PRESTART1.h ||
						x < PRESTART2.x || x > PRESTART2.x + PRESTART2.w || y < PRESTART2.y || y > PRESTART2.y + PRESTART2.h;
					PauseOutsideQuit = x < PQUIT1.x || x > PQUIT1.x + PQUIT1.w || y < PQUIT1.y || y > PQUIT1.y + PQUIT1.h ||
						x < PQUIT2.x || x > PQUIT2.x + PQUIT2.w || y < PQUIT2.y || y > PQUIT2.y + PQUIT2.h;

					if (PauseOutsideResume && PauseOutsideStart && PauseOutsideQuit) {
						flags->PSOutside = 1;
					} else {
						flags->PSOutside = 0;

						if (!PauseOutsideResume) { flags->PSResumeInside = 1; flags->PSRestartInside = 0; flags->PSRQuitInside = 0; }
						if (!PauseOutsideStart) { flags->PSResumeInside = 0; flags->PSRestartInside = 1; flags->PSRQuitInside = 0; }
						if (!PauseOutsideQuit) { flags->PSResumeInside = 0; flags->PSRestartInside = 0; flags->PSRQuitInside = 1; }
					}
					break;
				case SDL_MOUSEBUTTONDOWN: {
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (flags->PSResumeInside) flags->PSInResumeClick = 1; // If play is beging clicked
						if (flags->PSRestartInside) flags->PSInRestartClick = 1; // If settings is beging clicked
						if (flags->PSRQuitInside) flags->PSInQuitClick = 1; // If quit is beging clicked
					}
				}
				break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT) {
						if (flags->PSResumeInside) { // Resume
							flags->inPause = 0;
							gameTimer->unpauseTimer();
						}
						if (flags->PSRestartInside) {
							// From pause, play again
							flags->playing = 0;
							flags->inStart = 0;
							flags->inGameOver = 0;
							flags->inSettings = 0;
							flags->inPause = 0;
							startGame();
						}
							// From pause, go to main menu
						// !flags->playing&& flags->inStart && !flags->inGameOver && !flags->inSettings && !flags->inPause;
						if (flags->PSRQuitInside) { 
							restartFlags();
							flags->inPause = 0;
							flags->playing = 0;
							flags->inStart = 1;
							flags->inGameOver = 0;
							flags->inSettings = 0;
							flags->readyToChangeMusic = 1;
						}
					}
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
					case PAUSE_BUTTON:
						if (flags->pauseButtonClickFinished) {
							flags->inPause = 0;
							flags->pauseButtonClickFinished = 0;
							flags->pauseButtonClickInProgress = 1;

							if (flags->inPause) gameTimer->pauseTimer();
							else gameTimer->unpauseTimer();
						}
						break;
					default:
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
					case PAUSE_BUTTON:
						flags->pauseButtonClickFinished = 1;
						flags->pauseButtonClickInProgress = 0;
						break;
					default:
						break;
					}
					break;
				default:
					break;
				}

				// If unpaused, reset flags
				if (!flags->inPause) {
					flags->PSOutside = 1;
					flags->PSResumeInside = 0;
					flags->PSRestartInside = 0;
					flags->PSRQuitInside = 0;
					flags->PSInResumeClick = 0;
					flags->PSInRestartClick = 0;
					flags->PSInQuitClick = 0;
				}
			}
		}
	}
}

void Game::update() {
	// Handle inputs if currently playing
	if (flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings && !flags->inPause) {
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

	SDL_Rect heart1 = {0 + HEART_X_ALLOWANCE, HEART_Y_ALLOWANCE,
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

	character->animate(gRenderer, flags->takeDamage, SCREEN_WIDTH);
	poopBar->render(gRenderer);

	// If currently playing
	if (flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings) {
		if (*prmryBtnOpacity != 0) renderButtonPrmry();
		if (*leftBtnOpacity != 0) renderButtonLeft();
		if (*rightBtnOpacity != 0) renderButtonRight();
	}

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
	if (LIMIT && flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings && !flags->inPause) {
		// Play beep sound if 3 or 2 or 1 seconds left
		Uint32 timeLeft = gameTimer->getRawTime();
		if ((timeLeft >= 990 && timeLeft <= 1010) ||
			(timeLeft >= 1990 && timeLeft <= 2010) ||
			(timeLeft >= 2990 && timeLeft <= 3010) ||
			(timeLeft >= 3990 && timeLeft <= 4010)) {
			gameSounds->setSoundFX(ClassSoundFXState::countDownBeep);
			gameSounds->playSoundFX();
		}

		flags->win = poopBar->getPoopRemaining() == 0;

		// If player win
		if (flags->win) {
			gameTimer->stopTimer();
			flags->playing = 0;
			flags->inStart = 0;
			flags->inGameOver = 1;
			flags->inSettings = 0;
			flags->inPause = 0;
			flags->readyToChangeMusic = 1;
		}
	}

	// If LIMIT is toggled and not currently playing
	if (LIMIT && !(!flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings && !flags->inPause)) {
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
	} else if (!flags->playing && !flags->inGameOver && !flags->inStart && !flags->inSettings && flags->inPause) {
		std::cout << "Pausing the game" << '\n';
		// pause the game;
	}

	bool inGameOver = LIMIT && !flags->playing && !flags->inStart && flags->inGameOver && !flags->inSettings && !flags->inPause;
	bool inStartMenu = !flags->playing && flags->inStart && !flags->inGameOver && !flags->inSettings && !flags->inPause;
	bool playAgain = !flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings && !flags->inPause;
	bool inSettings = !flags->playing && !flags->inStart && !flags->inGameOver && flags->inSettings && !flags->inPause;
	bool inPause = flags->playing && !flags->inStart && !flags->inGameOver && !flags->inSettings && flags->inPause;
	
	// std::cout << flags->playing << ", " << flags->inStart << ", " << flags->inGameOver << ", " << flags->inSettings << '\n';
	// std::cout << inGameOver << ", " << inStartMenu << ", " << playAgain << '\n';

	if (inStartMenu) startMenu();
	if (playAgain) startGame();
	if (inGameOver) gameOver();
	if (inSettings) settings();
	if (inPause) pause();

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

	
	SDL_Rect SMDstRect = {(SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 60, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 40,
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

	if (!flags->doneReadingNotes) note();
}

void Game::note() {
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 150);
	SDL_Rect blackTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &blackTransparent);

	int wSM = 0;
	int hSM = 0;

	SDL_QueryTexture(imgNoteBG->mTexture, NULL, NULL, &wSM, &hSM);

	SDL_Rect srcRect = { (wSM / 3) * 0, 0, wSM / 3, hSM };

	if (!flags->NOTEoutside) {
		if (flags->NOTEokayClick) srcRect.x = (wSM / 3) * 2;
		else srcRect.x = (wSM / 3) * 1;
	}


	SDL_Rect SMDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 60, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 40,
	400, 500 };

	imgNoteBG->srcRect = &srcRect;
	imgNoteBG->render(gRenderer, &SMDstRect);

	SDL_Color blackSM = { 0, 0, 0, 255 };
	SDL_Rect dstRectSM1 = { 148, 90 , 340, 90 };
	textSM1->loadFromRenderedText(gRenderer, "H o w  to  p l a y", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 190 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, "-Smack green/red poop", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 210 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, " before shooting in the bowl.", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 250 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, "-Press L to p o o p.", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 290 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, "-Press J to smack green poop.", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 330 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, "-Press K to smack red poop.", blackSM, &dstRectSM1);

	dstRectSM1 = { 150, 370 , 320, 30 };
	textSM1->loadFromRenderedText(gRenderer, "-Avoid smacking brown poop or farts.", blackSM, &dstRectSM1);
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
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
	SDL_Rect whiteTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &whiteTransparent);

	int wSETT = 0;
	int hSETT = 0;

	SDL_QueryTexture(imgSettingsBG->mTexture, NULL, NULL, &wSETT, &hSETT);

	SDL_Rect srcRect = { (wSETT / 2) * 0, 0, wSETT / 2, hSETT };

	if (flags->SETinBack) srcRect.x = (wSETT / 2) * 1;

	SDL_Rect settingsDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 5, (SCREEN_HEIGHT / 2) - (hSETT / 3) - 30,
	500, 500 };

	imgSettingsBG->srcRect = &srcRect;
	imgSettingsBG->render(gRenderer, &settingsDstRect);

	// Draw sliders background
	SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);
	SDL_RenderFillRect(gRenderer, sliderMusicVol);
	SDL_RenderFillRect(gRenderer, sliderSFXVol);

	// Draw sliders handle
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderFillRect(gRenderer, sliderHandleMusicVol);
	SDL_RenderFillRect(gRenderer, sliderHandleSFXVol);
}

void Game::pause() {
	SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 150);
	SDL_Rect whiteTransparent = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(gRenderer, &whiteTransparent);

	int wSM = 0;
	int hSM = 0;

	SDL_QueryTexture(imgPauseBG->mTexture, NULL, NULL, &wSM, &hSM);

	SDL_Rect srcRect = { (wSM / 7) * 0, 0, wSM / 7, hSM };

	if (!flags->PSOutside) {
		if (flags->PSResumeInside) {
			if (flags->PSInResumeClick) srcRect.x = (wSM / 7) * 2;
			else srcRect.x = (wSM / 7) * 1;
		}

		if (flags->PSRestartInside) {
			if (flags->PSInRestartClick) srcRect.x = (wSM / 7) * 4;
			else srcRect.x = (wSM / 7) * 3;
		}

		if (flags->PSRQuitInside) {
			if (flags->PSInQuitClick) srcRect.x = (wSM / 7) * 6;
			else srcRect.x = (wSM / 7) * 5;
		}
	}


	SDL_Rect SMDstRect = { (SCREEN_WIDTH / 2) - ((srcRect.w) / 2) + 60, (SCREEN_HEIGHT / 2) - ((srcRect.h) / 3) - 40,
	400, 500 };

	imgPauseBG->srcRect = &srcRect;
	imgPauseBG->render(gRenderer, &SMDstRect);

	SDL_Color black = { 0, 0, 0, 255 };

	SDL_Rect dstRectGameOver ={ 165, 100 , 320, 100 };

	textGameOver->loadFromRenderedText(gRenderer, "PAUSED", black, &dstRectGameOver);
}

void Game::renderButtonPrmry() {
	if (flags->clickedButtonPrmry) {
		int diff = 4;
		*prmryBtnOpacity = (*prmryBtnOpacity - diff < 0) ? 0 : *prmryBtnOpacity - diff;
	}

	int wBTNPMRT = 0;
	int hBTNPMRY = 0;

	SDL_QueryTexture(imgButtons->mTexture, NULL, NULL, &wBTNPMRT, &hBTNPMRY);

	SDL_Rect srcRect = { 0, (hBTNPMRY / 3) * 0, wBTNPMRT, hBTNPMRY / 3 };

	SDL_Rect dstRect = { (SCREEN_WIDTH / 2) + 210, (SCREEN_HEIGHT / 2) - (hBTNPMRY / 5) - 3,
	wBTNPMRT / 2 + 20, hBTNPMRY / 3 };

	imgButtons->srcRect = &srcRect;
	SDL_SetTextureAlphaMod(imgButtons->mTexture, *prmryBtnOpacity);
	imgButtons->render(gRenderer, &dstRect);
}

void Game::renderButtonLeft() {
	if ((flags->clickedButtonLeft && flags->poopInProgress) || *leftBtnOpacity != 255) {
		int diff = 4;
		*leftBtnOpacity = (*leftBtnOpacity - diff < 0) ? 0 : *leftBtnOpacity - diff;
	}

	int wBTNPMRT = 0;
	int hBTNPMRY = 0;

	SDL_QueryTexture(imgButtons->mTexture, NULL, NULL, &wBTNPMRT, &hBTNPMRY);

	SDL_Rect srcRect = { 0, (hBTNPMRY / 3) * 1, wBTNPMRT, hBTNPMRY / 3 };

	// (SCREEN_WIDTH / 2) + 210, (SCREEN_HEIGHT / 2) - (hBTNPMRY / 5) + 120,
	SDL_Rect dstRect = { (SCREEN_WIDTH / 2) - 260, (SCREEN_HEIGHT / 2) - (hBTNPMRY / 5) + 120,
	wBTNPMRT / 2 + 20, hBTNPMRY / 3 };

	imgButtons->srcRect = &srcRect;
	SDL_SetTextureAlphaMod(imgButtons->mTexture, *leftBtnOpacity);
	imgButtons->render(gRenderer, &dstRect);
}

void Game::renderButtonRight() {
	if ((flags->clickedButtonRight && flags->poopInProgress) || *rightBtnOpacity != 255) {
		int diff = 4;
		*rightBtnOpacity = (*rightBtnOpacity - diff < 0) ? 0 : *rightBtnOpacity - diff;
	}

	int wBTNPMRT = 0;
	int hBTNPMRY = 0;

	SDL_QueryTexture(imgButtons->mTexture, NULL, NULL, &wBTNPMRT, &hBTNPMRY);

	SDL_Rect srcRect = { 0, (hBTNPMRY / 3) * 2, wBTNPMRT, hBTNPMRY / 3 };

	SDL_Rect dstRect = { (SCREEN_WIDTH / 2) + 210, (SCREEN_HEIGHT / 2) - (hBTNPMRY / 5) + 120,
	wBTNPMRT / 2 + 20, hBTNPMRY / 3 };

	imgButtons->srcRect = &srcRect;
	SDL_SetTextureAlphaMod(imgButtons->mTexture, *rightBtnOpacity);
	imgButtons->render(gRenderer, &dstRect);
}

void Game::restartFlags() {
	flags->playing = 1;
	flags->inStart = 0;
	flags->inGameOver = 0;
	flags->inSettings = 0;
	flags->inPause = 0;
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
	flags->SETinBack = 0;
	flags->clickedButtonPrmry = 0;
	flags->clickedButtonLeft = 0;
	flags->clickedButtonRight = 0;
	flags->pauseButtonClickFinished = 1;
	flags->pauseButtonClickInProgress = 0;
	*prmryBtnOpacity = 255;
	*rightBtnOpacity = 255;
	*leftBtnOpacity = 255;

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
	flags->PSOutside = 1;
	flags->PSResumeInside = 0;
	flags->PSRestartInside = 0;
	flags->PSRQuitInside = 0;
	flags->PSInResumeClick = 0;
	flags->PSInRestartClick = 0;
	flags->PSInQuitClick = 0;
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