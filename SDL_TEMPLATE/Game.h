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
	InputFlags* flags;
	Heart* imgHeart1;
	Heart* imgHeart2;
	Heart* imgHeart3;
	HeartStates heartStates;
	Character* character;
	TTF_Font* gFont;
	Timer* gameTimer;
	Timer* poopFXTimer;
	Timer* poopClickTimer;
	Text* textTimer;
	SDL_Rect* timerRect;
	PoopBar* poopBar;
	PoopFart* poopFart;
	
	/*GameImage* imgPlayer;
	GameImage* imgAnimation;*/

	GameController* controller1;
	GameSound* gameSounds;
	SDL_Event event;
	bool running;

private:
	bool isGameOver() const;

public:
	Game();
	~Game();

public:
	void init();
	void input();
	void update();
	void render();
	void close();
	bool isRunning() const;
};