#pragma once
#include "SDL.h"
class GameController {
private:
	SDL_GameController* gGameController;
public:
	GameController();
	~GameController();

	void init();
	void input();\
	void close();

	SDL_GameController* getGameController() const;
};

