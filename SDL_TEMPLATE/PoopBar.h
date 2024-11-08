#pragma once
#include "SDL.h"
#include "App_Info.h"

class PoopBar {
private:
	SDL_Texture* mTexture;
	static constexpr int POOP_COUNT = 20;
	SDL_Rect* rectBorder;
	SDL_Rect* rect[POOP_COUNT];
	Uint8 poopRemaining;

private:
	void updateEachRectPoop();

public:
	PoopBar();
	~PoopBar();

	void init(SDL_Renderer* renderer);
	void setXY(int x, int y);
	void render(SDL_Renderer* renderer);
	void close();

};