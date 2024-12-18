#pragma once
#include "GameController.h"
#include "GameSound.h"
#include "GameImage.h"
#include "Heart.h"
#include "Character.h"
#include "Text.h"
#include "Timer.h"
#include "PoopBar.h"
#include "PoopFart.h"
#include "InputFlags.h"

#include "App_Info.h"

struct HeartStates {
	unsigned int heart1 : 1;
	unsigned int heart2 : 1;
	unsigned int heart3 : 1;
};

class Game {
private:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	GameImage* imgBackground;
	GameImage* imgGameOverBG;
	GameImage* imgStartBG;
	GameImage* imgSettingsBG;
	GameImage* imgButtons;
	GameImage* imgPauseBG;
	GameImage* imgNoteBG;
	InputFlags* flags;
	Heart* imgHeart1;
	Heart* imgHeart2;
	Heart* imgHeart3;
	HeartStates heartStates;
	Character* character;
	TTF_Font* gFontTimer;
	TTF_Font* gFontGame;
	TTF_Font* gFontPlayAgain;
	Timer* gameTimer;
	Timer* poopFXTimer;
	Timer* poopClickTimer;
	Text* textTimer;
	Text* textGameOver;
	Text* textPlayAgain;
	Text* textSM1;
	Text* textSM2;
	Text* textSM3;
	Text* textPause;
	SDL_Rect* timerRect;
	PoopBar* poopBar;
	PoopFart* poopFart;
	SDL_Rect* sliderHandleMusicVol;
	SDL_Rect* sliderMusicVol;
	bool* sliderMusicDragging;
	uint16_t* musicVolume;
	SDL_Rect* sliderHandleSFXVol;
	SDL_Rect* sliderSFXVol;
	bool* sliderSFXDragging;
	uint16_t* SFXVolume;
	uint8_t* prmryBtnOpacity;
	uint8_t* leftBtnOpacity;
	uint8_t* rightBtnOpacity;
	
	/*GameImage* imgPlayer;
	GameImage* imgAnimation;*/

	GameController* controller1;
	GameSound* gameSounds;
	SDL_Event event;
	bool running;

private:
	void gameOver();
	bool isGameOver() const;
	void startGame();
	void note();
	void restartFlags();
	void initIcon();
	void renderButtonPrmry();
	void renderButtonLeft();
	void renderButtonRight();
	void resetMouseflags();
	void settings();
	void pause();

public:
	Game();
	~Game();

public:
	void init();
	void startMenu();
	void input();
	void update();
	void render();
	void close();
	bool isRunning() const;
};