#pragma once
#include "GameController.h"
#include "GameSound.h"
#include "GameImage.h"
#include "Heart.h"

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
	Heart* imgHeart1;
	Heart* imgHeart2;
	Heart* imgHeart3;
	HeartStates heartStates;
	/*GameImage* imgPlayer;
	GameImage* imgAnimation;*/

	GameController* controller1;
	GameSound* gameSounds;
	SDL_Event event;
	bool running;

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