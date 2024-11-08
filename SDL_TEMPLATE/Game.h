#pragma once
#include "GameController.h"
#include "GameSound.h"
#include "GameImage.h"

#include "App_Info.h"

class Game {
private:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;

	GameImage* imgBackground;
	GameImage* imgHeart;
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