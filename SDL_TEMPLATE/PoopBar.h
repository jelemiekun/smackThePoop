#pragma once
#include "SDL.h"
#include "App_Info.h"

class PoopBar {
private:
	SDL_Texture* mTexture;
	SDL_Rect* rectBorder;
	SDL_Rect* rect[POOP_COUNT];
	uint8_t poopRemaining;

private:
	void updateEachRectPoop();

public:
	PoopBar();
	~PoopBar();

	void init(SDL_Renderer* renderer);
	void setXY(int x, int y);
	void render(SDL_Renderer* renderer);
	void poopReleased();
	void close();

};