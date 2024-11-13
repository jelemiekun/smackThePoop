#pragma once
#include "SDL.h"
#include "App_Info.h"

class PoopBar {
private:
	constexpr static int POOP_COUNT = 15;

	SDL_Texture* mTexture;
	SDL_Rect* rectBorder;
	SDL_Rect* rect[POOP_COUNT];
	uint8_t poopRemaining;
	

private:
	void updateEachRectPoop();

public:
	PoopBar();
	~PoopBar();

	void init(SDL_Renderer*& renderer, const Uint16& SCREEN_WIDTH);
	void setXY(const int& x, const int& y);
	void render(SDL_Renderer*& renderer);
	void poopReleased();
	void resetPoopBar();
	uint8_t getPoopRemaining() const;
	void close();

};